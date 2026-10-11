#include "../pch.h"
#include "VialOfManaExtender.h"


namespace vialOfMana
{
    VialOfManaExtender::VialOfManaExtender()
        : H3ActiveObject<H3MapItemVialOfMana>(
            "EraPlugin.VialOfManaExtender.js",
            extender::HOTA_PICKUPABLE_OBJECT_TYPE,
            VIAL_OF_MANA_OBJECT_SUBTYPE, 
            -1,
            extender::ePageTreasure)
    {
    }

    VialOfManaExtender* VialOfManaExtender::instance = nullptr;
    VialOfManaExtender& VialOfManaExtender::Get()
    {
        if (!instance) instance = new VialOfManaExtender();
        return *instance;
    }

    BOOL VialOfManaExtender::InitNewGameMapItemSetup(H3MapItem* mapItem) const noexcept
    {
        if (const auto obj = GetFromMapItem(mapItem))
        {
            *obj = {};
            // Рандомим количество маны
            obj->manaPts = H3Random::Rand(MIN_MANA, MAX_MANA);

            return true;
        }

        return false;
    }

    BOOL VialOfManaExtender::VisitMapItem(
        H3Hero* hero, H3MapItem* mapItem, const H3Position pos,
        const BOOL isHuman) const noexcept
    {
        if (const auto vial = GetFromMapItem(mapItem))
        {
            char knowledge = THISCALL_2(char, 0x5BE240, hero, 3);  // H3Hero::GetHeroPrimary
            int maxManaPts = (int)(THISCALL_1(float, 0x4E4B20, hero) * (double)(10 * knowledge));// H3Hero::GetIntelligencePower
            
            if (maxManaPts >= 9999)
            {
                maxManaPts = 9999;
            } 

            int currManaPts = hero->spellPoints;
            int manaPtsFromVial = 0;

            if (currManaPts < maxManaPts)
            {
                manaPtsFromVial = mapItem->setup;
                int missedManaPts = maxManaPts - currManaPts;
                if (mapItem->setup > missedManaPts)
                    manaPtsFromVial = missedManaPts + (manaPtsFromVial - missedManaPts) / 2;
            }
            else
            {
                manaPtsFromVial = mapItem->setup / 2;
            }

            if (isHuman)
            {
                FASTCALL_12(void, 0x4F6C00, this->GetVisitingMessage().String(),
                    1, -1, -1, 35, manaPtsFromVial, -1, 0, -1, 0, -1, -777);
            }
            hero->spellPoints += manaPtsFromVial;
            if (hero->spellPoints > 9999)
            {
                hero->spellPoints = 9999;
            }

            H3AdventureManager* advMgr = P_AdventureManager->Get();
            if (isHuman)
            {
                THISCALL_4(void, 0x4032E0, advMgr->dlg, -1, 1, 1); // H3AdventureMgrDlg::RedrawHeroesSlots
            }

            THISCALL_4(void, 0x415D40, advMgr, 1, 1, 1);  // H3AdventureManager::UpdateInfoPanel

            // Delete the object
            THISCALL_4(void, 0x49DDE0, advMgr, mapItem, pos.Mixed(), isHuman);

            return true;
        }

        return false;
    }

    BOOL VialOfManaExtender::SetAiMapItemWeight(
        H3MapItem* mapItem, H3Hero* hero, const H3Player* player,
        int& aiMapItemWeight, int* moveDistance, const H3Position pos) const noexcept
    {
        if (GetFromMapItem(mapItem))
        {
            // Validate the raw destination against the active map before querying a cell.
            const INT32 destX = hero->destX;
            const INT32 destY = hero->destY;
            const INT32 destZ = hero->destZ;
            const auto& setup = P_Game->mainSetup;
            if (destX < 0 || destY < 0 || destX >= setup.mapSize || destY >= setup.mapSize ||
                destZ < 0 || destZ > 1 || (destZ == 1 && !setup.hasUnderground) || !setup.mapitems)
            {
                aiMapItemWeight = hero->aiDoubleSpellPointsEffectiveness;
                return true;
            }
            
            if (!moveDistance || *moveDistance <= 300)
            {
                aiMapItemWeight = hero->aiDoubleSpellPointsEffectiveness;
                return true;
            }

            // H3MainSetup::GetMapItem
            H3MapItem* destinationCell = THISCALL_4(H3MapItem*, 0x4086D0,
                &P_Game->mainSetup, destX, destY, destZ);

            if (!destinationCell)
            {
                aiMapItemWeight = hero->aiDoubleSpellPointsEffectiveness;
                return true;
            }

            int objType = static_cast<int>(destinationCell->objectType);

            if (objType == eObject::MAGIC_SPRING || 
                objType == eObject::MAGIC_WELL || 
                (objType == extender::HOTA_PICKUPABLE_OBJECT_TYPE && 
                    destinationCell->objectSubtype == VIAL_OF_MANA_OBJECT_SUBTYPE))
            {
                aiMapItemWeight = hero->aiDoubleSpellPointsEffectiveness;
                return true;
            }
            else
            {
                aiMapItemWeight = 0;
                return true;
            }
        }

        return false;
    }

    int VialOfManaExtender::AI_OnScouting_Value() const noexcept
    {
        return 10;
    }
}
