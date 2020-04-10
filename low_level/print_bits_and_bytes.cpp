//
// https://jameshfisher.com/2017/02/23/printing-bits/
//
#include <stdio.h>
#include <stdint.h>
#include <limits.h>
#include <errno.h>
#include <uchar.h>

#include "../common.h"

extern "C" {
    static auto HIRAGANA = U"ひらがな";
    static auto KANJI = U"漢字";

    static void print_byte_as_bits(char val) {
        for (int i = 7; 0 <= i; i--) {
            printf("%c", (val & (1 << i)) ? '1' : '.');
        }
    }

    static void print_bits(char const* ty, char const* val, unsigned char* bytes, size_t num_bytes) {
        printf("(%*s) %*s = [ ", 15, ty, 16, val);
        for (size_t i = 0; i < num_bytes; i++) {
            print_byte_as_bits(bytes[i]);
            printf(" ");
        }
        printf("]\n");
    }

    static void print_bytes(char const * ty, char const * val, unsigned char* bytes, size_t num_bytes) {
        printf("(%*s) %*s = [ ", 15, ty, 16, val);
        for (size_t i = 0; i < num_bytes; i++) {
            printf("%*u ", 3, bytes[i]);
        }
        printf("]\n");
    }

#define SHOW_BYTES(T,V) do { T x = V; print_bytes(#T, #V, (unsigned char*) &x, sizeof(x)); } while(0)

} // "C" 
#define SHOW_BITS(T,V) do { T x = V; print_bits(#T, #V, (unsigned char*) &x, sizeof(x) ); } while(0)

TU_REGISTER_NOT( []{

/*    SHOW_BITS(unsigned char, 'A');
    SHOW_BITS(signed char, 'A');
    SHOW_BITS(char, 'A');

 */ 
    // no can do
    // SHOW_BITS(char8_t, u8'ひ');

  /*
  SHOW_BITS(char16_t, u'ひ');
  SHOW_BITS(char32_t, U'ひ');
  SHOW_BITS(wchar_t, L'ひ');
   */

    printf("\n");
    SHOW_BITS(uint8_t, 0xFF);
    SHOW_BYTES(uint8_t, 0xFF);
    printf("\n");
    SHOW_BITS(uint16_t, 0xFF);
    SHOW_BYTES(uint16_t, 0xFF);
    printf("\n");
    SHOW_BITS(uint32_t, 0xFF);
    SHOW_BYTES(uint32_t, 0xFF);
    printf("\n");
    SHOW_BITS(uint64_t, 0xFF);
    SHOW_BYTES(uint64_t, 0xFF);
    /*
      SHOW_BITS(void*, &errno);
      SHOW_BITS(unsigned int, 1 << 1);
      SHOW_BITS(unsigned int, 1 << 2);
      SHOW_BITS(unsigned int, 1 << 4);
      SHOW_BITS(unsigned int, 1 << 8);
      SHOW_BITS(unsigned int, 1 << 16);
      */
});