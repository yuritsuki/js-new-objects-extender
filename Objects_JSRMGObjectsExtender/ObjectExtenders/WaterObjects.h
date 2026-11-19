#pragma once

#pragma pack(push, 4)
struct HotaReplayActionBase : H3Allocator
{
    // +00: vptr создаёт компилятор; исходная таблица HotA — 0x101EC250.

    // +00: размер сериализованных данных (kind + payload).
    virtual UINT32 GetSerializedSize() = 0;

    // +04, 10159110: удаляющий деструктор.
    virtual HotaReplayActionBase* vDelete(UINT32 flags)
    {
        this->~HotaReplayActionBase();
        if (flags & 1)
            H3Free(this);
        return this;
    }

    // +08: внутренний тип действия HotA.
    virtual INT32 GetKind() = 0;
    // +0C: чтение payload; второй стековый аргумент — версия сохранения.
    virtual BOOL8 Read(H3GzFile* file, INT32 saveVersion) = 0;
    // +10: запись payload; общий заголовок пишет вложенный интерфейс SoD.
    virtual BOOL8 Write(H3GzFile* file) = 0;
    // +14: воспроизведение действия.
    virtual INT32 Redo(BOOL8 redraw) = 0;
    // +18: отмена действия.
    virtual void Undo() = 0;

    H3ReplayAction action; // +04; размер базового объекта — 0x0C.

    // 10144BF0: инициализация интерфейса и выбор действующего игрока.
    explicit HotaReplayActionBase(INT8 player)
    {
        // Локальный аналог 10203514. Эти шесть слотов вызываются кодом SoD,
        // а семь C++-методов выше — через первичную таблицу владельца.
        static h3func interfaceVTable[] =
        {
            reinterpret_cast<h3func>(&InterfaceDelete),
            reinterpret_cast<h3func>(&InterfaceGetType),
            reinterpret_cast<h3func>(&InterfaceRead),
            reinterpret_cast<h3func>(&InterfaceWrite),
            reinterpret_cast<h3func>(&InterfaceRedo),
            reinterpret_cast<h3func>(&InterfaceUndo)
        };
        action.vTable = interfaceVTable;
        action.playerId = player;
        action._padding[0] = action._padding[1] = action._padding[2] = 0;
        if (player >= 0 && player < 8)
            return;

        H3Main* game = P_Main->Get();
        action.playerId = -1;
        for (INT32 id = 7; id >= 0; --id)
        {
            if (!game->players[id].isHuman && !game->isDead[id])
            {
                action.playerId = static_cast<INT8>(id);
                break;
            }
        }
        if (action.playerId < 0)
        {
            for (INT32 id = 7; id >= 0; --id)
            {
                if (game->players[id].isHuman && !game->isDead[id])
                {
                    action.playerId = static_cast<INT8>(id);
                    break;
                }
            }
        }
    }

    // 10095E00: выбор игрока/героя при воспроизведении.
    void SelectHero(INT32 heroId, BOOL8 redraw);

    // Невиртуальный: в таблице должно остаться ровно семь указанных слотов.
    ~HotaReplayActionBase() {}

private:
    // __fastcall принимает указатель интерфейса в ECX; EDX не используется.
    // Стековые аргументы совпадают с __thiscall-контрактом H3ReplayAction.
    // 10164000: удаление владельца по указателю на вложенный интерфейс.
    static void* __fastcall InterfaceDelete(
        H3ReplayAction* self, void*, UINT32 flags)
    {
        HotaReplayActionBase* holder = self
            ? reinterpret_cast<HotaReplayActionBase*>(reinterpret_cast<BYTE*>(self) - 4)
            : nullptr;
        if (!(flags & 1))
            return holder->vDelete(0);
        return holder ? holder->vDelete(1) : nullptr;
    }

    // 1002ACE0: внешний тип записи; 12 — расширение перечисления SoD.
    static eReplayAction __fastcall InterfaceGetType(H3ReplayAction*, void*)
    {
        return static_cast<eReplayAction>(12);
    }

    // 10164070: заголовок записи уже прочитан загрузчиком.
    static BOOL8 __fastcall InterfaceRead(
        H3ReplayAction* self, void*, H3GzFile* file, INT32 saveVersion)
    {
        HotaReplayActionBase* holder = self
            ? reinterpret_cast<HotaReplayActionBase*>(reinterpret_cast<BYTE*>(self) - 4)
            : nullptr;
        return holder->Read(file, saveVersion);
    }

    // 10175180: общий заголовок, затем payload конкретного действия.
    static BOOL8 __fastcall InterfaceWrite(
        H3ReplayAction* self, void*, H3GzFile* file)
    {
        HotaReplayActionBase* holder = self
            ? reinterpret_cast<HotaReplayActionBase*>(reinterpret_cast<BYTE*>(self) - 4)
            : nullptr;
        const INT8 playerId = holder->action.playerId;
        const INT32 kind = holder->GetKind();
        const UINT32 serializedSize = holder->GetSerializedSize();
        return file->Write(playerId)
            && file->Write(serializedSize)
            && file->Write(kind)
            && holder->Write(file);
    }

    // 101640A0: передаём redraw методу владельца.
    static INT32 __fastcall InterfaceRedo(
        H3ReplayAction* self, void*, BOOL8 redraw)
    {
        HotaReplayActionBase* holder = self
            ? reinterpret_cast<HotaReplayActionBase*>(reinterpret_cast<BYTE*>(self) - 4)
            : nullptr;
        return holder->Redo(redraw);
    }

    // 101640B0.
    static void __fastcall InterfaceUndo(H3ReplayAction* self, void*)
    {
        HotaReplayActionBase* holder = self
            ? reinterpret_cast<HotaReplayActionBase*>(reinterpret_cast<BYTE*>(self) - 4)
            : nullptr;
        holder->Undo();
    }
};

#pragma pack(pop)

UINT32 __stdcall WriteHotaReplayPosition(H3Position* position, H3GzFile* file);
extern bool g_ReplayIntermediateRedraw;
void WaterObjects();
