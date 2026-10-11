#include "../pch.h"
#include "SeafaringAcademyExtender.h"


namespace seafaringAcademy
{
    struct H3UniversityDlg : H3NetworkDlg
    {
        H3Hero* hero;
        H3DlgItem* _f_64;
        H3DlgItem* _f_68[4];
        int _f_74[15];
        int _f_B4[9];
        H3Vector<DWORD> vectorItemdCopy;
        H3Vector<DWORD> vectorItemdCopyA;
        int sskils[4];
    };

    H3UniversityDlg* g_UniversityDlg = nullptr;
    int g_SeafaringAcademyFlag = 0;
    H3Hero* g_HeroInUniversity = nullptr;
    // Глобалка для имен скиллов в главном окошке
    H3DlgText* g_SkillTexts[3];
    // Глобалка для имени скилла в окошке обучения
    H3DlgText* g_DlgText = nullptr;


    _LHF_(LoHook_005efa86)
    {
        if (*(BYTE*)(c->ebp + 0x10) != 2) // source = 2 - enum академии
            return 1;

        int id = c->ebx;
        DWORD* pcxRaw = CDECL_1(DWORD*, 0x617492, 52); // __cdecl new(unsigned int Size2Allocate)
        H3DlgPcx* dlgPcx = THISCALL_8(H3DlgPcx*, 0x44FFA0, pcxRaw, 179, 33, 104, 91, id, "acadbldg.pcx", 2048);// DlgPcx8::Ctor
        int nextId = ++c->ebx;
        c->eax = (_ptr_)dlgPcx;
        *(DWORD*)(c->ebp - 16) = nextId;

        c->return_address = 0x5EFB04;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_005efa30)
    {
        if (!g_SeafaringAcademyFlag)
            return EXEC_DEFAULT;

        LPCSTR name = RMGObjectInfo::GetObjectName(
            extender::HOTA_UNREACHABLE_OBJECT_TYPE, 
            SEAFARING_ACADEMY_OBJECT_SUBTYPE);

        c->edx = (_ptr_)name;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_005efb56)
    {
        if (!g_SeafaringAcademyFlag)
            return EXEC_DEFAULT;

        LPCSTR dlgHint = EraJS::read(
            H3String::Format(
                dlgHint_key,
                extender::HOTA_UNREACHABLE_OBJECT_TYPE,
                SEAFARING_ACADEMY_OBJECT_SUBTYPE
            ).String());

        c->edx = (_ptr_)dlgHint;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_005f0ede)
    {
        if (!g_SeafaringAcademyFlag)
            return EXEC_DEFAULT;

        LPCSTR dlgHint = EraJS::read(
            H3String::Format(
                dlgHint1_key,
                extender::HOTA_UNREACHABLE_OBJECT_TYPE,
                SEAFARING_ACADEMY_OBJECT_SUBTYPE
            ).String());

        c->eax = (_ptr_)dlgHint;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_005f0bbb)
    {
        if (!g_SeafaringAcademyFlag)
            return EXEC_DEFAULT;

        LPCSTR dlgHint = EraJS::read(
            H3String::Format(
                dlgHint1_key,
                extender::HOTA_UNREACHABLE_OBJECT_TYPE,
                SEAFARING_ACADEMY_OBJECT_SUBTYPE
            ).String());

        c->esi = (_ptr_)dlgHint;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_005f0fe4)
    {
        if (!g_SeafaringAcademyFlag)
            return EXEC_DEFAULT;

        LPCSTR dlgHint = EraJS::read(
            H3String::Format(
                dlgHint2_key,
                extender::HOTA_UNREACHABLE_OBJECT_TYPE,
                SEAFARING_ACADEMY_OBJECT_SUBTYPE
            ).String());

        c->eax = (_ptr_)dlgHint;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_005f1068)
    {
        if (!g_SeafaringAcademyFlag)
            return EXEC_DEFAULT;

        LPCSTR dlgHint = EraJS::read(
            H3String::Format(
                dlgHint3_key,
                extender::HOTA_UNREACHABLE_OBJECT_TYPE,
                SEAFARING_ACADEMY_OBJECT_SUBTYPE
            ).String());

        c->eax = (_ptr_)dlgHint;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_005eff83)
    {
        if (!g_SeafaringAcademyFlag)
            return EXEC_DEFAULT;

        LPCSTR dlgHint = EraJS::read(
            H3String::Format(
                dlgHint4_key,
                extender::HOTA_UNREACHABLE_OBJECT_TYPE,
                SEAFARING_ACADEMY_OBJECT_SUBTYPE
            ).String());

        c->eax = (_ptr_)dlgHint;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_005f12cc)
    {
        if (!g_SeafaringAcademyFlag)
            return EXEC_DEFAULT;

        LPCSTR dlgHint = EraJS::read(
            H3String::Format(
                dlgHint4_key,
                extender::HOTA_UNREACHABLE_OBJECT_TYPE,
                SEAFARING_ACADEMY_OBJECT_SUBTYPE
            ).String());

        c->ecx = (_ptr_)dlgHint;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_005efe87)
    {
        return !g_SeafaringAcademyFlag || c->eax < 24;
    }

