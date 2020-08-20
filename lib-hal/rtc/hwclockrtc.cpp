/**
 * @file  hwclockrtc.cpp
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

/*
 * PoC Code -> Do not use in production
 */

#include <cassert>

#include "hwclock.h"

#include "hal_i2c.h"

#include "debug.h"

#define BCD2DEC(val)	( ((val) & 0x0f) + ((val) >> 4) * 10 )
#define DEC2BCD(val)	( (((val) / 10) << 4) + (val) % 10 )

namespace rtc {
namespace reg {
static constexpr uint8_t SECONDS = 0x00;
static constexpr uint8_t MINUTES = 0x01;
static constexpr uint8_t HOURS = 0x02;
static constexpr uint8_t DAY = 0x03;
static constexpr uint8_t DATE = 0x04;
static constexpr uint8_t MONTH = 0x05;
static constexpr uint8_t YEAR = 0x06;
}  // namespace reg
namespace mcp7941x {
namespace bit {
static constexpr uint8_t ST = 0x80;
static constexpr uint8_t VBATEN = 0x08;
}  // namespace bit
}  // namespace mcp7941x
namespace i2caddress {
static constexpr uint8_t MCP7941X = 0x6F;
static constexpr uint8_t DS3231 = 0x68;
}  // namespace i2caddress
}  // namespace rtc

using namespace rtc;

void HwClock::RtcProbe() {
	if (HAL_I2C::IsConnected(i2caddress::MCP7941X, hal::i2c::NORMAL_SPEED)) {
		m_bIsConnected = true;
		m_nType = MCP7941X;
		m_nAddress = i2caddress::MCP7941X;

		DEBUG_EXIT
		return;
	}

	if (HAL_I2C::IsConnected(i2caddress::DS3231, hal::i2c::NORMAL_SPEED)) {
		m_bIsConnected = true;
		m_nType = DS3231;
		m_nAddress = i2caddress::DS3231;

		DEBUG_EXIT
		return;
	}
}

bool HwClock::RtcSet(const struct rtc_time *pRtcTime) {
	assert(pRtcTime != 0);

	if(!m_bIsConnected) {
		return false;
	}

	char registers[7];

	registers[reg::SECONDS] = DEC2BCD(pRtcTime->tm_sec          & 0x7f);
	registers[reg::MINUTES] = DEC2BCD(pRtcTime->tm_min          & 0x7f);
	registers[reg::HOURS]   = DEC2BCD(pRtcTime->tm_hour         & 0x1f);
	registers[reg::DAY]     = DEC2BCD(pRtcTime->tm_wday         & 0x07);
	registers[reg::DATE]    = DEC2BCD(pRtcTime->tm_mday         & 0x3f);
	registers[reg::MONTH]   = DEC2BCD((pRtcTime->tm_mon + 1)    & 0x1f);
	registers[reg::YEAR]    = DEC2BCD((pRtcTime->tm_year - 100) & 0xff);

	if (m_nType == MCP7941X) {
		registers[reg::SECONDS] |= mcp7941x::bit::ST;
		registers[reg::DAY] |=  mcp7941x::bit::VBATEN;
	}

	char data[8];

	data[0] = reg::SECONDS;
	data[1] = registers[0];
	data[2] = registers[1];
	data[3] = registers[2];
	data[4] = registers[3];
	data[5] = registers[4];
	data[6] = registers[5];
	data[7] = registers[6];

	FUNC_PREFIX(i2c_set_address(m_nAddress));
	FUNC_PREFIX(i2c_set_baudrate(hal::i2c::FULL_SPEED));

	FUNC_PREFIX(i2c_write(data, sizeof(data) / sizeof(data[0])));

	return true;
}

bool HwClock::RtcGet(struct rtc_time *pRtcTime) {
	assert(pRtcTime != 0);

	if(!m_bIsConnected) {
		return false;
	}

	char registers[7];

	registers[0] = reg::SECONDS;

	FUNC_PREFIX(i2c_set_address(m_nAddress));
	FUNC_PREFIX(i2c_set_baudrate(hal::i2c::FULL_SPEED));

	FUNC_PREFIX(i2c_write(registers, 1));
	FUNC_PREFIX(i2c_read(registers, sizeof(registers) / sizeof(registers[0])));

	pRtcTime->tm_sec  = BCD2DEC(registers[reg::SECONDS] & 0x7f);
	pRtcTime->tm_min  = BCD2DEC(registers[reg::MINUTES] & 0x7f);
	pRtcTime->tm_hour = BCD2DEC(registers[reg::HOURS]   & 0x3f);
	pRtcTime->tm_wday = BCD2DEC(registers[reg::DAY]     & 0x07);
	pRtcTime->tm_mday = BCD2DEC(registers[reg::DATE]    & 0x3f);
	pRtcTime->tm_mon  = BCD2DEC(registers[reg::MONTH]   & 0x1f) - 1;
	pRtcTime->tm_year = BCD2DEC(registers[reg::YEAR]) + 100;

	return true;
}