#pragma once

CHAR GetObjectFlagsVisitedByTeam(INT32 objId, INT32 objSubtype);
void SetObjectFlagsVisitedByTeam(INT32 objId, INT32 objSubtype, CHAR flags);
INT8 GetTeamBitOffset(int teamId);
BOOL IsObjectVisitedByTeam(INT32 objId, INT32 objSubtype, int teamId);
CHAR SetObjectFlags(H3Game *game, int teamId, CHAR teamFlags);
void ProcObjectFlagsVisitedByTeam(H3Hero *hero, INT32 objType, INT32 objSubtype);

