#include "leb128.h"
#include <stdbool.h>

// best way according to https://stackoverflow.com/a/40844126
int binaryLength(int n) {
  int i = 0;
  if (n >= 0x7FFF) {
    n >>= 16;
    i += 16;
  }
  if (n >= 0x7F) {
    n >>= 8;
    i += 8;
  }
  if (n >= 0x7) {
    n >>= 4;
    i += 4;
  }
  if (n >= 0x3) {
    n >>= 2;
    i += 2;
  }
  if (n >= 0x1) {
    n >>= 1;
    i += 1;
  }
  return i;
}

int sizeULEB128(int value) {
  if (value < 0) {
    value = -value;
  }
  int size = binaryLength(value) / 7;
  if (value % 7 != 0) {
    size++;
  }
  return size;
}

void writeULEB128(FILE *file, unsigned int value) {
  if (value == 0) {
    fputc(0, file);
  }
  while (value != 0) {
    char sevenFirstBits = value & 0x7f;
    // Shif value by 7 to the right
    value >>= 7;
    if (value != 0) {
      sevenFirstBits |= 0x80; // add a one if now the most significant
    }
    fputc(sevenFirstBits, file);
  }
}

// from https://en.wikipedia.org/wiki/LEB128#Encode_signed_integer
void writeSLEB128(FILE *file, int value) {
  bool more = true;
  bool negative = value < 0;

  unsigned int size = __CHAR_BIT__ * sizeof(int);

  while (more) {
    char byte = value & 0x7f; /* low-order 7 bits of value */
    value >>= 7;
    /* the following is only necessary if the implementation of >>= uses a
       logical shift rather than an arithmetic shift for a signed left operand
       this does not happen on most programming languages if "value" is in a
       signed type to begin with */
    if (negative) {
      value |= (~0 << (size - 7)); /* sign extend */
    }

    /* sign bit of byte is second high-order bit (0x40) */
    char sign_bit = byte & 0x40;
    if ((value == 0 && sign_bit == 0) || (value == -1 && sign_bit != 0)) {
      more = false;
    } else {
      byte |= 0x80; /* set high-order bit of byte */
    }
    fputc(byte, file);
  }
}
