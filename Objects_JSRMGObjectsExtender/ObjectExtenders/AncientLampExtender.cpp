#include "../pch.h"
#include "AncientLampExtender.h"

namespace ancientLamp
{
    void H3MapItemAncientLamp::Reset()
    {
        visited = 0;
    }

    AncientLampExtender::AncientLampExtender() 
        : H3ActiveObject<H3MapItemAncientLamp>(
            "EraPlugin.AncientLampExtender.js",
            extender::HOTA_PICKUPABLE_OBJECT_TYPE,
            ANCIENT_LAMP_OBJECT_SUBTYPE)
    {
    }

    AncientLampExtender* AncientLampExtender::instance = nullptr;
    AncientLampExtender& AncientLampExtender::Get()
    {
        if (!instance) instance = new AncientLampExtender();
        return *instance;
    }

    BOOL AncientLampExtender::InitNewGameMapItemSetup(H3MapItem* mapItem) const noexcept
    {
        if (const auto ancientLamp = GetFromMapItem(mapItem))
        {
            *ancientLamp = {};
            ancientLamp->creatureType = ANCIENT_LAMP_CREATURE_TYPE;
            ancientLamp->creatureCount = rand() % (ANCIENT_LAMP_MAX_CREATURE_COUNT - ANCIENT_LAMP_MIN_CREATURE_COUNT + 1) + ANCIENT_LAMP_MIN_CREATURE_COUNT; // from 4 to 6
            ancientLamp->Reset();

            return true;
        }

        return false;
    }

    BOOL AncientLampExtender::SetHintInH3TextBuffer(
        H3MapItem* mapItem, const H3Hero* hero, const int playerId,
        BOOL isRightClick) const noexcept
    {
        if (auto ancientLamp = GetFromMapItem(mapItem))
        {
            H3String objName = RMGObjectInfo::GetObjectName(mapItem);

            // Object scouting - use only for objects with "visited" property starting from 5 bit offset
            BOOL playerVisited = THISCALL_2(bool, 0x529B70, mapItem, H3HumanID::Get());

            if (playerVisited)
            {
                // Read string from json
                LPCSTR extraInfo = EraJS::read(H3String::Format(extraObjectInfo_key, objectType, objectSubtype).String());

                sprintf(h3_TextBuffer, "%s%s",
                    isRightClick ? "\n\n" : " ",
                    H3String::Format(extraInfo, P_CreatureInformation[ANCIENT_LAMP_CREATURE_TYPE].namePlural, ancientLamp->creatureCount).String());
                objName.Append(h3_TextBuffer);
            }

            sprintf(h3_TextBuffer, "%s", objName.String());

            return true;
        }

        return false;
    }

    BOOL AncientLampExtender::VisitMapItem(
        H3Hero *hero, H3MapItem *mapItem, const H3Position pos,
        const BOOL isHuman) const noexcept
    {
        if (auto ancientLamp = GetFromMapItem(mapItem))
        {
            if (isHuman)
            {
                FASTCALL_12(void, 0x4F6C00,
                    this->GetVisitingMessage().String(),
                    2, -1, -1, -1, 0, -1, 0, -1, 0, -1, -777);

                if (P_WindowManager->resultItemID != eControlId::CANCEL)
                {
                    INT16 creatureCount = ancientLamp->creatureCount;
                    H3RecruitMgr mgr(hero->army, false, ancientLamp->creatureType, creatureCount);
                    mgr.Run();
                    ancientLamp->creatureCount = creatureCount;
                }
            }
            else
            {
                INT16 creatureCount = ancientLamp->creatureCount;
                // AI::H3Hero::BuyCreaturesInRefugeeCamp
                FASTCALL_3(void, 0x5282F0, hero, ancientLamp->creatureType, &creatureCount);
                ancientLamp->creatureCount = creatureCount;
            }

            // set object as visited
            THISCALL_2(void, 0x4FC620, mapItem, hero->owner);

            if (ancientLamp->creatureCount <= 0)
            {
                // Delete the object
                THISCALL_4(void, 0x49DDE0, P_AdventureManager->Get(), mapItem, pos.Mixed(), true);
            }

            return true;
        }

        return false;
    }

    BOOL AncientLampExtender::SetAiMapItemWeight(
        H3MapItem* mapItem, H3Hero* hero, const H3Player* player,
        int& aiMapItemWeight, int* moveDistance, const H3Position pos) const noexcept
    {
        if (auto ancientLamp = GetFromMapItem(mapItem))
        {
            // AIHero_GetBuyingStacksValue_RefugeeCamp 0052ABF0
            // return AIHero_GetBuyingStacksValue_RefugeeCamp(hero, mapItem->SubType, mapItem->SetUp);
            aiMapItemWeight = FASTCALL_3(int, 0x52ABF0, hero, ancientLamp->creatureType, ancientLamp->creatureCount);

            return true;
        }

        return false;
    }

    int AncientLampExtender::AI_OnScouting_Value() const noexcept
    {
        return 10;
    }
}
