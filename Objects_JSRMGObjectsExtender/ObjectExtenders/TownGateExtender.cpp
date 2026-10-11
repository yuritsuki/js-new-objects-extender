#include "../pch.h"
#include "TownGateExtender.h"


namespace townGate
{
    LPCSTR g_TownGateName = nullptr;

    // id города для ИИ
    int g_townGateTargetTownId = -1;

    _LHF_(HookAfterHDModInit)
    {
        if (GetModuleHandleA("HD_WOG.dll"))
        {
            auto patch = globalPatcher->GetFirstPatchAt(0x41D6DB);
            if (patch)
            {
                patch->Undo();
            }
        }
        return EXEC_DEFAULT;
    }



    _LHF_(LoHook_0041d528)
    {
        if (!g_TownGateName)
            return EXEC_DEFAULT;

        for (int i = 0; i < 4; i++)
        {
            IntAt(c->ebp - 0x40 + i * 4) = 0;
        }
        //MemZero((void*)(c->ebp - 0x40), 0, 0x10u);   // 
        
        c->return_address = 0x41D538;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_0041da0b)
    {
        if (!g_TownGateName)
            return EXEC_DEFAULT;

        c->return_address = 0x41DA2B;
        return NO_EXEC_DEFAULT;
    }
    
    _LHF_(LoHook_005c1f57)
    {
        if (g_TownGateName)
            *(DWORD*)(c->ebp - 0x14) = (_ptr_)g_TownGateName;

        return EXEC_DEFAULT;
    }
    
    _LHF_(LoHook_005c2511)
    {
        if (!g_TownGateName)
            return EXEC_DEFAULT;

        DWORD* defObj = CDECL_1(DWORD*, 0x617492, 72); // __cdecl new(unsigned int Size2Allocate)
        H3DlgDef* def = THISCALL_12(H3DlgDef*, 0x4EA800, defObj, 117, 37, 0, 0, 3, "twngatem.def", 0, 0, 0, 0, 16);// DlgDef::Ctor

        if (def)
        {
            H3LoadedDef* loadedDef = def->GetDef();
            if (loadedDef)
            {
                def->SetWidth(loadedDef->widthDEF);
                def->SetHeight(loadedDef->heightDEF);
            }
        }
        c->eax = (int)def;

        c->return_address = 0x5C252E;
        return NO_EXEC_DEFAULT;
    }

    void TownGateExtender::CreatePatches()
    {
        static bool patchesCreated = false;
        if (patchesCreated) return;

        //0041D6DB снимаем лоухук ХД-мода
        _PI->WriteLoHook(0x4EEAF2, HookAfterHDModInit);
        


        // Зануляем стоимость очков передвижения
        _PI->WriteLoHook(0x41d528, LoHook_0041d528);

        // Зануляем стоимость очков маны
        _PI->WriteLoHook(0x41da0b, LoHook_0041da0b);

        // Подменяем имя заклинания на имя объекта
        _PI->WriteLoHook(0x5c1f57, LoHook_005c1f57);

        // Подменяем деф заклинания на деф объекта
        _PI->WriteLoHook(0x5c2511, LoHook_005c2511);

        patchesCreated = true;
    }

    TownGateExtender::TownGateExtender()
        : H3ActiveObject<H3MapItemTownGate>(
            "EraPlugin.TownGateExtender.js",
            extender::HOTA_OBJECT_TYPE,
            TOWN_GATE_OBJECT_SUBTYPE)
    {
        //CreatePatches();
    }

    TownGateExtender* TownGateExtender::instance = nullptr;
    TownGateExtender& TownGateExtender::Get()
    {
        if (!instance) instance = new TownGateExtender();
        return *instance;
    }

    // Хотовский вариант, требует хуки, но там hd_wog лоухук всё портит
    //BOOL TownGateExtender::VisitMapItem(
    //    H3Hero* hero, H3MapItem* mapItem, const H3Position pos,
    //    const BOOL isHuman) const noexcept
    //{
    //    // Посещается town gate
    //    if (GetFromMapItem(mapItem))
    //    {
    //        if (isHuman)
    //        {
    //            H3MouseManager* mouseMgr = P_MouseManager->Get();
    //            THISCALL_3(void, 0x50CEA0, mouseMgr, 0, 1); // H3MouseManager::SetCursor
    //            THISCALL_2(void, 0x50D7B0, mouseMgr, 1); // H3MouseManager::TurnMouseOn
    //            H3String objName = RMGObjectInfo::GetObjectName(mapItem);
    //            g_TownGateName = objName.String();

