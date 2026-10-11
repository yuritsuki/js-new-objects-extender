// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "../pch.h"



// Временное хранение клетки.
H3MapItem* TempCell = nullptr;





// Можно ли зайти в триггер объекта
BOOL8 TriggerIsUnmoveable(H3GlobalObjectSettings* objSettings, int terrain)
{
    // 10085530 (hota.dll version 1.7.3)

    if (terrain != eTerrain::WATER)
    {
        return objSettings->cannotEnter;
    }

    // На воде у большинства объектов несещаемые триггеры.
    switch (objSettings->objectID)
    {
    case 0: // Пустое место
    case 3: // Якорь
    case 8: // Лодка
    case 9: // Страж границы
    case 26: // Событие
    case 33: // Гарнизон
    case 34: // Герой
    case 36: // Грааль
    case 43: // Монолит входа
    case 44: // Монолит выхода
    case 45: // Двухсторонний монолит
    case 54: // Монстр
    case 70: // Случайный герой
    case 71: // Случайный монстр
    case 72: // Случайный монстр 1
    case 73: // Случайный монстр 2
    case 74: // Случайный монстр 3
    case 75: // Случайный монстр 4
    case 77: // Случайный город
    case 80: // Святилище
    case 98: // Город
    case 103: // Врата подземного мира
    case 111: // Водоворот
    case 162: // Случайный монстр 5
    case 163: // Случайный монстр 6
    case 164: // Случайный монстр 7
    case 212: // Пограничные ворота
    case 214: // Лагерь героя
    case 215: // Страж задания
        return objSettings->cannotEnter;
    default:
        return true;
        break;
    }
}


_LHF_(LoHook_WaterUnmoveableTriggers_1)
{
    H3MapItem* cell = reinterpret_cast<H3MapItem*>(c->ebx);
    H3GlobalObjectSettings* type = reinterpret_cast<H3GlobalObjectSettings*>(c->edx + c->edi);

    c->return_address = TriggerIsUnmoveable(type, cell->land) != 0
        ? 0x40E24D
        : 0x40E240;

    return NO_EXEC_DEFAULT;
}

_LHF_(LoHook_WaterUnmoveableTriggers_2)
{
    H3MapItem* cell = reinterpret_cast<H3MapItem*>(c->ebx);
    H3GlobalObjectSettings* type = reinterpret_cast<H3GlobalObjectSettings*>(c->ecx + c->edx);

    c->return_address = TriggerIsUnmoveable(type, cell->land) != 0
        ? 0x42E263
        : 0x42E26C;

    return NO_EXEC_DEFAULT;
}

_LHF_(LoHook_WaterUnmoveableTriggers_3)
{
    H3MapItem* cell = reinterpret_cast<H3MapItem*>(c->ebx);
    H3GlobalObjectSettings* type = reinterpret_cast<H3GlobalObjectSettings*>(c->eax + c->ecx);

    char result = TriggerIsUnmoveable(type, cell->land) != 0;

    c->return_address = 8 * result + 0x42E34F;
    return NO_EXEC_DEFAULT;
}

_LHF_(LoHook_WaterUnmoveableTriggers_SaveTempCell)
{
    TempCell = reinterpret_cast<H3MapItem*>(c->eax);

    return EXEC_DEFAULT;
}

_LHF_(LoHook_WaterUnmoveableTriggers_4)
{
    H3GlobalObjectSettings* type = reinterpret_cast<H3GlobalObjectSettings*>(c->edx + 16 * c->ecx);

    c->return_address = TriggerIsUnmoveable(type, TempCell->land) != 0
        ? 0x42F9C7
        : 0x42FA70;

    return NO_EXEC_DEFAULT;
}

_LHF_(LoHook_WaterUnmoveableTriggers_5)
{
    H3GlobalObjectSettings* type = reinterpret_cast<H3GlobalObjectSettings*>(c->eax + c->ecx);

    c->return_address = TriggerIsUnmoveable(type, TempCell->land) != 0
        ? 0x42FB61
        : 0x42FAF2;

    return NO_EXEC_DEFAULT;
}

