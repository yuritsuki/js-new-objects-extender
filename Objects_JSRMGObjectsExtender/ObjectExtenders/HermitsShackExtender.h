#pragma once

namespace hermitsShack
{
    // type 144, subtype 5;
    constexpr int HERMITS_SHACK_OBJECT_SUBTYPE = 5;

    constexpr LPCSTR noBasicOrAdvancedSkill_key =
        "RMG.objectGeneration.%d.%d.text.noBasicOrAdvancedSkill";

    struct H3MapItemHermitsShack
    {
        static int hermitsShackCounter;
        static constexpr LPCSTR ErmVariableFormat = "hermitsShack_%d_%d";

      public:
        INT32 id;

      public:
        static BOOL IsVisitedByHero(const H3MapItemHermitsShack *hermitsShack, const H3Hero *hero) noexcept;
        static void SetAsVisited(const H3MapItemHermitsShack* hermitsShack, const H3Hero* hero) noexcept;
    };

    class HermitsShackExtender final
        : public H3ActiveObject<H3MapItemHermitsShack>
    {
    private:
        static HermitsShackExtender* instance;

        void CreatePatches() override;

        HermitsShackExtender();

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



        H3String GetNoBasicOrAdvancedSkillMessage() const
        {
            return GetStringMessage(noBasicOrAdvancedSkill_key);
        }

    public:
        static HermitsShackExtender& Get();
    };

}
