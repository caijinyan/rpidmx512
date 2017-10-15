/**
 * @file sscan.h
 *
 */
/* Copyright (C) 2016, 2017 by Arjan van Vught mailto:info@raspberrypi-dmx.nl
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

#ifndef SSCAN_H_
#define SSCAN_H_

#include <stdint.h>

#define SSCAN_NAME_ERROR	0
#define SSCAN_VALUE_ERROR	1
#define SSCAN_OK			2

#ifdef __cplusplus
extern "C" {
#endif

extern int sscan_uint8_t(const char *, const char *, /*@out@*/uint8_t *);
extern int sscan_uint16_t(const char *, const char *, /*@out@*/uint16_t *);
extern int sscan_uint32_t(const char *, const char *, /*@out@*/uint32_t *);
extern int sscan_float(const char *, const char *, /*@out@*/float *);
extern int sscan_char_p(const char *, const char *, /*@out@*/char *, /*@out@*/uint8_t *);
extern int sscan_ip_address(const char *, const char *, /*@out@*/uint32_t *);
extern int sscan_uuid(const char *, const char *, /*@out@*/char *, /*@out@*/uint8_t *);
extern int sscan_i2c(const char *, /*@out@*/char *, /*@out@*/uint8_t *, /*@out@*/uint8_t *, /*@out@*/uint8_t *);

#ifdef __cplusplus
}
#endif

#endif /* SSCAN_H_ */
