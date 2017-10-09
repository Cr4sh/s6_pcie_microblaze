
backdoor_driver.obj: src/backdoor_driver.cpp
    $(CC) $(CFLAGS) src/backdoor_driver.cpp

debug.obj: src/debug.cpp
    $(CC) $(CFLAGS) src/debug.cpp

runtime.obj: src/runtime/runtime.cpp
    $(CC) $(CFLAGS) src/runtime/runtime.cpp

import.obj: src/runtime/import.cpp
    $(CC) $(CFLAGS) src/runtime/import.cpp

LLIBS = ntoskrnl.lib hal.lib

LOBJS = backdoor_driver.obj debug.obj runtime.obj import.obj