    //            H3AdventureManager* advMgr = P_AdventureManager->Get();
    //            THISCALL_2(void, 0x41D4E0, advMgr, 3);// H3AdventureManager::CastTownPortal

    //            g_TownGateName = nullptr;
    //        }
    //        else
    //        {
    //            H3Pathfinder* pfd = P_Pathfinder->Get();

    //            if (!pfd->arrowPath.IsEmpty())
    //            {
    //                auto** current = pfd->arrowPath.end();

    //                while (current != pfd->arrowPath.First())
    //                {
    //                    H3PathNode* pathNode = *--current;

    //                    if ((pathNode->access & 0x100) == 0)
    //                        continue;

    //                    if (!THISCALL_2(BOOL8, 0x42EC20, &pathNode->lastPoint, &pos))
    //                        continue;

    //                    THISCALL_7(void, 0x41DAB0, P_AdventureManager->Get(),
    //                        hero, pathNode->mixedPosition, 0, 0, 1, 0);
    //                }
    //            }
    //        }

    //        return true;
    //    }

    //    return false;
    //}

    BOOL TownGateExtender::VisitMapItem(
        H3Hero* hero, H3MapItem* mapItem, const H3Position pos,
        const BOOL isHuman) const noexcept
    {
        // Посещается town gate
        if (GetFromMapItem(mapItem))
        {
            if (isHuman)
            {
                // movecost free teleport
                //0041D531 + 3 = C8
                auto patch = _PI->WriteHexPatch(0x41D531 + 3, "00");

                // mana free teleport
                //0041DA0B -> 0041DA2B
                auto patch2 = _PI->WriteJmp(0x41DA0B, 0x41DA2B);

                // patch spell name
                // get new name
                H3String str = H3String::Format("{%s}", RMGObjectInfo::GetObjectName(mapItem)).String();
                LPCSTR objName = str.String();
                // store old name
                LPCSTR spellName = P_Spell[eSpell::TOWN_PORTAL].name;
                // patch
                P_Spell[eSpell::TOWN_PORTAL].name = objName;

                // patch spell def
                Era::RedirectFile("SpellScr.def:0_9.png", "data/pcx/twngatem.png");

                P_MouseManager->SetCursor(eAdvCursor::ARROWPOINTER, eCursor::ADVENTURE);
                P_MouseManager->TurnOn();
                // H3AdventureManager::CastTownPortal
                THISCALL_2(void, 0x41D4E0, P_AdventureManager->Get(), eSecSkillLevel::EXPERT);

                // restore old name
                P_Spell[eSpell::TOWN_PORTAL].name = spellName;
                // restore old def
                Era::RedirectFile("SpellScr.def:0_9.png", "");

                // undo patches
                patch->Destroy();
                patch2->Destroy();
            }
            else
            {
                if (g_townGateTargetTownId > -1)
                {
                    H3Town* targetTown = &P_Game->towns[g_townGateTargetTownId];
                    THISCALL_7(void, 0x41DAB0, P_AdventureManager->Get(), hero, H3Position(targetTown->x, targetTown->y, targetTown->z), 0, 0, 1, 0);
                    g_townGateTargetTownId = -1;
                }
            }

            return true;
        }

        return false;
    }

    BOOL TownGateExtender::SetAiMapItemWeight(
        H3MapItem* mapItem, H3Hero* hero, const H3Player* player,
        int& aiMapItemWeight, int* moveDistance, const H3Position pos) const noexcept
    {
        if (GetFromMapItem(mapItem))
        {
            H3Game* game = P_Game->Get();
            int playerTeamId = game->GetPlayerTeam(hero->owner);
            g_townGateTargetTownId = -1;

            for (size_t i = 0; i < limits::TOWNS_ON_MAP; i++)
            {
                H3Town* town = &game->towns[i];

                if (town->owner > -1 && town->visitingHero == -1 && playerTeamId == game->GetPlayerTeam(town->owner))
                {
                    // AI::H3Hero::CalculateMapPosWeight
                    int newAiTownVal = FASTCALL_3(int, 0x528520, hero, H3Position(town->x, town->y, town->z), moveDistance);

                    if (newAiTownVal > aiMapItemWeight)
                    {
                        aiMapItemWeight = newAiTownVal;
                        g_townGateTargetTownId = i;
                    }
                }
            }

            return true;
        }

        return false;
    }

    int TownGateExtender::AI_OnScouting_Value() const noexcept
    {
        return 200;
    }
}
