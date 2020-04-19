#pragma once

#include "targetver.h"

// exclude rarely-used stuff from Windows headers
#define WIN32_LEAN_AND_MEAN             

#include <stdint.h>
#include <windows.h>

#include "../include/iumbase.h"

#include "../common/image_policy.h"

#include "../rpc/params.h"
#include "../rpc/trustlet_debug.h"
