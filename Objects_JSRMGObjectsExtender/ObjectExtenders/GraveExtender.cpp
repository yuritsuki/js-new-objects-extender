#include "../pch.h"
#include "GraveExtender.h"

namespace grave
{
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

        if (mapItem->objectType == extender::ERA_OBJECT_TYPE && 
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

        if (mapItem->objectType == extender::ERA_OBJECT_TYPE && 
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

        if (mapItem->objectType == extender::ERA_OBJECT_TYPE && 
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

        if (mapItem->objectType == extender::ERA_OBJECT_TYPE && 
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
        if (THISCALL_1(int, 0x4FD220, cell) == extender::ERA_OBJECT_TYPE &&
            cell->objectSubtype == GRAVE_OBJECT_SUBTYPE)
        {
            return false;
        }
        return THISCALL_1(char, h->GetDefaultFunc(), cell);
    }

    //_LHF_(LoHook_0040EE89)
    //{
    //    int x = c->esi;
    //    int y = c->ebx;
    //    int z = IntAt(c->ebp + 0x10);

    //    H3MapItem* cell = reinterpret_cast<H3MapItem*>(c->edi);
    //    //H3MapItem* mapItem = P_AdventureMgr->Get()->GetMapItem(x, y, z);

    //    //if (mapItem->objectType == extender::ERA_OBJECT_TYPE && 
    //    if (THISCALL_1(int, 0x4FD220, cell) == extender::ERA_OBJECT_TYPE &&
    //        cell->objectSubtype == GRAVE_OBJECT_SUBTYPE)
    //    {
    //        H3Position pos = H3Position(x, y, z);
    //        // H3AdventureManager::DeleteObject
    //        THISCALL_4(void, 0x4AA820, P_AdventureManager->Get(), cell, pos, 1);
    //    }

    //    return EXEC_DEFAULT;
    //}

    _LHF_(LoHook_0040EE89)
    {
        int x = c->esi;
        int y = c->ebx;
        int z = IntAt(c->ebp + 0x10);

        H3MapItem* cell = reinterpret_cast<H3MapItem*>(c->edi);
        //H3MapItem* mapItem = P_AdventureMgr->Get()->GetMapItem(x, y, z);

        //if (mapItem->objectType == extender::ERA_OBJECT_TYPE && 
        if (THISCALL_1(int, 0x4FD220, cell) == extender::ERA_OBJECT_TYPE &&
            cell->objectSubtype == GRAVE_OBJECT_SUBTYPE)
        {
            H3Hero* hero = *reinterpret_cast<H3Hero**>(c->ebp - 0x10);
            GraveExtender* ext = &GraveExtender::Get();
            ext->GraveDiggingProccess(cell, hero);

            c->return_address = 0x40F080;
            return NO_EXEC_DEFAULT;
        }

        return EXEC_DEFAULT;
    }

    H3Hero* g_DiggingHero = nullptr;
    bool g_GraveDigging = false;
    H3MapItemGrave g_CellSetup;
    void __stdcall HiHook_0040ebf0(HiHook* h, H3AdventureManager* this_, int x, int y, int z)
    {
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
        if (THISCALL_1(int, 0x4FD220, cell) == extender::ERA_OBJECT_TYPE &&
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
        
        THISCALL_4(void, h->GetDefaultFunc(), this_, x, y, z);
    }

    _LHF_(LoHook_004aa9b3)
    {
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
            THISCALL_3(void, 0x4D7840, boat, eObject::BOAT, static_cast<int>(boat->index)); // Hero::Show
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
            sprintf(h3_TextBuffer, dig_key, extender::ERA_OBJECT_TYPE, GRAVE_OBJECT_SUBTYPE);
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
            THISCALL_4(char, 0x4E32E0, g_DiggingHero, artifactID, 1, 1); // H3Hero::AddArtifact
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

        if (THISCALL_1(int, 0x4FD220, cell) != extender::ERA_OBJECT_TYPE)
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
        
        if (THISCALL_1(int, 0x4FD220, cell) != extender::ERA_OBJECT_TYPE || 
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

        if (mapItem->objectType == extender::ERA_OBJECT_TYPE &&
            mapItem->objectSubtype == GRAVE_OBJECT_SUBTYPE)
        {
            c->return_address = 0x4915C0;
            return NO_EXEC_DEFAULT;
        }

        return EXEC_DEFAULT;
    }


    void GraveExtender::CreatePatches()
    {
        static bool hookInstalled = false;
        if (hookInstalled) return;

        // H3MapItem::IsValidToDig - если могила - копать можно
        _PI->WriteLoHook(0x4fd3b2, LoHook_004fd3b2);
        
        // H3AdventureManager::GetCursorType - патч курсора при наведении на могилу
        //_PI->WriteLoHook(0x40e28c, LoHook_0040e28c);
        
        // H3AdventureManager::MoveHero - патч для обхода логики BORGER_GATE
        //_PI->WriteLoHook(0x480e4b, LoHook_00480e4b);
        
        // H3AdventureManager::OnKeyPressed - при нажатии пробела, обход посещения могилы, стоя на ней
        //_PI->WriteLoHook(0x408d79, LoHook_00408d79);
        
        // H3MapItem::IsValidToDig - не плейсить грааль на могилу?
        // GENERATOR_6_PlaceGrail
        _PI->WriteHiHook(0x4c0674, CALL_, EXTENDED_, THISCALL_, HiHook_004c0674);
        _PI->WriteHiHook(0x4c070e, CALL_, EXTENDED_, THISCALL_, HiHook_004c0674);
        // unk func
        _PI->WriteHiHook(0x52ccb7, CALL_, EXTENDED_, THISCALL_, HiHook_004c0674);
        _PI->WriteHiHook(0x52d27e, CALL_, EXTENDED_, THISCALL_, HiHook_004c0674);
        
        // Процесс копания могилы (в хоте - только удаление могилы) - хотовский дубликат закомменчен рядом
        _PI->WriteLoHook(0x40EE89, LoHook_0040EE89);
        
        // H3AdventureManager::ProcDig - установка глобалок
        //_PI->WriteHiHook(0x40ebf0, SPLICE_, EXTENDED_, THISCALL_, HiHook_0040ebf0);
        
        // H3AdventureManager::DeleteObjectOnMap - прячем героя перед удалением?
        _PI->WriteLoHook(0x4aa9b3, LoHook_004aa9b3);
        
        // H3AdventureManager::ProcDig - прыжок, если копание могилы
        //_PI->WriteLoHook(0x40ec5a, LoHook_0040ec5a);
        //_PI->WriteLoHook(0x40ecc5, LoHook_0040ec5a);
        //_PI->WriteLoHook(0x40edda, LoHook_0040ec5a);

        // H3AdventureManager::ProcDig - прыжок, если у героя нет свободного места в рюкзаке
        //_PI->WriteLoHook(0x40eca8, LoHook_0040eca8);

        // H3AdventureManager::ProcDig - сама логика
        //_PI->WriteLoHook(0x40f04b, LoHook_0040f04b);

        // AIMapGoal
        //_PI->WriteLoHook(0x42ef00, LoHook_0042ef00);
        //_PI->WriteLoHook(0x42f0af, LoHook_0042f0af);

        // AI_HeroMove
        //_PI->WriteLoHook(0x42ff38, LoHook_0042ff38);

        // DlgDimDoor_ProcessAction
        //_PI->WriteLoHook(0x4915b7, LoHook_004915b7);

        hookInstalled = true;
    }

    GraveExtender::GraveExtender()
        : H3ActiveObject<H3MapItemGrave>(
            "EraPlugin.GraveExtender.js",
            extender::ERA_OBJECT_TYPE,
            GRAVE_OBJECT_SUBTYPE)
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
        if (auto grave = GetFromMapItem(mapItem))
        {
            *grave = {};
            const int intMax = 8;
            const int intMin = 3;
            int range = intMax - intMin + 1;
            grave->goldAmount = (rand() % range + intMin) * 500;  // rnd 3-8 included, *500

            //v121 = Random(0, 99);
            const int v121 = rand() % 100;
            //v122 = v121 >= 50 ? 4 : 2; //minor or treasure
            const int artLvl = v121 >= 50 ? 4 : 2;
            //v123 = map_item->SetUp & 0xFF80001F | ((GameMgr_GenRandomArt_ByLevel(o_Game, v122) & 0x3FF) << 13); 13 bit of warrior tomb is artId
            const int artId = THISCALL_2(int, 0x4C9190, P_Game->Get(), artLvl) & 0x3FF; // Restrict the artNum to 1023 (10 bit info)
            grave->artifactID = artId;

            return true;
        }
        return false;
    }

    void GraveExtender::GraveDiggingProccess(H3MapItem* mapItem, H3Hero* hero) const noexcept
    {
        H3MapItemGrave graveSetup;
        std::memcpy(&graveSetup, &mapItem->setup, sizeof(H3MapItemGrave));

        int artifactID = static_cast<int>(graveSetup.artifactID);
        int goldAmount = static_cast<int>(graveSetup.goldAmount);
        int ARTIFACT_BACKPACK_SLOTS = 64;


        if (THISCALL_2(int, 0x4D9330, hero, 1) >= ARTIFACT_BACKPACK_SLOTS) // H3Hero::CountArtifactsInBackpack
            artifactID = -1;

        if (THISCALL_1(char, 0x4BAA60, P_ActivePlayer->Get())) // H3Player::IsHuman
        {
            if (artifactID < 0)
            {
                FASTCALL_12(void, 0x4F6C00,
                    this->GetDiggingMessage().String(), 1, -1, -1, 36, goldAmount,
                    -1, 0, -1, 0, -1, 0);
            }
            else
            {
                FASTCALL_12(void, 0x4F6C00,
                    this->GetDiggingMessage().String(), 1, -1, -1, 8, artifactID,
                    36, goldAmount, -1, 0, -1, 0);
            }
            int* g_TerrainToAmbientMusicId = reinterpret_cast<int*>(IntAt(0x40867E + 2));
            THISCALL_2(void, 0x59A4E0, P_SoundManager->Get(), g_TerrainToAmbientMusicId[P_AdventureManager->lastTerrain]);

        }
        THISCALL_3(void, 0x4E3870, hero, eResource::GOLD, goldAmount); // H3Hero::AddResource

        if (artifactID >= 0)
        {
            H3Artifact artifact = H3Artifact(artifactID, -1);
            THISCALL_4(char, 0x4E32E0, hero, &artifact, 1, 1); // H3Hero::AddArtifact
            if (!THISCALL_1(char, 0x4BAA60, P_ActivePlayer->Get())) // H3Player::IsHuman
                THISCALL_1(void, 0x433B40, hero); // AI::H3Hero::EquipArtifacts
        }
        // add debuff
        if (!H3MapItemGrave::IsVisitedByHero(hero))
        {
            H3MapItemGrave::SetAsVisited(hero);
            hero->moraleBonus -= MORALE_PENALTY;
        }

        // Grave deleting and hole placing proc
        H3Point coordinates = mapItem->GetCoordinates();
        int x = coordinates.x;
        int y = coordinates.y;
        int z = coordinates.z;
        H3Position pos = H3Position(x, y, z);

        // H3AdventureManager::DeleteObject
        THISCALL_4(void, 0x4AA820, P_AdventureManager->Get(), mapItem, pos, 1);

        //Game::Place_Object(o_Game, vX, vY, L, HOLE, -1, -1);
        THISCALL_7(void, 0x4C9550, P_Game->Get(), x, y, z, eObject::HOLE, -1, -1);
    }

    BOOL GraveExtender::VisitMapItem(
        H3Hero *hero, H3MapItem *mapItem, const H3Position pos,
        const BOOL isHuman) const noexcept
    {
        if (GetFromMapItem(mapItem))
        {
            if (isHuman)
            {
                FASTCALL_12(void, 0x4F6C00,
                    this->GetVisitingMessage().String(),
                    1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -777);
            }
            else
            {
                hero->movement = 0;
                GraveDiggingProccess(mapItem, hero); // AI instantly digs (cheat)
            }
            return true;
        }

        return false;
    }

    BOOL GraveExtender::SetAiMapItemWeight(
        H3MapItem* mapItem, H3Hero* hero, const H3Player* player,
        int& aiMapItemWeight, int* moveDistance, const H3Position pos) const noexcept
    {
        if (GetFromMapItem(mapItem))
        {
            aiMapItemWeight = static_cast<int>(player->aIPlayer.resourceImportance[eResource::GOLD] * 2750.0);

            // H3Hero::CountArtifactsInBackpack
            if (THISCALL_2(int, 0x4D9330, hero, 1) < 64)
            {
                aiMapItemWeight += static_cast<int>(player->aIPlayer.turnValueOfAvgArtifact);
            }

            int movement = hero->movement;
            if (*moveDistance < movement)
            {
                *moveDistance = movement;
            }
            *moveDistance += hero->CalcMaxMovement();

            return true;
        }

        return false;
    }
}
