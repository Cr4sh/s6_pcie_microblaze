@echo off
if exist ..\driver.sys python ..\make_header.py -i ..\driver.sys -o ..\driver_sys.h -n driver
if exist ..\driver_debug.sys python ..\make_header.py -i ..\driver_debug.sys -o ..\driver_debug_sys.h -n driver
pause