    _LHF_(LoHook_005f0e53)
    {
        if (!g_SeafaringAcademyFlag)
            return EXEC_DEFAULT;

        c->ebx = 3;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_005efc01)
    {
        if (g_SeafaringAcademyFlag)
            c->edx += 51;

        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_005efcaf)
    {
        if (g_SeafaringAcademyFlag)
            c->ecx += 51;

        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_005ef98e)
    {
        if (!g_SeafaringAcademyFlag)
            return 1;

        c->esp -= 4;
        *(DWORD*)c->esp = (_ptr_)"ACADEMYW.pcx";

        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_005f0ba0)
    {
        if (!g_SeafaringAcademyFlag)
            return EXEC_DEFAULT;

        c->return_address = *(BYTE*)(c->ecx + c->eax + 0xC9) > 2u ? 0x5F0BAB : 0x5F0C0C;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_005f0c81)
    {
        if (!g_SeafaringAcademyFlag || !*(BYTE*)(c->ecx + c->eax + 201))
            return EXEC_DEFAULT;

        c->return_address = 0x5F0CEA;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_005f0d04)
    {
        if (!g_SeafaringAcademyFlag)
            return EXEC_DEFAULT;

        int heroSkillLevel =
            static_cast<int>(
                g_HeroInUniversity->secSkill[*(DWORD*)(c->ebx + 0x10)]
                );

        const unsigned int lookupIndex =
            heroSkillLevel <= 2 ? heroSkillLevel : 2;

        c->edx = reinterpret_cast<const DWORD*>(0x6A75D8)[lookupIndex];

        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_005f0d82)
    {
        if (!g_SeafaringAcademyFlag)
            return EXEC_DEFAULT;

        int mastery =
            static_cast<int>(
                g_HeroInUniversity->secSkill[*(DWORD*)(c->ebx + 0x10)]
                );

        c->edx += mastery > 2 ? 2 : mastery;

        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_005efe49)
    {
        if (g_SeafaringAcademyFlag)
            g_SkillTexts[(c->ebx - (_ptr_)g_UniversityDlg - 120) / 20u] = (H3DlgText*)c->eax; // c->eax = H3DlgText
        
        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_005f01ad)
    {
        if (g_SeafaringAcademyFlag)
            g_DlgText = (H3DlgText*)c->eax;

        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_005f0d88)
    {
        if (!g_SeafaringAcademyFlag)
            return EXEC_DEFAULT;

        const unsigned int currentMastery =
            static_cast<unsigned char>(
                g_HeroInUniversity->secSkill[*(DWORD*)(c->ebx + 0x10)]
                );

        // Для выбора текста достаточно трёх состояний:
        // 0 -> обучение Basic
        // 1 -> улучшение до Advanced
        // 2/3 -> улучшение до Expert / дальнейшее состояние
        const unsigned int textIndex =
            currentMastery > 2 ? 2 : currentMastery;

        unsigned int controlIndex = static_cast<unsigned int>(
            (c->ebx - (_ptr_)g_UniversityDlg - 116) / 20u);

        H3DlgText* textItem =
            controlIndex < 3
            ? g_SkillTexts[controlIndex]
            : g_DlgText;

        const auto* skillLevelTexts =
            reinterpret_cast<const LPCSTR*>(0x6A75D8);

        textItem->SetText(skillLevelTexts[textIndex]);

        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_005ef891)
    {
        if (g_SeafaringAcademyFlag)
        {
            const int skillId = c->eax >> 4;

            const unsigned int mastery =
                static_cast<unsigned char>(
                    g_HeroInUniversity->secSkill[skillId]
                    );

            const unsigned int cappedMastery =
                mastery > 2 ? 2 : mastery;

            c->ecx += 4 * cappedMastery;
        }

        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_005f0eca)
    {
        if (!g_SeafaringAcademyFlag)
            return 1;

        c->return_address = *(BYTE*)(c->ebx + c->eax + 201) > 2u ? 0x5F0ED5 : 0x5F0F24;
        return 0;
    }

