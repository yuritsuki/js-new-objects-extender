#pragma once

namespace dreamTeacher
{
    constexpr int DREAM_TEACHER_OBJECT_SUBTYPE = 0;
    constexpr float EXP_GIVEN = 500.0f;
    constexpr float ADDITIONAL_EXP_GIVEN_FOR_EACH_VISIT = 500.0f;

    struct H3MapItemDreamTeacher
    {
        static int dreamTeacherCounter;

        static constexpr LPCSTR ErmVariableFormat = "dreamTeacher_%d_%d"; // visited or not
        static constexpr LPCSTR VisitedObjectsCount = "dreamTeacher_%d"; // heroId

      public:
        INT32 id;

      public:
        static BOOL IsVisitedByHero(const H3MapItemDreamTeacher * dreamTeacher, const H3Hero *hero) noexcept;
        static void SetAsVisited(const H3MapItemDreamTeacher* dreamTeacher, const H3Hero* hero) noexcept;
        static int GetVisitedObjectsCountByHero(const H3Hero* hero) noexcept;
        static void SetVisitedObjectsCountByHero(const H3Hero* hero, int visitedObjCount) noexcept;
    };

    class DreamTeacherExtender final
        : public H3ActiveObject<H3MapItemDreamTeacher>
    {
    private:
        static DreamTeacherExtender* instance;

        void CreatePatches() override;

        DreamTeacherExtender();

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

    public:
        static DreamTeacherExtender& Get();
    };
}
