#include "../pch.h"
#include "GraveExtender.h"
#include <new>

namespace grave
{
    _LHF_(LoHook_AdvMgr_GetObjectRightClickText)
    {
        // 1014A240: отдельное имя в тексте ПКМ, вместо цветных врат.
        H3MapItem* cell = reinterpret_cast<H3MapItem*>(c->ebx);

        // ....

        // 1014A7BE
        if (cell->objectType != GRAVE_OBJECT_TYPE || cell->objectSubtype != GRAVE_OBJECT_SUBTYPE)
            return EXEC_DEFAULT;

        // В оригинале 1014A7C9: создаём локальную строку пропущенной ветки SoD.
        H3String* objectName = reinterpret_cast<H3String*>(c->ebp - 0x30);
        ::new (objectName) H3String();
        LPCSTR name = extender::GetObjectName(GRAVE_OBJECT_TYPE, GRAVE_OBJECT_SUBTYPE);
        c->esi = reinterpret_cast<int>(name ? name : h3_NullString);
        c->return_address = 0x41398F;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_AdvMgr_GetObjectCommand)
    {
        // 10149880: отдельное имя в строке команды.
        H3MapItem* cell = reinterpret_cast<H3MapItem*>(c->ebx);

        // 
        if (cell->objectType != GRAVE_OBJECT_TYPE || cell->objectSubtype != GRAVE_OBJECT_SUBTYPE)
            return EXEC_DEFAULT;

        LPCSTR name = extender::GetObjectName(GRAVE_OBJECT_TYPE, GRAVE_OBJECT_SUBTYPE);
        c->edi = reinterpret_cast<int>(name ? name : h3_NullString);
        c->return_address = 0x40B18B;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_0042F0C7)
    {
        // 10137BC0, ветка могилы.
        H3MapItem* cell = *reinterpret_cast<H3MapItem**>(c->ebp + 8);
        if (cell->objectType != GRAVE_OBJECT_TYPE || cell->objectSubtype != GRAVE_OBJECT_SUBTYPE)
            return EXEC_DEFAULT;
        c->return_address = 0x42F0D0;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_004323DB)
    {
        // 1013B100. scouting-оценка объектов
        H3MapItem* cell = reinterpret_cast<H3MapItem*>(c->eax);

        switch (cell->objectType)
        {
        case 142:                                   // Склад (в Objects_CommonHotaObjectsPack не сделано)
        case 215:                                   // Страж прохода
            c->eax = 10;
            return NO_EXEC_DEFAULT;
        case 212:
        {
            INT16 objSubtype = cell->objectSubtype;
            if (objSubtype == 1000)                 // врата заданий
            {
                c->eax = 10;
                return NO_EXEC_DEFAULT;
            }
            else if (objSubtype == 1001)            // могила
            {
                c->eax = 1;
                return NO_EXEC_DEFAULT;
            }
            else
            {
                return EXEC_DEFAULT;
            }
        }
        case 213:                                   // Гильдия наёмников
            c->eax = 1;
            return NO_EXEC_DEFAULT;
        case 220:                                   // Рудник
            c->eax = 20;
            return NO_EXEC_DEFAULT;
        default:
            return EXEC_DEFAULT;
        }
    }








    // Для чтения сохраненных настроек из файла (информация сохраняется в файл в редакторе)
    /*
    H3Vector<HotaGraveRewardSettings> g_GraveDefinitions;   // 0x104A6E64
    */
    static bool g_GraveGroupGeneration = false;             // 0x104A7908

    // Для чтения сохраненных настроек из файла (информация сохраняется в файл в редакторе)
    /*
    BOOL HotaGraveRewardSettings::Read(H3GzFile* file)
    {
        // 100DF8D0: mode, artifact, затем два значения с однобайтовыми флагами.
        return file->Read(randomReward) && file->Read(artifactId) &&
            file->Read(gold) && file->Read(firstRewardFlag) &&
            file->Read(secondReward) && file->Read(secondRewardFlag);
    }

    _LHF_(LoHook_004FDED7)
    {
        // 10137F50: очистка настроек наград могилы.
        g_GraveDefinitions.RemoveAll();
        return EXEC_DEFAULT;
    }

    BOOL __stdcall ReadAndRegisterGraveRewardSettings(H3GzFile* file, UINT32* setup)
    {
        // 10155100 - Читает настройки награды, добавляет запись в таблицу и сохраняет её индекс.
        HotaGraveRewardSettings definition;
        // Для могилы 212/1001 читаем запись награды.
        if (!definition.Read(file))
            return FALSE;
        *setup = g_GraveDefinitions.Size();
        g_GraveDefinitions.Add(definition);
        return TRUE;
    }


    int __stdcall HiHook_00503450(HiHook* h, H3MainSetup* setup, H3GzFile* file,
        H3ObjectDetails* details, int mapVersion)
    {
        // 1013A1A0 -> 10155100 -> 100DF8D0, только ветка могилы.
        const int result = THISCALL_4(int, h->GetDefaultFunc(), setup, file, details, mapVersion);
        if (result < 0) return result;
        const H3ObjectAttributes& attributes = setup->objectAttributes[details->num];
        if (attributes.type != GRAVE_OBJECT_TYPE || attributes.subtype != GRAVE_OBJECT_SUBTYPE)
            return result;
        if (!ReadAndRegisterGraveRewardSettings(file, &details->setup))
            return -1;
        return result;
    }

    void __stdcall HiHook_00640A90(HiHook* h, H3RmgObject* self,
        H3GzFile* file, void* writeContext)
    {
        // 10167100, только 212/1001. Остальные виды пишет прежний обработчик.
        THISCALL_3(void, h->GetDefaultFunc(), self, file, writeContext);
        const H3RmgObjectProperties* prototype = self->properties->prototype;
        if (prototype->type != GRAVE_OBJECT_TYPE || prototype->subtype != GRAVE_OBJECT_SUBTYPE)
            return;
        const INT32 random = -1, amount = 1;
        const UINT8 flag = 0;
        file->Write(random);
        file->Write(random);
        for (int i = 0; i < 2; ++i)
        {
            file->Write(amount);
            file->Write(flag);
        }
    }

    */

