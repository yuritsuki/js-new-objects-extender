#pragma once


namespace junkman {
    // type 144, subtype 7
    constexpr int JUNKMAN_OBJECT_SUBTYPE = 7;

    struct H3MapItemJunkman
    {

    };

    class JunkmanExtender final
        : public H3ActiveObject<H3MapItemJunkman>
    {
    private:
        static JunkmanExtender* instance;

        void CreatePatches() override final;

        JunkmanExtender();

        BOOL VisitMapItem(
            H3Hero* currentHero,
            H3MapItem* mapItem,
            H3Position pos,
            BOOL isHuman
        ) const noexcept override final;

        //int AI_OnScouting_Value() const noexcept override final;

    public:
        static JunkmanExtender& Get();
    };
}
