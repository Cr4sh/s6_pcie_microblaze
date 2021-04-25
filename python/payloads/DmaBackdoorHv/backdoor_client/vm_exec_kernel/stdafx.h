#pragma warning(disable: 4200)

extern "C"
{
#include <ntifs.h>
#include <stdio.h>
#include <stdint.h>
#include <stdarg.h>
#include <ntimage.h>
#include "undocnt.h"
}

#include "debug.h"
#include "common.h"
#include "asm/common_asm.h"
#include "dll_inject_shellcode.h"

#include "../vm_exec.h"
#include "../common/peimage.h"

#include "../vmx.h"
#include "../backdoor_client.h"
#include "../backdoor_library.h"