    _LHF_(LoHook_005f0f4a)
    {
        if (!g_SeafaringAcademyFlag || !*(BYTE*)(c->ebx + c->eax + 201))
            return 1;

        c->return_address = 0x5F0F64;
        return 0;
    }

    _LHF_(LoHook_005f0fed)
    {
        if (!g_SeafaringAcademyFlag)
            return EXEC_DEFAULT;

        const int skillId = c->ebx;

        const unsigned int mastery =
            static_cast<unsigned char>(
                g_HeroInUniversity->secSkill[skillId]
                );

        const unsigned int cappedMastery = mastery > 2 ? 2 : mastery;
        c->ecx = reinterpret_cast<const DWORD*>(0x6A75D8)[cappedMastery];

        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_005f1390)
    {
        if (!g_SeafaringAcademyFlag)
            return EXEC_DEFAULT;

        const int skillId = c->esi;

        const unsigned int mastery =
            static_cast<unsigned char>(
                g_HeroInUniversity->secSkill[skillId]
                );

        const unsigned int cappedMastery = mastery > 2 ? 2 : mastery;
        c->eax = reinterpret_cast<const DWORD*>(0x6A75D8)[cappedMastery];

        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_005f13f6)
    {
        if (g_SeafaringAcademyFlag)
        {
            const int skillId = c->esi;

            const unsigned int mastery =
                static_cast<unsigned char>(
                    g_HeroInUniversity->secSkill[skillId]
                    );

            const unsigned int cappedMastery = mastery > 2 ? 2 : mastery;
            c->edx += cappedMastery;
        }

        return EXEC_DEFAULT;
    }

