/**
 * @file ws28xxdmxmulti.h
 *
 */
/* Copyright (C) 2019-2021 by Arjan van Vught mailto:info@orangepi-dmx.nl
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

#ifndef WS28XXDMXMULTI_H_
#define WS28XXDMXMULTI_H_

#include <stdint.h>

#include "lightset.h"

#include "ws28xxmulti.h"

#include "rgbmapping.h"

namespace ws28xxdmxmulti {
enum class Source {
	ARTNET,
	E131
};
}  // namespace ws28xxdmxmulti

class WS28xxDmxMulti final: public LightSet {
public:
	WS28xxDmxMulti(ws28xxdmxmulti::Source tSrc);
	~WS28xxDmxMulti() override;

	void Initialize();

	void Start(uint8_t nPort) override;
	void Stop(uint8_t nPort) override;

	void SetData(uint8_t nPort, const uint8_t *pData, uint16_t nLength) override;

	void Blackout(bool bBlackout);

	void SetLEDType(ws28xx::Type tWS28xxMultiType);
	ws28xx::Type GetLEDType() {
		if (m_pLEDStripe != nullptr) {
			return m_pLEDStripe->GetLEDType();
		}
		return m_tLedType;
	}

	void SetRgbMapping(TRGBMapping tRGBMapping) {
		m_tRGBMapping = tRGBMapping;
	}

	void SetLowCode(uint8_t nLowCode) {
		m_nLowCode = nLowCode;
	}

	void SetHighCode(uint8_t nHighCode) {
		m_nHighCode = nHighCode;
	}

	void SetLEDCount(uint16_t nLedCount);
	uint32_t GetLEDCount() {
		return m_nLedCount;
	}

	void SetActivePorts(uint8_t nActiveOutputs);
	uint32_t GetActivePorts() {
		return m_nActiveOutputs;
	}

	uint32_t GetUniverses() {
		return m_nUniverses;
	}

	void SetUseSI5351A(bool bUse) {
		m_bUseSI5351A = bUse;
	}
	bool GetUseSI5351A() {
		return m_bUseSI5351A;
	}

	ws28xxmulti::Board GetBoard() {
		if (m_pLEDStripe != nullptr) {
			return m_pLEDStripe->GetBoard();
		}
		return ws28xxmulti::Board::UNKNOWN;
	}

	void Print() override;

private:
	void UpdateMembers();

private:
	ws28xxdmxmulti::Source m_tSrc;
	ws28xx::Type m_tLedType;

	TRGBMapping m_tRGBMapping;
	uint8_t m_nLowCode;
	uint8_t m_nHighCode;

	uint32_t m_nLedCount;
	uint32_t m_nActiveOutputs;

	WS28xxMulti *m_pLEDStripe;

	bool m_bIsStarted { false };
	bool m_bBlackout { false };

	uint32_t m_nUniverses;

	uint32_t m_nBeginIndexPortId1;
	uint32_t m_nBeginIndexPortId2;
	uint32_t m_nBeginIndexPortId3;
	uint32_t m_nChannelsPerLed;

	uint32_t m_nPortIdLast;
	bool m_bUseSI5351A { false };
};

#endif /* WS28XXDMXMULTI_H_ */