    _LHF_(LoHook_0048080B)
    {
        // 1013B010, только ветка могилы. Врата задания/цветные врата не меняем.
        H3MapItem* cell = reinterpret_cast<H3MapItem*>(c->ebx);
        if (cell->objectSubtype != GRAVE_OBJECT_SUBTYPE) return EXEC_DEFAULT;
        c->return_address = 0x4805D5;
        return NO_EXEC_DEFAULT;
    }

    


#pragma pack(push, 4)
    struct HotaGravePlacementReplay final : HotaReplayActionBase
    {
        // HotA vTable 101EC760; размер 0x20, интерфейс SoD по +04.
        INT32 objectIndex = -1;     // +0C
        H3Position position{};     // +10
        INT32 heroId = -1;          // +14
        INT32 savedSubtype = -1;    // +18: заполняется при Undo.
        INT32 savedSetup = -1;      // +1C
        explicit HotaGravePlacementReplay(INT8 player) : HotaReplayActionBase(player) {}
        UINT32 GetSerializedSize() override { return 16; } // 10178250
        INT32 GetKind() override { return 7; }             // 1002AD00
        HotaReplayActionBase* vDelete(UINT32 flags) override
        {
            this->~HotaGravePlacementReplay();
            if (flags & 1) H3Free(this);
            return this;
        }
        BOOL8 Read(H3GzFile* file, INT32) override
        {
            // 10176BE0. Undo-поля в сохранение не входят.
            return file->Read(objectIndex) && file->Read(position) && file->Read(heroId);
        }
        BOOL8 Write(H3GzFile* file) override
        {
            // 1015BF10 / 10095170.
            return file->Write(objectIndex) &&
                WriteHotaReplayPosition(&position, file) >= sizeof(position) && file->Write(heroId);
        }
        INT32 Redo(BOOL8 redraw) override
        {
            // 10176E20. Используем общий интерфейс и выбор героя из WaterObjects.
            H3Game* game = P_Game->Get();
            H3AdventureManager* adv = P_AdventureManager->Get();
            const BOOL8 draw = redraw && g_ReplayIntermediateRedraw;
            H3Hero* selected = heroId >= 0 ? game->GetHero(heroId) : nullptr;
            BOOL visible = FALSE;
            if (!IntAt(0x6987E0))
            {
                visible = FASTCALL_4(BOOL, 0x41A6B0, position.GetX(), position.GetY(),
                    position.GetZ(), IntAt(0x69CD08));
                if (selected)
                    visible = visible || FASTCALL_4(BOOL, 0x41A6B0, selected->x,
                        selected->y, selected->z, IntAt(0x69CD08));
            }
            visible = visible && draw;
            if (selected)
            {
                SelectHero(heroId, static_cast<BOOL8>(visible));
                if (visible) adv->ShowCoordinates(selected->x, selected->y, static_cast<UINT8>(selected->z));
            }
            H3MapItem* cell = game->mainSetup.GetMapItem(position.GetX(), position.GetY(), position.GetZ());
            H3Hero* hero = nullptr;
            H3Boat* boat = nullptr;
            if (cell->objectType == eObject::HERO)
            {
                hero = game->GetHero(cell->setup);
                THISCALL_1(void, 0x4D7950, hero);
            }
            if (cell->objectType == eObject::BOAT)
            {
                boat = &game->boats[cell->setup];
                THISCALL_1(void, 0x4D7950, boat);
            }
            THISCALL_3(int, 0x506170, &game->mainSetup, objectIndex, 0);
            cell->objectSubtype = static_cast<UINT16>(savedSubtype);
            cell->setup = savedSetup;
            if (boat) THISCALL_3(void, 0x4D7840, boat, eObject::BOAT, static_cast<UINT8>(boat->index));
            if (hero) THISCALL_3(void, 0x4D7840, hero, eObject::HERO, hero->id);
            if (visible)
            {
                THISCALL_2(void, 0x40F7D0, adv, 0);
                THISCALL_7(void, 0x412BA0, adv, adv->screenPosition, 1, 0, 0, 0, 0);
                THISCALL_3(void, 0x40F1D0, adv, 0, 0);
            }
            if (heroId >= 0) THISCALL_3(void, 0x4175E0, adv, 0, draw);
            return 1;
        }
        void Undo() override
        {
            // 101782C0.
            H3Game* game = P_Game->Get();
            H3MapItem* cell = game->mainSetup.GetMapItem(position.GetX(), position.GetY(), position.GetZ());
            H3Hero* hero = nullptr;
            H3Boat* boat = nullptr;
            if (cell->objectType == eObject::HERO)
            {
                hero = game->GetHero(cell->setup);
                THISCALL_1(void, 0x4D7950, hero);
            }
            if (cell->objectType == eObject::BOAT)
            {
                boat = &game->boats[cell->setup];
                THISCALL_1(void, 0x4D7950, boat);
            }
            savedSubtype = cell->objectSubtype;
            savedSetup = cell->setup;
            THISCALL_4(void, 0x4AA820, P_AdventureManager->Get(), cell, position, 0);
            if (boat) THISCALL_3(void, 0x4D7840, boat, eObject::BOAT, static_cast<UINT8>(boat->index));
            if (hero) THISCALL_3(void, 0x4D7840, hero, eObject::HERO, hero->id);
        }
    };
#pragma pack(pop)
    static_assert(sizeof(HotaGravePlacementReplay) == 0x20, "HotA grave placement replay");

