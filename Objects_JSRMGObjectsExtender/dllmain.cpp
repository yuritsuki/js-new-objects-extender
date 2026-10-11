#include "pch.h"

#include "ObjectExtenders/AncientLampExtender.h"
#include "ObjectExtenders/AltarOfManaExtender.h"
#include "ObjectExtenders/DreamTeacherExtender.h"
#include "ObjectExtenders/GraveExtender.h"
#include "ObjectExtenders/HermitsShackExtender.h"
#include "ObjectExtenders/HillFortExtender.h"
#include "ObjectExtenders/JetsamExtender.h"
#include "ObjectExtenders/JunkmanExtender.h"
#include "ObjectExtenders/MineralSpringExtender.h"
#include "ObjectExtenders/ObservatoryExtender.h"
#include "ObjectExtenders/ProspectorExtender.h"
#include "ObjectExtenders/SeaBarrelExtender.h"
#include "ObjectExtenders/SeafaringAcademyExtender.h"
#include "ObjectExtenders/SkeletonTransformerExtender.h"
#include "ObjectExtenders/TempleOfLoyaltyExtender.h"
#include "ObjectExtenders/TownGateExtender.h"
#include "ObjectExtenders/TrailblazerExtender.h"
#include "ObjectExtenders/VialOfManaExtender.h"
#include "ObjectExtenders/WarlocksLabExtender.h"
#include "ObjectExtenders/WateringPlaceExtender.h"

Patcher *globalPatcher = nullptr;
PatcherInstance *_PI = nullptr;

namespace
{
constexpr LPCSTR INSTANCE_NAME = "EraPlugin.Objects_JSRMGObjectsExtender.js";
}

BOOL APIENTRY DllMain(HMODULE module, DWORD reason, LPVOID)
{
    if (reason == DLL_PROCESS_ATTACH)
    {
        globalPatcher = GetPatcher();
        _PI = globalPatcher->CreateInstance(INSTANCE_NAME);
        Era::ConnectEra(module, INSTANCE_NAME);

        InstallExtraHints(_PI);

        // These hooks support shared team-visit state and water-object behavior
        // used by several of the migrated extenders.
        FlagsExtender_Init();
        WaterObjects();

        ancientLamp::AncientLampExtender::Get().Register();
        altarOfMana::AltarOfManaExtender::Get().Register();
        dreamTeacher::DreamTeacherExtender::Get().Register();
        grave::GraveExtender::Get().Register();
        hermitsShack::HermitsShackExtender::Get().Register();
        hillFort::HillFortExtender::Get().Register();
        jetsam::JetsamExtender::Get().Register();
        junkman::JunkmanExtender::Get().Register();
        mineralSpring::MineralSpringExtender::Get().Register();
        observatory::ObservatoryExtender::Get().Register();
        prospector::ProspectorExtender::Get().Register();
        seaBarrel::SeaBarrelExtender::Get().Register();
        seafaringAcademy::SeafaringAcademyExtender::Get().Register();
        skeletonTransformer::SkeletonTransformerExtender::Get().Register();
        templeOfLoyalty::TempleOfLoyaltyExtender::Get().Register();
        townGate::TownGateExtender::Get().Register();
        trailblazer::TrailblazerExtender::Get().Register();
        vialOfMana::VialOfManaExtender::Get().Register();
        warlocksLab::WarlocksLabExtender::Get().Register();
        wateringPlace::WateringPlaceExtender::Get().Register();
    }
    return TRUE;
}
