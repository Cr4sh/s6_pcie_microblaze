OUTNAME = backdoor_driver

ALL: $(OUTNAME).sys

CC = cl.exe

CFLAGS = /nologo -I".\src" -I"$(DDK_INC_PATH)" -I"$(CRT_INC_PATH)" -DDBG -D_AMD64_=1 /Gz /Gy /GS- /O1 /Os /c

AM = ml64.exe

AMFLAGS = /nologo /c

common_asm.obj: src/asm/amd64/common_asm.asm
    $(AM) $(AMFLAGS) src/asm/amd64/common_asm.asm

include makefile.inc

LN = link.exe

LFLAGS = /LIBPATH:$(DDK_LIB_PATH)\..\amd64 /LIBPATH:$(CRT_LIB_PATH)\..\amd64 /NODEFAULTLIB /DEF:backdoor_driver.def /OPT:REF /OPT:ICF /INCREMENTAL:NO /SAFESEH:NO /STACK:0x40000,0x1000 /DRIVER /BASE:0x10000 /ALIGN:0x80 /SUBSYSTEM:native /ENTRY:DriverEntry /OUT:..\$(OUTNAME).sys

$(OUTNAME).sys: $(LOBJS) common_asm.obj
    $(LN) $(LFLAGS) $(LOBJS) common_asm.obj $(LLIBS)

clean:
    @del *.obj 