    H3ReplayAction* CreateGravePlacementReplay(INT8 player)
    {
        HotaGravePlacementReplay* holder = new HotaGravePlacementReplay(player);
        return holder ? &holder->action : nullptr;
    }

    void AddGravePlacementReplay(H3Game* game, INT32 objectIndex, H3Position position, INT32 heroId)
    {
        // 10159AA0.
        HotaGravePlacementReplay* holder = new HotaGravePlacementReplay(static_cast<INT8>(IntAt(0x69CCF4)));
        holder->objectIndex = objectIndex;
        holder->position = position;
        holder->heroId = heroId;
        game->replayActions.Add(&holder->action);
    }

    BOOL __stdcall HandleGraveNetworkMessage(DWORD* message)
    {
        // 10159130, только команда 3000/4 — постановка ямы после раскопок.
        if (message[2] != 3000 || message[3] != 52 || message[5] != 4 || message[9] != eObject::HOLE)
            return FALSE;
        H3AdventureManager* adv = P_AdventureManager->Get();
        H3Game* game = P_Game->Get();
        AddGravePlacementReplay(game, game->mainSetup.objectDetails.Size(),
            H3Position(static_cast<INT16>(message[6]), static_cast<INT16>(message[7]), static_cast<INT8>(message[8])), message[12]);
        THISCALL_7(void, 0x4C9550, game, message[6], message[7], message[8],
            message[9], message[10], message[11]);
        THISCALL_2(void, 0x40F7D0, adv, 0);
        THISCALL_7(void, 0x412BA0, adv, adv->screenPosition, 1, 0, 0, 0, 0);
        THISCALL_3(void, 0x40F1D0, adv, 0, 0);
        // Владение пакетом остаётся у вызывающего обработчика.
        return TRUE;
    }

    INT32 __stdcall HiHook_00481930(HiHook* h, H3AdventureManager* adv, DWORD* message)
    {
        // 10159C60: обработка сообщения на карте приключений.
        // Сообщением владеет AdvMgr_Action_FromNetMsg.
        if (HandleGraveNetworkMessage(message))
            return 0;
        return THISCALL_2(INT32, h->GetDefaultFunc(), adv, message);
    }

    _LHF_(LoHook_00405DCE)
    {
        // 1015A570: направляем только пакет ямы 3000/4 в 00481930.
        const DWORD* message = reinterpret_cast<const DWORD*>(c->esi);
        if (c->eax != 3000 || message[3] != 52 || message[5] != 4 || message[9] != eObject::HOLE)
            return EXEC_DEFAULT;
        c->return_address = 0x405DDC;
        return NO_EXEC_DEFAULT;
    }

    BOOL8 __stdcall HiHook_00588A70(HiHook* h, H3SelectScenarioDialog* dlg, DWORD* message, BOOL8* closeDialog)
    {
        // 1015A040: обработка сообщения в окне выбора сценария.
        if (HandleGraveNetworkMessage(message))
        {
            // При обходе default освобождаем пакет, которым владеет 00588A70.
            H3Free(message);
            return FALSE;
        }
        return THISCALL_3(BOOL8, h->GetDefaultFunc(), dlg, message, closeDialog);
    }

    _LHF_(LoHook_00473D3B)
    {
        // 10157FC0: обработка сообщения во время боя.
        if (!HandleGraveNetworkMessage(reinterpret_cast<DWORD*>(c->esi)))
            return EXEC_DEFAULT;
        c->return_address = 0x473E13; // Штатное освобождение сообщения.
        return NO_EXEC_DEFAULT;
    }

    void __stdcall HiHook_0040EE9C(HiHook* h, H3Game* game, int x, int y, int z,
        int type, int subtype, int setup)
    {
        // 10158620: штатная постановка ямы остаётся внутри ProcDig.
        DWORD message[13] = { DWORD(-1), 0, 3000, 52, 0, 4,
            DWORD(x), DWORD(y), DWORD(z), DWORD(type), DWORD(subtype), DWORD(setup),
            DWORD(P_ActivePlayer->currentHero) };
        THISCALL_1(void, 0x481CB0, message);
        AddGravePlacementReplay(game, game->mainSetup.objectDetails.Size(), H3Position(x, y, z),
            P_ActivePlayer->currentHero);
        THISCALL_7(void, h->GetDefaultFunc(), game, x, y, z, type, subtype, setup);
    }




















    

    BOOL H3MapItemGrave::IsVisitedByHero(H3Hero* hero) noexcept
    {
        sprintf(newMapItem::buffer, ErmVariableFormat, hero->id);
        return Era::GetAssocVarIntValue(newMapItem::buffer);
    }
    void H3MapItemGrave::SetAsVisited(H3Hero* hero) noexcept
    {
        sprintf(newMapItem::buffer, ErmVariableFormat, hero->id);
        Era::SetAssocVarIntValue(newMapItem::buffer, 1);
    }

