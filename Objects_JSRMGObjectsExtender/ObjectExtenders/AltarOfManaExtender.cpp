#include "../pch.h"
#include "AltarOfManaExtender.h"


namespace altarOfMana
{
    int g_AltarOfMana = 0;

    _LHF_(LoHook_00560572)
    {
        if (g_AltarOfMana)
            c->eax = 3;

        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_0056146a)
    {
        if (!g_AltarOfMana)
            return EXEC_DEFAULT;

        c->esp -= 4;
        *(DWORD*)c->esp = (_ptr_)"altrmana.pcx";
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_00561e71)
    {
        if (!g_AltarOfMana)
            return EXEC_DEFAULT;

        c->return_address = 0x5621AF;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_00563507)
    {
        if (!g_AltarOfMana)
            return EXEC_DEFAULT;

        c->return_address = 0x563516;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_00564eec)
    {
        if (!g_AltarOfMana)
            return 1;

        c->edx = *(DWORD*)(c->esi + 120) + c->edi * *(DWORD*)(c->ebp + 12) / 10 - c->edi * *(DWORD*)(c->ebx + 312) / 10;
        c->edi = *(DWORD*)(c->ebp + 12);

        c->return_address = 0x564F38;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_005630af)
    {
        return g_AltarOfMana == 0;
    }

    _LHF_(LoHook_005630e1)
    {
        if (!g_AltarOfMana)
            return EXEC_DEFAULT;

        c->eax = static_cast<INT32>(c->edx) / 10;

        c->return_address = 0x5630F2;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_00564b5c)
    {
        if (!g_AltarOfMana)
            return 1;

        H3Hero* hero = (H3Hero*)c->ecx;
        int exp = c->eax;
        int knowledge = THISCALL_2(int, 0x5BE240, hero, 3); // H3Hero::GetHeroPrimary
        double maxMana = THISCALL_1(float, 0x4E4B20, hero) * (double)(10 * knowledge);// H3Hero::GetIntelligencePower
        int currMana = static_cast<int>(hero->spellPoints);
        int maxManaToFill = 4 * (int)maxMana;
        if (currMana <= maxManaToFill)
        {
            int v8 = currMana + exp;
            hero->spellPoints = currMana + exp;

            if (exp < 0 || v8 < 0 || v8 >= maxManaToFill)
                hero->spellPoints = maxManaToFill;
        }
        c->return_address = 0x564B66;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_005627c8)
    {
        if (!g_AltarOfMana)
            return EXEC_DEFAULT;

        H3Hero* hero = reinterpret_cast<H3Hero*>(c->ecx);

        const int knowledge = static_cast<unsigned char>(THISCALL_2(char, 0x5BE240, hero, 3));
        const float intelligenceMultiplier = THISCALL_1(float, 0x4E4B20, hero);

        const int calculatedMaxMana =
            static_cast<int>(
                intelligenceMultiplier * (10.0f * knowledge)
                );

        const int maxMana =
            calculatedMaxMana * 4 >= 9999
            ? 9999
            : calculatedMaxMana * 4;

        const int missingMana =
            maxMana >= hero->spellPoints
            ? maxMana - hero->spellPoints
            : 0;

        c->edx = missingMana;
        c->return_address = 0x5627DC;

        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_005602a1)
    {
        if (g_AltarOfMana)
        {
            LPCSTR dlgHint = EraJS::read(
                H3String::Format(
                    dlgHint_key,
                    extender::HOTA_UNREACHABLE_OBJECT_TYPE,
                    ALTAR_OF_MANA_OBJECT_SUBTYPE
                ).String());

            c->edx = (_ptr_)dlgHint;
        }
            
        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_0056036f)
    {
        if (g_AltarOfMana)
        {
            LPCSTR dlgHint = EraJS::read(
                H3String::Format(
                    dlgHint1_key,
                    extender::HOTA_UNREACHABLE_OBJECT_TYPE,
                    ALTAR_OF_MANA_OBJECT_SUBTYPE
                ).String());

            c->edx = (_ptr_)dlgHint;
        }

        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_00563210)
    {
        if (g_AltarOfMana)
        {
            LPCSTR dlgHint = EraJS::read(
                H3String::Format(
                    dlgHint2_key,
                    extender::HOTA_UNREACHABLE_OBJECT_TYPE,
                    ALTAR_OF_MANA_OBJECT_SUBTYPE
                ).String());

            c->eax = (_ptr_)dlgHint;
        }
        
        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_005622de)
    {
        if (g_AltarOfMana)
        {
            LPCSTR dlgHint = EraJS::read(
                H3String::Format(
                    dlgHint3_key,
                    extender::HOTA_UNREACHABLE_OBJECT_TYPE,
                    ALTAR_OF_MANA_OBJECT_SUBTYPE
                ).String());

            c->ecx = (_ptr_)dlgHint;
        }
            
        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_00561a40)
    {
        if (g_AltarOfMana)
        {
            LPCSTR dlgHint = EraJS::read(
                H3String::Format(
                    dlgHint3_key,
                    extender::HOTA_UNREACHABLE_OBJECT_TYPE,
                    ALTAR_OF_MANA_OBJECT_SUBTYPE
                ).String());

            c->edx = (_ptr_)dlgHint;
        }

        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_00564948)
    {
        if (g_AltarOfMana)
        {
            LPCSTR dlgHint = EraJS::read(
                H3String::Format(
                    dlgHint4_key,
                    extender::HOTA_UNREACHABLE_OBJECT_TYPE,
                    ALTAR_OF_MANA_OBJECT_SUBTYPE
                ).String());

            c->ecx = (_ptr_)dlgHint;
        }

        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_PatchObjectName)
    {
        if (g_AltarOfMana)
        {
            LPCSTR name = RMGObjectInfo::GetObjectName(
                extender::HOTA_UNREACHABLE_OBJECT_TYPE,
                ALTAR_OF_MANA_OBJECT_SUBTYPE);

            c->eax = (_ptr_)name;
        }

        return EXEC_DEFAULT;
    }







