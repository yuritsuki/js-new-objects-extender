#include "../pch.h"
#include "TeamVisitFlags.h"

namespace
{
constexpr LPCSTR TEAM_VISITED_FORMAT = "object_%d_%d_visitedByTeams";
}

CHAR GetObjectFlagsVisitedByTeam(const INT32 objId, const INT32 objSubtype)
{
    libc::sprintf(h3_TextBuffer, TEAM_VISITED_FORMAT, objId, objSubtype);
    return static_cast<CHAR>(Era::GetAssocVarIntValue(h3_TextBuffer));
}

void SetObjectFlagsVisitedByTeam(const INT32 objId, const INT32 objSubtype, const CHAR flags)
{
    libc::sprintf(h3_TextBuffer, TEAM_VISITED_FORMAT, objId, objSubtype);
    Era::SetAssocVarIntValue(h3_TextBuffer, flags);
}

INT8 GetTeamBitOffset(const int teamId)
{
    return static_cast<INT8>(1 << teamId);
}

BOOL IsObjectVisitedByTeam(const INT32 objId, const INT32 objSubtype, const int teamId)
{
    return GetObjectFlagsVisitedByTeam(objId, objSubtype) & GetTeamBitOffset(teamId);
}

CHAR SetObjectFlags(H3Game *game, const int teamId, CHAR teamFlags)
{
    for (int playerId = 0; playerId < 8; ++playerId)
    {
        if (game->mapInfo.playerTeam[playerId] == teamId)
            teamFlags |= static_cast<CHAR>(1u << playerId);
    }
    return teamFlags;
}

void ProcObjectFlagsVisitedByTeam(H3Hero *hero, const INT32 objType, const INT32 objSubtype)
{
    H3Game *game = P_Game->Get();
    const int teamId = game->mapInfo.playerTeam[hero->owner];
    const CHAR flags = SetObjectFlags(game, teamId, GetObjectFlagsVisitedByTeam(objType, objSubtype));
    SetObjectFlagsVisitedByTeam(objType, objSubtype, flags);
}

