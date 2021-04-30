@echo off
if exist ..\dma_shell_kernel.sys python ..\make_header.py -i ..\dma_shell_kernel.sys -o ..\dma_shell_kernel_sys.h -n driver
if exist ..\dma_shell_kernel_debug.sys python ..\make_header.py -i ..\dma_shell_kernel_debug.sys -o ..\dma_shell_kernel_debug_sys.h -n driver
pause