_LHF_(LoHook_WaterUnmoveableTriggers_6)
{
    H3MapItem* cell = reinterpret_cast<H3MapItem*>(c->ebx);
    H3GlobalObjectSettings* type = reinterpret_cast<H3GlobalObjectSettings*>(c->eax + c->ecx);

    c->return_address = TriggerIsUnmoveable(type, cell->land) != 0
        ? 0x48085A
        : 0x4805D5;

    return NO_EXEC_DEFAULT;
}

_LHF_(LoHook_WaterUnmoveableTriggers_7)
{
    H3MapItem* cell = reinterpret_cast<H3MapItem*>(c->ebx);
    H3GlobalObjectSettings* type = reinterpret_cast<H3GlobalObjectSettings*>(c->eax + c->ecx);
    
    c->return_address = TriggerIsUnmoveable(type, cell->land) != 0
        ? 0x480E15
        : 0x480E48;

    return NO_EXEC_DEFAULT;
}

_LHF_(LoHook_WaterUnmoveableTriggers_8)
{
    H3MapItem* cell = reinterpret_cast<H3MapItem*>(c->edi);

    H3GlobalObjectSettings* type = reinterpret_cast<H3GlobalObjectSettings*>(c->edx + c->ecx);

    if (TriggerIsUnmoveable(type, cell->land))
    {
        c->return_address = 0x4B2A9D;
    }
    else
    {
        c->return_address = 0x4B2A6D;
    }

    return NO_EXEC_DEFAULT;
}

_LHF_(LoHook_WaterUnmoveableTriggers_9)
{
    H3MapItem* cell = reinterpret_cast<H3MapItem*>(c->eax);
    H3GlobalObjectSettings* type = &P_GlobalObjectSettings[cell->objectType];

    c->return_address = TriggerIsUnmoveable(type, cell->land) != 0
        ? 0x4E602B
        : 0x4E6031;

    return NO_EXEC_DEFAULT;
}

_LHF_(LoHook_WaterUnmoveableTriggers_SaveEdi)
{
    TempCell = reinterpret_cast<H3MapItem*>(c->edi);

    return EXEC_DEFAULT;
}

_LHF_(LoHook_WaterUnmoveableTriggers_10)
{
    H3GlobalObjectSettings* type = reinterpret_cast<H3GlobalObjectSettings*>(c->edx + c->ecx);

    if (TriggerIsUnmoveable(type, TempCell->land))
    {
        c->eax = (c->eax & ~0xFF);
    }
    else
    {
        c->eax = (c->eax & ~0xFF) | 1;
    }

    return NO_EXEC_DEFAULT;
}


void WaterObjectsPassability()
{
    // Непроходимые клетки на воде (1).
    _PI->WriteLoHook(0x40E23A, LoHook_WaterUnmoveableTriggers_1);
    // Непроходимые клетки на воде (2).
    _PI->WriteLoHook(0x42E25D, LoHook_WaterUnmoveableTriggers_2);
    // Непроходимые клетки на воде (3).
    _PI->WriteLoHook(0x42E349, LoHook_WaterUnmoveableTriggers_3);
    // Сохраняем клетку.
    _PI->WriteLoHook(0x42F9A2, LoHook_WaterUnmoveableTriggers_SaveTempCell);
    // Непроходимые клетки на воде (4).
    _PI->WriteLoHook(0x42F9B8, LoHook_WaterUnmoveableTriggers_4);
    // Непроходимые клетки на воде (5).
    _PI->WriteLoHook(0x42FAEC, LoHook_WaterUnmoveableTriggers_5);
    // Непроходимые клетки на воде (6).
    _PI->WriteLoHook(0x480850, LoHook_WaterUnmoveableTriggers_6);
    // Непроходимые клетки на воде (7).
    _PI->WriteLoHook(0x480E0F, LoHook_WaterUnmoveableTriggers_7);
    // Непроходимые клетки на воде (8).
    _PI->WriteLoHook(0x4B2A67, LoHook_WaterUnmoveableTriggers_8);
    // Непроходимые клетки на воде (9).
    _PI->WriteLoHook(0x4E6019, LoHook_WaterUnmoveableTriggers_9);
    // Непроходимые клетки на воде (сохраняем edi).
    _PI->WriteLoHook(0x56AE2E, LoHook_WaterUnmoveableTriggers_SaveEdi);
    // Непроходимые клетки на воде (10).
    _PI->WriteLoHook(0x56B207, LoHook_WaterUnmoveableTriggers_10);
}




