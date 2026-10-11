#include "../pch.h"
#include "SeaBarrelExtender.h"


namespace seaBarrel
{
    SeaBarrelExtender::SeaBarrelExtender()
        : H3ActiveObject<H3MapItemSeaBarrel>(
            "EraPlugin.SeaBarrelExtender.js",
            extender::HOTA_PICKUPABLE_OBJECT_TYPE,
            SEA_BARREL_OBJECT_SUBTYPE,
            -1,
            extender::ePageTreasure)
    {
    }

    SeaBarrelExtender* SeaBarrelExtender::instance = nullptr;
    SeaBarrelExtender& SeaBarrelExtender::Get()
    {
        if (!instance) instance = new SeaBarrelExtender();
        return *instance;
    }

    BOOL SeaBarrelExtender::InitNewGameMapItemSetup(H3MapItem* mapItem) const noexcept
    {
        if (const auto seaBarrel = GetFromMapItem(mapItem))
        {
            *seaBarrel = {};
            // Рандомим тип ресурса
            seaBarrel->resType = static_cast<INT16>(rand() % 4 + 1); // от 0 до 4 включительно
            // Пустой результат и ртуть не сдвигаем.
            // Значения 2, 3, 4 превращаются в 3, 4, 5.
            if (seaBarrel->resType > 1)
            {
                seaBarrel->resType += 1;
            }
            // Для непустой бочки задаём от 3 до 6 ресурсов.
            if (seaBarrel->resType > 0)
            {
                seaBarrel->resQty = static_cast<INT16>(3 + rand() % 4); // от 3 до 6 включительно
            }

            return true;
        }

        return false;
    }

    BOOL SeaBarrelExtender::VisitMapItem(H3Hero* hero, H3MapItem* mapItem, const H3Position pos,
        const BOOL isHuman) const noexcept
    {
        if (const auto seaBarrel = GetFromMapItem(mapItem))
        {
            int resType = seaBarrel->resType;
            int resQty = seaBarrel->resQty;

            if (resQty)
            {
                if (isHuman)
                {
                    FASTCALL_12(void, 0x4F6C00, this->GetVisitingMessage().String(),
                        1, -1, -1, resType, resQty, -1, 0, -1, 0, -1, -777);
                }
                // add resource
                THISCALL_3(void, 0x4E3870, hero, resType, resQty);
            }
            else if (isHuman)
            {
                FASTCALL_12(void, 0x4F6C00, this->GetVisitedMessage().String(),
                    1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -777);
            }

            // Delete the object
            THISCALL_4(void, 0x49DDE0, P_AdventureManager->Get(), mapItem, pos.Mixed(), isHuman);

            return true;
        }

        return false;
    }

    BOOL SeaBarrelExtender::SetAiMapItemWeight(
        H3MapItem* mapItem, H3Hero* hero, const H3Player* player,
        int& aiMapItemWeight, int* moveDistance, const H3Position pos) const noexcept
    {
        if (GetFromMapItem(mapItem))
        {
            aiMapItemWeight = (int)(
                (player->aIPlayer.resourceImportance[3]
                    + player->aIPlayer.resourceImportance[1]
                    + player->aIPlayer.resourceImportance[4]
                    + player->aIPlayer.resourceImportance[5])
                * 0.9);

            return true;
        }

        return false;
    }
}
