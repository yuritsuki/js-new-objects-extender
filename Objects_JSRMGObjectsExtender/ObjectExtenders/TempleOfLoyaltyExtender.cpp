#include "../pch.h"
#include "TempleOfLoyaltyExtender.h"


namespace templeOfLoyalty
{
    BOOL H3MapItemTempleOfLoyalty::IsVisitedByHero(H3Hero* hero) noexcept
    {
        sprintf(newMapItem::buffer, ErmVariableFormat, hero->id);
        return Era::GetAssocVarIntValue(newMapItem::buffer);
    }
    void H3MapItemTempleOfLoyalty::SetAsVisited(H3Hero* hero) noexcept
    {
        sprintf(newMapItem::buffer, ErmVariableFormat, hero->id);
        Era::SetAssocVarIntValue(newMapItem::buffer, 1);
    }
    void H3MapItemTempleOfLoyalty::SetAsNotVisited(H3Hero* hero) noexcept
    {
        sprintf(newMapItem::buffer, ErmVariableFormat, hero->id);
        Era::SetAssocVarIntValue(newMapItem::buffer, 0);
    }

    _LHF_(LoHook_TempleOfLoyality_GetMorale)
    {
        H3Hero* hero = *reinterpret_cast<H3Hero**>(c->ebp + 0x8);

        if (hero && H3MapItemTempleOfLoyalty::IsVisitedByHero(hero))
        {
            // Одно мировоззрение.
            if (c->edi > 1) c->edi = 1;
            // Не проверяем Альянс Ангелов.
            c->return_address = 0x44AC8C;
            return EXEC_DEFAULT;
        }

        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_TempleOfLoyality_GetMoraleBonusDescr)
    {
        H3Hero* hero = *reinterpret_cast<H3Hero**>(c->ebp + 0x14);

        if (hero && H3MapItemTempleOfLoyalty::IsVisitedByHero(hero))
        {
            // Одно мировоззрение.
            if (c->edi > 1) c->edi = 1;
            // Не проверяем Альянс Ангелов.
            c->return_address = 0x44B975;
            return EXEC_DEFAULT;
        }

        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_TempleOfLoyality_AI_SaveVisit1)
    {
        H3Hero* hero = reinterpret_cast<H3Hero*>(c->edi);

        if (hero && H3MapItemTempleOfLoyalty::IsVisitedByHero(hero))
        {
            // Запоминаем (в al).
            c->eax &= 0xFFFFFF00;
            c->eax |= 2;
        }

        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_TempleOfLoyality_AI_SaveVisit2)
    {
        H3Hero* hero = *reinterpret_cast<H3Hero**>(c->ebp - 0x30);

        if (hero && H3MapItemTempleOfLoyalty::IsVisitedByHero(hero))
        {
            // Запоминаем (в al).
            c->eax &= 0xFFFFFF00;
            c->eax |= 2;
        }

        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_TempleOfLoyality_AI_SaveVisit3)
    {
        H3Hero* hero = reinterpret_cast<H3Hero*>(c->esi);

        if (hero && H3MapItemTempleOfLoyalty::IsVisitedByHero(hero))
        {
            // Запоминаем (в al).
            c->eax &= 0xFFFFFF00;
            c->eax |= 2;
        }

        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_TempleOfLoyality_AI_SaveVisit4)
    {
        H3Hero* hero = reinterpret_cast<H3Hero*>(c->ebx);

        if (hero && H3MapItemTempleOfLoyalty::IsVisitedByHero(hero))
        {
            // Запоминаем (в al).
            c->eax &= 0xFFFFFF00;
            c->eax |= 2;
        }

        return EXEC_DEFAULT;
    }

    _LHF_(LoHook_TempleOfLoyality_AI_CheckFractions1)
    {
        // Пополнение стека ИИ.
        H3CreatureSwapper* swapper = reinterpret_cast<H3CreatureSwapper*>(c->ebx);

        // Нет штрафа за мировоззрения (al == 2).
        if ((c->eax & 0xFF) == 2)
        {
            //// Все фракции - как замок.
            //FracCountsTable[1] += FracCountsTable[0];
            //FracCountsTable[0] = 0;
            //for (_int_ i = 2; i < nCastles; i++)
            //{
            //    FracCountsTable[1] += FracCountsTable[i];
            //    FracCountsTable[i] = 0;
            //}

            // 1 фракция.
            swapper->alignmentCount = 1;

            // Не проверяем Альянс Ангелов.
            c->return_address = 0x42C11E;

            return NO_EXEC_DEFAULT;
        }
        else
        {
            return EXEC_DEFAULT;
        }
    }

