# b_itoa
The fastest (that I know of) integer printer

Printing numbers seems to be a common topic in some forums.
Although this function is limited to integers, it shows that 
some mindset change can easily trasnlate into a 4x-20x performance
improvement compare to all other methods (that I know of).


Most integer printing functions deal with two problems-
- integer division (or modulu) to obtain the individual digits out of a number
- reversing the order of the printed digits

I'm not aware of any CPU specific way of handling the digit extraction
except by division (or multiplication), 
but it is fairly simple to use lookup tables to extract 2 (or maybe even 4) digits at a time.
This code extracts two digits at a time.

As for order reversal- it's only a problem if you're writing to memory.
Since we know 64 bit numbers always fit within 20 decimal digits, 
only 3 * 64bit registers are needed to store the intermediate representation.
Inserting bytes into a register is fairly trivial, and can be done
left-to-right or right-to-left (for little or big-endian machines).

Working on 8 bytes at a time, lets the compiler easily unroll loops.

The attached code performs at least 4x faster than any other code I have previously seen,
and roughly 10x-20x faster than sprintf, using gcc 8.2 on my laptop.

```
boaz@boaz-VirtualBox:~/faster_itoa$ cc *.c -O3 -march=native
boaz@boaz-VirtualBox:~/faster_itoa$ ./a.out 
[sprintf] Time: 16.952611 secs, checksum: 775620936
[b_itoa] Time: 1.332887 secs, checksum: 775620936
```

And if to compare to some other benchmarks:

https://gist.github.com/anonymous/7259276

http://www.zverovich.net/2013/09/07/integer-to-string-conversion-in-cplusplus.html


```
Integer To String Test
[sprintf] Numbers: 240000000	Total:  1823402786	Time: 42.5248sec	Rate:  5643769.5307nums/sec
[timo   ] Numbers: 240000000	Total:  1823402786	Time: 13.0718sec	Rate: 18360100.5508nums/sec
[hopman ] Numbers: 240000000	Total:  1823402786	Time: 12.6880sec	Rate: 18915580.7945nums/sec
[voigt  ] Numbers: 240000000	Total:  1823402786	Time: 12.3306sec	Rate: 19463798.2306nums/sec
[karma  ] Numbers: 240000000	Total:  1823402786	Time:  9.8035sec	Rate: 24481135.5996nums/sec
[so     ] Numbers: 240000000	Total:  1823402786	Time:  8.0742sec	Rate: 29724278.4008nums/sec
[boaz   ] Numbers: 240000000	Total:  1823402786	Time:  2.4787sec	Rate: 96826665.2636nums/sec
```