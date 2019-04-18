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

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include "b_itoa.h"

static double the_time() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return ts.tv_sec + ts.tv_nsec*1e-9;
}

int main() {
  char buffer[64];

  {
    printf("[sprintf]\n");
    unsigned n = 0;
    double t0 = the_time();
    for (uint64_t i=0; i<100000000; ++i) {
        sprintf(buffer, "%ld", i*34);
        n += buffer[0] + buffer[12];
    }
    double t1 = the_time();
    printf(" Time: %f secs, checksum: %u\n", (t1-t0), n);
  }

  {
    printf("[b_itoa]\n");
    unsigned n = 0;
    double t0 = the_time();
    for (uint64_t i=0; i<100000000; ++i) {
        b_itoa(i*34, buffer);
        n += buffer[0] + buffer[12];
    }
    double t1 = the_time();
    printf(" Time: %f secs, checksum: %u\n", (t1-t0), n);
  }
}