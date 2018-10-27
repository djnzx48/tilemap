/* Zilog Z80 CPU Emulator C API
  ____    ____    ___ ___     ___
 / __ \  / ___\  / __` __`\  / __`\
/\ \/  \/\ \__/_/\ \/\ \/\ \/\  __/
\ \__/\_\ \_____\ \_\ \_\ \_\ \____\
 \/_/\/_/\/_____/\/_/\/_/\/_/\/____/
Copyright (C) 1999-2018 Manuel Sainz de Baranda y Goñi.
Released under the terms of the GNU General Public License v3. */

#ifndef _emulation_CPU_Z80_H_
#define _emulation_CPU_Z80_H_

#include <Z/API/Z/hardware/CPU/architecture/z80.h>
#include <Z/ABIs/generic/emulation.h>

// SNO
typedef void(*ZContextHook)(void *context, zuint16 address);

typedef struct {
	zusize	  cycles;
	ZZ80State state;
	Z16Bit	  xy;
	zuint8	  r7;
	Z32Bit	  data;
	void*	  callback_context;

	ZContext16BitAddressRead8Bit  read;
	ZContext16BitAddressWrite8Bit write;
	ZContext16BitAddressRead8Bit  in;
	ZContext16BitAddressWrite8Bit out;
	ZContextRead32Bit	      int_data;
	ZContextSwitch		      halt;
	ZContextHook		      hook;	// SNO
} Z80;

Z_C_SYMBOLS_BEGIN

#ifndef CPU_Z80_ABI
#	ifdef CPU_Z80_STATIC
#		define CPU_Z80_ABI
#	else
#		define CPU_Z80_ABI Z_API
#	endif
#endif

CPU_Z80_ABI extern ZCPUEmulatorABI const abi_emulation_cpu_z80;

#ifndef CPU_Z80_API
#	ifdef CPU_Z80_STATIC
#		define CPU_Z80_API
#	else
#		define CPU_Z80_API Z_API
#	endif
#endif

CPU_Z80_API void   z80_power(Z80 *object, zboolean state);
CPU_Z80_API void   z80_reset(Z80 *object);
CPU_Z80_API zusize z80_run  (Z80 *object, zusize cycles);
CPU_Z80_API void   z80_nmi  (Z80 *object);
CPU_Z80_API void   z80_int  (Z80 *object, zboolean state);

Z_C_SYMBOLS_END

#endif /* _emulation_CPU_Z80_H_ */