    void SeafaringAcademyExtender::CreatePatches()
    {
        static bool patchesCreated = false;
        if (patchesCreated) return;

        // Подменяем картинку сверху от скиллов
        _PI->WriteLoHook(0x5efa86, LoHook_005efa86);
        // Подменяем имя объекта
        _PI->WriteLoHook(0x5efa30, LoHook_005efa30);
        // Подменяем описание в главном окне диалога
        _PI->WriteLoHook(0x5efb56, LoHook_005efb56);
        // Подменяем хинт, если навык уже выучен
        _PI->WriteLoHook(0x5f0ede, LoHook_005f0ede);
        _PI->WriteLoHook(0x5f0bbb, LoHook_005f0bbb);
        // Подменяем описание в окне обучения
        _PI->WriteLoHook(0x5f0fe4, LoHook_005f0fe4);
        // Подменяем хинт про оплату за обучение
        _PI->WriteLoHook(0x5f1068, LoHook_005f1068);
        // Подменяем хинт про выход из диалога
        _PI->WriteLoHook(0x5eff83, LoHook_005eff83);
        _PI->WriteLoHook(0x5f12cc, LoHook_005f12cc);
        // ???
        _PI->WriteLoHook(0x5efe87, LoHook_005efe87);
        // Изменяем счетчик скиллов
        _PI->WriteLoHook(0x5f0e53, LoHook_005f0e53);
        _PI->WriteLoHook(0x5f1278, LoHook_005f0e53);
        _PI->WriteLoHook(0x5f14f5, LoHook_005f0e53);
        // Подменяем координату Х дефов скиллов
        _PI->WriteLoHook(0x5efc01, LoHook_005efc01);
        // Патчим sizeX в конструкторе дефа
        _PI->WriteLoHook(0x5efcaf, LoHook_005efcaf);
        _PI->WriteLoHook(0x5efd2d, LoHook_005efcaf);
        // Подменяем задник диалога
        _PI->WriteLoHook(0x5ef98e, LoHook_005ef98e);
        // Перед "Ваш герой уже владеет этим навыком" - какой-то прыжок
        _PI->WriteLoHook(0x5f0ba0, LoHook_005f0ba0);
        _PI->WriteLoHook(0x5f0c81, LoHook_005f0c81);
        // ???
        _PI->WriteLoHook(0x5f0d04, LoHook_005f0d04);
        _PI->WriteLoHook(0x5f0d82, LoHook_005f0d82);
        // Стэшим в глобалки тексты скиллов
        _PI->WriteLoHook(0x5efe49, LoHook_005efe49);
        _PI->WriteLoHook(0x5f01ad, LoHook_005f01ad);
        // Подменяем установку имени навыков
        _PI->WriteLoHook(0x5f0d88, LoHook_005f0d88);
        //
        _PI->WriteLoHook(0x5ef891, LoHook_005ef891);
        //
        _PI->WriteLoHook(0x5f0eca, LoHook_005f0eca);
        //
        _PI->WriteLoHook(0x5f0f4a, LoHook_005f0f4a);
        //
        _PI->WriteLoHook(0x5f0fed, LoHook_005f0fed);
        //
        _PI->WriteLoHook(0x5f1390, LoHook_005f1390);
        //
        _PI->WriteLoHook(0x5f13f6, LoHook_005f13f6);

        patchesCreated = true;
    }

    SeafaringAcademyExtender::SeafaringAcademyExtender()
        : H3ActiveObject<H3MapItemSeafaringAcademy>(
            "EraPlugin.SeafaringAcademyExtender.js",
            extender::HOTA_UNREACHABLE_OBJECT_TYPE,
            SEAFARING_ACADEMY_OBJECT_SUBTYPE)
    {
        CreatePatches();
    }

    SeafaringAcademyExtender* SeafaringAcademyExtender::instance = nullptr;
    SeafaringAcademyExtender& SeafaringAcademyExtender::Get()
    {
        if (!instance) instance = new SeafaringAcademyExtender();
        return *instance;
    }

    BOOL SeafaringAcademyExtender::InitNewGameMapItemSetup(H3MapItem* mapItem) const noexcept
    {
        if (auto setup = GetFromMapItem(mapItem))
        {
            // У хотовцев 2 режима, зависящих от первого бита в setup, я оставил один
            constexpr int SKILL_COUNT = h3::limits::SECONDARY_SKILLS;

            std::vector<INT8> availableSkills;
            availableSkills.reserve(SKILL_COUNT);

            for (int skillId = 0; skillId < SKILL_COUNT; ++skillId)
            {
                const bool forbidden =
                    skillId == eSecondary::NAVIGATION || // обязательный скилл
                    skillId == eSecondary::NECROMANCY || // не может быть никогда
                    P_Game->bannedSkills[skillId] != 0; // скилл не запрещен на карте

                if (!forbidden)
                    availableSkills.push_back(static_cast<INT8>(skillId));
            }

            if (availableSkills.size() < 2)
            {
                setup->skillId1 = -1;
                setup->skillId2 = -1;
                setup->visited = false;
                setup->u03 = 0;
                return true;
            }

            const auto takeRandomSkill = [&]() -> INT8
                {
                    const int index = H3Random::Rand(
                        0,
                        static_cast<int>(availableSkills.size()) - 1
                    );

                    const INT8 skillId = availableSkills[index];

                    // Сохраняем порядок оставшихся ID, как в H3IndexVector.
                    availableSkills.erase(availableSkills.begin() + index);
                    return skillId;
                };

            setup->skillId1 = takeRandomSkill();
            setup->skillId2 = takeRandomSkill();
            setup->visited = false;
            setup->u03 = 0;

            return true;
        }

        return false;
    }

