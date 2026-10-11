#pragma once

namespace ancientLamp
{
    // type 145, subtype 0
    constexpr int ANCIENT_LAMP_OBJECT_SUBTYPE = 0;

    constexpr int ANCIENT_LAMP_CREATURE_TYPE = 37;
    constexpr int ANCIENT_LAMP_MIN_CREATURE_COUNT = 4;
    constexpr int ANCIENT_LAMP_MAX_CREATURE_COUNT = 6;

    struct H3MapItemAncientLamp
    {
      public:
        unsigned _u1 : 5;
        /** @brief [05] which players have come by*/
        unsigned visited : 8;
        /** @brief [13]*/
        unsigned creatureType : 10;
        /** @brief [23]*/
        unsigned creatureCount : 9;

        void Reset();
    };

    class AncientLampExtender final
        : public H3ActiveObject<H3MapItemAncientLamp>
    {
    private:
        static AncientLampExtender* instance;

        AncientLampExtender();

        BOOL InitNewGameMapItemSetup(
            H3MapItem* mapItem) const noexcept override final;

        BOOL SetHintInH3TextBuffer(
            H3MapItem* mapItem,
            const H3Hero* hero,
            int playerId,
            BOOL isRightClick) const noexcept override final;

        BOOL VisitMapItem(
            H3Hero* hero,
            H3MapItem* mapItem,
            H3Position pos,
            BOOL isHuman) const noexcept override final;

        BOOL SetAiMapItemWeight(
            H3MapItem* mapItem,
            H3Hero* hero,
            const H3Player* player,
            int& aiMapItemWeight,
            int* moveDistance,
            H3Position pos) const noexcept override final;

        int AI_OnScouting_Value() const noexcept override final;

    public:
        static AncientLampExtender& Get();
    };
}
