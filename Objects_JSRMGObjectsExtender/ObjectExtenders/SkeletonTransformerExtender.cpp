#include "../pch.h"
#include "SkeletonTransformerExtender.h"


namespace skeletonTransformer {
    SkeletonTransformerExtender::SkeletonTransformerExtender()
        : H3ActiveObject<H3MapItemSkeletonTransformer>(
            "EraPlugin.SkeletonTransformerExtender.js",
            extender::HOTA_OBJECT_TYPE,
            SKELETON_TRANSFORMER_OBJECT_SUBTYPE)
    {
    }

    SkeletonTransformerExtender* SkeletonTransformerExtender::instance = nullptr;
    SkeletonTransformerExtender& SkeletonTransformerExtender::Get()
    {
        if (!instance) instance = new SkeletonTransformerExtender();
        return *instance;
    }

    BOOL SkeletonTransformerExtender::VisitMapItem(
        H3Hero* hero, H3MapItem* mapItem, const H3Position pos,
        const BOOL isHuman) const noexcept
    {
        if (GetFromMapItem(mapItem))
        {
            if (isHuman)
            {
                H3Army* army = &hero->army;
                char dlgBuffer[1024]; // Memory for dlg
                H3Dlg* dlg = (H3Dlg*)dlgBuffer;
                THISCALL_2(void, 0x0565770, dlg, army);         // Dlg_SkeletTransformer_Create
                P_MouseManager->SetCursor(0, eCursor::ADVENTURE);// Change cursor to default
                THISCALL_2(void, 0x05FFA20, dlg, 0);            // H3BaseDlg::ShowAndRun
                THISCALL_1(void, 0x05661E0, dlg);               // Dlg_SkeletTransformer_Destroy
                memset(Era::z[1], 0, 512);                      // Clear memory
            }

            return true;
        }

        return false;
    }

    //int SkeletonTransformerExtender::AI_OnScouting_Value() const noexcept
    //{
    //    return 1;
    //}
}
