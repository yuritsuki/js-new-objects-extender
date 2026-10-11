#include "../pch.h"
#include "JetsamExtender.h"


namespace jetsam {
    JetsamExtender::JetsamExtender()
        : H3ActiveObject<H3MapItemJetsam>(
            "EraPlugin.JetsamExtender.js",
            extender::HOTA_PICKUPABLE_OBJECT_TYPE,
            JETSAM_OBJECT_SUBTYPE,
            -1,
            extender::ePageTreasure)
    {
    }

    JetsamExtender* JetsamExtender::instance = nullptr;
    JetsamExtender& JetsamExtender::Get()
    {
        if (!instance) instance = new JetsamExtender();
        return *instance;
    }

    BOOL JetsamExtender::InitNewGameMapItemSetup(H3MapItem* mapItem) const noexcept
    {

        if (const auto jetsam = GetFromMapItem(mapItem))
        {
            *jetsam = {};
            // Рандомим тип ресурса
            jetsam->state = rand() % 3; // от 0 до 3 включительно

            return true;
        }

        return false;
    }

    BOOL JetsamExtender::VisitMapItem(H3Hero* hero, H3MapItem* mapItem, const H3Position pos,
        const BOOL isHuman) const noexcept
    {
        if (const auto seaBarrel = GetFromMapItem(mapItem))
        {
            switch (mapItem->setup)
            {
            case 0:
                if (isHuman)
                {
                    FASTCALL_12(void, 0x4F6C00,
                        this->GetVisitedMessage().String(),
                        1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -777);
                }
                break;

            case 1:
                if (isHuman)
                {
                    FASTCALL_12(void, 0x4F6C00,
                        this->GetVisitingState1Message().String(),
                        1, -1, -1, eResource::ORE, 5, -1, 0, -1, 0, -1, -777);
                }
                // add resource
                THISCALL_3(void, 0x4E3870, hero, eResource::ORE, 5);
                break;

            case 2:
                if (isHuman)
                {
                    FASTCALL_12(void, 0x4F6C00,
                        this->GetVisitingState2Message().String(),
                        1, -1, -1, eResource::ORE, 5, eResource::GOLD, 200, -1, 0, -1, -777);
                }
                // add resource
                THISCALL_3(void, 0x4E3870, hero, eResource::ORE, 5);
                THISCALL_3(void, 0x4E3870, hero, eResource::GOLD, 200);
                break;

            case 3:
                if (isHuman)
                {
                    FASTCALL_12(void, 0x4F6C00,
                        this->GetVisitingState3Message().String(),
                        1, -1, -1, eResource::ORE, 10, eResource::GOLD, 500, -1, 0, -1, -777);
                }
                // add resource   
                THISCALL_3(void, 0x4E3870, hero, eResource::ORE, 10);
                THISCALL_3(void, 0x4E3870, hero, eResource::GOLD, 500);
                break;

            default:
                break;
            }

            // Delete the object
            THISCALL_4(void, 0x49DDE0, P_AdventureManager->Get(), mapItem, pos.Mixed(), 1);

            return true;
        }

        return false;
    }

    BOOL JetsamExtender::SetAiMapItemWeight(
        H3MapItem* mapItem, H3Hero* hero, const H3Player* player,
        int& aiMapItemWeight, int* moveDistance, const H3Position pos) const noexcept
    {
        if (auto objSetup = GetFromMapItem(mapItem))
        {
            aiMapItemWeight = (int)(player->aIPlayer.resourceImportance[2] * DoubleAt(0x63AC48)
                + player->aIPlayer.resourceImportance[6] * DoubleAt(0x6405A8));

            return true;
        }

        return false;
    }
}
