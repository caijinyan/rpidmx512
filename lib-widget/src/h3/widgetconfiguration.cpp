/**
 * @file widgetconfiguration.cpp
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

#include <cassert>

#include "widgetconfiguration.h"
#include "storewidget.h"

#include "dmx.h"

void WidgetConfiguration::Store(const struct _widget_params *widget_params) {
	assert(StoreWidget::Get() != nullptr);

	if (widget_params->break_time != s_nBreakTime) {
		s_nBreakTime = widget_params->break_time;
		Dmx::Get()->SetDmxBreakTime((static_cast<float>((s_nBreakTime)) * 10.67));
		StoreWidget::Get()->UpdateBreakTime(widget_params->break_time);
	}

	if (widget_params->mab_time != s_nMabTime) {
		s_nMabTime = widget_params->mab_time;
		Dmx::Get()->SetDmxMabTime((static_cast<float>((s_nMabTime)) * 10.67));
		StoreWidget::Get()->UpdateMabTime(widget_params->mab_time);
	}

	if (widget_params->refresh_rate != s_nRefreshRate) {
		s_nRefreshRate = widget_params->refresh_rate;
		Dmx::Get()->SetDmxPeriodTime(widget_params->refresh_rate == 0 ? 0 : (1000000U / widget_params->refresh_rate));
		StoreWidget::Get()->UpdateRefreshRate(widget_params->refresh_rate);
	}
}
