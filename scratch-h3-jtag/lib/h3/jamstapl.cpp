/*
 * jamstapl.cpp
 *
 */

#include <jamstapl.h>
#include <stdint.h>
#include <stdio.h>
#include <cassert>


extern "C" {
void jbi_jtag_io_init(void);
}

void JamSTAPL::PlatformInit() {
	jbi_jtag_io_init();
}
