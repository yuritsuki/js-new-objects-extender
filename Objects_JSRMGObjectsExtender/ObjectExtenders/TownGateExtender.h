#pragma once


namespace townGate
{
    // type 146, subtype 3
    constexpr int TOWN_GATE_OBJECT_SUBTYPE = 3;

    struct H3MapItemTownGate
    {

    };

    class TownGateExtender final
        : public H3ActiveObject<H3MapItemTownGate>
    {
    private:
        static TownGateExtender* instance;

        void CreatePatches() override final;

        TownGateExtender();

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
        static TownGateExtender& Get();
    };
}