    //
    _LHF_(LoHook_005f0002)
    {
        H3DlgPcx* pcx = (H3DlgPcx*)c->eax;
        int playerId = THISCALL_1(int, 0x4CE6E0, P_Game->Get()); // H3Game::GetPlayerID
        THISCALL_2(void, 0x4501D0, pcx, playerId); // DlgStaticPcx8_Colorize

        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_005d332b)
    {
        THISCALL_3(void, 0x5FF800, c->ebp - 0x128, -1, -1); // H3BaseDlg::SetPos
        
        return EXEC_DEFAULT;
    }




    void AltarOfManaExtender::CreatePatches()
    {
        static bool patchesCreated = false;
        if (patchesCreated) return;

        // H3SacrificeDlg::Ctor - переключаем режим на жертвование существ
        _PI->WriteLoHook(0x560572, LoHook_00560572);

        // H3SacrificeDlg::SacrificeCreaturesCtor
        // Подменяем задник
        _PI->WriteLoHook(0x56146a, LoHook_0056146a);
        // Перепрыгиваем через создание кнопок артефактов
        _PI->WriteLoHook(0x561e71, LoHook_00561e71);

        // H3SacrifaceDlg::CreaturesView::Prepare - перепрыгиваем виртуальную функцию?
        _PI->WriteLoHook(0x563507, LoHook_00563507);

        // Хз че за функция - че-то с опытом
        _PI->WriteLoHook(0x564eec, LoHook_00564eec);

        // Хз че за функция
        // Скип
        _PI->WriteLoHook(0x5630af, LoHook_005630af);
        // Количество даваемой маны = опыт / 10
        _PI->WriteLoHook(0x5630e1, LoHook_005630e1);
        
        // Хз че за функция - расчет даваемой маны, заменяем опыт на ману
        _PI->WriteLoHook(0x564b5c, LoHook_00564b5c);

        // Хз че за функция - рассчитываем ману и пополняем
        _PI->WriteLoHook(0x5627c8, LoHook_005627c8);

        // Подменяем элементы диалога
        // H3SacrificeDlg::Ctor
        _PI->WriteLoHook(0x5602a1, LoHook_005602a1);
        _PI->WriteLoHook(0x56036f, LoHook_0056036f);
        // Хз че за функция
        _PI->WriteLoHook(0x563210, LoHook_00563210);
        // Подменяем одну строку на другую
        _PI->WriteDword(0x5622da, DwordAt(0x5622DA) - 48);
        _PI->WriteLoHook(0x5622de, LoHook_005622de);
        // H3SacrificeDlg::SacrificeCreaturesCtor
        _PI->WriteLoHook(0x561a40, LoHook_00561a40);
        _PI->WriteLoHook(0x5618f0, LoHook_00561a40);
        // Хз че за функция
        _PI->WriteLoHook(0x564948, LoHook_00564948);

        // Подменяем имя объекта
        _PI->WriteLoHook(0x561671, LoHook_PatchObjectName);

        //// H3UniversityDlg::Ctor - ???
        //_PI->WriteLoHook(0x5f0002, LoHook_005f0002);
        //// H3TownManager::VisitUniversity
        //_PI->WriteLoHook(0x5d332b, LoHook_005d332b);
        //// H3UniversityDlg::Ctor
        //_PI->WriteDword(0x5f039a, "Box64x30.pcx");
        //_PI->WriteDword(0x5f047e, "Box64x30.pcx");
        //_PI->WriteByte(0x5f03a1, 32);
        //_PI->WriteByte(0x5f0485, 32);

        patchesCreated = true;
    }

