/*
MIT License

Copyright (c) 2019 Boaz Sedan (boaz@1533.io)

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include "b_itoa.h"

#define MK(a,b) ((unsigned short)a+'0' + ((unsigned short)(b+'0')<<8))
#define MK1(a) MK(a,0), MK(a,1), MK(a,2), MK(a,3), MK(a,4), MK(a,5), MK(a,6), MK(a,7), MK(a,8), MK(a,9)
static const unsigned short TBL[100] = {
MK1(0), MK1(1), MK1(2), MK1(3), MK1(4), MK1(5), MK1(6), MK1(7), MK1(8), MK1(9)
};
#undef MK1
#undef MK

unsigned b_utoa(uint64_t n, char *buf) {
    if (!n) {
        buf[0] = '0';
        return 1;
    }
    unsigned i = 0;
    uint64_t digs2 = 0;
    uint64_t digs1 = 0;
    uint64_t digs0 = 0;
    if (n>=99999999U) { // 8 or more chars
        if (n>=9999999999999999ULL) { // 16 or more chars
            for (unsigned j=0; j<4; ++j) {
                uint64_t n10 = n/100;
                digs2 = (digs2 << 16) + TBL[(n - (n10*100))];
                n = n10;
            }
            for (unsigned j=0; j<4; ++j) {
                uint64_t n10 = n/100;
                digs1 = (digs1 << 16) + TBL[(n - (n10*100))];
                n = n10;
            }
        } else {
            for (unsigned j=0; j<4; ++j) {
                uint64_t n10 = n/100;
                digs1 = (digs1 << 16) + TBL[(n - (n10*100))];
                n = n10;
            }
        }
    }

    if (n) { // remaining characters
        do {
            uint64_t n10 = n/100;
            digs0 = (digs0 << 16) + TBL[(n - (n10*100))];
            n = n10;
            i+=2;
        } while (n);
        if ((digs0&255)=='0') {
            digs0>>=8;
            i--;
        }
    }
    *(uint64_t*)buf = digs0;
    if (digs1) {
        buf += i;
        *(uint64_t*)buf = digs1;
        if (digs2) {
            buf += 8;
            *(uint64_t*)buf = digs2;
            return i+16;
        }
        return i+8;
    }
    return i;
}

unsigned b_itoa(int64_t n, char *buf) {
    if (n>=0) {
        return b_utoa(n, buf);
    } else {
        *buf++ = '-';
        return 1 + b_utoa(-n, buf);
    }
}
