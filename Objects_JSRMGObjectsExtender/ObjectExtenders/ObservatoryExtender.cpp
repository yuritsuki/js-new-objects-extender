#include "../pch.h"
#include "ObservatoryExtender.h"


namespace observatory {
    BOOL H3MapItemObservatory::IsVisitedByHero(H3Hero* hero) noexcept
    {
        sprintf(newMapItem::buffer, ErmVariableFormat, hero->id);
        return Era::GetAssocVarIntValue(newMapItem::buffer);
    }
    void H3MapItemObservatory::SetAsVisited(H3Hero* hero) noexcept
    {
        sprintf(newMapItem::buffer, ErmVariableFormat, hero->id);
        Era::SetAssocVarIntValue(newMapItem::buffer, 1);
    }
    void H3MapItemObservatory::SetAsNotVisited(H3Hero* hero) noexcept
    {
        sprintf(newMapItem::buffer, ErmVariableFormat, hero->id);
        Era::SetAssocVarIntValue(newMapItem::buffer, 0);
    }

    INT32 __stdcall HiHook_004E42E0(HiHook* h, H3Hero* this_)
    {
        // Get scouting radius
        INT32 result = THISCALL_1(INT32, h->GetDefaultFunc(), this_);

        if (H3MapItemObservatory::IsVisitedByHero(this_))
        {
            result += VISION_RADIUS_GIVEN;
        }
        return result;
    }

    void ObservatoryExtender::CreatePatches()
    {
        static bool hookInstalled = false;
        if (hookInstalled) return;

        _PI->WriteHiHook(0x4E42E0, SPLICE_, EXTENDED_, THISCALL_, HiHook_004E42E0);
        hookInstalled = true;
    }

    ObservatoryExtender::ObservatoryExtender()
        : H3ActiveObject<H3MapItemObservatory>(
            "EraPlugin.ObservatoryExtender.js",
            extender::HOTA_OBJECT_TYPE,
            WATER_OBSERVATORY_OBJECT_SUBTYPE)
    {
        CreatePatches();
    }

    ObservatoryExtender* ObservatoryExtender::instance = nullptr;
    ObservatoryExtender& ObservatoryExtender::Get()
    {
        if (!instance) instance = new ObservatoryExtender();
        return *instance;
    }

    BOOL ObservatoryExtender::SetHintInH3TextBuffer(H3MapItem* mapItem, const H3Hero* hero, const int playerId, BOOL isRightClick) const noexcept
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
                const bool isVisitedByHero = H3MapItemObservatory::IsVisitedByHero(const_cast <H3Hero*> (hero));
                AddHeroVisitedHint(&objName, isRightClick, isVisitedByHero);
            }

            sprintf(h3_TextBuffer, "%s", objName.String());

            return true;
        }

        return false;
    }

    BOOL ObservatoryExtender::VisitMapItem(H3Hero* hero, H3MapItem* mapItem, const H3Position pos,
        const BOOL isHuman) const noexcept
    {
        if (GetFromMapItem(mapItem))
        {
            const bool isVisitedByHero = H3MapItemObservatory::IsVisitedByHero(hero);

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
                H3MapItemObservatory::SetAsVisited(hero);

                if (isHuman)
                {
                    FASTCALL_12(void, 0x4F6C00,
                        this->GetVisitingMessage().String(),
                        1, -1, -1, 39, VISION_RADIUS_GIVEN, -1, 0, -1, 0, -1, -777);
                }

                if (hero->owner <= 7 && hero->x >= 0)
                {
                    int radius = hero->GetScoutingRadius();

                    //if (P_NetworkGame && !IntAt(0x698450))
                    //{
                    //    H3Position pos(hero->x, hero->y, hero->z);
                    //    H3NetworkMsg::CreateReveal(&networkMsg, pos, hero->owner, radius);
                    //    MEMORY[0x481CB0](&networkMsg);          // H3NetworkData::SendShortCommand
                    //}

                    //H3Game::OpenArea
                    THISCALL_7(void, 0x49CDD0, P_Game->Get(), hero->x, hero->y, hero->z, hero->owner, radius, 0);
                }
            }
            ProcObjectFlagsVisitedByTeam(hero, objectType, objectSubtype);

            return true;
        }

        return false;
    }

    BOOL ObservatoryExtender::SetAiMapItemWeight(H3MapItem* mapItem, H3Hero* hero, const H3Player* player,
        int& aiMapItemWeight, int* moveDistance, const H3Position pos) const noexcept
    {
        if (GetFromMapItem(mapItem))
        {
            const bool isVisitedByHero = H3MapItemObservatory::IsVisitedByHero(hero);

            if (!isVisitedByHero)
            {
                aiMapItemWeight = 2 * (8 - P_Game->date.day) * (hero->maxMovement / 100) / 8;
            }

            return true;
        }

        return false;
    }

    int ObservatoryExtender::AI_OnScouting_Value() const noexcept
    {
        return 50;
    }
}
