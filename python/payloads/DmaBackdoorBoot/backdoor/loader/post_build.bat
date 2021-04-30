@echo off
if exist ..\loader.dll python ..\make_header.py -i ..\loader.dll -o ..\loader_dll.h -n loader
if exist ..\loader_debug.dll python ..\make_header.py -i ..\loader_debug.dll -o ..\loader_debug_dll.h -n loader
pause
