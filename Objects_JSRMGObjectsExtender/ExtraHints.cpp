// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "pch.h"
constexpr LPCSTR EXTRA_HINT_KEY = "jsExtraHints.%d";

#pragma pack(push, 1)
struct JSDlg8Item
{
    INT32 pictureType;
    INT32 pictureSubtype;
    H3String text;
    H3String message;
    INT32 defIndex;
    INT32 field2C;
    INT32 field30;
    INT32 pictureFrameHeight;
    INT32 pictureFrameWidth;
    INT32 field3C;
    INT32 field40;
    INT32 field44;
    INT32 fullWidth;
};
#pragma pack(pop)
static_assert(sizeof(JSDlg8Item) == 0x4C, "JS dialog item layout mismatch");





_LHF_(LoHook_AdvMgr_GetObjectRightClickText)
{
    H3Game* game = P_Game->Get();
    H3MapItem* cell = reinterpret_cast<H3MapItem*>(c->ebx);
    H3Hero* hero = *reinterpret_cast<H3Hero**>(c->ebp - 0x10);
    int playerId = IntAt(0x6977DC);
    int teamId = THISCALL_2(int, 0x4A55D0, game, playerId);
    UINT8 playerBit = IntAt(c->ebp - 0x18);
    eObject type = static_cast<eObject>(cell->objectType);

    LPCSTR* g_ExtraObjectInfo = reinterpret_cast<LPCSTR*>(IntAt(0x40B395 + 2));

    if (type == eObject::STABLES)
    {
        if (H3HumanID::Get() > 7 || !IsObjectVisitedByTeam(eObject::STABLES, 0, teamId))
        {
            return EXEC_DEFAULT;
        }
        LPCSTR objName = P_ObjectName[eObject::STABLES];
        LPCSTR extraInfo = EraJS::read(H3String::Format("RMG.objectGeneration.%d.%d.text.hint", eObject::STABLES, 0).String());
        extraInfo = H3String::Format(extraInfo, H3StablesBonus::Get()).String();

        if ((*(WORD*)&cell->mirror & 0x1000) != 0 && hero)
        {
            sprintf_s(h3_TextBuffer, 0x300u, "%s\n%s\n\n%s", objName, extraInfo, P_GeneralText->GetText((hero->flags & 2) ? 354 : 355));
        }
        else
        {
            sprintf_s(h3_TextBuffer, 0x300u, "%s\n%s", objName, extraInfo);
        }

        c->return_address = 0x4159C0;
        return NO_EXEC_DEFAULT;
    }

    else if (type == eObject::UNIVERSITY)
    {
        LPCSTR objName = P_ObjectName[eObject::UNIVERSITY];

        // Object scouting - daemon_n сделал посещённость универа от 5 битов
        BOOL playerVisited = THISCALL_2(bool, 0x529B70, cell, H3HumanID::Get());

        // Стандартный хинт без посещения - добавляем строку старого расширения хинта
        if ((cell->access & 0x10) == 0 ||
            playerId > 7 ||
            !playerVisited
            )
        {
            LPCSTR extraInfo = g_ExtraObjectInfo[24];
            sprintf_s(h3_TextBuffer, 0x300u, "%s\n%s",
                objName, extraInfo);

            c->return_address = 0x4159C0;
            return NO_EXEC_DEFAULT;
        }

        // For visited there is a GEM erm code
    }

    // For RMC-hint there is a GEM erm code
    //else if (type == eObject::REFUGEE_CAMP)

    return EXEC_DEFAULT;
}

