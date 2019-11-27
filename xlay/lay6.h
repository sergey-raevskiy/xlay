#pragma once

#include <windows.h>

#define ASSERT_SIZE(_struct, size) \
    C_ASSERT(sizeof(_struct) == (size))

#define ASSERT_FIELD_OFFSET(_struct, field, off) \
    C_ASSERT(FIELD_OFFSET(_struct, field) == (off))

#pragma pack(push, 1)
template <UCHAR max_len>
struct LAY_String
{
    UCHAR len;
    UCHAR data[max_len];
};
#pragma pack(pop)

ASSERT_SIZE(LAY_String<10>, 11);

#pragma pack(push, 1)
struct LAY_FileHeader
{
    UCHAR magic[4];
    DWORD num_boards;
};
#pragma pack(pop)

ASSERT_SIZE(LAY_FileHeader, 8);

#pragma pack(push, 1)
struct LAY_BoardHeader
{
    LAY_String<30> name;
    UCHAR pad[534 - 31];
};
#pragma pack(pop)

ASSERT_SIZE(LAY_BoardHeader, 534);
