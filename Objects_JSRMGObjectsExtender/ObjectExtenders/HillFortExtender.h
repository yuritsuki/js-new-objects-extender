#pragma once

namespace hillFort
{
    // type 35, subtype 1
    constexpr int HILL_FORT_OBJECT_SUBTYPE = 1;

    struct H3MapHillFort
    {

    };

    class HillFortExtender final
        : public H3ActiveObject<H3MapHillFort>
    {
    private:
        static HillFortExtender* instance;

        void CreatePatches() override final;

        HillFortExtender();

        BOOL SetAiMapItemWeight(
            H3MapItem* mapItem,
            H3Hero* currentHero,
            const H3Player* activePlayer,
            int& aiMapItemWeight,
            int* moveDistance,
            H3Position pos
        ) const noexcept override final;

    public:
        static HillFortExtender& Get();
    };
}