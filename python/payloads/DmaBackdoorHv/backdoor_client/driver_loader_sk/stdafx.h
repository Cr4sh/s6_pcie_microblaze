extern "C"
{
#include <ntifs.h>
#include <stdio.h>
#include <stdarg.h>
#include <ntimage.h>
}

#pragma intrinsic(memset)
#pragma intrinsic(memcpy)
#pragma intrinsic(strcmp)

#include "stdint.h"
#include "common.h"
#include "asm/common_asm.h"

#include "runtime/import.h"
#include "runtime/import/securekernel.h"
#include "runtime/runtime.h"
#include "runtime/std.h"

#include "debug.h"

#include "../vmx.h"
#include "../backdoor_client.h"
#include "../backdoor_library.h"
#include "../common/loader.h"
#include "../driver_loader_sk.h"
