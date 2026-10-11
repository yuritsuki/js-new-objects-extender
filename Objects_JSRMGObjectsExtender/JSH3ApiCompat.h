#pragma once

// H3API_JS names used by the migrated fork, with the original HotA/game
// identifiers and addresses. Keep these local to this consumer plugin.
namespace h3
{
using ::INT32;
#pragma pack(push, 4)
struct H3CreatureSwapper
{
    H3Army *army;
    H3Army *adjacentArmy;
    CHAR hasAngelicAlliance;
    INT16 morale;
    INT16 alignmentCount;
    CHAR alignments[10];
    INT32 armyValueIncrease;
    INT16 improvement;
};
#pragma pack(pop)
static_assert(sizeof(H3CreatureSwapper) == 0x20, "H3CreatureSwapper layout mismatch");

namespace jsObjectIds
{
enum eHotaObject : INT32
{
    H_DECORATIVE = 139,
    PUZZLE = 140,
    SPECIAL_FIELD = 141,
    WAREHOUSE = 142,
    ACTIVE = 144,
    TYPE_145 = 145,
    TYPE_146 = 146,
    UNTRIGGERABLE = 212,
};

enum eHotaObjectActiveType : INT32
{
    TEMPLE_OF_LOYALTY = 0,
    SKELETON_TRANSFORMER = 1,
    COLOSSEUM_OF_THE_MAGI = 2,
    WATERING_PLACE = 3,
    MINERAL_SPRING = 4,
    HERMITS_SHACK = 5,
    GAZEBO = 6,
    JUNKMAN = 7,
    DERRICK = 8,
    WARLOCKS_LAB = 9,
    PROSPECTOR = 10,
    TRAILBLAZER = 11,
};

enum eHotaObjectType145 : INT32
{
    ANCIENT_LAMP = 0,
    SEA_BARREL = 1,
    JETSAM = 2,
    VIAL_OF_MANA = 3,
};

enum eHotaObjectType146 : INT32
{
    SEAFARING_ACADEMY = 0,
    OBSERVATORY = 1,
    ALTAR_OF_MANA = 2,
    TOWN_GATE = 3,
    ANCIENT_ALTAR = 4,
};

enum eHotaUntriggerableObject : INT32
{
    QUEST_GATE = 1000,
    GRAVE = 1001,
};

} // namespace jsObjectIds

using eHotaObject = jsObjectIds::eHotaObject;
using eHotaObjectActiveType = jsObjectIds::eHotaObjectActiveType;
using eHotaObjectType145 = jsObjectIds::eHotaObjectType145;
using eHotaObjectType146 = jsObjectIds::eHotaObjectType146;
using eHotaUntriggerableObject = jsObjectIds::eHotaUntriggerableObject;

enum eMarketType : INT32
{
    TRADE_RESOURCES = 0,
    TRANSFER_RESOURCES = 1,
    BUY_ARTIFACTS = 2,
    SELL_ARTIFACTS = 3,
    SELL_CREATURES = 4,
    LEAVE_DIALOG = 30722,
};

enum eMarketBuilding : INT32
{
    TOWN_MARKET = 0,
    TRADING_POST = 1,
    BLACK_MARKET = 2,
    FREELANCERS_GUILD = 3,
    JUNKMAN = 4,
    WARLOCKS_LAB = 5,
};
} // namespace h3

#define P_MarketHero (*(h3::H3Hero **)0x6AAAE0)
#define P_ArtifactMerchant (*(h3::H3ArtifactMerchant **)0x6AAADC)
#define P_ActivePlayerMarkets (*(INT32 *)0x6AAB00)
#define P_MarketType (*(h3::eMarketType *)0x6AAB0C)
#define P_MarketBuilding (*(h3::eMarketBuilding *)0x6AAB2C)
#define P_MarketSelectedSlotIndex (*(INT32 *)0x6AAAF8)
#define P_NetworkGame (*(h3::INT32 *)0x69959C)

// Source-level aliases used by the pre-extender fork. These names map to
// the current H3API layout and are intentionally local to this plugin.
#define destX dest_x
#define destY dest_y
#define destZ dest_z
#define GetPlayerTeam(playerId) mapInfo.playerTeam[playerId]

void InstallExtraHints(PatcherInstance *patcher) noexcept;

