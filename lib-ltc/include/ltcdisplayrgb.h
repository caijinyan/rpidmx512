/**
 * @file ltcdisplayws28xx.h
 */
/*
 * Copyright (C) 2019-2020 by hippy mailto:dmxout@gmail.com
 * Copyright (C) 2019-2020 by Arjan van Vught mailto:info@orangepi-dmx.nl
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

#ifndef LTCDISPLAYRGB_H_
#define LTCDISPLAYRGB_H_

#include <stdint.h>

#include "ltcdisplayrgbset.h"

#include "ws28xx.h"
#include "rgbmapping.h"

enum class LtcDisplayRgbType {
	WS28XX, RGBPANEL
};

enum class LtcDisplayRgbWS28xxType {
	SEGMENT, MATRIX
};

enum class LtcDisplayRgbColonBlinkMode {
	OFF, DOWN, UP
};

enum class LtcDisplayRgbColourIndex {
	DIGIT, COLON, MESSAGE, LAST
};

struct LtcDisplayWS28xxDefaults {
	static constexpr auto LED_TYPE = WS2812B;
	static constexpr auto COLOUR_DIGIT = 0x00FF0000;
	static constexpr auto COLOUR_COLON = 0x00FFFC00;
	static constexpr auto COLOUR_MESSAGE = 0x00FFFFFF;
	static constexpr auto COLON_BLINK_MODE = LtcDisplayRgbColonBlinkMode::UP;
	static constexpr auto MASTER = 0xFF;
	static constexpr auto GLOBAL_BRIGHTNESS = 0xFF;
};

class LtcDisplayRgb {
public:
	LtcDisplayRgb(LtcDisplayRgbType tRgbType, LtcDisplayRgbWS28xxType tWS28xxType);
	~LtcDisplayRgb();

	void SetMapping(TRGBMapping tMapping) {
		m_tMapping = tMapping;
	}

	void SetMaster(uint8_t nValue) {
		m_nMaster = nValue;
	}

	void SetColonBlinkMode(LtcDisplayRgbColonBlinkMode tColonBlinkMode) {
		m_tColonBlinkMode = tColonBlinkMode;
	}

	void SetColour(uint32_t nRGB, LtcDisplayRgbColourIndex tIndex) {
		if (tIndex >= LtcDisplayRgbColourIndex::LAST) {
			return;
		}
		m_aColour[static_cast<uint32_t>(tIndex)] = nRGB;
	}

	void Init(TWS28XXType tLedType = TWS28XXType::WS2812B);
	void Print();

	void Run();

	void Show(const char *pTimecode);
	void ShowSysTime(const char *pSystemTime);
	void ShowFPS(ltc::type tTimeCodeType);
	void ShowSource(ltc::source tSource);
	void ShowInfo(const char *pInfo);

	void WriteChar(uint8_t nChar, uint8_t nPos = 0);

	static LtcDisplayRgb *Get() {
		return s_pThis;
	}

	void SetMessage(const char *pMessage, uint32_t nSize);
	void SetRGB(uint8_t nRed, uint8_t nGreen, uint8_t nBlue, LtcDisplayRgbColourIndex tIndex);

private:
	void SetRGB(uint32_t nRGB, LtcDisplayRgbColourIndex tIndex);
	void SetRGB(const char *pHexString);
	uint32_t hexadecimalToDecimal(const char *pHexValue, uint32_t nLength = 6);
	void ShowMessage();

private:
	LtcDisplayRgbType m_tDisplayRgbType;
	LtcDisplayRgbWS28xxType m_tDisplayRgbWS28xxType;
	uint8_t m_nIntensity{LtcDisplayWS28xxDefaults::GLOBAL_BRIGHTNESS};
	int32_t m_nHandle{-1};
	char m_Buffer[64];
  	TRGBMapping m_tMapping{RGB_MAPPING_UNDEFINED};
  	TWS28XXType m_tLedType{WS28XX_UNDEFINED};
	uint32_t m_aColour[static_cast<uint32_t>(LtcDisplayRgbColourIndex::LAST)];
	uint32_t m_nMaster{LtcDisplayWS28xxDefaults::MASTER};
	bool m_bShowMsg{false};
	char m_aMessage[LTCDISPLAY_MAX_MESSAGE_SIZE];
	uint32_t m_nMsgTimer{0};
	uint32_t m_nColonBlinkMillis{0};
	char m_nSecondsPrevious{60};
	LtcDisplayRgbColonBlinkMode m_tColonBlinkMode{LtcDisplayWS28xxDefaults::COLON_BLINK_MODE};

	LtcDisplayRgbSet *m_pLtcDisplayRgbSet{nullptr};

	struct TLtcDisplayRgbColours m_tColours;
	struct TLtcDisplayRgbColours m_tColoursMessage;
	struct TLtcDisplayRgbColours m_tColoursColons;

	static LtcDisplayRgb *s_pThis;
};

#endif /* LTCDISPLAYRGB_H_ */
