#include "../pch.h"
#include "DreamTeacherExtender.h"

namespace dreamTeacher
{
    int H3MapItemDreamTeacher::dreamTeacherCounter = 0;

    BOOL H3MapItemDreamTeacher::IsVisitedByHero(const H3MapItemDreamTeacher* dreamTeacher, const H3Hero* hero) noexcept
    {
        sprintf(h3_TextBuffer, ErmVariableFormat, dreamTeacher->id, hero->id);
        return Era::GetAssocVarIntValue(h3_TextBuffer);
    }
    void H3MapItemDreamTeacher::SetAsVisited(const H3MapItemDreamTeacher* dreamTeacher, const H3Hero* hero) noexcept
    {
        sprintf(h3_TextBuffer, ErmVariableFormat, dreamTeacher->id, hero->id);
        Era::SetAssocVarIntValue(h3_TextBuffer, 1);
    }
    int H3MapItemDreamTeacher::GetVisitedObjectsCountByHero(const H3Hero* hero) noexcept
    {
        sprintf(h3_TextBuffer, VisitedObjectsCount, hero->id);
        return Era::GetAssocVarIntValue(h3_TextBuffer);
    }
    void H3MapItemDreamTeacher::SetVisitedObjectsCountByHero(const H3Hero* hero, int visitedObjCount) noexcept
    {
        sprintf(h3_TextBuffer, VisitedObjectsCount, hero->id);
        Era::SetAssocVarIntValue(h3_TextBuffer, visitedObjCount);
    }

    _LHF_(Game__NewGameBeforeSetObjectsInitialParameters)
    {
        H3MapItemDreamTeacher::dreamTeacherCounter = 0;

        return EXEC_DEFAULT;
    }

    void DreamTeacherExtender::CreatePatches()
    {
        static bool hookInstalled = false;
        if (hookInstalled) return;

        _PI->WriteLoHook(0x04BFCBE, Game__NewGameBeforeSetObjectsInitialParameters);
        hookInstalled = true;
    }

    DreamTeacherExtender::DreamTeacherExtender()
        : H3ActiveObject<H3MapItemDreamTeacher>(
            "EraPlugin.DreamTeacherExtender.js",
            extender::ERA_OBJECT_TYPE,
            DREAM_TEACHER_OBJECT_SUBTYPE)
    {
        CreatePatches();
    }

    DreamTeacherExtender* DreamTeacherExtender::instance = nullptr;
    DreamTeacherExtender& DreamTeacherExtender::Get()
    {
        if (!instance) instance = new DreamTeacherExtender();
        return *instance;
    }

    BOOL DreamTeacherExtender::InitNewGameMapItemSetup(H3MapItem* mapItem) const noexcept
    {
        if (auto dreamTeacher = GetFromMapItem(mapItem))
        {
            dreamTeacher->id = H3MapItemDreamTeacher::dreamTeacherCounter++;
            return true;
        }

        return false;
    }

    BOOL DreamTeacherExtender::SetHintInH3TextBuffer(H3MapItem* mapItem, const H3Hero* hero, const int playerId, BOOL isRightClick) const noexcept
    {

        if (auto dreamTeacher = GetFromMapItem(mapItem))
        {
            H3String objName = RMGObjectInfo::GetObjectName(mapItem);
            int teamId = THISCALL_2(int, 0x4A55D0, P_Game->Get(), playerId);
            BOOL teamVisited = IsObjectVisitedByTeam(objectType, objectSubtype, teamId);

            // Add extra hint if object is visited at least once by the team
            if (teamVisited)
            {
                AddExtraInfoHint(&objName, isRightClick);
            }

            // Add "Visited/Not visited" hint
            if (hero)
            {
                const bool isVisitedByHero = H3MapItemDreamTeacher::IsVisitedByHero(dreamTeacher, hero);
                AddHeroVisitedHint(&objName, isRightClick, isVisitedByHero);
            }

            sprintf(h3_TextBuffer, "%s", objName.String());

            return true;
        }

        return false;
    }

    BOOL DreamTeacherExtender::VisitMapItem(
        H3Hero *hero, H3MapItem *mapItem, const H3Position pos,
        const BOOL isHuman) const noexcept
    {

        if (auto dreamTeacher = GetFromMapItem(mapItem))
        {
            const bool isVisitedByHero = H3MapItemDreamTeacher::IsVisitedByHero(dreamTeacher, hero);
            if (isVisitedByHero)
            {
                if (isHuman)
                {
                    FASTCALL_12(void, 0x4F6C00,
                        this->GetVisitedMessage().String(),
                        1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -777);
                }
            }
            else
            {
                ProcObjectFlagsVisitedByTeam(hero, objectType, objectSubtype);

                // get visited obj counter
                int visitedObjCount = H3MapItemDreamTeacher::GetVisitedObjectsCountByHero(hero);
            
                // calculate exp given considering the learning power
                float heroLearningPower = h3functions::GetHeroLearningPower(hero);
                int expGiven = static_cast<int>((EXP_GIVEN + (ADDITIONAL_EXP_GIVEN_FOR_EACH_VISIT * visitedObjCount)) * heroLearningPower);

                if (isHuman)
                {
                    FASTCALL_12(void, 0x4F6C00,
                        this->GetVisitingMessage().String(),
                        1, -1, -1, ePictureCategories::EXPERIENCE, expGiven, -1, 0, -1, 0, -1, -777);
                }
            
                // give exp
                THISCALL_4(void, 0x04E3620, hero, expGiven, 1, 1);

                // set obj as visited
                H3MapItemDreamTeacher::SetAsVisited(dreamTeacher, hero);

                // increase visited obj counter
                visitedObjCount += 1;
                H3MapItemDreamTeacher::SetVisitedObjectsCountByHero(hero, visitedObjCount);
            }

            return true;
        }

        return false;
    }

    BOOL DreamTeacherExtender::SetAiMapItemWeight(
        H3MapItem* mapItem, H3Hero* hero, const H3Player* player,
        int& aiMapItemWeight, int* moveDistance, const H3Position pos) const noexcept
    {
        if (auto dreamTeacher = GetFromMapItem(mapItem))
        {
            const bool isVisitedByHero = H3MapItemDreamTeacher::IsVisitedByHero(dreamTeacher, hero);

            if (!isVisitedByHero)
            {
                // get visited obj counter
                int visitedObjCount = H3MapItemDreamTeacher::GetVisitedObjectsCountByHero(hero);

                // pseudocode addr 0052BB89 (TreeofKnowledge) and 00529833 (LearningStone)

                // calculate exp given considering the learning power
                float heroLearningPower = h3functions::GetHeroLearningPower(hero);
                int expGiven = static_cast<int>((EXP_GIVEN + (ADDITIONAL_EXP_GIVEN_FOR_EACH_VISIT * visitedObjCount)) * heroLearningPower);

                // pseudocode addr 0052BB89
                //*(_QWORD *)&v8 = (__int64)(hero->turnExperienceToRVRatio * 1000.0);
                aiMapItemWeight = static_cast<int>(expGiven * hero->AI_experienceEffectiveness);
            }

            return true;
        }

        return false;
    }
}
