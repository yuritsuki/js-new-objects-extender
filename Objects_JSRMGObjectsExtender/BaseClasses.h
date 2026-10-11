#pragma once

struct RMGObjectInfo
{
    static LPCSTR GetObjectName(const int type, const int subtype) noexcept
    {
        return extender::GetObjectName(type, subtype);
    }

    static LPCSTR GetObjectName(const H3MapItem *item) noexcept
    {
        return item ? extender::GetObjectName(item) : nullptr;
    }
};

template <typename TMapItem> class H3MapObject : public extender::ObjectExtender
{
  protected:
    static constexpr LPCSTR objectInfo_key = "RMG.objectGeneration.%d.%d.text.info";
    static constexpr LPCSTR extraObjectInfo_key = "RMG.objectGeneration.%d.%d.text.hint";
    int objectType = eObject::NO_OBJ;
    int objectSubtype = eObject::NO_OBJ;

    H3MapObject(LPCSTR instanceName, const int type, const int subtype, const int aiScouting = -1,
        const extender::eRmgDlgObjectPage page = extender::eRmgDlgObjectPage::ePageUnknown)
        : ObjectExtender(globalPatcher->CreateInstance(instanceName)), objectType(type), objectSubtype(subtype)
    {
        AddUniqueObjectInfo(type, subtype, aiScouting, page);
        m_isInited = TRUE;
    }

    ~H3MapObject() override = default;

    TMapItem *GetFromMapItem(H3MapItem *item) const noexcept
    {
        return item ? reinterpret_cast<TMapItem *>(&item->setup) : nullptr;
    }
    const TMapItem *GetFromMapItem(const H3MapItem *item) const noexcept
    {
        return item ? reinterpret_cast<const TMapItem *>(&item->setup) : nullptr;
    }
    void *GetSetupFromMapItem(H3MapItem *item) const noexcept
    {
        return item && item->objectType == objectType && item->objectSubtype == objectSubtype ? &item->setup : nullptr;
    }

    BOOL SetDefaultHint(H3MapItem *item) const noexcept
    {
        const auto name = RMGObjectInfo::GetObjectName(item);
        if (!name)
            return FALSE;
        libc::sprintf(h3_TextBuffer, "%s", name);
        return TRUE;
    }
    BOOL SetHintInH3TextBuffer(H3MapItem *item, const H3Hero *, int, BOOL) const noexcept override
    {
        return SetDefaultHint(item);
    }

    // JS object implementations use a one-argument map setup callback.
    virtual BOOL InitNewGameMapItemSetup(H3MapItem *) const noexcept { return FALSE; }
    BOOL InitNewGameMapItemSetup(H3MapItem *item, int, int) const noexcept override
    {
        return InitNewGameMapItemSetup(item);
    }

    void AddExtraInfoHint(H3String *name, const BOOL isRightClick) const noexcept
    {
        const LPCSTR extra = EraJS::read(H3String::Format(
            "RMG.objectGeneration.%d.%d.text.hint", objectType, objectSubtype).String());
        if (extra && name)
        {
            libc::sprintf(h3_TextBuffer, "%s%s", isRightClick ? "\n" : " ", extra);
            name->Append(h3_TextBuffer);
        }
    }

    void AddHeroVisitedHint(H3String *name, const BOOL isRightClick, const BOOL visited) const noexcept
    {
        if (name)
        {
            libc::sprintf(h3_TextBuffer, "%s%s", isRightClick ? "\n\n" : " ",
                          P_GeneralText->GetText(visited ? 354 : 355));
            name->Append(h3_TextBuffer);
        }
    }

  public:
    H3RmgObjectGenerator *CreateRMGObjectGen(const extender::RMGObjectProperties &info,
                                              BOOL) const noexcept override
    {
        if (info.type != objectType || (objectSubtype >= 0 && info.subtype != objectSubtype))
            return nullptr;
        return extender::ObjectExtender::CreateDefaultH3RmgObjectGenerator(info);
    }
};

template <typename TMapItem> class H3VisitingObject : public H3MapObject<TMapItem>
{
  protected:
    using H3MapObject<TMapItem>::H3MapObject;
    ~H3VisitingObject() override = default;

    H3String GetStringMessage(LPCSTR key) const
    {
        H3String text = H3String::Format("{%s}", extender::GetObjectName(this->objectType, this->objectSubtype));
        text.Append(EraJS::read(H3String::Format(key, this->objectType, this->objectSubtype).String()));
        return text;
    }
    H3String GetVisitingMessage() const { return GetStringMessage("RMG.objectGeneration.%d.%d.text.visit"); }
    H3String GetVisitedMessage() const { return GetStringMessage("RMG.objectGeneration.%d.%d.text.visited"); }
    H3String GetCannotVisitMessage() const { return GetStringMessage("RMG.objectGeneration.%d.%d.text.cannotVisit"); }
};

template <typename TMapItem> class H3ActiveObject : public H3VisitingObject<TMapItem>
{
  protected:
    using H3VisitingObject<TMapItem>::H3VisitingObject;
    ~H3ActiveObject() override = default;

    virtual int AI_OnScouting_Value() const noexcept { return 0; }
    virtual BOOL AI_MapGoal_Value(H3MapItem *, H3Hero *, const H3Player *, int &, int *, H3Position) const noexcept
    {
        return FALSE;
    }
    virtual BOOL AI_Scouting_Value(H3MapItem *, H3Hero *, const H3Player *, int &, int *, H3Position) const noexcept
    {
        return FALSE;
    }

  public:
    BOOL Register() noexcept
    {
        const int scoutingWeight = AI_OnScouting_Value();
        if (scoutingWeight)
        {
            for (auto *info : this->GetObjectSubtypesInfo())
            {
                if (info)
                    info->aiScoutingWeight = scoutingWeight;
            }
        }
        return extender::ObjectExtender::Register();
    }

   /* BOOL SetAiMapItemWeight(H3MapItem *item, H3Hero *hero, const H3Player *player, int &weight,
                            int *moveDistance, const H3Position pos) const noexcept override
    {
        if (AI_MapGoal_Value(item, hero, player, weight, moveDistance, pos))
            return TRUE;
        if (AI_Scouting_Value(item, hero, player, weight, moveDistance, pos))
            return TRUE;
        const int scouting = AI_OnScouting_Value();
        if (scouting)
        {
            weight = scouting;
            return TRUE;
        }
        return FALSE;
    }*/
};

