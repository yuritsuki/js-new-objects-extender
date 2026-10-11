#pragma once


namespace jetsam {
    // type 145, subtype 2
    constexpr int JETSAM_OBJECT_SUBTYPE = 2;

    constexpr LPCSTR visit_1state_key =
        "RMG.objectGeneration.%d.%d.text.visit1state";

    constexpr LPCSTR visit_2state_key =
        "RMG.objectGeneration.%d.%d.text.visit2state";

    constexpr LPCSTR visit_3state_key =
        "RMG.objectGeneration.%d.%d.text.visit3state";

    struct H3MapItemJetsam
    {
        INT32 state;
    };

    class JetsamExtender final
        : public H3ActiveObject<H3MapItemJetsam>
    {
    private:
        static JetsamExtender* instance;

        JetsamExtender();

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

        H3String GetVisitingState1Message() const
        {
            return this->GetStringMessage(visit_1state_key);
        }

        H3String GetVisitingState2Message() const
        {
            return this->GetStringMessage(visit_2state_key);
        }

        H3String GetVisitingState3Message() const
        {
            return this->GetStringMessage(visit_3state_key);
        }

    public:
        static JetsamExtender& Get();
    };
}
