#pragma once

namespace grave
{
    // HotA: type 212, subtype 1001;
    constexpr int GRAVE_OBJECT_TYPE = eObject::BORDER_GATE;
    constexpr int GRAVE_OBJECT_SUBTYPE = eHotaUntriggerableObject::GRAVE;
    constexpr int MORALE_PENALTY = 3;

    constexpr LPCSTR dig_key =
        "RMG.objectGeneration.%d.%d.text.dig";

    struct H3MapItemGrave
    {
      static constexpr LPCSTR ErmVariableFormat = "grave_%d";

      public:
          /** @brief [00]*/
          UINT16    goldAmount;
          /** @brief [02]*/
          INT16     artifactID;

      public:
        static BOOL IsVisitedByHero(H3Hero* hero) noexcept;
        static void SetAsVisited(H3Hero* hero) noexcept;
        static void SetAsNotVisited(H3Hero* hero) noexcept;
    };


    // Для чтения сохраненных настроек из файла (информация сохраняется в файл в редакторе)
    /*
    // 100DF8D0: запись настроек могилы; порядок полей в H3M отличается от памяти.
#pragma pack(push, 1)
    struct HotaGraveRewardSettings
    {
        INT32 randomReward = -1;    // +00: 0 — заданная награда, иначе случайная.
        UINT8 firstRewardFlag = 0;  // +04
        INT32 gold = 0;             // +05
        UINT8 secondRewardFlag = 0; // +09
        INT32 secondReward = 0;     // +0A: для могилы не используется.
        INT32 artifactId = -1;      // +0E
        BOOL Read(H3GzFile* file);
    };
#pragma pack(pop)
    static_assert(sizeof(HotaGraveRewardSettings) == 0x12, "HotA grave reward settings");

    */
    // 10159AA0 / 101EC760. Фабрика вызывается общим загрузчиком replay.
    H3ReplayAction* CreateGravePlacementReplay(INT8 player);

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

        H3String GetDiggingMessage() const
        {
            return this->GetStringMessage(dig_key);
        }

        static GraveExtender& Get();
    };
}
