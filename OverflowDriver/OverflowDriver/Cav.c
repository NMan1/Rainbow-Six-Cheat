#pragma once
#include "Features.h"
#include <wdftypes.h>

BOOLEAN EnableMarker(DWORD64 Player)
{
	unsigned char Toggle = 1;
	DWORD64 ComponentChain = Read(Player + OFFSET_ENTITY_COMPONENT, sizeof(DWORD64));
	if (!ComponentChain)
		return FALSE;

	DWORD64 ComponentList = Read(ComponentChain + OFFSET_ENTITY_COMPONENT_LIST, sizeof(DWORD64));
	if (!ComponentList)
		return FALSE;

	for (auto current_component = 0x80; current_component < 0xf0; current_component += sizeof(PVOID))
	{
		DWORD64 Component = Read(ComponentList + current_component, sizeof(DWORD64));
		if (!Component)
			continue;

		DWORD64 VT = BaseAddrR6 + BASE_OFFSET_ENTITY_MARKER_VT_OFFSET;
		if (Read(Component, sizeof(DWORD64)) != VT)
			continue;

		if (Read(Component + OFFSET_ESPCHAIN_WRITE, sizeof(BYTE)) != 0x1)
			Write(Component + OFFSET_ESPCHAIN_WRITE, &Toggle, sizeof(unsigned char));		
	}
	return;
}

BOOLEAN CavRun(DWORD64 LocalPlayer)
{
	for (int i = 0; i < GetMaxEntitys(); i++)
	{
		DWORD64 Entity = GetEntityByID(i, LocalPlayer);
		if (!Entity)
			continue;

		if (!EnableMarker(Entity))
			continue;
	}

	return TRUE;
}
