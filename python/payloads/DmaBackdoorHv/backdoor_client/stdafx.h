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

#include "common.h"
#include "common_asm.h"
#include "loader.h"
#include "virtual_mem.h"
#include "service.h"
#include "winio.h"

#include "../backdoor_client.h"

#include "../udis86/extern.h"
