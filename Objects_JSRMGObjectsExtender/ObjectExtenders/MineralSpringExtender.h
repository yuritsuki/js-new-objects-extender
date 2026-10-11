#pragma once


namespace mineralSpring {
    // type 144, subtype 4
    constexpr int MINERAL_SPRING_OBJECT_SUBTYPE = 4;
    constexpr int MOVE_POINTS_GIVEN = 600;
    constexpr int LUCK_GIVEN = 1;

    struct H3MapItemMineralSpring
    {
        static constexpr LPCSTR ErmVariableFormat = "mineralSpring_%d";

        static BOOL IsVisitedByHero(const H3Hero* hero) noexcept;
        static void SetAsVisited(const H3Hero* hero) noexcept;
        static void SetAsNotVisited(const H3Hero* hero) noexcept;
    };

    class MineralSpringExtender final
        : public H3ActiveObject<H3MapItemMineralSpring>
    {
    private:
        static MineralSpringExtender* instance;

        MineralSpringExtender();

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
        static MineralSpringExtender& Get();
    };
}