_LHF_(LoHook_AdvMgr_GetObjectCommand)
{
    // hover-hint

    H3Game* game = P_Game->Get();
    H3MapItem* cell = reinterpret_cast<H3MapItem*>(c->ebx);
    H3Hero* hero = *reinterpret_cast<H3Hero**>(c->ebp - 0x10);
    int playerId = IntAt(0x6977DC);
    int teamId = THISCALL_2(int, 0x4A55D0, game, playerId);
    UINT8 playerBit = IntAt(c->ebp - 0x14);
    eObject type = static_cast<eObject>(cell->objectType);
    LPCSTR* g_ExtraObjectInfo = reinterpret_cast<LPCSTR*>(IntAt(0x40B395 + 2));

    if (type == eObject::STABLES)
    {
        if (H3HumanID::Get() > 7 || !IsObjectVisitedByTeam(eObject::STABLES, 0, teamId))
        {
            return EXEC_DEFAULT;
        }
        LPCSTR objName = P_ObjectName[eObject::STABLES];
        LPCSTR extraInfo = EraJS::read(H3String::Format("RMG.objectGeneration.%d.%d.text.hint", eObject::STABLES, 0).String());
        extraInfo = H3String::Format(extraInfo, H3StablesBonus::Get()).String();

        if ((*(WORD*)&cell->mirror & 0x1000) != 0 && hero)
        {
            sprintf_s(h3_TextBuffer, 0x300u, "%s %s %s", objName, extraInfo, P_GeneralText->GetText((hero->flags & 2) ? 354 : 355));
        }
        else
        {
            sprintf_s(h3_TextBuffer, 0x300u, "%s %s", objName, extraInfo);
        }

        c->return_address = 0x40D09B;
        return NO_EXEC_DEFAULT;
    }

    else if (type == eObject::UNIVERSITY)
    {
        LPCSTR objName = P_ObjectName[eObject::UNIVERSITY];

        // Object scouting - daemon_n сделал посещённость универа от 5 битов
        BOOL playerVisited = THISCALL_2(bool, 0x529B70, cell, H3HumanID::Get());

        // Стандартный хинт без посещения - добавляем строку старого расширения хинта
        if ((cell->access & 0x10) == 0 ||
            playerId > 7 ||
            !playerVisited
            )
        {
            LPCSTR extraInfo = g_ExtraObjectInfo[24];
            sprintf_s(h3_TextBuffer, 0x300u, "%s %s",
                objName, extraInfo);
        }

        // Если посетили, добавляем имена скиллов
        else
        {
            int* skills = THISCALL_1(int*, 0x405DA0, cell);

            sprintf_s(h3_TextBuffer, 0x300u, "%s (%s, %s, %s, %s)",
                objName,
                P_SecondarySkillInfo[skills[0]].name,
                P_SecondarySkillInfo[skills[1]].name,
                P_SecondarySkillInfo[skills[2]].name,
                P_SecondarySkillInfo[skills[3]].name);
        }


        c->edi = (int)h3_TextBuffer;
        c->return_address = 0x40D059;
        return NO_EXEC_DEFAULT;
    }

    // For RMC-hint there is a GEM erm code
    else if (type == eObject::REFUGEE_CAMP)
    {
        if (playerId > 7 || !IsObjectVisitedByTeam(eObject::REFUGEE_CAMP, cell->objectSubtype, teamId))
        {
            return EXEC_DEFAULT;
        }
        LPCSTR objName = P_ObjectName[eObject::REFUGEE_CAMP];
        LPCSTR extraInfo = EraJS::read(H3String::Format("RMG.objectGeneration.%d.%d.text.hint", eObject::REFUGEE_CAMP, -1).String());
        sprintf_s(h3_TextBuffer, 0x300u, "%s %s", objName,
            H3String::Format(extraInfo, P_CreatureInformation->Get()[cell->objectSubtype].namePlural, cell->setup).String());
        c->edi = (int)h3_TextBuffer;

        c->return_address = 0x40D059;
        return NO_EXEC_DEFAULT;
    }

    return EXEC_DEFAULT;
}

//_LHF_(LoHook_004A5D18)
//{
//    H3Hero* hero = reinterpret_cast<H3Hero*>(c->esi);
//    ProcObjectFlagsVisitedByTeam(hero, eObject::STABLES, 0);
//
//    return EXEC_DEFAULT;
//}

void __stdcall HiHook_004A5D10(
    HiHook* h, H3AdventureManager* advMgr, H3Hero* hero,
    H3MapItem* cell, char isHuman)
{
    THISCALL_4(void, h->GetDefaultFunc(), advMgr, hero, cell, isHuman);
    ProcObjectFlagsVisitedByTeam(hero, eObject::STABLES, cell->objectSubtype);

}

void __stdcall HiHook_004A4270(
    HiHook* h, H3AdventureManager* advMgr, H3Hero* hero,
    H3MapItem* cell, char isHuman)
{
    THISCALL_4(void, h->GetDefaultFunc(), advMgr, hero, cell, isHuman);
    ProcObjectFlagsVisitedByTeam(hero, eObject::REFUGEE_CAMP, cell->objectSubtype);

}



/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////

// ХД-мод
bool isHdMod = false;

// Очки передвижения героя перед посещением объекта
int movementPts = 0;

// Посещаемая клетка
H3MapItem* object = nullptr;





void AddStringToDlgItem(JSDlg8Item* item, int picSubtype)
{
    if (picSubtype != 0)
    {
        sprintf(h3_TextBuffer, picSubtype > 0 ? "+%d" : "%d", picSubtype);
        H3String res(h3_TextBuffer);
        item->message.Append(res);
    }
}