    void H3MapItemGrave::SetAsNotVisited(H3Hero* hero) noexcept
    {
        sprintf(newMapItem::buffer, ErmVariableFormat, hero->id);
        Era::SetAssocVarIntValue(newMapItem::buffer, 0);
    }

    _LHF_(LoHook_004fd3b2)
    {
        H3MapItem* mapItem = reinterpret_cast<H3MapItem*>(c->ecx);

        if (c->eax == GRAVE_OBJECT_TYPE && 
            mapItem->objectSubtype == GRAVE_OBJECT_SUBTYPE)
        {
            c->return_address = 0x4FD3C6;
            return NO_EXEC_DEFAULT;
        }

        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_0040e28c)
    {
        H3MapItem* mapItem = reinterpret_cast<H3MapItem*>(c->eax);

        if (mapItem->objectType == GRAVE_OBJECT_TYPE && 
            mapItem->objectSubtype == GRAVE_OBJECT_SUBTYPE)
        {
            c->return_address = 0x40E2A6;
            return NO_EXEC_DEFAULT;
        }

        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_00480e4b)
    {
        H3MapItem* mapItem = reinterpret_cast<H3MapItem*>(c->ebx);

        if (mapItem->objectType == GRAVE_OBJECT_TYPE && 
            mapItem->objectSubtype == GRAVE_OBJECT_SUBTYPE)
        {
            c->return_address = 0x480F9B;
            return NO_EXEC_DEFAULT;
        }

        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_00408d79)
    {
        H3MapItem* mapItem = reinterpret_cast<H3MapItem*>(c->eax);

        if (mapItem->objectType == GRAVE_OBJECT_TYPE && 
            mapItem->objectSubtype == GRAVE_OBJECT_SUBTYPE)
        {
            c->return_address = 0x40968C;
            return NO_EXEC_DEFAULT;
        }

        return EXEC_DEFAULT;
    }

    char __stdcall HiHook_004c0674(HiHook* h, H3MapItem* cell)
    {
        // H3MapItem::GetRealObjectType
        if (THISCALL_1(int, 0x4FD220, cell) == GRAVE_OBJECT_TYPE &&
            cell->objectSubtype == GRAVE_OBJECT_SUBTYPE)
        {
            return false;
        }
        return THISCALL_1(char, h->GetDefaultFunc(), cell);
    }

    _LHF_(LoHook_0040EE89)
    {
        // 10139EB0. Награда сохранена обёрткой до удаления объекта.
        H3MapItem* cell = reinterpret_cast<H3MapItem*>(c->edi);
        if (THISCALL_1(int, 0x4FD220, cell) == GRAVE_OBJECT_TYPE &&
            cell->objectSubtype == GRAVE_OBJECT_SUBTYPE)
        {
            const H3Position pos(c->esi, c->ebx, IntAt(c->ebp + 0x10));
            // H3AdventureManager::DeleteObject. Яму затем ставит штатный код.
            THISCALL_4(void, 0x4AA820, P_AdventureManager->Get(), cell, pos, 1);
        }
        return EXEC_DEFAULT;
    }

    H3Hero* g_DiggingHero = nullptr;
    bool g_GraveDigging = false;
    H3MapItemGrave g_CellSetup;
    int __stdcall HiHook_0040ebf0(HiHook* h, H3AdventureManager* this_, int x, int y, int z)
    {
        // 10138840. Исходные аргументы default не меняем.
        const int originalX = x, originalY = y, originalZ = z;
        H3Game* gameMgr = P_Game->Get();
        // H3Game::GetHero
        H3Hero* hero = THISCALL_2(H3Hero*, 0x4317D0, gameMgr, P_ActivePlayer->currentHero);

        g_DiggingHero = hero;
        if (x == -1)
        {
            x = hero->x;
            y = hero->y;
            z = hero->z;
        }
        // H3MainSetup::GetMapItem
        H3MapItem* cell = THISCALL_4(H3MapItem*, 0x4086D0, &gameMgr->mainSetup, x, y, z);
        // H3MapItem::GetRealObjectType
        if (THISCALL_1(int, 0x4FD220, cell) == GRAVE_OBJECT_TYPE &&
            cell->objectSubtype == GRAVE_OBJECT_SUBTYPE)
        {
            g_GraveDigging = true;
            // H3MapItem::GetRealSetup
            DWORD setup = THISCALL_1(DWORD, 0x4FD280, cell);
            //g_CellSetup = *reinterpret_cast<H3MapItemGrave*>(&setup);
            std::memcpy(&g_CellSetup, &setup, sizeof(setup));
        }
        else
        {
            g_GraveDigging = false;
            g_CellSetup = {};
        }
        
        return THISCALL_4(int, h->GetDefaultFunc(), this_, originalX, originalY, originalZ);
    }