    BOOL SeafaringAcademyExtender::SetHintInH3TextBuffer(
        H3MapItem* mapItem, const H3Hero* hero, const int playerId,
        BOOL isRightClick) const noexcept
    {
        if (auto setup = GetFromMapItem(mapItem))
        {
            H3String objName = RMGObjectInfo::GetObjectName(mapItem);

            BOOL playerVisited = (1 << playerId) & setup->visited;

            if (!playerVisited)
            {
                LPCSTR info = EraJS::read(H3String::Format(objectInfo_key, objectType, objectSubtype).String());
                sprintf_s(h3_TextBuffer, 0x300u, isRightClick ? "%s\n%s" : "%s %s", objName.String(), info);
            }
            
            else
            {
                char buffer[768];
                LPCSTR format = EraJS::read(H3String::Format(extraObjectInfo_key, objectType, objectSubtype).String());
                sprintf_s(buffer, 0x300u, format,
                    P_SecondarySkillInfo[setup->skillId1].name,
                    P_SecondarySkillInfo[setup->skillId2].name,
                    P_SecondarySkillInfo[eSecondary::NAVIGATION].name);
                
                sprintf_s(h3_TextBuffer, 0x300u, isRightClick ? "%s\n%s" : "%s %s", objName.String(), buffer);
            }

            return true;
        }

        return false;
    }

    BOOL SeafaringAcademyExtender::VisitMapItem(H3Hero* hero, H3MapItem* mapItem, const H3Position pos,
        const BOOL isHuman) const noexcept
    {
        if (auto academy = GetFromMapItem(mapItem))
        {
            if (!isHuman)
            {
            //    P_MarketHero = hero;
            //    P_ArtifactMerchant = &P_Game->artifactMerchant;
            //    P_ActivePlayerMarkets = 10;  // markets amount owned by player
            //    P_MarketType = eMarketType::TRADE_RESOURCES;
            //    P_MarketBuilding = static_cast<eMarketBuilding>(WARLOCKS_LAB_MARKET_BUILDING);

            //    // Run dlg
            //    CDECL_0(void, 0x5EA530);
            }

            else
            {
                H3MouseManager* mouseMgr = P_MouseManager->Get();
                THISCALL_3(void, 0x50CEA0, mouseMgr, 0, 1); // H3MouseManager::SetCursor
                THISCALL_2(void, 0x50D7B0, mouseMgr, 1); // H3MouseManager::TurnMouseOn
                int skills[4] = { academy->skillId1, academy->skillId2, eSecondary::NAVIGATION, eSecondary::NAVIGATION };
                //g_UniversityDlg = static_cast<H3UniversityDlg*>(std::malloc(sizeof(H3UniversityDlg)));
                char memory[264];
                g_UniversityDlg = reinterpret_cast<H3UniversityDlg*>(memory);
                g_SeafaringAcademyFlag = 1;
                g_HeroInUniversity = hero;

                THISCALL_4(void, 0x5EF8D0, memory, hero, skills, 2); // H3UniversityDlg::Ctor
                THISCALL_3(void, 0x5FF800, memory, -1, -1); // H3BaseDlg::SetPos
                THISCALL_2(void, 0x5F0DF0, memory, 0); // H3UniversityDlg::Run
                THISCALL_1(void, 0x4AA6B0, memory); // H3UniversityDlg::Dtor

                g_UniversityDlg = nullptr;
                g_HeroInUniversity = nullptr;
                g_SeafaringAcademyFlag = 0;
            }

            ProcObjectFlagsVisitedByTeam(hero, objectType, objectSubtype);
            // set object as visited
            if (hero->owner <= 7)
                academy->visited |= 1 << hero->owner;

            return true;
        }

        return false;
    }

    BOOL SeafaringAcademyExtender::SetAiMapItemWeight(
        H3MapItem* mapItem, H3Hero* hero, const H3Player* player,
        int& aiMapItemWeight, int* moveDistance, const H3Position pos) const noexcept
    {
        //if (auto setup = GetFromMapItem(mapItem))
        //{
        //    return true;
        //}

        return false;
    }

    //int SeafaringAcademyExtender::AI_OnScouting_Value() const noexcept
    //{
    //    return 50;
    //}
}
