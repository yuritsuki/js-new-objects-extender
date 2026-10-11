#pragma once


namespace templeOfLoyalty {
    // type 144, subtype 0
    constexpr int TEMPLE_OF_LOYALTY_OBJECT_SUBTYPE = 0;

    struct H3MapItemTempleOfLoyalty
    {
        static constexpr LPCSTR ErmVariableFormat = "templeOfLoyalty_%d";

    public:
        static BOOL IsVisitedByHero(H3Hero* hero) noexcept;
        static void SetAsVisited(H3Hero* hero) noexcept;
        static void SetAsNotVisited(H3Hero* hero) noexcept;
    };

    class TempleOfLoyaltyExtender final
        : public H3ActiveObject<H3MapItemTempleOfLoyalty>
    {
    private:
        static TempleOfLoyaltyExtender* instance;

        void CreatePatches() override final;

        TempleOfLoyaltyExtender();

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
        static TempleOfLoyaltyExtender& Get();
    };
}
