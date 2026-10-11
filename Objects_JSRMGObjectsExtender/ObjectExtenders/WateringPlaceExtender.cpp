#include "../pch.h"
#include "WateringPlaceExtender.h"


namespace wateringPlace
{
    BOOL H3MapItemWateringPlace::IsVisitedByHero(const H3Hero* hero) noexcept
    {
        sprintf(h3_TextBuffer, ErmVariableFormat, hero->id);

        return Era::GetAssocVarIntValue(h3_TextBuffer);
    }
    void H3MapItemWateringPlace::SetAsVisited(H3Hero* hero) noexcept
    {
        sprintf(newMapItem::buffer, ErmVariableFormat, hero->id);
        Era::SetAssocVarIntValue(newMapItem::buffer, 1);
    }
    void H3MapItemWateringPlace::SetAsNotVisited(H3Hero* hero) noexcept
    {
        sprintf(newMapItem::buffer, ErmVariableFormat, hero->id);
        Era::SetAssocVarIntValue(newMapItem::buffer, 0);
    }

    _ERH_(OnEveryDay)
    {
        for (const auto heroId : P_Game->players[P_CurrentPlayerID].heroIDs)
        {
            if (heroId >= 0)
            {
                H3Hero* hero = P_Game->GetHero(heroId);

                if (H3MapItemWateringPlace::IsVisitedByHero(hero))
                {
                    H3MapItemWateringPlace::SetAsNotVisited(hero);
                    hero->movement += WATERING_PLACE_MOVE_POINTS_GIVEN;
                }
            }

        }
    }

    void WateringPlaceExtender::CreatePatches()
    {
        static bool handlerRegistered = false;
        if (handlerRegistered) return;
        Era::RegisterHandler(OnEveryDay, "OnEveryDay");
        handlerRegistered = true;
    }

    WateringPlaceExtender::WateringPlaceExtender()
        : H3ActiveObject<H3MapItemWateringPlace>(
            "EraPlugin.WateringPlaceExtender.js",
            extender::HOTA_OBJECT_TYPE,
            WATERING_PLACE_OBJECT_SUBTYPE)
    {
        CreatePatches();
    }

    WateringPlaceExtender* WateringPlaceExtender::instance = nullptr;
    WateringPlaceExtender& WateringPlaceExtender::Get()
    {
        if (!instance) instance = new WateringPlaceExtender();
        return *instance;
    }

    BOOL WateringPlaceExtender::SetHintInH3TextBuffer(H3MapItem* mapItem, const H3Hero* hero, const int interactPlayerId,
        const BOOL isRightClick) const noexcept
    {
        if (GetFromMapItem(mapItem))
        {
            H3String objName = RMGObjectInfo::GetObjectName(mapItem);
            int teamId = THISCALL_2(int, 0x4A55D0, P_Game->Get(), interactPlayerId);
            BOOL teamVisited = IsObjectVisitedByTeam(objectType, objectSubtype, teamId);

            // Add extra hint if object is visited at least once by the team
            if (teamVisited)
            {
                AddExtraInfoHint(&objName, isRightClick);
            }

            // Add "Visited/Not visited" hint
            if (hero)
            {
                const bool isVisitedByHero = H3MapItemWateringPlace::IsVisitedByHero(hero);
                AddHeroVisitedHint(&objName, isRightClick, isVisitedByHero);
            }

            sprintf(h3_TextBuffer, "%s", objName.String());

            return true;
        }

        return false;
    }

    BOOL WateringPlaceExtender::VisitMapItem(H3Hero* hero, H3MapItem* mapItem, const H3Position pos,
        const BOOL isHuman) const noexcept
    {
        if (GetFromMapItem(mapItem))
        {
            // H3MapItem::SetAsVisited
            //INT16 playerId = static_cast<INT16>(hero->owner);
            //THISCALL_2(void, 0x4FC620, mapItem, playerId);

            const bool isVisitedByHero = H3MapItemWateringPlace::IsVisitedByHero(hero);

            if (isVisitedByHero)
            {
                if (isHuman)
                {
                    FASTCALL_12(void, 0x4F6C00, this->GetVisitedMessage().String(),
                        1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -777);
                }
            }
            // What is it?
            //else if ((mapItem->setup & 1) != 0)
            //{
            //    if (isHuman)
            //    {
            //        FASTCALL_12(void, 0x4F6C00, this->GetVisitedMessage().String(),
            //            1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -777);
            //    }
            //}
            else
            {
                ProcObjectFlagsVisitedByTeam(hero, objectType, objectSubtype);

                if (isHuman)
                {
                    FASTCALL_12(void, 0x4F6C00, this->GetVisitingMessage().String(),
                        2, -1, -1, 
                        38, -hero->movement,
                        37, WATERING_PLACE_MOVE_POINTS_GIVEN,
                        -1, 0, -1, 0);

                    if (P_WindowManager->resultItemID != eControlId::CANCEL)
                    {
                        hero->movement = 0;
                        H3MapItemWateringPlace::SetAsVisited(hero);

                        if (hero->maxMovement < MAX_HERO_MOVEMENT_POINTS)
                        {
                            hero->maxMovement += WATERING_PLACE_MOVE_POINTS_GIVEN;
                            ClampMovementPoints(hero->maxMovement);
                        }
                        
                        // H3AdventureMgrDlg::RedrawHeroesSlots
                        THISCALL_4(void, 0x4032E0, P_AdventureManager->dlg, -1, 1, 1);
                    }
                }
                // AI::H3Hero::CalculateMapPosWeight_
                else if (hero->dest_x == -1 && THISCALL_2(int, 0x52C1F0, hero, pos) > 0)
                {
                    hero->movement = 0;
                    H3MapItemWateringPlace::SetAsVisited(hero);

                    if (hero->maxMovement < MAX_HERO_MOVEMENT_POINTS)
                    {
                        hero->maxMovement += WATERING_PLACE_MOVE_POINTS_GIVEN;
                        ClampMovementPoints(hero->maxMovement);
                    }
                }
            }

            return true;
        }

        return false;
    }

    BOOL WateringPlaceExtender::SetAiMapItemWeight(
        H3MapItem* mapItem, H3Hero* hero, const H3Player* player,
        int& aiMapItemWeight, int* moveDistance, const H3Position pos) const noexcept
    {
        if (GetFromMapItem(mapItem))
        {
            const bool isVisitedByHero = H3MapItemWateringPlace::IsVisitedByHero(hero);

            if (isVisitedByHero
                //|| THISCALL_2(BOOL8, 0x529B70, mapItem, player->ownerID) && (mapItem->setup & 1) != 0
                || *moveDistance >= 0 && *moveDistance > hero->movement
                || hero->movement >= 500
                )
            {
                aiMapItemWeight = 0;
                return true;
            }
            *moveDistance = 0;
            aiMapItemWeight = 10000;
            return true;
        }

        return false;
    }

    int WateringPlaceExtender::AI_OnScouting_Value() const noexcept
    {
        return 1;
    }
}
