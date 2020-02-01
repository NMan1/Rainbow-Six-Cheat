#pragma once
#include <ntifs.h>

BOOLEAN CleanUnloadedDrivers();
NTSTATUS CleanPiDDB();