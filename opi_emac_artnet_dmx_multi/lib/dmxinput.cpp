/**
 * @file dmxinput.cpp
 *
 */
/* Copyright (C) 2020 by Arjan van Vught mailto:info@orangepi-dmx.nl
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>
#include <h3/dmxmultiinput.h>

#include "dmxinput.h"

#include "dmx.h"

#include "debug.h"

DmxInput::DmxInput() {
	DEBUG_ENTRY

	for (uint32_t i = 0; i < sizeof(m_bIsStarted) / sizeof(m_bIsStarted[0]); i++) {
		m_bIsStarted[i] = false;
	}

	DEBUG_EXIT
}

void DmxInput::Start(uint8_t nPort) {
	DEBUG_ENTRY
	DEBUG_PRINTF("nPort=%d", static_cast<int>(nPort));

	if (m_bIsStarted[nPort]) {
		DEBUG_EXIT
		return;
	}

	m_DmxMultiInput.StartData(nPort);

	m_bIsStarted[nPort] = true;

	DEBUG_EXIT
}

void DmxInput::Stop(uint8_t nPort) {
	DEBUG_ENTRY
	DEBUG_PRINTF("nPort=%d", static_cast<int>(nPort));

	if (!m_bIsStarted[nPort]) {
		DEBUG_EXIT
		return;
	}

	m_DmxMultiInput.StopData(nPort);

	m_bIsStarted[nPort] = false;

	DEBUG_EXIT
}

const uint8_t *DmxInput::Handler(uint8_t nPort, uint16_t &nLength, uint32_t &nUpdatesPerSecond) {
	const uint8_t *pDmx = m_DmxMultiInput.GetDmxAvailable(nPort);

	nUpdatesPerSecond = m_DmxMultiInput.GetUpdatesPerSeconde(nPort);

	if (pDmx != nullptr) {
		const auto *dmx_data = reinterpret_cast<const struct _dmx_data*>(pDmx);
		nLength = dmx_data->statistics.slots_in_packet;
		return (pDmx + 1);
	}

	nLength = 0;
	return nullptr;
}