////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

// Максимальное количество лодок (ограничено 1 байтом)
constexpr UINT8 MAX_BOATS_COUNT = 255;

// Является ли текущий диалог диалогом таверны на воде.
bool IsWaterTavern = false;

// Пока не ясно, что это
bool IsBoatCreation = false;

// Ключ для сообщения о превышении количества лодок при посещении водной тюрьмы
constexpr LPCSTR noAvalBoats_key = "RMG.objectGeneration.%d.%d.text.noAvalBoats";

// Подтип лодки, построенной у другого игрока.
int NetMsg_BoatSubt;






// Посадка героя в лодку на том месте, где он стоит (герой должен быть на карте и не в лодке, если лодки там же нет, она создаётся, если есть - она должна там стоять с начала игры).
int HeroPlaceToBoat(H3Game* game, H3Hero* hero, char boatSubtype, H3Position coords, char playerId, bool notAddToReplayTurn)
{
    // Преобразуем координаты.
    int x, y, z;
    H3Position::UnpackXYZ(coords, x, y, z);

    // Если надо - добавляем информацию для просмотра хода и оппонента онлайн.
    //if (!notAddToReplayTurn)
    //{
    //    sub_10145120(game, 4);

    //    // Делаем это только при постановке самого героя.

    //    // Информация для оппонента.
    //    //PlaceBoat_SendMsg(game, coords, o_ActivePlayerID, boat_subtype);

    //    // Добавляем информмацию для просмотра хода.
    //    //CALL_3(void, __thiscall, 0x49C570, this, boat, coords);
    //}

    IsBoatCreation = true;
    int boat_ix = THISCALL_7(int, 0x4BAF10, game, x, y, z, playerId, notAddToReplayTurn, boatSubtype);
    IsBoatCreation = false;

    // Не удалсь создать лодку.
    if (boat_ix < 0)
    {
        return 0;

        //H3Boat boatObj;
        //THISCALL_1(void, 0x4D76E0, &boatObj); // H3Boat::Ctor
        //boatObj.exists = 0;
        //game->boats.Insert(game->boats.end(), boatObj);

        //IsBoatCreation = true;
        //int boat_ix = THISCALL_7(int, 0x4BAF10, game, x, y, z, playerId, notAddToReplayTurn, boatSubtype);
        //IsBoatCreation = false;

        //if (boat_ix < 0)
        //{
        //    return 0;
        //}
    }

    H3Boat* boat = &game->boats[boat_ix];

    // Сажаем героя в лодку.
    // Убираем лодку (или героя) из информации клетки.
    THISCALL_1(void, 0x4D7950, boat); // H3Hero::Hide

    //// Если надо - добавляем информацию для просмотра хода и оппонента онлайн.
    //if (!notAddToReplayTurn)
    //{
    //    MEMORY[0x49C1D0](game, boat, 1, hero->id); // H3Game::ReplayAction_HideBoat
    //    sub_10145120(game, 2);
    //    v11 = 1;
    //    MEMORY[0x49C790](game, hero, playerId, coords, v11);// H3Game::ReplayAction_PlaceHero
    //}

    // Настраиваем героя.
    hero->flags |= 0x40000; // Герой в лодке
    hero->flyPower = -1; // Герой больше не в полёте
    hero->waterwalkPower = -1; // Герой больше не ходит по воде

    // Пересчитываем ходьбу героя в водную (new_mp = old_mp*new_max/old_max).
    if (!(hero->flags & 0x1000000))
    {
        int new_max = THISCALL_2(int, 0x4E4C00, hero, true);
        if (hero->maxMovement)
        {
            hero->movement = hero->movement * new_max / hero->maxMovement;
        }
        else
        {
            hero->movement = 0;
        }
        hero->maxMovement = new_max;
    }

    // Настраиваем лодку.
    boat->heroId = hero->id; // Герой в лодке
    boat->hasHero = true; // Внутри есть герой
    boat->owner = playerId; // Владелец лодки

    return true;
}

