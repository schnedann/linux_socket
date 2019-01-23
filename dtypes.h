#ifndef DTYPES_H
#define DTYPES_H

#include <cstdint>
#include <cstddef>

//--------------------------------------------------

using  u8 =  uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using  s8 =  int8_t;
using s16 = int16_t;
using s32 = int32_t;
using s64 = int64_t;

//--------------------------------------------------

typedef union cutype16_u{
  u8  a[2];
  u16 b;
}cutype16_t;

typedef union cutype32_u{
  u8  a[4];
  u16 b[2];
  u32 c;
}cutype32_t;

typedef union cutype64_u{
  u8  a[8];
  u16 b[4];
  u32 c[2];
  u64 d;
}cutype64_t;

//--------------------------------------------------

#endif