    _LHF_(LoHook_004aa9b3)
    {
        // 1013C270
        H3MapItem* cell = reinterpret_cast<H3MapItem*>(c->edx);
        H3Hero* hero = nullptr;
        H3Boat* boat = nullptr;
        H3Game* gameMgr = P_Game->Get();

        if (*(DWORD*)&cell->objectType == eObject::HERO)
        {
            hero = THISCALL_2(H3Hero*, 0x4317D0, gameMgr, cell->setup); // H3Game::GetHero
            THISCALL_1(void, 0x4D7950, hero); // Hero::Hide
        }
        if (*(DWORD*)&cell->objectType == eObject::BOAT)
        {
            boat = &gameMgr->boats[cell->setup];
            THISCALL_1(void, 0x4D7950, boat); // Hero::Hide
        }

        cell->setup = -1;

        if (boat)
            THISCALL_3(void, 0x4D7840, boat, eObject::BOAT, static_cast<UINT8>(boat->index)); // Hero::Show
        if (hero)
            THISCALL_3(void, 0x4D7840, hero, eObject::HERO, hero->id); // Hero::Show
        
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_0040ec5a)
    {
        if (!g_GraveDigging)
        {
            return EXEC_DEFAULT;
        }
            
        c->return_address = 0x40F185;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_0040eca8)
    {
        if (!g_GraveDigging)
        {
            return EXEC_DEFAULT;
        }
            
        c->return_address = 0x40ED13;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_0040f04b)
    {
        if (!g_GraveDigging)
            return EXEC_DEFAULT;

        
        int artifactID = static_cast<int>(g_CellSetup.artifactID);
        int goldAmount = static_cast<int>(g_CellSetup.goldAmount);
        int ARTIFACT_BACKPACK_SLOTS = 64;       

        if (THISCALL_2(int, 0x4D9330, g_DiggingHero, 1) >= ARTIFACT_BACKPACK_SLOTS) // H3Hero::CountArtifactsInBackpack
            artifactID = -1;

        if (THISCALL_1(char, 0x4BAA60, P_ActivePlayer->Get())) // H3Player::IsHuman
        {
            sprintf(h3_TextBuffer, dig_key, GRAVE_OBJECT_TYPE, GRAVE_OBJECT_SUBTYPE);
            LPCSTR message = EraJS::read(h3_TextBuffer);
            if (artifactID < 0)
            {
                FASTCALL_12(void, 0x4F6C00,
                    message, 1, -1, -1, 36, goldAmount,
                    -1, 0, -1, 0, -1, 0);
            }
            else
            {
                FASTCALL_12(void, 0x4F6C00,
                    message, 1, -1, -1, 8, artifactID,
                    36, goldAmount, -1, 0, -1, 0);
            }
            int* g_TerrainToAmbientMusicId = reinterpret_cast<int*>(IntAt(0x40867E + 2));
            THISCALL_2(void, 0x59A4E0, P_SoundManager->Get(), g_TerrainToAmbientMusicId[P_AdventureManager->lastTerrain]);

        }
        THISCALL_3(void, 0x4E3870, g_DiggingHero, eResource::GOLD, goldAmount); // H3Hero::AddResource
        
        if (artifactID >= 0)
        {
            H3Artifact artifact(artifactID, -1);
            THISCALL_4(char, 0x4E32E0, g_DiggingHero, &artifact, 1, 1); // H3Hero::AddArtifact
            if (!THISCALL_1(char, 0x4BAA60, P_ActivePlayer->Get())) // H3Player::IsHuman
                THISCALL_1(void, 0x433B40, g_DiggingHero); // AI::H3Hero::EquipArtifacts
        }
        // add debuff
        if (!H3MapItemGrave::IsVisitedByHero(g_DiggingHero))
        {
            H3MapItemGrave::SetAsVisited(g_DiggingHero);
            g_DiggingHero->moraleBonus -= MORALE_PENALTY;
        }
        
        c->return_address = 0x40F080;
        return NO_EXEC_DEFAULT;
    }

    char isGrave = false;
    _LHF_(LoHook_0042ef00)
    {
        isGrave = false;
        H3MapItem* cell = reinterpret_cast<H3MapItem*>(c->eax);

        if (THISCALL_1(int, 0x4FD220, cell) != GRAVE_OBJECT_TYPE)
            return EXEC_DEFAULT;

        if (cell->objectSubtype == GRAVE_OBJECT_SUBTYPE)
        {
            c->return_address = 0x42EF52;
            isGrave = true;
        }
        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_0042f0af)
    {
        if (!isGrave)
            return EXEC_DEFAULT;

        c->return_address = 0x42F0C1;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_0042ff38)
    {
        H3MapItem* cell = THISCALL_2(H3MapItem*, 0x42ED80, P_Game->Get(), (H3Position)c->eax);
        
        if (THISCALL_1(int, 0x4FD220, cell) != GRAVE_OBJECT_TYPE || 
            cell->objectSubtype != GRAVE_OBJECT_SUBTYPE)
            return EXEC_DEFAULT;

        c->eax = 0;
        c->ecx = 0;

        c->return_address = 0x42FF71;
        return NO_EXEC_DEFAULT;
    }

    _LHF_(LoHook_004915b7)
    {
        H3MapItem* mapItem = reinterpret_cast<H3MapItem*>(c->eax);

        if (mapItem->objectType == GRAVE_OBJECT_TYPE &&
            mapItem->objectSubtype == GRAVE_OBJECT_SUBTYPE)
        {
            c->return_address = 0x4915C0;
            return NO_EXEC_DEFAULT;
        }

        return EXEC_DEFAULT;
    }




    ///////////////////////////////////////////////////////////////////////////
    

    BOOL8 __stdcall H3Pathfinder_EnterTrigger(
        HiHook* h,
        H3Pathfinder* pathfinder,
        H3Hero* hero,
        H3PathNode* node,
        int movementLimit,
        INT32 searchType)
    {
        // 10028920: перенесена только ветка могилы 212/1001.
        const H3MapItem* cell = node->GetMapItem();
        if (cell->objectType == GRAVE_OBJECT_TYPE && cell->objectSubtype == GRAVE_OBJECT_SUBTYPE)
            return TRUE;

        return THISCALL_5(BOOL8, h->GetDefaultFunc(), pathfinder, hero, node, movementLimit, searchType);
    }