    AltarOfManaExtender::AltarOfManaExtender()
        : H3ActiveObject<H3MapItemAltarOfMana>(
            "EraPlugin.AltarOfManaExtender.js",
            extender::HOTA_UNREACHABLE_OBJECT_TYPE,
            ALTAR_OF_MANA_OBJECT_SUBTYPE)
    {
        CreatePatches();
    }

    AltarOfManaExtender* AltarOfManaExtender::instance = nullptr;
    AltarOfManaExtender& AltarOfManaExtender::Get()
    {
        if (!instance) instance = new AltarOfManaExtender();
        return *instance;
    }

    BOOL AltarOfManaExtender::VisitMapItem(H3Hero* hero, H3MapItem* mapItem, const H3Position pos,
        const BOOL isHuman) const noexcept
    {
        if (GetFromMapItem(mapItem))
        {
            if (isHuman)
            {
                H3MouseManager* mouseMgr = P_MouseManager->Get();
                THISCALL_3(void, 0x50CEA0, mouseMgr, 0, 1); // H3MouseManager::SetCursor
                THISCALL_2(void, 0x50D7B0, mouseMgr, 1); // H3MouseManager::TurnMouseOn
                g_AltarOfMana = 1;
                int playerId = THISCALL_1(int, 0x4CE6E0, P_Game->Get()); // H3Game::GetPlayerID
                char dlg[572]; // 0x23C в иде и на стеке
                THISCALL_3(void, 0x560060, &dlg, hero, playerId);    // H3SacrificeDlg::Ctor
                THISCALL_3(void, 0x5FF800, &dlg, -1, -1); // H3BaseDlg::SetPos
                THISCALL_2(void, 0x565640, &dlg, 0); // H3SacrificeDlg::Run
                THISCALL_3(void, 0x40EBE0, P_AdventureManager->Get(), 0, 0); // AdvMan_40EBE0
                THISCALL_1(void, 0x562630, &dlg); // H3SacrificeDlg::Dtor
                g_AltarOfMana = 0;
            }

            return true;
        }

        return false;
    }

    //int AltarOfManaExtender::AI_OnScouting_Value() const noexcept
    //{
    //    return 1;
    //}
}
