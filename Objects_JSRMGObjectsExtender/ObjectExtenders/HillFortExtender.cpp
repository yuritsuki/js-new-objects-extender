#include "../pch.h"
#include "HillFortExtender.h"

namespace hillFort {
    H3MapItem* currentHillFort = nullptr;

    _LHF_(HillFort_AIMapWeight)
    {
        if (currentHillFort && currentHillFort->objectSubtype == HILL_FORT_OBJECT_SUBTYPE)
        {
            //Pattern used: visit HILL_FORT 005292EC
            int monType = c->esi;
            auto* crInfo = &P_CreatureInformation->Get()[monType];

            if (crInfo->level > 4)
            {
                c->return_address = 0x05293C7;
            }
            else
            {
                float goldUpgCost = static_cast<float>(IntAt(c->ebp - 0x1C));
                c->eax = static_cast<int>(goldUpgCost * 2.0);

                c->return_address = 0x0529368;
            }
            c->esi = (int)crInfo;

            return NO_EXEC_DEFAULT;
        }

        return EXEC_DEFAULT;
    }

    // Set the upper hint "Upgraded" for 6+lvl creatures
    _LHF_(HillFort_OnSetHintAsUpgraded)
    {
        int heroId = P_ActivePlayer->Get()->currentHero;
        H3Hero* hero = P_Game->GetHero(heroId);
        auto mapItem = P_AdventureManager->Get()->GetMapItem(hero->x, hero->y, hero->z);

        if (mapItem->objectSubtype != HILL_FORT_OBJECT_SUBTYPE)
        {
            return EXEC_DEFAULT;
        }

        int monLvl = IntAt(c->ebx + 0x4);

        if (monLvl > 4)
        {
            c->return_address = 0x04E81B4;
            return NO_EXEC_DEFAULT;
        }

        return EXEC_DEFAULT;
    }

    // Skip the upper hint "Free" for 1lvl creatures
    _LHF_(HillFort_JumpOverFreeUpgrade)
    {
        int heroId = P_ActivePlayer->Get()->currentHero;
        H3Hero* hero = P_Game->GetHero(heroId);
        auto mapItem = P_AdventureManager->Get()->GetMapItem(hero->x, hero->y, hero->z);

        if (mapItem->objectSubtype != HILL_FORT_OBJECT_SUBTYPE)
        {
            return EXEC_DEFAULT;
        }
        c->return_address = 0x04E80E0;

        return NO_EXEC_DEFAULT;
    }

    // Recalc upgrade cost
    _LHF_(HillFort_PatchUpgradeCost)
    {
        int heroId = P_ActivePlayer->Get()->currentHero;
        H3Hero* hero = P_Game->GetHero(heroId);
        auto mapItem = P_AdventureManager->Get()->GetMapItem(hero->x, hero->y, hero->z);

        if (mapItem->objectSubtype != HILL_FORT_OBJECT_SUBTYPE)
        {
            return EXEC_DEFAULT;
        }
        float goldForGrade = FloatAt(c->ebp - 0x14);
        c->eax = static_cast<int>(goldForGrade * 2.0);

        return EXEC_DEFAULT;
    }

    // Install yellow arrow (is already upgraded status) for 6+lvl creatures
    _LHF_(HillFort_JumpOverAvailableUpgrade)
    {
        int heroId = P_ActivePlayer->Get()->currentHero;
        H3Hero* hero = P_Game->GetHero(heroId);
        auto mapItem = P_AdventureManager->Get()->GetMapItem(hero->x, hero->y, hero->z);

        if (mapItem->objectSubtype != HILL_FORT_OBJECT_SUBTYPE)
        {
            return EXEC_DEFAULT;
        }
        int monLvl = IntAt(c->ebx + 0x4);

        if (monLvl > 4)
        {
            c->return_address = 0x04E8309;
            return NO_EXEC_DEFAULT;
        }

        return EXEC_DEFAULT;
    }

    _LHF_(HillFort_AIVisit)
    {
        int heroId = P_ActivePlayer->Get()->currentHero;
        H3Hero* hero = P_Game->GetHero(heroId);
        auto mapItem = P_AdventureManager->Get()->GetMapItem(hero->x, hero->y, hero->z);

        if (mapItem->objectSubtype != HILL_FORT_OBJECT_SUBTYPE)
        {
            return EXEC_DEFAULT;
        }

        int monType = c->esi;

        if (monType == -1)
        {
            return EXEC_DEFAULT;
        }

        auto crInfo = P_CreatureInformation->Get()[monType];

        if (crInfo.level > 4)
        {
            c->return_address = 0x0528193;

            return NO_EXEC_DEFAULT;
        }

        return EXEC_DEFAULT;
    }

    void HillFortExtender::CreatePatches()
    {
        static bool hookInstalled = false;
        if (hookInstalled) return;

        _PI->WriteLoHook(0x052934F, HillFort_AIMapWeight);
        _PI->WriteLoHook(0x04E804D, HillFort_OnSetHintAsUpgraded);
        _PI->WriteLoHook(0x04E808B, HillFort_JumpOverFreeUpgrade);
        _PI->WriteLoHook(0x04E8139, HillFort_PatchUpgradeCost);
        _PI->WriteLoHook(0x04E8312, HillFort_JumpOverAvailableUpgrade);
        _PI->WriteLoHook(0x05280AB, HillFort_AIVisit);

        hookInstalled = true;
    }

    // const H3MapItem* HillFortExtender::currentShrineHint = nullptr;
    HillFortExtender::HillFortExtender()
        : H3ActiveObject<H3MapHillFort>(
            "EraPlugin.HillFortExtender.js",
            eObject::HILL_FORT,
            HILL_FORT_OBJECT_SUBTYPE)
    {
        CreatePatches();
    }

    HillFortExtender* HillFortExtender::instance = nullptr;
    HillFortExtender& HillFortExtender::Get()
    {
        if (!instance) instance = new HillFortExtender();
        return *instance;
    }

    BOOL HillFortExtender::SetAiMapItemWeight(
        H3MapItem *mapItem, H3Hero *hero, const H3Player *player,
        int &aiMapItemWeight, int* moveDistance, const H3Position pos) const noexcept
    {
        currentHillFort = mapItem;

        return false;
    }
}
