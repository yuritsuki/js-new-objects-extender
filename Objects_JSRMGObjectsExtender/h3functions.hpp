#pragma once


constexpr int MAX_HERO_MOVEMENT_POINTS = 1000000;
inline void ClampMovementPoints(int& movePts)
{
    if (movePts >= MAX_HERO_MOVEMENT_POINTS)
        movePts = MAX_HERO_MOVEMENT_POINTS;
    else
        movePts = movePts;
}

constexpr UINT8 MAX_HERO_PRIMARY_SKILL_VALUE = 252; // точно? (уточнить из PRIMA patches)
inline void ClampPrimarySkill(INT8* primarySkill)
{
    if (*primarySkill > MAX_HERO_PRIMARY_SKILL_VALUE)
    {
        *primarySkill = MAX_HERO_PRIMARY_SKILL_VALUE;
    }
}


namespace h3functions
{
inline int NeedExpoToNextLevel(const int lvl)
{
    return FASTCALL_1(int, 0x04DA690, lvl);
}

inline float GetHeroLearningPower(const H3Hero *hero)
{
    return THISCALL_1(float, 0x04E4AB0, hero);
}
inline void HeroLearnSpell(H3Hero *hero, const int spellId)
{
    THISCALL_2(void, 0x04D95A0, hero, spellId);
}
inline void HeroAddResource(H3Hero *hero, const int resourceId, const int amount)
{
    THISCALL_3(void, 0x04E3870, hero, resourceId, amount);
}
inline int GetAIHeroSpellValue(const H3Hero *hero, const int spellId)
{
    return FASTCALL_2(int, 0x0527B20, hero, spellId);
}
inline void SetMapItemDef(const H3MapItem *mapItem)
{
    THISCALL_2(void, 0x04C9650, H3Main::Get(), mapItem);
}
} // namespace h3functions
