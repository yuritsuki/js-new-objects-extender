#pragma once


namespace vialOfMana
{
    // 145, subtype 3
    constexpr int VIAL_OF_MANA_OBJECT_SUBTYPE = 3;
    constexpr int MIN_MANA = 30;
    constexpr int MAX_MANA = 60;

    struct H3MapItemVialOfMana
    {
        INT32 manaPts;
    };

    class VialOfManaExtender final
        : public H3ActiveObject<H3MapItemVialOfMana>
    {
    private:
        static VialOfManaExtender* instance;

        VialOfManaExtender();

        BOOL InitNewGameMapItemSetup(
            H3MapItem* mapItem
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
        static VialOfManaExtender& Get();
    };
}