void SetDefNameToDlgItem(JSDlg8Item* item, LPCSTR defName)
{
    UINT len = strlen(defName);
    if (THISCALL_3(char, 0x404A40, &item->text, len, 1))
    {
        item->text = defName;
    }
}











_LHF_(LoHook_004F61DD)
{
    int picType = c->edi;

    if (picType >= 37 && picType <= 43)
    {
        JSDlg8Item* item = reinterpret_cast<JSDlg8Item*>(c->ebx);
        int picSubtype = c->eax; // количество

        // +/- очки движения
        if (picType == 37 || picType == 38)
        {
            AddStringToDlgItem(item, picSubtype);
            SetDefNameToDlgItem(item, "PMOVE.def");

            item->defIndex = (picType == 37) ? 0 : 1;
        }

        // +/- радиус разведки
        else if (picType == 39 || picType == 40)
        {
            AddStringToDlgItem(item, picSubtype);
            SetDefNameToDlgItem(item, "PSCOUT.def");

            item->defIndex = (picType == 39) ? 0 : 1;
        }

        // +1 морали и удачи в одной картинке
        else if (picType == 41)
        {
            AddStringToDlgItem(item, picSubtype);
            SetDefNameToDlgItem(item, "PMANDL.def");

            item->defIndex = 0;
        }

        // pathfinding (снижение цены шага)
        else if (picType == 42)
        {
            sprintf(h3_TextBuffer, EXTRA_HINT_KEY, picType);
            H3String res(EraJS::read(h3_TextBuffer));
            item->message.Append(res);
            SetDefNameToDlgItem(item, "PTRAIL.def");

            item->defIndex = 0;
        }

        // эффект маяка (+ очки передвижения по морю)
        else if (picType == 43)
        {
            AddStringToDlgItem(item, picSubtype);
            SetDefNameToDlgItem(item, "PNAVIG.def");

            item->defIndex = 0;
        }

        c->return_address = 0x4F6229;
        return NO_EXEC_DEFAULT;
    }

    return EXEC_DEFAULT;
}

_LHF_(LoHook_004F60E7)
{
    JSDlg8Item* item = reinterpret_cast<JSDlg8Item*>(c->ebx);
    int picSubtype = c->eax; // количество
    AddStringToDlgItem(item, picSubtype);

    return EXEC_DEFAULT;
}

void __stdcall HiHook_004A8160(
    HiHook* h, H3AdventureManager* this_, H3Hero* hero,
    H3MapItem* cell, H3Position pos, bool isHuman)
{
    movementPts = hero->movement;
    object = cell;
    THISCALL_5(void, h->GetDefaultFunc(), this_, hero, cell, pos, isHuman);
}

void __stdcall HiHook_004A8904(
    HiHook* h, LPCSTR message, int messageType, int posX, int posY,
    ePictureCategories type1, int subtype1,
    ePictureCategories type2, int subtype2,
    int par, int time2Show,
    ePictureCategories type3, int subtype3)
{
    FASTCALL_12(void, h->GetDefaultFunc(),
        message, messageType, posX, posY,
        type1, ByteAt(0x4A88C8 + 2),
        38, -movementPts,
        par, time2Show, type3, subtype3);
}

void __stdcall HiHook_004A2162(
    HiHook* h, LPCSTR message, int messageType, int posX, int posY,
    ePictureCategories type1, int subtype1,
    ePictureCategories type2, int subtype2,
    int par, int time2Show,
    ePictureCategories type3, int subtype3)
{
    //H3MapitemFountainFortune setup = *reinterpret_cast<H3MapitemFountainFortune*>(&object->setup);
    int luckBonus = static_cast<int>(object->setup << 15) >> 28;
    FASTCALL_12(void, h->GetDefaultFunc(),
        message, messageType, posX, posY,
        type1, luckBonus,
        type2, subtype2,
        par, time2Show, type3, subtype3);
}

void __stdcall HiHook_004A22E3(
    HiHook* h, LPCSTR message, int messageType, int posX, int posY,
    ePictureCategories type1, int subtype1,
    ePictureCategories type2, int subtype2,
    int par, int time2Show,
    ePictureCategories type3, int subtype3)
{
    int movementPtsBonus = IntAt(0x4A2347 + 1);

    FASTCALL_12(void, h->GetDefaultFunc(),
        message, messageType, posX, posY,
        type1, 1,
        37, movementPtsBonus,
        par, time2Show, type3, subtype3);
}

