#pragma once
#pragma warning(disable: 4200)
#pragma warning(disable: 4996)

#include "targetver.h"

// exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN             

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include <TlHelp32.h>

#include "debug.h"

#include "../vm_exec.h"
#include "../common/common.h"
