#pragma once


namespace trailblazer
{
    // type 144, subtype 11
    constexpr int TRAILBLAZER_OBJECT_SUBTYPE = 11;
    constexpr INT32 TRAILBLAZER_STEP_COST = 75;

    struct H3MapItemTrailblazer
    {
        static constexpr LPCSTR ErmVariableFormat = "trailblazer_%d"; // heroid

        static BOOL IsVisitedByHero(H3Hero* hero, int terrainType) noexcept;
        static void SetAsVisited(H3Hero* hero, int terrainType) noexcept;
        static void SetAsNotVisited(H3Hero* hero) noexcept;
    };

    class TrailblazerExtender final
        : public H3ActiveObject<H3MapItemTrailblazer>
    {
    private:
        static TrailblazerExtender* instance;

        void CreatePatches() override final;

        TrailblazerExtender();

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
        static TrailblazerExtender& Get();
    };
}