    _LHF_(LoHook_TempleOfLoyality_AI_CheckFractions2)
    {
        // Восстанавливаем затёртую команду.
        c->ebx = c->esi;

        // Нет штрафа за мировоззрения (al == 2).
        if ((c->eax & 0xFF) == 2)
        {
            // Первая фракция - замок.
            c->ebx = 0;

            // Не проверяем Альянс Ангелов.
            c->return_address = 0x42C7CC;
            return NO_EXEC_DEFAULT;
        }
        else
        {
            return EXEC_DEFAULT;
        }
    }

    _LHF_(LoHook_TempleOfLoyality_AI_CheckFractions3)
    {
        // Восстанавливаем затёртую команду.
        IntAt(c->ebp - 0x18) = c->edi;

        // Нет штрафа за мировоззрения (al == 2).
        if ((c->eax & 0xFF) == 2)
        {
            // Первая фракция - замок.
            IntAt(c->ebp - 0x18) = 0;

            // Не проверяем Альянс Ангелов.
            c->return_address = 0x42C8FC;
            return NO_EXEC_DEFAULT;
        }
        else
        {
            return EXEC_DEFAULT;
        }
    }

    void TempleOfLoyaltyExtender::CreatePatches()
    {
        static bool patchesCreated = false;

        if (patchesCreated) return;

        // Храм Лояльности: модифицируем получение боевого духа.
        _pi->WriteLoHook(0x44AB77, LoHook_TempleOfLoyality_GetMorale);
        // Храм Лояльности: модифицируем получение описания бонусов боевого духа.
        _pi->WriteLoHook(0x44B85B, LoHook_TempleOfLoyality_GetMoraleBonusDescr);

        // Храм Лояльности: запоминаем посещённость для ИИ.
        _PI->WriteLoHook(0x42BB22, LoHook_TempleOfLoyality_AI_SaveVisit1);
        _PI->WriteLoHook(0x431D16, LoHook_TempleOfLoyality_AI_SaveVisit2);
        _PI->WriteLoHook(0x52489F, LoHook_TempleOfLoyality_AI_SaveVisit1);
        _PI->WriteLoHook(0x52585D, LoHook_TempleOfLoyality_AI_SaveVisit3);
        _PI->WriteLoHook(0x5262EB, LoHook_TempleOfLoyality_AI_SaveVisit1);
        _PI->WriteLoHook(0x526320, LoHook_TempleOfLoyality_AI_SaveVisit3);
        _PI->WriteLoHook(0x528034, LoHook_TempleOfLoyality_AI_SaveVisit3);
        _PI->WriteLoHook(0x528349, LoHook_TempleOfLoyality_AI_SaveVisit3);
        _PI->WriteLoHook(0x528F9E, LoHook_TempleOfLoyality_AI_SaveVisit4);
        _PI->WriteLoHook(0x52923D, LoHook_TempleOfLoyality_AI_SaveVisit4);
        _PI->WriteLoHook(0x5292AF, LoHook_TempleOfLoyality_AI_SaveVisit1);
        _PI->WriteLoHook(0x52A030, LoHook_TempleOfLoyality_AI_SaveVisit4);
        _PI->WriteLoHook(0x52AC49, LoHook_TempleOfLoyality_AI_SaveVisit3);
        _PI->WriteLoHook(0x52B5F9, LoHook_TempleOfLoyality_AI_SaveVisit3);
        _PI->WriteLoHook(0x52C199, LoHook_TempleOfLoyality_AI_SaveVisit3);

        // Храм Лояльности: ИИ проверяет фракции (1).
        _PI->WriteLoHook(0x42C079, LoHook_TempleOfLoyality_AI_CheckFractions1);
        // Храм Лояльности: ИИ проверяет фракции (2).
        _PI->WriteLoHook(0x42C76D, LoHook_TempleOfLoyality_AI_CheckFractions2);
        // Храм Лояльности: ИИ проверяет фракции (3).
        _PI->WriteLoHook(0x42C8A1, LoHook_TempleOfLoyality_AI_CheckFractions3);

        patchesCreated = true;

    }

