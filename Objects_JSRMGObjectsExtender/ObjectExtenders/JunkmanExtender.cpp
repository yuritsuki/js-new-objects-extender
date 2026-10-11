#include "../pch.h"
#include "JunkmanExtender.h"


namespace junkman {
    int H3Hero_CountArtifactsInBackpack(H3Hero* hero)
    {
        int count = 0;

        for (H3Artifact& art : hero->backpackArtifacts)
        {
            if (art.id != eArtifact::NONE && art.id != eArtifact::CATAPULT && art.id != eArtifact::BALLISTA
                && art.id != eArtifact::AMMO_CART && art.id != eArtifact::FIRST_AID_TENT && art.id != eArtifact::SPELL_SCROLL
                && art.id != eArtifact::GRAIL && art.id < limits::ARTIFACTS)
            {
                count++;
            }
        }
        return count;
    }

    _LHF_(LoHook_005EE880)
    {
        if (IntAt(0x6AAB2C) == 4)
        {
            //H3Hero* marketHero = *reinterpret_cast<H3Hero**>(0x6AAAE0);
            int slotId = c->eax;
            int a = 5;
            //eArtifact artId1 = static_cast<eArtifact>(marketHero->bodyArtifacts[slotId].id);

            eArtifact artId = eArtifact::NONE;
            // Артефакт на теле
            if (slotId < 19)
            {
                artId = static_cast<eArtifact>(P_MarketHero->bodyArtifacts[slotId].id);
            }
            // В рюкзаке
            else
            {
                artId = static_cast<eArtifact>(P_MarketHero->backpackArtifacts[slotId - 19].id);
            }

            if (artId == eArtifact::SPELL_SCROLL)
            {
                LPCSTR mes = EraJS::read(
                    H3String::Format("RMG.objectGeneration.%d.%d.text.scrollsNotAllowed", eHotaObject::ACTIVE, eHotaObjectActiveType::JUNKMAN)
                    .String());

                FASTCALL_12(void, 0x4F6C00, mes, 1, -1, -1, -1, 0, -1, 0, -1, 0, -1, 0);

                c->return_address = 0x5EE9A9;
                return NO_EXEC_DEFAULT;
            }
        }
        return EXEC_DEFAULT;
    }

    //_LHF_(LoHook_005EC3A7)
    //{
    //    H3BaseDlg* dlg = (H3BaseDlg*)c->ebx;
    //    if (IntAt(0x6AAB2C) == 4)
    //    {
    //        // H3BaseDlg::RemoveControlState
    //        THISCALL_3(void, 0x5FF520, dlg, 16, 4102);
    //        THISCALL_3(void, 0x5FF520, dlg, 17, 4102);
    //        THISCALL_3(void, 0x5FF520, dlg, 18, 4102);
    //        THISCALL_3(void, 0x5FF520, dlg, 1000, 6);
    //        THISCALL_3(void, 0x5FF520, dlg, 1001, 6);
    //        THISCALL_3(void, 0x5FF520, dlg, 1002, 6);
    //    }
    //    else
    //    {
    //        THISCALL_3(void, 0x5FF490, dlg, 16, 6);               // H3BaseDlg::AddControlState
    //        THISCALL_3(void, 0x5FF520, dlg, 16, 4096);
    //        THISCALL_3(void, 0x5FF490, dlg, 17, 6);
    //        THISCALL_3(void, 0x5FF520, dlg, 17, 4096);
    //        THISCALL_3(void, 0x5FF490, dlg, 18, 6);
    //        THISCALL_3(void, 0x5FF520, dlg, 18, 4096);
    //    }
    //    return 1;
    //}

