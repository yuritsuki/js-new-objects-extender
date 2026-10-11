#include "../pch.h"
#include "TrailblazerExtender.h"


namespace trailblazer {
    BOOL H3MapItemTrailblazer::IsVisitedByHero(H3Hero* hero, int terrainType) noexcept
    {
        sprintf(newMapItem::buffer, ErmVariableFormat, hero->id);
        int bitmask = Era::GetAssocVarIntValue(newMapItem::buffer);
        return (bitmask & (1 << terrainType)) != 0;
    }
    void H3MapItemTrailblazer::SetAsVisited(H3Hero* hero, int terrainType) noexcept
    {
        sprintf(newMapItem::buffer, ErmVariableFormat, hero->id);
        int bitmask = Era::GetAssocVarIntValue(newMapItem::buffer);
        bitmask |= (1 << terrainType);
        Era::SetAssocVarIntValue(newMapItem::buffer, bitmask);
    }
    void H3MapItemTrailblazer::SetAsNotVisited(H3Hero* hero) noexcept
    {
        sprintf(newMapItem::buffer, ErmVariableFormat, hero->id);
        Era::SetAssocVarIntValue(newMapItem::buffer, 0);
    }

    int __stdcall GetStepCost(HiHook* h, H3MapItem* mapItem, char direction, signed int movementLeft, int pathfindSkill,
        int road, int flyPower, int waterWalkPower, int nativeLand, int nomadsCount)
    {
        INT8 targetLandType = mapItem->land;

        if (H3Hero* hero = P_ActivePlayer->GetActiveHero())
        {
            if (!road)
            {
                const bool isVisitedByHero = H3MapItemTrailblazer::IsVisitedByHero(hero, targetLandType);
                if (isVisitedByHero)
                {
                    int stepTrailCost = TRAILBLAZER_STEP_COST;
                    if ((direction & 1) != 0)
                    {
                        stepTrailCost = static_cast<int>(static_cast<float>(stepTrailCost) * 1.4142135f);
                    }

                    return stepTrailCost;
                }
            }
        }

        return FASTCALL_9(int, h->GetDefaultFunc(), mapItem, direction, movementLeft, pathfindSkill, road, flyPower,
            waterWalkPower, nativeLand, nomadsCount);
    }

    void TrailblazerExtender::CreatePatches()
    {
        static bool patchesCreated = false;
        if (patchesCreated) return;
        _pi->WriteHiHook(0x4B14A0, FASTCALL_, GetStepCost);
        patchesCreated = true;
    }

    TrailblazerExtender::TrailblazerExtender()
        : H3ActiveObject<H3MapItemTrailblazer>(
            "EraPlugin.TrailblazerExtender.js",
            extender::HOTA_OBJECT_TYPE,
            TRAILBLAZER_OBJECT_SUBTYPE)
    {
        CreatePatches();
    }

    TrailblazerExtender* TrailblazerExtender::instance = nullptr;
    TrailblazerExtender& TrailblazerExtender::Get()
    {
        if (!instance) instance = new TrailblazerExtender();
        return *instance;
    }

    BOOL TrailblazerExtender::SetHintInH3TextBuffer(H3MapItem* mapItem, const H3Hero* hero, int playerId, BOOL isRightClick) const noexcept
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
                const bool isVisitedByHero = H3MapItemTrailblazer::IsVisitedByHero((H3Hero*)hero, mapItem->land);
                AddHeroVisitedHint(&objName, isRightClick, isVisitedByHero);
            }

            sprintf(h3_TextBuffer, "%s", objName.String());

            return true;
        }

        return false;
    }

    BOOL TrailblazerExtender::VisitMapItem(H3Hero* hero, H3MapItem* mapItem, const H3Position pos,
        const BOOL isHuman) const noexcept
    {
        if (GetFromMapItem(mapItem))
        {
            const int terrainType = mapItem->land;
            const bool isVisitedByHero = H3MapItemTrailblazer::IsVisitedByHero(hero, terrainType);

            if (!isVisitedByHero)
            {
                H3MapItemTrailblazer::SetAsVisited(hero, terrainType);
                if (isHuman)
                {
                    FASTCALL_12(void, 0x4F6C00,
                        this->GetVisitingMessage().String(),
                        1, -1, -1, 42, 0, -1, 0, -1, 0, -1, -777);
                }
            }

            else
            {
                if (isHuman)
                {
                    FASTCALL_12(void, 0x4F6C00,
                        this->GetVisitedMessage().String(),
                        1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -777);
                }
            }

            ProcObjectFlagsVisitedByTeam(hero, objectType, objectSubtype);

            return true;
        }

        return false;
    }

    BOOL TrailblazerExtender::SetAiMapItemWeight(
        H3MapItem* mapItem, H3Hero* hero, const H3Player* player,
        int& aiMapItemWeight, int* moveDistance, const H3Position pos) const noexcept
    {
        if (GetFromMapItem(mapItem))
        {
            const bool isVisitedByHero = H3MapItemTrailblazer::IsVisitedByHero(hero, mapItem->land);

            if (!isVisitedByHero)
            {
                const int currDay = P_Game->Get()->date.day;
                const int weekProfit = 250 * (8 - currDay);
                if (*moveDistance < weekProfit)
                {
                    *moveDistance = 0;
                    aiMapItemWeight = 10000;
                }
                else
                {
                    aiMapItemWeight = 50;
                    *moveDistance -= weekProfit;
                }
            }

            return true;
        }

        return false;
    }

    int TrailblazerExtender::AI_OnScouting_Value() const noexcept
    {
        return 10;
    }
}