    TempleOfLoyaltyExtender::TempleOfLoyaltyExtender()
        : H3ActiveObject<H3MapItemTempleOfLoyalty>(
            "EraPlugin.TempleOfLoyaltyExtender.js",
            extender::HOTA_OBJECT_TYPE,
            TEMPLE_OF_LOYALTY_OBJECT_SUBTYPE)
    {
        CreatePatches();
    }

    TempleOfLoyaltyExtender* TempleOfLoyaltyExtender::instance = nullptr;
    TempleOfLoyaltyExtender& TempleOfLoyaltyExtender::Get()
    {
        if (!instance) instance = new TempleOfLoyaltyExtender();
        return *instance;
    }

    BOOL TempleOfLoyaltyExtender::SetHintInH3TextBuffer(H3MapItem* mapItem, const H3Hero* hero, const int playerId, BOOL isRightClick) const noexcept
    {
        if (GetFromMapItem(mapItem))
        {
            H3String objName = RMGObjectInfo::GetObjectName(mapItem);
            int teamId = THISCALL_2(int, 0x4A55D0, P_Game->Get(), playerId);
            BOOL teamVisited = IsObjectVisitedByTeam(objectType, objectSubtype, teamId);

            // Add extra hint if object is visited at least once by the team
            if (teamVisited)
            {
                AddExtraInfoHint(&objName, isRightClick);
            }

            // Add "Visited/Not visited" hint
            if (hero)
            {
                const bool isVisitedByHero = H3MapItemTempleOfLoyalty::IsVisitedByHero(const_cast <H3Hero*> (hero));
                AddHeroVisitedHint(&objName, isRightClick, isVisitedByHero);
            }

            sprintf(h3_TextBuffer, "%s", objName.String());

            return true;
        }

        return false;
    }

    BOOL TempleOfLoyaltyExtender::VisitMapItem(H3Hero* hero, H3MapItem* mapItem, const H3Position pos,
        const BOOL isHuman) const noexcept
    {
        if (GetFromMapItem(mapItem))
        {
            const bool isVisitedByHero = H3MapItemTempleOfLoyalty::IsVisitedByHero(hero);

            if (isVisitedByHero)
            {
                if (isHuman)
                {
                    FASTCALL_12(void, 0x4F6C00,
                        this->GetVisitedMessage().String(),
                        1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -777);
                }
            }

            else
            {
                ProcObjectFlagsVisitedByTeam(hero, objectType, objectSubtype);
                H3MapItemTempleOfLoyalty::SetAsVisited(hero);
                if (isHuman)
                {
                    FASTCALL_12(void, 0x4F6C00,
                        this->GetVisitingMessage().String(),
                        1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -777);
                }
            }

            return true;
        }

        return false;
    }

    BOOL TempleOfLoyaltyExtender::SetAiMapItemWeight(
        H3MapItem* mapItem, H3Hero* hero, const H3Player* player,
        int& aiMapItemWeight, int* moveDistance, const H3Position pos) const noexcept
    {
        if (GetFromMapItem(mapItem))
        {
            const bool isVisitedByHero = H3MapItemTempleOfLoyalty::IsVisitedByHero(hero);

            // Объект не посещён.
            if (!isVisitedByHero)
            {
                if (*moveDistance > hero->movement)
                {
                    aiMapItemWeight = 0;
                    return true;
                }

                int fractionsCount = THISCALL_2(int, 0x44A880, &hero->army, 0);

                // Нет падения боевого дух из-за количества фракций
                if (fractionsCount <= 1)
                {
                    // В армии только нежить - не надо посещать храм лояльности.
                    if (THISCALL_1(bool, 0x44A7F0, &hero->army))
                    {
                        aiMapItemWeight = 0;
                        return true;
                    }
                    else
                    {
                        // Минимальная надбавка за возможность дальнейшего появления стеков других фракций.
                        aiMapItemWeight = THISCALL_1(INT32, 0x44A950, &hero->army) / 200;
                        return true;
                    }
                }
                // Есть падение боевого духа из-за количества фракций.
                else
                {
                    // Ценность того, насколько повысится боевой дух.
                    aiMapItemWeight = THISCALL_2(int, 0x5281D0, hero, fractionsCount - 1);
                    return true;
                }

            }
            // Объект уже посещён.
            else
            {
                aiMapItemWeight = 0;
                return true;
            }
        }

        return false;
    }

    int TempleOfLoyaltyExtender::AI_OnScouting_Value() const noexcept
    {
        return 1;
    }
}
