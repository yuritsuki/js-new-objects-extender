#include "../pch.h"
#include "ProspectorExtender.h"


namespace prospector {
    void H3MapItemProspector::Reset()
    {
        resourceAmount = 0;
        visited = 0;
        resourceType = FASTCALL_2(INT32, 0x50C7C0, 1, 2) * 3;
        if (resourceType == eResource::SULFUR) resourceAmount = 5;
        else resourceAmount = 500;
    }

    ProspectorExtender::ProspectorExtender()
        : H3ActiveObject<H3MapItemProspector>(
            "EraPlugin.ProspectorExtender.js",
            extender::HOTA_OBJECT_TYPE,
            PROSPECTOR_OBJECT_SUBTYPE)
    {
    }

    ProspectorExtender* ProspectorExtender::instance = nullptr;
    ProspectorExtender& ProspectorExtender::Get()
    {
        if (!instance) instance = new ProspectorExtender();
        return *instance;
    }

    BOOL ProspectorExtender::InitNewGameMapItemSetup(H3MapItem* mapItem) const noexcept
    {
        if (auto prospector = GetFromMapItem(mapItem))
        {
            prospector->Reset();
            return true;
        }
        return false;
    }

    BOOL ProspectorExtender::InitNewWeekMapItemSetup(H3MapItem* mapItem) const noexcept
    {
        if (auto prospector = GetFromMapItem(mapItem))
        {
            prospector->Reset();
            return true;
        }
        return false;
    }

    BOOL ProspectorExtender::SetHintInH3TextBuffer(H3MapItem* mapItem, const H3Hero* hero, const int playerId, BOOL isRightClick) const noexcept
    {
        if (GetFromMapItem(mapItem))
        {
            H3String objName = RMGObjectInfo::GetObjectName(mapItem);

            const bool isVistedByPlayer = THISCALL_2(bool, 0x529B70, mapItem, H3HumanID::Get());
            if (isVistedByPlayer)
            {
                sprintf(h3_TextBuffer, "%s%s", isRightClick ? "\n\n" : " ", P_GeneralText->GetText(354));
                objName.Append(h3_TextBuffer);
            }

            sprintf(h3_TextBuffer, "%s", objName.String());

            return true;
        }

        return false;
    }

    BOOL ProspectorExtender::VisitMapItem(H3Hero* hero, H3MapItem* mapItem, const H3Position pos,
        const BOOL isHuman) const noexcept
    {
        if (auto objSetup = GetFromMapItem(mapItem))
        {
            // set object as visited
            THISCALL_2(void, 0x4FC620, mapItem, hero->owner);

            const INT16 resType = objSetup->resourceType;
            const INT16 resQty = objSetup->resourceAmount;

            if (resQty > 0)
            {
                if (isHuman)
                {
                    FASTCALL_12(void, 0x4F6C00,
                        this->GetVisitingMessage().String(),
                        1, -1, -1, resType, resQty, -1, 0, -1, 0, -1, -777);
                }

                // add resourcse
                THISCALL_3(void, 0x4E3870, hero, resType, resQty);

                // remove resources from the object
                objSetup->resourceAmount = 0;
            }
            else if (isHuman)
            {
                FASTCALL_12(void, 0x4F6C00,
                    this->GetVisitedMessage().String(),
                    1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -777);
            }
            return true;
        }

        return false;
    }

    BOOL ProspectorExtender::SetAiMapItemWeight(
        H3MapItem* mapItem, H3Hero* hero, const H3Player* player,
        int& aiMapItemWeight, int* moveDistance, const H3Position pos) const noexcept
    {
        if (auto objSetup = GetFromMapItem(mapItem))
        {
            const bool isVisitedByPlayer = THISCALL_2(bool, 0x529B70, mapItem, player->ownerID);
            UINT16 resNum = objSetup->resourceAmount;

            // if AI player didnt' visit or there are resources
            if (!isVisitedByPlayer || resNum)
            {
                UINT16 resType = objSetup->resourceType;

                aiMapItemWeight =
                    static_cast<int>(player->aIPlayer.resourceImportance[resType] * resNum); // static_cast<double*>()
            }
            return true;
        }

        return false;
    }

    int ProspectorExtender::AI_OnScouting_Value() const noexcept
    {
        return 10;
    }
}
