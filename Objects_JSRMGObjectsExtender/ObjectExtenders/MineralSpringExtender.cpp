#include "../pch.h"
#include "MineralSpringExtender.h"


namespace mineralSpring {
    BOOL H3MapItemMineralSpring::IsVisitedByHero(const H3Hero* hero) noexcept
    {
        sprintf(newMapItem::buffer, ErmVariableFormat, hero->id);
        return Era::GetAssocVarIntValue(newMapItem::buffer);
    }
    void H3MapItemMineralSpring::SetAsVisited(const H3Hero* hero) noexcept
    {
        sprintf(newMapItem::buffer, ErmVariableFormat, hero->id);
        Era::SetAssocVarIntValue(newMapItem::buffer, 1);
    }
    void H3MapItemMineralSpring::SetAsNotVisited(const H3Hero* hero) noexcept
    {
        sprintf(newMapItem::buffer, ErmVariableFormat, hero->id);
        Era::SetAssocVarIntValue(newMapItem::buffer, 0);
    }

    MineralSpringExtender::MineralSpringExtender()
        : H3ActiveObject<H3MapItemMineralSpring>(
            "EraPlugin.MineralSpringExtender.js",
            extender::HOTA_OBJECT_TYPE,
            MINERAL_SPRING_OBJECT_SUBTYPE)
    {
    }

    MineralSpringExtender* MineralSpringExtender::instance = nullptr;
    MineralSpringExtender& MineralSpringExtender::Get()
    {
        if (!instance) instance = new MineralSpringExtender();
        return *instance;
    }

    BOOL MineralSpringExtender::SetHintInH3TextBuffer(H3MapItem* mapItem, const H3Hero* hero, const int playerId, BOOL isRightClick) const noexcept
    {
        if (GetFromMapItem(mapItem))
        {
            H3String objName = RMGObjectInfo::GetObjectName(mapItem);
            int teamId = THISCALL_2(int, 0x4A55D0, P_Game->Get(), playerId);
            BOOL teamVisited = IsObjectVisitedByTeam(objectType, objectSubtype, teamId);

            // Add extra hint if object is visited at least once by the team
            if (teamVisited)
            {
                AddExtraInfoHint(&objName, isRightClick);
            }

            // Add "Visited/Not visited" hint
            if (hero)
            {
                const bool isVisitedByHero = H3MapItemMineralSpring::IsVisitedByHero(hero);
                AddHeroVisitedHint(&objName, isRightClick, isVisitedByHero);
            }
            sprintf(h3_TextBuffer, "%s", objName.String());

            return true;
        }

        return false;
    }

    BOOL MineralSpringExtender::VisitMapItem(H3Hero* hero, H3MapItem* mapItem, const H3Position pos,
        const BOOL isHuman) const noexcept
    {
        if (GetFromMapItem(mapItem))
        {
            const bool isVisitedByHero = H3MapItemMineralSpring::IsVisitedByHero(hero);
            if (isVisitedByHero)
            {
                if (isHuman)
                {
                    FASTCALL_12(void, 0x4F6C00,
                        this->GetVisitedMessage().String(),
                        1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -777);
                }
            }
            else
            {
                if (isHuman)
                {
                    FASTCALL_12(void, 0x4F6C00,
                        this->GetVisitingMessage().String(),
                        1, -1, -1, 11, 1, 37, MOVE_POINTS_GIVEN, -1, 0, -1, -777);
                }
                ProcObjectFlagsVisitedByTeam(hero, objectType, objectSubtype);
                H3MapItemMineralSpring::SetAsVisited(hero);
                hero->luckBonus += LUCK_GIVEN;

                if (hero->movement < MAX_HERO_MOVEMENT_POINTS)
                {
                    hero->movement += MOVE_POINTS_GIVEN;
                    ClampMovementPoints(hero->movement);
                }

                if (hero->maxMovement < MAX_HERO_MOVEMENT_POINTS)
                {
                    hero->maxMovement += MOVE_POINTS_GIVEN;
                    ClampMovementPoints(hero->maxMovement);
                }

                if (isHuman)
                {
                    // H3AdventureMgrDlg::RedrawHeroesSlots
                    THISCALL_4(void, 0x4032E0, P_AdventureManager->dlg, -1, 1, 1);
                }
            }

            return true;
        }

        return false;
    }

    BOOL MineralSpringExtender::SetAiMapItemWeight(H3MapItem* mapItem, H3Hero* hero, const H3Player* player,
        int& aiMapItemWeight, int* moveDistance, const H3Position pos) const noexcept
    {
        if (GetFromMapItem(mapItem))
        {
            const bool isVisitedByHero = H3MapItemMineralSpring::IsVisitedByHero(hero);

            if (!isVisitedByHero)
            {
                const int checkDist = MOVE_POINTS_GIVEN / 2;

                if (*moveDistance < checkDist)
                {
                    *moveDistance = 0;
                    aiMapItemWeight = 10000;
                }
                else
                {
                    *moveDistance -= checkDist;
                    aiMapItemWeight = THISCALL_2(int, 0x528260, hero, LUCK_GIVEN);
                }
            }

            return true;
        }

        return false;
    }

    int MineralSpringExtender::AI_OnScouting_Value() const noexcept
    {
        return 1;
    }
}
