@echo off
python ..\make_header.py -i ..\winio.sys -o ..\winio_sys.h -n winio_sys
python ..\make_header.py -i ..\driver_loader.sys -o ..\driver_loader_sys.h -n driver_loader_sys
python ..\make_header.py -i ..\driver_loader_sk.sys -o ..\driver_loader_sk_sys.h -n driver_loader_sk_sys
python ..\make_header.py -i ..\vm_exec_kernel.sys -o ..\vm_exec_kernel_sys.h -n vm_exec_kernel_sys
pause
