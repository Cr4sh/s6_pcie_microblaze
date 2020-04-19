/* -----------------------------------------------------------------------------
 * extern.h
 *
 * Copyright (c) 2004, 2005, 2006, Vivek Mohan <vivek@sig9.com>
 * All rights reserved. See LICENSE
 * -----------------------------------------------------------------------------
 */
#ifndef UD_EXTERN_H
#define UD_EXTERN_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include "types.h"

/* ============================= PUBLIC API ================================= */

extern void __stdcall ud_init(struct ud*);

extern void __stdcall ud_set_mode(struct ud*, uint8_t);

extern void __stdcall ud_set_pc(struct ud*, uint64_t);

extern void __stdcall ud_set_input_hook(struct ud*, int (*)(struct ud*));

extern void __stdcall ud_set_input_buffer(struct ud*, uint8_t*, size_t);

#ifndef __UD_STANDALONE__

extern void __stdcall ud_set_input_file(struct ud*, FILE*);

#endif /* __UD_STANDALONE__ */

extern void __stdcall ud_set_vendor(struct ud*, unsigned);

extern void __stdcall ud_set_syntax(struct ud*, void (*)(struct ud*));

extern void __stdcall ud_input_skip(struct ud*, size_t);

extern int __stdcall ud_input_end(struct ud*);

extern unsigned int __stdcall ud_decode(struct ud*);

extern unsigned int __stdcall ud_disassemble(struct ud*);

extern void __stdcall ud_translate_intel(struct ud*);

extern void __stdcall ud_translate_att(struct ud*);

extern char* __stdcall ud_insn_asm(struct ud* u);

extern uint8_t* __stdcall ud_insn_ptr(struct ud* u);

extern uint64_t __stdcall ud_insn_off(struct ud*);

extern char* __stdcall ud_insn_hex(struct ud*);

extern unsigned int __stdcall ud_insn_len(struct ud* u);

/* ========================================================================== */

#ifdef __cplusplus
}
#endif
#endif
