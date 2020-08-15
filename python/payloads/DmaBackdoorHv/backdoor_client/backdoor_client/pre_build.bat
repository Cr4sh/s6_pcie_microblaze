@echo off
python ..\make_header.py -i ..\winio.sys -o ..\winio_sys.h -n winio_sys
python ..\make_header.py -i ..\driver_loader.sys -o ..\driver_loader_sys.h -n driver_loader_sys
pause