void __stdcall HiHook_004A9156(
    HiHook* h, LPCSTR message, int messageType, int posX, int posY,
    ePictureCategories type1, int subtype1,
    ePictureCategories type2, int subtype2,
    int par, int time2Show,
    ePictureCategories type3, int subtype3)
{
    int movementPtsBonus = IntAt(0x698B5C);

    FASTCALL_12(void, h->GetDefaultFunc(),
        message, messageType, posX, posY,
        43, movementPtsBonus,
        type2, subtype2,
        par, time2Show, type3, subtype3);
}

void __stdcall HiHook_004A398D(
    HiHook* h, LPCSTR message, int messageType, int posX, int posY,
    ePictureCategories type1, int subtype1,
    ePictureCategories type2, int subtype2,
    int par, int time2Show,
    ePictureCategories type3, int subtype3)
{
    int movementPtsBonus = IntAt(0x4A39F4 + 1);

    FASTCALL_12(void, h->GetDefaultFunc(),
        message, messageType, posX, posY,
        type1, 1,
        37, movementPtsBonus,
        par, time2Show, type3, subtype3);
}

void __stdcall HiHook_004A41B1(
    HiHook* h, LPCSTR message, int messageType, int posX, int posY,
    ePictureCategories type1, int subtype1,
    ePictureCategories type2, int subtype2,
    int par, int time2Show,
    ePictureCategories type3, int subtype3)
{
    int movementPtsBonus = IntAt(0x4A421A + 1);

    // Хд-мод не умеет обрабатывать 3 картинки
    if (!isHdMod)
    {
        FASTCALL_12(void, h->GetDefaultFunc(),
            message, messageType, posX, posY,
            type1, 1,
            type2, 1,
            par, time2Show, 37, movementPtsBonus);
    }
    else
    {
        FASTCALL_12(void, h->GetDefaultFunc(),
            message, messageType, posX, posY,
            41, 1,
            37, movementPtsBonus,
            par, time2Show, type3, subtype3);
    }
}

void __stdcall HiHook_004A5DF0(
    HiHook* h, LPCSTR message, int messageType, int posX, int posY,
    ePictureCategories type1, int subtype1,
    ePictureCategories type2, int subtype2,
    int par, int time2Show,
    ePictureCategories type3, int subtype3)
{
    int movementPtsBonus = IntAt(0x698AE4);

    FASTCALL_12(void, h->GetDefaultFunc(),
        message, messageType, posX, posY,
        37, movementPtsBonus,
        type2, subtype2,
        par, time2Show, type3, subtype3);
}

void __stdcall HiHook_004A5E54(
    HiHook* h, LPCSTR message, int messageType, int posX, int posY,
    ePictureCategories type1, int subtype1,
    ePictureCategories type2, int subtype2,
    int par, int time2Show,
    ePictureCategories type3, int subtype3)
{
    int movementPtsBonus = IntAt(0x698AE4);

    FASTCALL_12(void, h->GetDefaultFunc(),
        message, messageType, posX, posY,
        37, movementPtsBonus,
        ePictureCategories::CREATURE, eCreature::CHAMPION,
        par, time2Show, type3, subtype3);
}

void __stdcall HiHook_004A7B91(
    HiHook* h, LPCSTR message, int messageType, int posX, int posY,
    ePictureCategories type1, int subtype1,
    ePictureCategories type2, int subtype2,
    int par, int time2Show,
    ePictureCategories type3, int subtype3)
{
    int movementPtsBonus = IntAt(0x4A7BF5 + 1);

    FASTCALL_12(void, h->GetDefaultFunc(),
        message, messageType, posX, posY,
        type1, 1,
        37, movementPtsBonus,
        par, time2Show, type3, subtype3);
}

_LHF_(LoHook_AfterHDModInit_ExtraHints)
{
    h3::H3DLL wndPlugin("hd_wog.dll");
    if (wndPlugin.dataSize)
    {
        isHdMod = true;
    }

    return EXEC_DEFAULT;
}


