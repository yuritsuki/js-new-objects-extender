#pragma once


namespace prospector {
    // type 144, subtype 10
    constexpr int PROSPECTOR_OBJECT_SUBTYPE = 10;

    struct H3MapItemProspector// : H3MapitemMysticGarden
    {
        unsigned resourceType : 5;
        /** @brief [05] which players have come by*/
        unsigned visited : 8;
        /** @brief [13]*/
        unsigned resourceAmount : 10;
        unsigned _u1 : 9;

    public:
        void Reset();
    };

    class ProspectorExtender final
        : public H3ActiveObject<H3MapItemProspector>
    {
    private:
        static ProspectorExtender* instance;

        ProspectorExtender();

        BOOL InitNewGameMapItemSetup(
            H3MapItem* mapItem
        ) const noexcept override final;

        BOOL InitNewWeekMapItemSetup(
            H3MapItem* mapItem
        ) const noexcept override final;

        BOOL SetHintInH3TextBuffer(
            H3MapItem* mapItem,
            const H3Hero* currentHero,
            int playerId,
            BOOL isRightClick
        ) const noexcept override final;

        BOOL VisitMapItem(
            H3Hero* currentHero,
            H3MapItem* mapItem,
            H3Position pos,
            BOOL isHuman
        ) const noexcept override final;

        BOOL SetAiMapItemWeight(
            H3MapItem* mapItem,
            H3Hero* currentHero,
            const H3Player* activePlayer,
            int& aiMapItemWeight,
            int* moveDistance,
            H3Position pos
        ) const noexcept override final;

        int AI_OnScouting_Value() const noexcept override final;

    public:
        static ProspectorExtender& Get();
    };
}
