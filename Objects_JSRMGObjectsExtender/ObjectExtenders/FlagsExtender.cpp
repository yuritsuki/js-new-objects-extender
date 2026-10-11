// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "../pch.h"
#include "GraveExtender.h"
#include "TempleOfLoyaltyExtender.h"
#include "WateringPlaceExtender.h"
#include "MineralSpringExtender.h"
#include "TrailblazerExtender.h"
#include "ObservatoryExtender.h"


LPCSTR effectDesc_key = "RMG.objectGeneration.%d.%d.text.effectDesc";



_LHF_(LoHook_Crbank_MoraleModifList)
{
	H3Hero* hero = reinterpret_cast<H3Hero*>(c->ebx);
	H3String* str = (H3String*)(c->ebp - 0x28);
	//UINT32 flagBits = H3HeroExtFlagsManager::Get(hero->id);

	// TODO: Погост.
	//if (flagBits & eHeroExtFlags::CHURCHYARD_PENALTY)
	//{
	//	// Сообщение.
	//	str->Append("\n");
	//	str->Append(EraJS::read(
	//		H3String::Format(H3HeroExtFlagsManager::graveEffectDesc_key, extender::ERA_OBJECT_TYPE, grave::GRAVE_OBJECT_SUBTYPE)
	//		.String()));

	//	// Счётчик боевого духа.
	//	IntAt(c->ebp - 0x10) -= 3;
	//}

	// Могила
	if (grave::H3MapItemGrave::IsVisitedByHero(hero))
	{
		// Сообщение.
		str->Append("\n");
		str->Append(EraJS::read(H3String::Format(effectDesc_key, extender::ERA_OBJECT_TYPE, grave::GRAVE_OBJECT_SUBTYPE).String()));
		// Счётчик боевого духа.
		IntAt(c->ebp - 0x10) -= grave::MORALE_PENALTY;
	}

	return EXEC_DEFAULT;
}

_LHF_(LoHook_004DD195)
{
	H3Hero* hero = reinterpret_cast<H3Hero*>(c->ebx);
	H3String* str = (H3String*)(c->ebp - 0x28);

	// Минеральный источник
	if (mineralSpring::H3MapItemMineralSpring::IsVisitedByHero(hero))
	{
		// Сообщение.
		str->Append("\n");
		str->Append(EraJS::read(H3String::Format(effectDesc_key, eHotaObject::ACTIVE, eHotaObjectActiveType::MINERAL_SPRING).String()));
		// Счётчик удачи.
		IntAt(c->ebp - 0x10) += mineralSpring::LUCK_GIVEN;
	}

	return EXEC_DEFAULT;
}

void __stdcall HiHook_H3HeroReset(HiHook* h, H3Hero* hero, INT16 heroId)
{
	THISCALL_2(void, h->GetDefaultFunc(), hero, heroId);
	templeOfLoyalty::H3MapItemTempleOfLoyalty::SetAsNotVisited(hero);
	wateringPlace::H3MapItemWateringPlace::SetAsNotVisited(hero);
	mineralSpring::H3MapItemMineralSpring::SetAsNotVisited(hero);
	trailblazer::H3MapItemTrailblazer::SetAsNotVisited(hero);

	observatory::H3MapItemObservatory::SetAsNotVisited(hero);

	grave::H3MapItemGrave::SetAsNotVisited(hero);
}

_ERH_(OnAfterBattleUniversal)
{
	for (size_t i = 0; i < 2; i++)
	{
		H3Hero* hero = P_CombatMgr->hero[i];
		if (hero)
		{
			mineralSpring::H3MapItemMineralSpring::SetAsNotVisited(hero);
			grave::H3MapItemGrave::SetAsNotVisited(hero);

		}	
	}
}

int __stdcall HiHook_OnNewPlayerDay(HiHook* h, H3Hero* hero)
{
	int result = THISCALL_1(int, h->GetDefaultFunc(), hero);
	templeOfLoyalty::H3MapItemTempleOfLoyalty::SetAsNotVisited(hero);

	return result;
}

//_ERH_(OnEveryDay)
//{
//	for (size_t i = 0; i < h3::limits::HEROES; i++)
//	{
//		H3Hero* hero = P_Game->GetHero(i);
//	}
//}

void __stdcall HiHook_OnNewWeek(HiHook* h, H3Game* this_)
{
	THISCALL_1(void, h->GetDefaultFunc(), this_);

	// Heroes flags
	for (size_t i = 0; i < P_HeroCount; i++)
	{
		H3Hero* hero = P_Game->GetHero(i);
		trailblazer::H3MapItemTrailblazer::SetAsNotVisited(hero);
		observatory::H3MapItemObservatory::SetAsNotVisited(hero);
	}
	
	// Refugee camp - reset player flags
	for (size_t i = 0; i < P_CreatureCount; i++)
	{
		SetObjectFlagsVisitedByTeam(eObject::REFUGEE_CAMP, i, 0);
	}
}


void FlagsExtender_Init()
{
	// Описание морали героя - список положительных/отрицательных бонусов к боевому духу героя.
	_PI->WriteLoHook(0x4DC8C7, LoHook_Crbank_MoraleModifList);

	// Описание удачи героя - список положительных/отрицательных бонусов к удаче героя.
	_PI->WriteLoHook(0x4DD195, LoHook_004DD195);

	// Сброс флагов модификаторов при сбросе героя
	_PI->WriteHiHook(0x4D8990, SPLICE_, EXTENDED_, THISCALL_, HiHook_H3HeroReset);

	// Сброс флагов модификаторов героев после битвы.
	Era::RegisterHandler(OnAfterBattleUniversal, "OnAfterBattleUniversal");

	// Сброс флагов при наступлении хода игрока
	_PI->WriteHiHook(0x4C70A5, CALL_, EXTENDED_, THISCALL_, HiHook_OnNewPlayerDay);
	_PI->WriteHiHook(0x4C70F8, CALL_, EXTENDED_, THISCALL_, HiHook_OnNewPlayerDay);
	// TODO!!!: проверить вот это: sub_1013C190

	// Сброс флагов при наступлении хода красного игрока
	//Era::RegisterHandler(OnEveryDay, "OnEveryDay");

	// Сброс флагов при наступлении новой недели
	_PI->WriteHiHook(0x4C8440, SPLICE_, EXTENDED_, THISCALL_, HiHook_OnNewWeek);

}