void InstallExtraHints(PatcherInstance *patcher) noexcept
{
    if (!patcher) return;
    // RMC hints
    patcher->WriteLoHook(0x41391C, LoHook_AdvMgr_GetObjectRightClickText);
    // mouseover hints
    patcher->WriteLoHook(0x40B158, LoHook_AdvMgr_GetObjectCommand);
    
    // Stables - player flags
    // Хотовский лоухук - плохое решение
    //patcher->WriteLoHook(0x4A5D18, LoHook_004A5D18);
    patcher->WriteHiHook(0x4A5D10, SPLICE_, EXTENDED_, THISCALL_, HiHook_004A5D10);

    // Refugee camp - player flags
    patcher->WriteHiHook(0x4A4270, SPLICE_, EXTENDED_, THISCALL_, HiHook_004A4270);



    ///////////////////////////////////////////////////////////////

    // Позже чтения текстовиков - проверяем, активен ли hd-mod
    patcher->WriteLoHook(0x4EEAF2, LoHook_AfterHDModInit_ExtraHints);

    // Расширяем кейс в конструкторе JSDlg8Item
    patcher->WriteLoHook(0x4F61DD, LoHook_004F61DD);

    // Точечно редактируем кейсы в конструкторе JSDlg8Item
    // Позитивная удача
    //patcher->WriteLoHook(0x4F60E7, LoHook_004F60E7);
    //// Позитивная мораль
    //patcher->WriteLoHook(0x4F5FEE, LoHook_004F60E7);
    //// Негативная удача
    //patcher->WriteLoHook(0x4F618D, LoHook_004F60E7);
    //// Негативная мораль
    //patcher->WriteLoHook(0x4F6094, LoHook_004F60E7);

    // Сохраняем некоторые атрибуты героя перед посещением объекта
    patcher->WriteHiHook(0x4A8160, SPLICE_, EXTENDED_, THISCALL_, HiHook_004A8160);

    

    // Обновленные b_MsgBox'ы при посещении объектов
    // Банки на - мораль, если разграблены
    //patcher->WriteByte(0x4AC210 + 1, -1);

    // Буй - 11
    patcher->WriteByte(0x4A8638 + 1, 1);

    // Лебединое Озеро - 14 (- мув поинты не влезают в 1 байт)
    patcher->WriteHiHook(0x4A8904, CALL_, EXTENDED_, FASTCALL_, HiHook_004A8904);

    // Домик Фей - 28
    patcher->WriteByte(0x4A8BBE + 1, 1);

    // Фонтан Удачи - 30 (надо извлекать биты из setup)
    //patcher->WriteHiHook(0x4A2162, CALL_, EXTENDED_, FASTCALL_, HiHook_004A2162);

    // Фонтан Молодости - 31 (+ мув поинты не влезают в 1 байт)
    patcher->WriteHiHook(0x4A22E3, CALL_, EXTENDED_, FASTCALL_, HiHook_004A22E3);

    // Идол Удачи - 38
    patcher->WriteByte(0x4A0FB4 + 1, 1);
    patcher->WriteByte(0x4A0FAA + 1, 1);
    patcher->WriteByte(0x4A1018 + 1, 1);
    patcher->WriteByte(0x4A1061 + 1, 1);

    // Маяк - 43 (+ мув поинты не влезают в 1 байт)
    patcher->WriteHiHook(0x4A9156, CALL_, EXTENDED_, FASTCALL_, HiHook_004A9156);

    // Русалки - 52
    patcher->WriteByte(0x4A9378 + 1, 1);

    // Оазис - 56 (+ мув поинты не влезают в 1 байт)
    patcher->WriteHiHook(0x4A398D, CALL_, EXTENDED_, FASTCALL_, HiHook_004A398D);

    // Флаг Единства - 64 (+ мув поинты не влезают в 1 байт, и 3 картинки)
    patcher->WriteHiHook(0x4A41B1, CALL_, EXTENDED_, FASTCALL_, HiHook_004A41B1);

    // Конюшня - 94 (+ мув поинты не влезают в 1 байт)
    // case 1 - посещение, без кавалеристов
    patcher->WriteHiHook(0x4A5DF0, CALL_, EXTENDED_, FASTCALL_, HiHook_004A5DF0);
    // case 3 - посещение, с кавалеристами
    patcher->WriteHiHook(0x4A5E54, CALL_, EXTENDED_, FASTCALL_, HiHook_004A5E54);

    // Храм - 96
    // На оба кейса один колл
    patcher->WriteByte(0x4A5F43 + 1, 1);
    patcher->WriteByte(0x4A5F0A + 1, 1);

    // Гробница Воина - 108 (у хд-мода захардкоженная внутри мода логика)
    /*patcher->WriteByte(0x4A79F3 + 1, -3);
    patcher->WriteByte(0x4A79EB + 1, -1);
    patcher->WriteByte(0x4A79E0 + 1, -1);*/

    // Водоем - 110 (+ мув поинты не влезают в 1 байт)
    patcher->WriteHiHook(0x4A7B91, CALL_, EXTENDED_, FASTCALL_, HiHook_004A7B91);
}