// Отсылка информации о ставящейся лодке.
void PlaceBoat_SendMsg(H3Game* game, _dword_ coords, int playerId, int boatSubtype)
{
    // Информация для оппонента.
    _dword_ net_msg[7 + 1];
    net_msg[0] = -1;
    net_msg[1] = 0;
    net_msg[2] = 1057;
    net_msg[3] = 28 + 4;
    net_msg[4] = 0;
    net_msg[5] = coords;
    net_msg[6] = playerId;
    net_msg[7] = boatSubtype; // Подтип

    // Отсылаем информацию.
    THISCALL_1(void, 0x481CB0, net_msg); // H3NetworkData::SendShortCommand
}

// Отсылка информации о ставящемся герое.
void PlaceHero_SendMsg(H3Game* game, H3Hero* hero, _dword_ coords, int playerId)
{
    // Подтип лодки.
    int boat_subtype = -1;

    // Герой в лодке.
    if (hero->flags & 0x40000)
    {
        // Лодка героя.
        H3Boat* boat = THISCALL_3(H3Boat*, 0x4CE5C0, game, hero->id, true);

        // Лодка есть - берём подтип.
        if (boat)
        {
            boat_subtype = boat->par1;
        }
    }

    // Информация для оппонента.
    _dword_ net_msg[8 + 1];
    net_msg[0] = -1;
    net_msg[1] = 0;
    net_msg[2] = 1060;
    net_msg[3] = 32 + 4;
    net_msg[4] = 0;
    net_msg[5] = hero->id;
    net_msg[6] = coords;
    net_msg[7] = playerId;
    net_msg[8] = boat_subtype; // Подтип лодки (-1 - нет лодки)

    // Отсылаем информацию.
    THISCALL_1(void, 0x481CB0, net_msg);
}










_LHF_(LoHook_004BAF82)
{
    // Максимум 254 до вставки лодки в вектор
    if (c->edx >= MAX_BOATS_COUNT)
    {
        // В return -1
        c->return_address = 0x4BAFCB;
    }
    // Перепрыгиваем 5 байт в любом случае, чтобы не выполнять стандартную инструкцию
    return NO_EXEC_DEFAULT;
}

// При посещении водной таверны, запоминаем, что посещение длится сейчас.
void __stdcall HiHook_ObjTavernVisit_SetTavernIfWater(HiHook* h, _dword_ coords)
{
    // Определяем водную таверну (по клетке, где она стоит).
    if (THISCALL_2(H3MapItem*, 0x412B30, P_AdventureManager->Get(), coords)->land == 8)
    {
        IsWaterTavern = true;
    }

    // Обрабатываем посещение таверны.
    STDCALL_1(void, h->GetDefaultFunc(), coords);

    // Посещение водной таверны завершено.
    IsWaterTavern = false;
}

// Запрещаем покупку героя при отсутствии лодок.
_LHF_(LoHook_TavernDialog_RestrictIfNoBoats)
{
    // Если таверна на воде и лодок уже максимальное число, запрещаем покупку героя.
    if (IsWaterTavern && THISCALL_1(int, 0x4CCA90, P_Game->Get()) >= MAX_BOATS_COUNT)
    {
        c->return_address = 0x5D8624;
        return NO_EXEC_DEFAULT;
    }
    // Иначе - как обычно.
    else
    {
        return EXEC_DEFAULT;
    }
}

// При создании героя во внешней таверне сажаем его в лодку, если это на воде.
void __stdcall HiHook_PlaceHeroWater_ObjTavern(
    HiHook* h, H3Hero* this_, int playerId, _dword_ coords, char resetFlags)
{
    // Клетка - водная...
    if (THISCALL_2(H3MapItem*, 0x412B30, P_AdventureManager->Get(), coords)->land == 8)
    {
        // Посадка героя в лодку.
        HeroPlaceToBoat(P_Game->Get(), this_, 1, coords, playerId, false);

        if (resetFlags)
        {
            // Сбрасываем флаг таверны.
            this_->flags &= ~0x20000;

            // В самой постановке не будем сбрасывать флаги.
            resetFlags = false;
        }
    }

    // Создание героя.
    THISCALL_4(void, h->GetDefaultFunc(), this_, playerId, coords, resetFlags);
}

