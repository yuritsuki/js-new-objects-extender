#pragma once

namespace grave
{
    // type 141, subtype 1;
    constexpr int GRAVE_OBJECT_SUBTYPE = 1;
    constexpr int MORALE_PENALTY = 3;

    constexpr LPCSTR dig_key =
        "RMG.objectGeneration.%d.%d.text.dig";

    struct H3MapItemGrave
    {
      static constexpr LPCSTR ErmVariableFormat = "grave_%d";

      public:
          /** @brief [00]*/
          INT16     goldAmount;
          /** @brief [02]*/
          INT16     artifactID;

      public:
        static BOOL IsVisitedByHero(H3Hero* hero) noexcept;
        static void SetAsVisited(H3Hero* hero) noexcept;
        static void SetAsNotVisited(H3Hero* hero) noexcept;
    };

    class GraveExtender final
        : public H3ActiveObject<H3MapItemGrave>
    {
    private:
        static GraveExtender* instance;

        void CreatePatches() override;

        GraveExtender();

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
        void GraveDiggingProccess(H3MapItem* mapItem, H3Hero* hero) const noexcept;

        H3String GetDiggingMessage() const
        {
            return this->GetStringMessage(dig_key);
        }

        static GraveExtender& Get();
    };
}
