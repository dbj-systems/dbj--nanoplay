
## DBJ*Nanolib roadmap
### In no particular order (for now)

1. #### pool allocator written in C
   - Good examples so far
     - in C
       - seems [simple but workable](https://github.com/jobtalle/pool) 
       - can be used as a key building block
     - [C++, but clean](https://github.com/cacay/MemoryPool) 
        - because it is not "modern" and obfuscated
        - [this fork](https://github.com/billyquith/MemoryPool) has benchmarking added
1. #### tako out `dbj--nanolib/nonstd`  to `dbj_no_std`
2. #### Use -- WIN specific low level stuff
    3. [strsafe.h](https://docs.microsoft.com/en-gb/windows/win32/menurc/strsafe-ovw?redirectedfrom=MSDN)
    4. or is is [ntstrsafe.h](https://docs.microsoft.com/en-us/windows-hardware/drivers/ddi/ntstrsafe/) ?
    5. is [RtlAnsiStringToUnicodeString](https://docs.microsoft.com/en-gb/windows-hardware/drivers/ddi/wdm/nf-wdm-rtlansistringtounicodestring) actually to be used isntead of ...
    6. is it feasible to try and extract wincpprt [hstring](https://docs.microsoft.com/en-us/windows/uwp/cpp-and-winrt-apis/strings) as the ultimate win string?
    7. The fact is it primarily encapsulates [HSTRING](https://docs.microsoft.com/en-us/windows/win32/winrt/hstring) and UTF-16 WIN.

---------------------------------------------------------------------  

[![dbj();](https://dbj.org/wp-content/uploads/2015/12/cropped-dbj-icon-e1486129719897.jpg)](http://www.dbj.org "dbj")