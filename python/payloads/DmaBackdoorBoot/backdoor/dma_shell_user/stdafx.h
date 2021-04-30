#pragma warning(disable: 4200)
#pragma warning(disable: 4996)

#define _WIN32_WINNT  0x0501

#include <windows.h>
#include <Accctrl.h>
#include <Aclapi.h>
#include <sddl.h>
#include <shlwapi.h>
#include <shlobj.h>
#include <TlHelp32.h>

#include "common\ntdll_defs.h"
#include "common\ntdll_undocnt.h"

#include "runtime_user\import.h"
#include "runtime_user\import_common.h"
#include "runtime_user\import\kernel32.h"
#include "runtime_user\import\ntdll.h"
#include "runtime_user\std.h"
#include "runtime_user\runtime.h"

#include "common\common.h"
#include "common\debug.h"
#include "common\misc.h"
#include "common\peimage.h"

#include "dma_shell_kernel\dma_shell_kernel.h"

#include "backdoor.h"
#include "dma_shell.h"
#include "transport.h"
#include "driver_defs.h"
#include "driver_ctl.h"
