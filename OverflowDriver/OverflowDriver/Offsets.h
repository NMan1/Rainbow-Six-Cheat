#pragma once
#include <ntifs.h>

/* Cheat Offsets */

/*~~~~~~~~~~~~~~~~~~GAME_MAN~~~~~~~~~~~~~~~~~*/
extern UINT64 BASE_OFFSET_GAME_MAN;
extern UINT64 OFFSET_MAX_ENTITYS;
extern UINT64 OFFSET_ENTITY_LIST1;
extern UINT64 OFFSET_ENTITY_LIST2;
extern UINT64 OFFSET_ENTITY_RE;
extern UINT64 OFFSET_ENTITY_COUNT;
extern UINT64 OFFSET_ENTITY_REPLICATION;
extern UINT64 OFFSET_ENTITY_REPLICATION_TEAM;
/*~~~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~~*/


/*~~~~~~~~~~~~~~~~~~RENDER_MAN~~~~~~~~~~~~~~~*/
extern UINT64 BASE_OFFSET_RENDER_MAN;
extern UINT64 OFFSET_CAMERA[4];
extern UINT64 OFFSET_VIEWFOVX;
extern UINT64 OFFSET_VIEWFOVY;
extern UINT64 OFFSET_VIEWFORWARD;
extern UINT64 OFFSET_VIEWUP;
extern UINT64 OFFSET_VIEWRIGHT;
extern UINT64 OFFSET_VIEWTRANSLATION;
/*~~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~*/


/*~~~~~~~~~~~~~~~~~~NET_MAN~~~~~~~~~~~~~~~~*/
extern UINT64 BASE_OFFSET_NET_MAN;
extern UINT64 OFFSET_NOCLIP[3];
/*~~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~*/


/*~~~~~~~~~~~~~~~~~~CHAM_MAN~~~~~~~~~~~~~~~*/
extern UINT64 BASE_OFFSET_CHAM_MAN;
extern UINT64 OFFSET_CHAMSMAIN;

extern UINT64 OFFSET_DIS[2];
extern UINT64 OFFSET_OPP;
extern UINT64 OFFSET_RGB[3];
/*~~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~*/


/*~~~~~~~~~~~~~~~~~~~FOV_MAN~~~~~~~~~~~~~~~*/
extern UINT64 BASE_OFFSET_FOV_MAN;
extern UINT64 OFFSET_FOV[2];
extern UINT64 OFFSET_CHARFOV;
extern UINT64 OFFSET_WEPFOV;
/*~~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~*/


/*~~~~~~~~~~~~~~~~~~ROUND_MAN~~~~~~~~~~~~~~*/
extern UINT64 BASE_OFFSET_ROUND_MAN;
extern UINT64 OFFSET_ROUND;
/*~~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~*/


/*~~~~~~~~~~~~~~~~~~PROFILE_MAN~~~~~~~~~~~~*/
extern UINT64 BASE_OFFSET_PROFILE_MAN;
extern UINT64 OFFSET_PROFILE_CONTAINER;
extern UINT64 OFFSET_PROFILE_LOCALENTITY;
extern UINT64 OFFSET_PROFILE_SKELETON;
extern UINT64 OFFSET_PROFILE_VIEWANGLE[2];
extern UINT64 OFFSET_PROFILE_SILENTVIEWANGLE;

extern UINT64 OFFSET_SPEED[4];
extern UINT64 OFFSET_WEAPON[3];
extern UINT64 OFFSET_WEPSPREAD;
extern UINT64 OFFSET_BULLET[4];
extern UINT64 OFFSET_NOFLASH[4];

extern UINT64 OFFSET_HEAD;
extern UINT64 OFFSET_NECK;
extern UINT64 OFFSET_RIGHT_HAND;
extern UINT64 OFFSET_CHEST;
extern UINT64 OFFSET_STOMACH;
extern UINT64 OFFSET_PELVIS;
extern UINT64 OFFSET_FEET;
/*~~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~~~*/


/*~~~~~~~~~~~~~~~~~~SETTINGS_MAN~~~~~~~~~~~~~*/
extern UINT64 BASE_OFFSET_SETTINGS_MAN;
extern UINT64 WIDTH;
extern UINT64 HEIGHT;
/*~~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~~~*/


/*~~~~~~~~~~~~~~~~~~MARKER_MAN~~~~~~~~~~~~~~~*/
extern UINT64 BASE_OFFSET_ENTITY_MARKER_VT_OFFSET;
extern UINT64 OFFSET_ENTITY_COMPONENT;
extern UINT64 OFFSET_ENTITY_COMPONENT_LIST;
extern UINT64 OFFSET_MAINCOMPONENT_ESPCHAIN;
extern UINT64 OFFSET_ESPCHAIN_WRITE;
/*~~~~~~~~~~~~~~~~~~~END~~~~~~~~~~~~~~~~~~~~~*/

extern UINT64 BASE_OFFSET_NORECOIL;
extern UINT64 BASE_OFFSET_UNLOCK_ALL;
extern UINT64 BASE_OFFSET_SILENT;

/* Cheat Variabls*/
extern DWORD64		KernelBase;
extern DWORD64		BaseAddrR6;
extern PEPROCESS	R6Process;
extern ULONG		PID_R6;

/*
0 = round - swap
1 = operator selection
2 = prep - phase
3 = action - phase
4 = end of round
5 = main menu
*/

/*
0 - Default player model camera
1 - On camera
2 - On drone
3 - Watching kill cam / replay
*/