// При создании случайного героя в городе на старте карты сажаем его в лодку, если это на воде.
void __stdcall HiHook_PlaceHeroWater(
    HiHook* h, H3Hero* this_, int playerId, _dword_ coords, char resetFlags)
{
    // Клетка - водная...
    if (THISCALL_2(H3MapItem*, 0x412B30, P_AdventureManager->Get(), coords)->land == 8)
    {
        // Посадка героя в лодку.
        HeroPlaceToBoat(P_Game->Get(), this_, 1, coords, playerId, false);

        // Не стираем флаг лодки.
        if (resetFlags)
        {
            // Сбрасываем флаг таверны.
            this_->flags &= ~0x20000;

            // В самой постановке не будем сбрасывать флаги.
            resetFlags = false;
        }
    }

    // Создание героя.
    THISCALL_4(void, h->GetDefaultFunc(), this_, playerId, coords, resetFlags);
}

// При появлении героя из лагеря героя на воде, помещаем его в лодку.
_LHF_(LoHook_ReplacePlaceHolderWithHero_Boat)
{
    // Герой.
    H3Hero* hero = (H3Hero*)c->ebx;

    // Если герой стоит на воде, сажаем его в лодку.
    if (THISCALL_4(H3MapItem*, 0x4086D0, P_AdventureManager->map, hero->x, hero->y, hero->z)->land == 8)
    {
        HeroPlaceToBoat(P_Game->Get(), hero, 1, hero->mixedPosition, hero->owner, true);
        // Лодка героя.
        H3Boat* boat = THISCALL_3(H3Boat*, 0x4CE5C0, P_Game->Get(), hero->id, true);

        // Ставим героя.
        THISCALL_3(void, 0x4D7840, hero, 34, hero->id);
    }

    return EXEC_DEFAULT;
}

// При загрузке героя на воде, помещаем его в лодку.
_LHF_(LoHook_LoadHero_Boat)
{
    // Герой.
    H3Hero* hero = (H3Hero*)c->esi;

    // Если герой стоит на воде, сажаем его в лодку.
    if (THISCALL_4(H3MapItem*, 0x4086D0, P_AdventureManager->map, hero->x, hero->y, hero->z)->land == 8)
    {
        HeroPlaceToBoat(P_Game->Get(), hero, 1, hero->mixedPosition, hero->owner, true);
        // Лодка героя.
        H3Boat* boat = THISCALL_3(H3Boat*, 0x4CE5C0, P_Game->Get(), hero->id, true);

        // Ставим героя.
        THISCALL_3(void, 0x4D7840, hero, 34, hero->id);
    }

    return EXEC_DEFAULT;
}

// Модифицируем расчёт ИИ ценности тюрьмы.
int __stdcall HiHook_AI_GetPrisonValue(HiHook* h, H3MapItem* cell, DWORD* player)
{
    // Если тюрьма на воде и лодок уже максимальное число, героя купить нельзя и объект посещать не нужно.
    if (cell->land == 8 && THISCALL_1(int, 0x4CCA90, P_Game->Get()) >= MAX_BOATS_COUNT)
    {
        return 0;
    }
    // Иначе - стандартный расчёт ценности.
    else
    {
        return FASTCALL_2(int, h->GetDefaultFunc(), cell, player);
    }
}

// При посещении тюрьмы запрещаем покупку героя, если уже есть 64 лодки.
void __stdcall HiHook_Prison_Visit(
    HiHook* h, H3AdventureManager* this_, H3Hero* hero, H3MapItem* cell, 
    _dword_ coords, bool isHuman)
{
    // Если тюрьма на воде и лодок уже максимальное число, героя купить нельзя.
    if (cell->land == 8 && THISCALL_1(int, 0x4CCA90, P_Game->Get()) >= MAX_BOATS_COUNT)
    {
        // Если это не ИИ, показываем сообщение.
        if (isHuman)
        {
            //noAvalBoats_key
            LPCSTR dlgHint = EraJS::read(
                H3String::Format(
                    noAvalBoats_key,
                    eObject::PRISON, 0).String());

            FASTCALL_12(void, 0x4F6C00, dlgHint, 1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -777);
        }
    }
    // Иначе - стандартное посещение.
    else
    {
        THISCALL_5(void, h->GetDefaultFunc(), this_, hero, cell, coords, isHuman);
    }
}

