#pragma once


namespace seaBarrel
{
    // 145, subtype 1
    constexpr int SEA_BARREL_OBJECT_SUBTYPE = 1;

    struct H3MapItemSeaBarrel
    {
        INT16 resType;
        INT16 resQty;
    };

    class SeaBarrelExtender final
        : public H3ActiveObject<H3MapItemSeaBarrel>
    {
    private:
        static SeaBarrelExtender* instance;

        SeaBarrelExtender();

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

    public:
        static SeaBarrelExtender& Get();
    };
}
