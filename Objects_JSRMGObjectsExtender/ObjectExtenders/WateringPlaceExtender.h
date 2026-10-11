#pragma once


namespace wateringPlace
{
    // 144
    constexpr int WATERING_PLACE_OBJECT_SUBTYPE = 3;
    constexpr int WATERING_PLACE_MOVE_POINTS_GIVEN = 1000;

    struct H3MapItemWateringPlace
    {
        static constexpr LPCSTR ErmVariableFormat = "wateringPlace_%d";

        static inline BOOL IsVisitedByHero(const H3Hero* hero) noexcept;
        static void SetAsVisited(H3Hero* hero) noexcept;
        static void SetAsNotVisited(H3Hero* hero) noexcept;
    };

    class WateringPlaceExtender final
        : public H3ActiveObject<H3MapItemWateringPlace>
    {
    private:
        static WateringPlaceExtender* instance;

        WateringPlaceExtender();

        void CreatePatches() override final;

        BOOL SetHintInH3TextBuffer(
            H3MapItem* mapItem,
            const H3Hero* currentHero,
            int interactPlayerId,
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
        static WateringPlaceExtender& Get();
    };
}
