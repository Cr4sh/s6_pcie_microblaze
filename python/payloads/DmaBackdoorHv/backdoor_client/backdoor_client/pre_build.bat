@echo off
python ..\make_header.py -i ..\wnBios64.sys -o ..\wnBios64_sys.h -n driver
python ..\make_header.py -i ..\driver_loader.sys -o ..\driver_loader_sys.h -n driver_loader_sys
python ..\make_header.py -i ..\driver_loader_sk.sys -o ..\driver_loader_sk_sys.h -n driver_loader_sk_sys
python ..\make_header.py -i ..\vm_exec_kernel.sys -o ..\vm_exec_kernel_sys.h -n vm_exec_kernel_sys
pause