// При появлении героя из тюрьмы, сажаем его в лодку, если надо.
_LHF_(LoHook_PrisonCreateHero)
{
    // Герой.
    H3Hero* hero = P_Game->GetHero(c->ebx);

    // Координаты постановки героя.
    _dword_ coords = DwordAt(c->ebp + 16);

    // Если герой стоит на воде, сажаем его в лодку.
    if (THISCALL_2(H3MapItem*, 0x412B30, P_AdventureManager->Get(), coords)->land == 8)
    {
        HeroPlaceToBoat(P_Game->Get(), hero, 1, coords, IntAt(0x69CCF4), false);
    }

    return EXEC_DEFAULT;
}

// При появлении героя из тюрьмы, сбрасываем у него все флаги, кроме флага лодки.
_LHF_(LoHook_PrisonCreateHero_ResetFlags)
{
    H3Hero* hero = (H3Hero*)c->esi;
    // Сбрасываем все флаги, кроме флага лодки.
    hero->flags &= 0x40000u;

    return NO_EXEC_DEFAULT;
}

// Отправляем сообщение нового формата при постройке лодки.
_LHF_(LoHook_CreateBoat_NetMsg)
{
    if (!IsBoatCreation)
    {
        // Отсылаем сообщение.
        PlaceBoat_SendMsg(P_Game->Get(), c->edi, IntAt(0x69CCF4), ByteAt(c->ebp + 28));
    }

    c->return_address = 0x4BB054;
    return NO_EXEC_DEFAULT;
}

// Берём подтип лодки из сообщения.
_LHF_(LoHook_CreateBoat_NetMsg_GetSubtype)
{
    // Заменяем параметр.
    NetMsg_BoatSubt = IntAt(c->esi + 28);

    return EXEC_DEFAULT;
}

// При создании лодки от удалённого игрока заменяем подтип лодки.
int __stdcall HiHook_PlaceBoat_NetMsg_SetSubtype(HiHook* h, H3Game* this_, INT16 x, INT16 y, INT16 z, INT8 player_id, BOOL8 not_add_to_replay, INT8 subtype)
{
    // Заменяем подтип лодки.
    return THISCALL_7(int, h->GetDefaultFunc(), this_, x, y, z, player_id, 0, NetMsg_BoatSubt);
}

// Отправляем сообщение нового формата при постановке героя.
_LHF_(LoHook_PlaceHero_NetMsg)
{
    // Отсылаем сообщение.
    PlaceHero_SendMsg(P_Game->Get(), (H3Hero*)c->esi, DwordAt(c->ebp + 12), DwordAt(c->ebp + 8));

    c->return_address = 0x4D7C82;
    return NO_EXEC_DEFAULT;
}

// Отправляем сообщение нового формата при постановке героя из тюрьмы.
_LHF_(LoHook_PlaceHero_NetMsg_Prison)
{
    H3Game* game = P_Game->Get();
    // Отсылаем сообщение.
    PlaceHero_SendMsg(game, game->GetHero(c->ebx), DwordAt(c->ebp + 16), IntAt(0x69CCF4));

    c->return_address = 0x4A3E7F;
    return NO_EXEC_DEFAULT;
}

// Берём подтип лодки из сообщения.
_LHF_(LoHook_PlaceHero_NetMsg_InBoat)
{
    H3Hero* hero = (H3Hero*)c->eax;
    int boat_subt = IntAt(c->esi + 32);

    // Герой не в лодке.
    if (boat_subt == -1)
    {
        hero->flags = 0;
    }
    // Герой будет в лодке, но её надо ещё создать.
    else
    {
        hero->flags &= 0x40000u;
        if ((hero->flags & 0x40000) == 0)
        {
            HeroPlaceToBoat(P_Game->Get(), hero, boat_subt, IntAt(c->esi + 24), IntAt(c->esi + 28), false);
        }
    }

    return EXEC_DEFAULT;
}

// Учитываем возможность нахождения героя в лодке при создании информации для просмотра хода.
void __stdcall HiHook_PlaceHero_AddToReplay(
    HiHook* h, H3Game* this_, H3Hero* hero, 
    INT8 player_id, _dword_ coords, bool place_in_boat)
{
    // Если герой в лодке - при просмотре он будет ставиться в лодке.
    THISCALL_5(void, h->GetDefaultFunc(), this_, hero, player_id, 
        coords, ((hero->flags & 0x40000) ? true : place_in_boat));
}


