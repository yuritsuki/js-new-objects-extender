#pragma once


namespace observatory {
    // type 146, subtype 1
    constexpr int WATER_OBSERVATORY_OBJECT_SUBTYPE = 1;
    constexpr int VISION_RADIUS_GIVEN = 2;

    struct H3MapItemObservatory
    {
        static constexpr LPCSTR ErmVariableFormat = "observatory_%d"; //heroid

        static BOOL IsVisitedByHero(H3Hero* hero) noexcept;
        static void SetAsVisited(H3Hero* hero) noexcept;
        static void SetAsNotVisited(H3Hero* hero) noexcept;
    };

    class ObservatoryExtender final
        : public H3ActiveObject<H3MapItemObservatory>
    {
    private:
        static ObservatoryExtender* instance;

        ObservatoryExtender();

        void CreatePatches() override final;

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
        static ObservatoryExtender& Get();
    };
}
