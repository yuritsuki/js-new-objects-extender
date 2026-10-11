#include "../pch.h"
#include "HermitsShackExtender.h"

namespace hermitsShack
{
    int H3MapItemHermitsShack::hermitsShackCounter = 0;

    BOOL H3MapItemHermitsShack::IsVisitedByHero(const H3MapItemHermitsShack* hermitsShack, const H3Hero* hero) noexcept
    {
        sprintf(newMapItem::buffer, ErmVariableFormat, hermitsShack->id, hero->id);
        return Era::GetAssocVarIntValue(newMapItem::buffer);
    }
    void H3MapItemHermitsShack::SetAsVisited(const H3MapItemHermitsShack* hermitsShack, const H3Hero* hero) noexcept
    {
        sprintf(newMapItem::buffer, H3MapItemHermitsShack::ErmVariableFormat, hermitsShack->id, hero->id);
        Era::SetAssocVarIntValue(newMapItem::buffer, 1);
    }

    _LHF_(Game__NewGameBeforeSetObjectsInitialParameters)
    {
        H3MapItemHermitsShack::hermitsShackCounter = 0;

        return EXEC_DEFAULT;
    }

    void HermitsShackExtender::CreatePatches()
    {
        static bool hookInstalled = false;
        if (hookInstalled) return;

        _PI->WriteLoHook(0x04BFCBE, Game__NewGameBeforeSetObjectsInitialParameters);
        hookInstalled = true;
    }

    HermitsShackExtender::HermitsShackExtender()
        : H3ActiveObject<H3MapItemHermitsShack>(
            "EraPlugin.HermitsShackExtender.js",
            extender::HOTA_OBJECT_TYPE,
            HERMITS_SHACK_OBJECT_SUBTYPE)
    {
        CreatePatches();
    }

    HermitsShackExtender* HermitsShackExtender::instance = nullptr;
    HermitsShackExtender& HermitsShackExtender::Get()
    {
        if (!instance) instance = new HermitsShackExtender();
        return *instance;
    }

    BOOL HermitsShackExtender::InitNewGameMapItemSetup(H3MapItem* mapItem) const noexcept
    {

        if (auto hermitsShack = GetFromMapItem(mapItem))
        {
            hermitsShack->id = H3MapItemHermitsShack::hermitsShackCounter++;
            return true;
        }

        return false;
    }

    BOOL HermitsShackExtender::SetHintInH3TextBuffer(H3MapItem* mapItem, const H3Hero* hero, const int playerId, BOOL isRightClick) const noexcept
    {

        if (auto hermitsShack = GetFromMapItem(mapItem))
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
                const bool isVisitedByHero = H3MapItemHermitsShack::IsVisitedByHero(hermitsShack, hero);
                AddHeroVisitedHint(&objName, isRightClick, isVisitedByHero);
            }

            sprintf(h3_TextBuffer, "%s", objName.String());

            return true;
        }

        return false;
    }

    BOOL HermitsShackExtender::VisitMapItem(
        H3Hero *hero, H3MapItem *mapItem, const H3Position pos,
        const BOOL isHuman) const noexcept
    {

        if (auto hermitsShack = GetFromMapItem(mapItem))
        {
            ProcObjectFlagsVisitedByTeam(hero, objectType, objectSubtype);

            const bool isVisitedByHero = H3MapItemHermitsShack::IsVisitedByHero(hermitsShack, hero);
            if (isVisitedByHero)
            {
                if (isHuman)
                {
                    FASTCALL_12(void, 0x4F6C00,
                        this->GetVisitedMessage().String(),
                        1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -777);
                }
                return true;
            }


            // Check basic or advanced secondary skills
            int SECONDARY_SKILLS = h3::limits::SECONDARY_SKILLS;
            std::vector<int> skillIds;
            
            int i = 0;
            while (i < SECONDARY_SKILLS)
            {
                if (hero->secSkill[i] < 3 && hero->secSkill[i] > 0)
                {
                    skillIds.push_back(i);
                }
                i++;
            }

            // Ќет скиллов дл€ апа
            if (skillIds.empty())
            {
                // и все слоты зан€ты экспертными скиллами
                if (hero->secSkillCount >= 8)
                {
                    if (isHuman)
                    {
                        FASTCALL_12(void, 0x4F6C00,
                            this->GetCannotVisitMessage().String(),
                            1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -777);
                    }
                }
                // есть слоты, но нет базовых/продвинутых скиллов
                else
                {
                    if (isHuman)
                    {
                        FASTCALL_12(void, 0x4F6C00,
                            this->GetNoBasicOrAdvancedSkillMessage().String(),
                            1, -1, -1, -1, 0, -1, 0, -1, 0, -1, -777);
                    }
                }
            }
            // ≈сть скиллы
            else
            {
                int size = static_cast<int>(skillIds.size());
                int random = H3Random::Rand(1, size);
                int skillId = skillIds[random - 1];
                int skillLvl = int(hero->secSkill[skillId]);

                if (isHuman)
                {
                    FASTCALL_12(void, 0x4F6C00,
                        this->GetVisitingMessage().String(),
                        1, -1, -1, 20, 3 * (skillId + 1) + (unsigned __int8)hero->secSkill[skillId],
                        -1, 0, -1, 0, -1, -777);
                }

                hero->LearnSecondarySkill(skillId, 1);
                H3MapItemHermitsShack::SetAsVisited(hermitsShack, hero);

            }

            return true;
        }

        return false;
    }

    BOOL HermitsShackExtender::SetAiMapItemWeight(
        H3MapItem* mapItem, H3Hero* hero, const H3Player* player,
        int& aiMapItemWeight, int* moveDistance, const H3Position pos) const noexcept
    {
        if (auto hermitsShack = GetFromMapItem(mapItem))
        {
            const bool isVisitedByHero = H3MapItemHermitsShack::IsVisitedByHero(hermitsShack, hero);

            if (!isVisitedByHero)
            {
                // Pattern used: return AIHero_GetObjectPosWeight_WitchHut(hero, mapItem); 0052BDE0

                const int heroOwner = hero->owner;
                bool heroHasBasicOrAdvancedSkill = false;
                int i = 0;
                while (i < 28 && !heroHasBasicOrAdvancedSkill)
                {
                    if (hero->secSkill[i] < 3 && hero->secSkill[i] > 0)
                    {
                        heroHasBasicOrAdvancedSkill = true;
                        break;
                    }
                    i++;
                }
                if (!heroHasBasicOrAdvancedSkill)
                {
                    return false;
                }

                aiMapItemWeight = static_cast<int>(
                    static_cast<float>(hero->ExpNeedToNextLevel(hero->level)) * hero->AI_experienceEffectiveness);

                return true;
            }
        }

        return false;
    }

    int HermitsShackExtender::AI_OnScouting_Value() const noexcept
    {
        return 50;
    }
}
