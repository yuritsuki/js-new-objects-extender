#pragma once


namespace seafaringAcademy
{
    // type 146, subtype 0
    constexpr int SEAFARING_ACADEMY_OBJECT_SUBTYPE = 0;

    //constexpr int WARLOCKS_LAB_MARKET_BUILDING = 5;

    constexpr LPCSTR dlgHint_key = "RMG.objectGeneration.%d.%d.text.dlgHint";
    constexpr LPCSTR dlgHint1_key = "RMG.objectGeneration.%d.%d.text.dlgHint1";
    constexpr LPCSTR dlgHint2_key = "RMG.objectGeneration.%d.%d.text.dlgHint2";
    constexpr LPCSTR dlgHint3_key = "RMG.objectGeneration.%d.%d.text.dlgHint3";
    constexpr LPCSTR dlgHint4_key = "RMG.objectGeneration.%d.%d.text.dlgHint4";

    struct H3MapItemSeafaringAcademy
    {
        char skillId1;
        char skillId2;
        char visited;
        char u03;
    };

    class SeafaringAcademyExtender final
        : public H3ActiveObject<H3MapItemSeafaringAcademy>
    {
    private:
        static SeafaringAcademyExtender* instance;

        void CreatePatches() override final;

        SeafaringAcademyExtender();

        BOOL InitNewGameMapItemSetup(
            H3MapItem* mapItem) const noexcept override final;

        BOOL SetHintInH3TextBuffer(
            H3MapItem* mapItem,
            const H3Hero* hero,
            int playerId,
            BOOL isRightClick) const noexcept override final;

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

        //int AI_OnScouting_Value() const noexcept override final;

    public:
        static SeafaringAcademyExtender& Get();
    };
}