    BOOL8 __stdcall HiHook_0056AE10(HiHook* h, H3Pathfinder* this_, H3Hero* hero,
        H3PathNode* node, INT32 movementLimit, INT32 searchType)
    {
        // 1001F290 могила проходима без ключа соответствующего цвета.

        return H3Pathfinder_EnterTrigger(h, this_, hero, node, movementLimit, searchType);
    }

    _LHF_(LoHook_0054670F)
    {
        // 1016F2A0, только могила.
        const H3RmgObjectGenerator* generator = reinterpret_cast<H3RmgObjectGenerator*>(c->ecx);
        if (generator->type != GRAVE_OBJECT_TYPE || generator->subtype != GRAVE_OBJECT_SUBTYPE ||
            (ByteAt(c->ebp + 0x18) && g_GraveGroupGeneration)) return EXEC_DEFAULT;
        c->return_address = 0x546926;
        return NO_EXEC_DEFAULT;
    }
    _LHF_(LoHook_00546AD1)
    {
        // 1016F310: могила завершает группу.
        H3RmgObject* object = *reinterpret_cast<H3RmgObject**>(c->ebp - 0x0C);
        const H3RmgObjectProperties* prototype = object->properties->prototype;
        if (prototype->type != GRAVE_OBJECT_TYPE || prototype->subtype != GRAVE_OBJECT_SUBTYPE)
            return EXEC_DEFAULT;
        c->return_address = 0x546BB3;
        return NO_EXEC_DEFAULT;
    }
    _LHF_(LoHook_00546C90)
    {
        // 10167A80: у одиночной могилы нет внешней охраны.
        const H3RmgLocalMap* group = *reinterpret_cast<H3RmgLocalMap**>(c->ebp + 0x0C);
        if (group->localObjects.Size() != 1) return EXEC_DEFAULT;
        const H3RmgObjectProperties* prototype = group->localObjects[0]->properties->prototype;
        if (prototype->type != GRAVE_OBJECT_TYPE || prototype->subtype != GRAVE_OBJECT_SUBTYPE)
            return EXEC_DEFAULT;
        c->return_address = 0x546DD8;
        return NO_EXEC_DEFAULT;
    }
    _LHF_(LoHook_00531E53)
    {
        // 1017DB90, исключение могилы в двух проверках размещения на дороге.
        const H3RmgObjectProperties* prototype = *reinterpret_cast<H3RmgObjectProperties**>(c->ebp + 8);
        if (prototype->type != GRAVE_OBJECT_TYPE || prototype->subtype != GRAVE_OBJECT_SUBTYPE)
            return EXEC_DEFAULT;
        H3RmgMap* map = *reinterpret_cast<H3RmgMap**>(c->ebp - 0x14);
        H3RmgMapItem* cell = map->GetMapItem(IntAt(c->ebp - 8), IntAt(c->ebp - 12), IntAt(c->ebp + 0x14));
        if ((*reinterpret_cast<const DWORD*>(cell) & 0x3C000000) == 0) return EXEC_DEFAULT;
        c->return_address = 0x531F87;
        return NO_EXEC_DEFAULT;
    }

    void __stdcall UpdateGraveRevisitButtonState()
    {
        // 10018230: только запрет повторного посещения могилы.
        if (ByteAt(0x696A54)) // Во время просмотра хода кнопку не меняем.
            return;

        H3Player* player = P_ActivePlayer->Get();
        if (!THISCALL_1(BOOL8, 0x4BAA40, player) || player->currentHero < 0)
            return;

        H3Hero* hero = P_Game->GetHero(player->currentHero);
        if (!hero)
            return;
        H3MapItem* cell = P_Game->mainSetup.GetMapItem(hero->x, hero->y, hero->z);
        if (cell->objectType != GRAVE_OBJECT_TYPE || cell->objectSubtype != GRAVE_OBJECT_SUBTYPE)
            return;

        H3AdventureMgrDlg* dlg = P_AdventureManager->dlg;
        if (dlg)
        {
            if (H3DlgItem* button = dlg->GetH3DlgItem(3000))
                button->SendCommand(5, 16392); // Добавляем затенение и перерисовываем кнопку.
        }
    }

    _LHF_(LoHook_0041DCE7)
    {
        // 10003340: выбор героя, показ карты приключений, телепортация.
        if (THISCALL_1(BOOL8, 0x4BAA40, P_ActivePlayer->Get()))
            UpdateGraveRevisitButtonState();
        return EXEC_DEFAULT;
    }

    void __stdcall HiHook_0047FCA0(HiHook* h, H3AdventureManager* adv,
        H3Hero* hero, INT32 direction, INT32 arg3, INT32 arg4)
    {
        // 10003DB0: обновление после отрисовки/перемещения героя.
        THISCALL_5(void, h->GetDefaultFunc(), adv, hero, direction, arg3, arg4);
        if (THISCALL_1(BOOL8, 0x4BAA40, P_ActivePlayer->Get()))
            UpdateGraveRevisitButtonState();
    }

    static HiHook* g_GraveBoardBoatHook = nullptr; // 102056D0, дескриптор DIRECT-хука 004A0C80.

    void __fastcall HiHook_004A0C80(H3AdventureManager* adv, void* unusedEdx,
        H3Hero* hero, H3MapItem* boat)
    {
        // 10001230: DIRECT/THISCALL, ECX = adv; EDX не используется.
        THISCALL_3(void, g_GraveBoardBoatHook->GetDefaultFunc(), adv, hero, boat);
        if (THISCALL_1(BOOL8, 0x4BAA40, P_ActivePlayer->Get()))
            UpdateGraveRevisitButtonState();
    }

