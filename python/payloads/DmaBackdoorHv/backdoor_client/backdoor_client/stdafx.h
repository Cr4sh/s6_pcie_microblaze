#pragma once
#pragma warning(disable: 4996)

#include "targetver.h"

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <tchar.h>
#include <conio.h>
#include <errno.h>
#include <sys/types.h>  
#include <sys/stat.h> 

#include <windows.h>
#include <TlHelp32.h>

#include "../common/common.h"
#include "../common/loader.h"
#include "../common/image_policy.h"

#include "../vmx.h"
#include "../backdoor_client.h"
#include "../backdoor_library.h"
#include "../backdoor_library/virtual_mem.h"

#include "../udis86/extern.h"

#include "service.h"
#include "winio.h"
#include "backdoor_debug.h"
