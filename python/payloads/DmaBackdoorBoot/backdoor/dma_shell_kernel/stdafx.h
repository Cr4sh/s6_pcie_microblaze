#pragma warning(disable: 4200)

extern "C"
{

#include <stdio.h>
#include <stdarg.h>
#include <ntddk.h>
#include <ntimage.h>

}

#include "common\common.h"

#include "dma_shell_kernel.h"
#include "debug.h"

#include "backdoor.h"