    void __stdcall HiHook_00417790(HiHook* h, H3AdventureManager* adv,
        INT32 townId, INT32 mode, BOOL8 arg3)
    {
        // 10001E70: обновление после выбора города, если младший байт mode равен нулю.
        THISCALL_4(void, h->GetDefaultFunc(), adv, townId, mode, arg3);
        if (!static_cast<BOOL8>(mode))
            UpdateGraveRevisitButtonState();
    }

    _LHF_(LoHook_00407F6D)
    {
        // 10004E40: обновление после движения по выбранному маршруту, только ветка могилы.
        UpdateGraveRevisitButtonState();
        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_004E1C75)
    {
        // 100027B0: обновление при выходе из окна героя.
        UpdateGraveRevisitButtonState();
        return EXEC_DEFAULT;
    }

    void GraveExtender::CreatePatches()
    {
        static bool hookInstalled = false;
        if (hookInstalled) return;

        ///////////////////////////// Object_Init /////////////////////////////

        // 1014FA4A - RMC
        // В HotA 0x413919; используем общую с ExtraHints точку без перекрытия инструкций.
        _PI->WriteLoHook(0x41391C, LoHook_AdvMgr_GetObjectRightClickText);

        // Hover Hint
        _PI->WriteLoHook(0x40B158, LoHook_AdvMgr_GetObjectCommand);

        // ....
        
        // 1015006C - AIMapGoal (не сделано в кастомайзере)
        _PI->WriteLoHook(0x42F0C7, LoHook_0042F0C7);

        // Частные случаи для AIHero_GetScoutingWeight
        _PI->WriteLoHook(0x4323DB, LoHook_004323DB);

        // ....

        // 10151C26
        // см. flagsextender - _ERH_(OnAfterBattleUniversal)
        //_PI->WriteHiHook(0x4DA780, SPLICE_, EXTENDED_, THISCALL_, HiHook_HeroModFlags_ResetAfterBattle);

        // ....



        // Для чтения сохраненных настроек из файла (информация сохраняется в файл в редакторе)

        // 10152744 - Очистка таблиц с наградами
        // +/-: сделано только для могил
        //_PI->WriteLoHook(0x4FDED7, LoHook_004FDED7);

        // ....

        // 
        //_PI->WriteHiHook(0x503450, SPLICE_, EXTENDED_, THISCALL_, HiHook_00503450); // 1013A1A0
  
        // 1015337F
        _PI->WriteLoHook(0x48080B, LoHook_0048080B);

        // ....

        // H3MapItem::IsValidToDig - если могила - копать можно
        _PI->WriteLoHook(0x4fd3b2, LoHook_004fd3b2);
        
        // H3AdventureManager::GetCursorType - патч курсора при наведении на могилу
        _PI->WriteLoHook(0x40e28c, LoHook_0040e28c);
        
        // H3AdventureManager::MoveHero - патч для обхода логики BORGER_GATE
        _PI->WriteLoHook(0x480e4b, LoHook_00480e4b);
        
        // H3AdventureManager::OnKeyPressed - при нажатии пробела, обход посещения могилы, стоя на ней
        _PI->WriteLoHook(0x408d79, LoHook_00408d79);
        
        // H3MapItem::IsValidToDig - не плейсить грааль на могилу?
        // GENERATOR_6_PlaceGrail
        _PI->WriteHiHook(0x4c0674, CALL_, EXTENDED_, THISCALL_, HiHook_004c0674);
        _PI->WriteHiHook(0x4c070e, CALL_, EXTENDED_, THISCALL_, HiHook_004c0674);
        // unk func
        _PI->WriteHiHook(0x52ccb7, CALL_, EXTENDED_, THISCALL_, HiHook_004c0674);
        _PI->WriteHiHook(0x52d27e, CALL_, EXTENDED_, THISCALL_, HiHook_004c0674);
        
        // Процесс копания могилы: хотовский порядок — удаление, штатная яма, затем награда.
        _PI->WriteLoHook(0x40EE89, LoHook_0040EE89);
        
        // H3AdventureManager::ProcDig - установка глобалок
        _PI->WriteHiHook(0x40ebf0, SPLICE_, EXTENDED_, THISCALL_, HiHook_0040ebf0);
        
        // H3AdventureManager::DeleteObjectOnMap - прячем героя перед удалением?
        _PI->WriteLoHook(0x4aa9b3, LoHook_004aa9b3);
        
        // H3AdventureManager::ProcDig - прыжок, если копание могилы
        _PI->WriteLoHook(0x40ec5a, LoHook_0040ec5a);
        _PI->WriteLoHook(0x40ecc5, LoHook_0040ec5a);
        _PI->WriteLoHook(0x40edda, LoHook_0040ec5a);

        // H3AdventureManager::ProcDig - прыжок, если у героя нет свободного места в рюкзаке
        _PI->WriteLoHook(0x40eca8, LoHook_0040eca8);

        // H3AdventureManager::ProcDig - сама логика
        _PI->WriteLoHook(0x40f04b, LoHook_0040f04b);

        // AIMapGoal
        _PI->WriteLoHook(0x42ef00, LoHook_0042ef00);
        _PI->WriteLoHook(0x42f0af, LoHook_0042f0af);

        // AI_HeroMove
        _PI->WriteLoHook(0x42ff38, LoHook_0042ff38);

        // 101537C0 - DlgDimDoor_ProcessAction
        _PI->WriteLoHook(0x4915b7, LoHook_004915b7);


        ///////////////////////////// H3Pathfinder_Init /////////////////////////////
        
        // 100236DA
        _PI->WriteHiHook(0x56AE10, SPLICE_, EXTENDED_, THISCALL_, HiHook_0056AE10);


        ///////////////////////////// func_1015A5B0 /////////////////////////////

        // 1015A829
        _PI->WriteHiHook(0x481930, SPLICE_, EXTENDED_, THISCALL_, HiHook_00481930);

        // Для доставки 3000/4 из AdvMgr_Action_FromNetMsg в 00481930.
        _PI->WriteLoHook(0x405DCE, LoHook_00405DCE); // 1015A570

        // 
        _PI->WriteHiHook(0x588A70, SPLICE_, EXTENDED_, THISCALL_, HiHook_00588A70);

        // 1015A909
        _PI->WriteLoHook(0x473D3B, LoHook_00473D3B);

        // 1015AA57 - ProcDig - PlaceObject
        _PI->WriteHiHook(0x40EE9C, CALL_, EXTENDED_, THISCALL_, HiHook_0040EE9C);


        ///////////////////////////// ExtendButtonsMaybe /////////////////////////////

        // 10014A6D / 10014A98 / 10014AC3: общий патч 10003340.
        _PI->WriteLoHook(0x417E01, LoHook_0041DCE7);
        _PI->WriteLoHook(0x419A84, LoHook_0041DCE7);
        _PI->WriteLoHook(0x41DCE7, LoHook_0041DCE7);

        // 10014B11: патч 10003DB0.
        _PI->WriteHiHook(0x47FCA0, SPLICE_, EXTENDED_, THISCALL_, HiHook_0047FCA0);

        // 10014B5A: патч 10001230, DIRECT без дополнительного аргумента HiHook*.
        g_GraveBoardBoatHook = _PI->WriteHiHook(0x4A0C80, SPLICE_, DIRECT_, THISCALL_, HiHook_004A0C80);

        // 10014BAF: патч 10001E70.
        _PI->WriteHiHook(0x417790, SPLICE_, EXTENDED_, THISCALL_, HiHook_00417790);

        // 10014D8B: патч 10004E40.
        _PI->WriteLoHook(0x407F6D, LoHook_00407F6D);

        // 10014DB6: патч 100027B0.
        _PI->WriteLoHook(0x4E1C75, LoHook_004E1C75);






        hookInstalled = true;
    }

