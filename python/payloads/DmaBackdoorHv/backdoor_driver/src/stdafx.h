extern "C"
{
#include <ntifs.h>
#include <stdio.h>
#include <stdarg.h>
#include <ntimage.h>
#include "undocnt.h"
}

#include "common.h"
#include "asm/common_asm.h"

#include "runtime/import.h"
#include "runtime/import/ntoskrnl.h"
#include "runtime/runtime.h"

#include "debug.h"