    int __stdcall HiHook_H3SellArtifactDlg_WindowHandler(HiHook* h, H3BaseDlg* dlg, H3Msg* msg)
    {
        //int result = THISCALL_2(int, h->GetDefaultFunc(), dlg, msg);

        // Клик по стрелке рюкзака
        if (msg->subtype == eMsgSubtype::LBUTTON_CLICK && (msg->itemId == 130 || msg->itemId == 131))
        {
            int a = P_MarketSelectedSlotIndex;

            //// Нет выбранного артефакта.
            //P_MarketSelectedSlotIndex = -1;

            ////H3SellArtifactDlg::Proc(dlg, 1);
            //THISCALL_2(void, 0x5EC280, dlg, 1);
            //int v28 = 0;
            //if (v28)
            //{
            //    msg->itemId = 10;
            //    msg->subtype = eMsgSubtype::END_DIALOG;
            //    return 2;
            //}
            //return 1;
            return THISCALL_2(int, h->GetDefaultFunc(), dlg, msg);
        }
        else {
            return THISCALL_2(int, h->GetDefaultFunc(), dlg, msg);
        }


        //return result;
    }

    void JunkmanExtender::CreatePatches()
    {
        static bool patchesCreated = false;
        if (patchesCreated) return;

        // Запрет продавать у старьевщика свитки кликом мыши
        //_pi->WriteLoHook(0x5EE880, LoHook_005EE880);

        //// Убираем кнопки переключения на торговлю ресурсами/покупаку артефактов
        //_pi->WriteLoHook(0x5EC3A7, LoHook_005EC3A7);

        // Делаем слот со свитком невалидным при переключении (запрет продавать свитки)
        //_PI->WriteHiHook(0x5EE360, SPLICE_, EXTENDED_, THISCALL_, HiHook_H3SellArtifactDlg_WindowHandler);
        //LoHook_ArtSell_Add5Slot_ArtSellSelect
        //LoHook_ArtSell_Backpack_ResetEmptySlotSelection

        patchesCreated = true;
    }

    JunkmanExtender::JunkmanExtender()
        : H3ActiveObject<H3MapItemJunkman>(
            "EraPlugin.JunkmanExtender.js",
            extender::HOTA_OBJECT_TYPE,
            JUNKMAN_OBJECT_SUBTYPE)
    {
        CreatePatches();
    }

    JunkmanExtender* JunkmanExtender::instance = nullptr;
    JunkmanExtender& JunkmanExtender::Get()
    {
        if (!instance) instance = new JunkmanExtender();
        return *instance;
    }

    BOOL JunkmanExtender::VisitMapItem(H3Hero* hero, H3MapItem* mapItem, const H3Position pos,
        const BOOL isHuman) const noexcept
    {
        if (GetFromMapItem(mapItem))
        {
            if (isHuman)
            {
                // patch object name
                H3String str = H3String::Format("{%s}", RMGObjectInfo::GetObjectName(mapItem)).String();
                LPCSTR objName = str.String();
                const DWORD prevStr = DwordAt(0x5EC3CE + 2); // store object name
                DwordAt(0x5EC3CE + 2) = (int)&objName; // patch object name

                // jump button building in dlg build (check in debugger)
                auto patch = _pi->WriteHexPatch(0x5E5F7C, "E9 FA000000"); // if jump is less than 128 byte, use EB

                // Town_ArtMarket dlg after init
                P_MarketHero = hero;
                P_ArtifactMerchant = &P_Game->artifactMerchant;
                P_ActivePlayerMarkets = 7;
                P_MarketType = eMarketType::SELL_ARTIFACTS;
                //P_MarketBuilding = eMarketBuilding::JUNKMAN;

                // Run dlg
                CDECL_0(void, 0x5EA530);

                // restore global vars
                //IntAt(0x6AAB0C) = 0; // marker type
                IntAt(0x6AAB00) = 0; // markets amount owned by player
                DwordAt(0x5EC3CE + 2) = prevStr; // restore object name

                // cancel patch
                patch->Destroy();
            }
            //else
            //{
            //    AI_H3Hero_VisitJunkman(hero, 7);
            //}

            return true;
        }

        return false;
    }

    //int JunkmanExtender::AI_OnScouting_Value() const noexcept
    //{
    //    return 1;
    //}
}