void WaterTavernsAndPrisons()
{

    // Увеличиваем лимит лодок с 64 до 255 - нельзя, трехбайтовая инструкция,
    // максимальный положительный imm8, корректно представимый через 83 /7, — 0x7F
    //_PI->WriteByte(0x4BAF82 + 2, 0xff);
    _PI->WriteLoHook(0x4BAF82, LoHook_004BAF82);

    // При посещении водной таверны, запоминаем, что посещение длится сейчас.
    _PI->WriteHiHook(0x5d8290, SPLICE_, EXTENDED_, STDCALL_, HiHook_ObjTavernVisit_SetTavernIfWater);

    // Запрещаем покупку героя при отсутствии лодок.
    _PI->WriteLoHook(0x5d860d, LoHook_TavernDialog_RestrictIfNoBoats);

    // H3Hero::PlaceInMap
    // При создании героя внешней таверной сажаем его в лодку, если это на воде.
    _PI->WriteHiHook(0x4d7b4b, CALL_, EXTENDED_, THISCALL_, HiHook_PlaceHeroWater_ObjTavern);

    // При создании случайного героя в городе сажаем его в лодку, если это на воде.
    _PI->WriteHiHook(0x5c1827, CALL_, EXTENDED_, THISCALL_, HiHook_PlaceHeroWater);

    // При появлении героя из лагеря героя на воде, помещаем его в лодку.
    _PI->WriteLoHook(0x4868d6, LoHook_ReplacePlaceHolderWithHero_Boat);

    // При загрузке героя на воде, помещаем его в лодку.
    _PI->WriteLoHook(0x4cac15, LoHook_LoadHero_Boat);

    // Модифицируем расчёт ИИ ценности тюрьмы.
    _PI->WriteHiHook(0x52a880, SPLICE_, EXTENDED_, FASTCALL_, HiHook_AI_GetPrisonValue);

    // При посещении тюрьмы запрещаем покупку героя, если уже есть max лодки.
    _PI->WriteHiHook(0x4a3b20, SPLICE_, EXTENDED_, THISCALL_, HiHook_Prison_Visit);

    // При появлении героя из тюрьмы, сажаем его в лодку, если надо.
    _PI->WriteLoHook(0x4a3c54, LoHook_PrisonCreateHero);

    // При появлении героя из тюрьмы, сбрасываем у него все флаги, кроме флага лодки.
    _PI->WriteLoHook(0x4a3d64, LoHook_PrisonCreateHero_ResetFlags);

    // Отправляем сообщение нового формата при постройке лодки.
    _PI->WriteLoHook(0x4bb027, LoHook_CreateBoat_NetMsg);

    // Берём подтип лодки из сообщения.
    _PI->WriteLoHook(0x481a04, LoHook_CreateBoat_NetMsg_GetSubtype);

    // При создании лодки от удалённого игрока заменяем подтип лодки.
    _PI->WriteHiHook(0x481a3d, CALL_, EXTENDED_, THISCALL_, HiHook_PlaceBoat_NetMsg_SetSubtype);

    // Отправляем сообщение нового формата при постановке героя.
    _PI->WriteLoHook(0x4d7c4b, LoHook_PlaceHero_NetMsg);

    // Отправляем сообщение нового формата при постановке героя из тюрьмы.
    _PI->WriteLoHook(0x4a3e53, LoHook_PlaceHero_NetMsg_Prison);

    // Берём подтип лодки из сообщения.
    _PI->WriteLoHook(0x481b15, LoHook_PlaceHero_NetMsg_InBoat);

    // Стираем собственную настройку временных модификаторов флагов.
    _PI->WriteCodePatch(0x481b47, "%n", 10);

    // Учитываем возможность нахождения героя в лодке при создании информации для просмотра хода.
    _PI->WriteHiHook(0x4D7B99, CALL_, EXTENDED_, THISCALL_, HiHook_PlaceHero_AddToReplay); // Постановка героя
    _PI->WriteHiHook(0x4A3CBB, CALL_, EXTENDED_, THISCALL_, HiHook_PlaceHero_AddToReplay); // Освобождение из тюрьмы

}




void WaterObjects()
{
    WaterObjectsPassability();
    WaterTavernsAndPrisons();
}