    GraveExtender::GraveExtender()
        : H3ActiveObject<H3MapItemGrave>(
            "EraPlugin.GraveExtender.js",
            GRAVE_OBJECT_TYPE,
            GRAVE_OBJECT_SUBTYPE, 1)
    {
        CreatePatches();
    }

    GraveExtender* GraveExtender::instance = nullptr;
    GraveExtender& GraveExtender::Get()
    {
        if (!instance) instance = new GraveExtender();
        return *instance;
    }

    BOOL GraveExtender::InitNewGameMapItemSetup(H3MapItem* mapItem) const noexcept
    {
        // 10148BA0, ветка 212/1001.
        if (!mapItem || mapItem->objectType != GRAVE_OBJECT_TYPE ||
            mapItem->objectSubtype != GRAVE_OBJECT_SUBTYPE)
            return false;

        H3MapItemGrave reward{};
        // Для чтения сохраненных настроек из файла (информация сохраняется в файл в редакторе)
        /*
        const int index = mapItem->setup;
        if (index >= 0 && index < static_cast<int>(g_GraveDefinitions.Size()) &&
            g_GraveDefinitions[index].randomReward == 0)
        {
            reward.goldAmount = static_cast<UINT16>(g_GraveDefinitions[index].gold);
            reward.artifactID = static_cast<INT16>(g_GraveDefinitions[index].artifactId);
        }
        else
        */
        {
            reward.goldAmount = static_cast<UINT16>(500 * H3Random::Rand(3, 8));
            reward.artifactID = static_cast<INT16>(THISCALL_2(int, 0x4C9190, P_Game->Get(), 6));
        }
        std::memcpy(&mapItem->setup, &reward, sizeof(reward));
        return true;
    }

    BOOL GraveExtender::VisitMapItem(
        H3Hero *hero, H3MapItem *mapItem, const H3Position pos,
        const BOOL isHuman) const noexcept
    {
        // 1013A170/1013A8A0: могилу посещают только через штатные раскопки.
        // В частности, ИИ не получает награду сразу после прибытия.
        return mapItem && mapItem->objectType == GRAVE_OBJECT_TYPE &&
            mapItem->objectSubtype == GRAVE_OBJECT_SUBTYPE;
    }

    BOOL GraveExtender::SetAiMapItemWeight(
        H3MapItem* mapItem, H3Hero* hero, const H3Player* player,
        int& aiMapItemWeight, int* moveDistance, const H3Position pos) const noexcept
    {
        // 10147E80, ветка могилы.
        if (GetFromMapItem(mapItem))
        {
            aiMapItemWeight = static_cast<int>(player->aIPlayer.resourceImportance[eResource::GOLD] * 2750.0);

            // H3Hero::CountArtifactsInBackpack
            if (THISCALL_2(int, 0x4D9330, hero, 1) < 64)
            {
                aiMapItemWeight += static_cast<int>(player->aIPlayer.turnValueOfAvgArtifact);
            }

            if (*moveDistance < hero->movement)
                *moveDistance = hero->movement;

            *moveDistance += hero->CalcMaxMovement();

            return true;
        }

        return false;
    }
}
