#pragma once


namespace altarOfMana
{
    // type 146, subtype 2
    constexpr int ALTAR_OF_MANA_OBJECT_SUBTYPE = 2;

    constexpr LPCSTR dlgHint_key = "RMG.objectGeneration.%d.%d.text.dlgHint";
    constexpr LPCSTR dlgHint1_key = "RMG.objectGeneration.%d.%d.text.dlgHint1";
    constexpr LPCSTR dlgHint2_key = "RMG.objectGeneration.%d.%d.text.dlgHint2";
    constexpr LPCSTR dlgHint3_key = "RMG.objectGeneration.%d.%d.text.dlgHint3";
    constexpr LPCSTR dlgHint4_key = "RMG.objectGeneration.%d.%d.text.dlgHint4";

    struct H3MapItemAltarOfMana
    {
        // Собственных данных в mapItem->setup пока нет.
    };

    class AltarOfManaExtender final
        : public H3ActiveObject<H3MapItemAltarOfMana>
    {
    private:
        static AltarOfManaExtender* instance;

        void CreatePatches() override final;

        AltarOfManaExtender();

        BOOL VisitMapItem(
            H3Hero* currentHero,
            H3MapItem* mapItem,
            H3Position pos,
            BOOL isHuman
        ) const noexcept override final;

        //int AI_OnScouting_Value() const noexcept override final;

    public:
        static AltarOfManaExtender& Get();
    };
}
