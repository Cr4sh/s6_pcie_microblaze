#pragma warning(disable: 4200)

extern "C"
{

#include <stdio.h>
#include <stdarg.h>
#include <ntddk.h>
#include <ntimage.h>

#include "undocnt.h"

}

#include "runtime_kernel\import\ntoskrnl.h"
#include "runtime_kernel\import\hal.h"
#include "runtime_kernel\import.h"
#include "runtime_kernel\runtime.h"

#include "runtime_user\std.h"
#include "runtime_user\import_common.h"

#include "common\common.h"
#include "common\peimage.h"
#include "common\compress.h"
#include "common\rc4.h"

#include "driver_defs.h"
#include "backdoor.h"

#include "debug.h"
#include "misc.h"
#include "inject.h"

