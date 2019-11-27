#pragma once

#include <windows.h>
#include <atlstr.h>

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

    CStringA str()
    {
        return CStringA(LPCSTR(data), len);
    }
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

enum
{
    LAY_C1,
    LAY_S1,
    LAY_C2,
    LAY_S2,
    LAY_I1,
    LAY_I2,
    LAY_O,
};

#pragma pack(push, 1)
struct LAY_BoardHeader
{
    LAY_String<30> name;

    UCHAR __pad0[4];
    DWORD size_x;
    DWORD size_y;

    UCHAR ground_pane[7];
    UCHAR __pad1[5];
    UCHAR active_grid_val_lo;
    UCHAR __pad1_1[18];

    UCHAR active_layer;
    UCHAR __pad2[3]; // DWORD?

    UCHAR layer_visible[7];
    UCHAR show_scanned_copy_top;
    UCHAR show_scanned_copy_bottom;

    LAY_String<200> scanned_copy_top_path;
    LAY_String<200> scanned_copy_bottom_path;

    DWORD unk1;
    DWORD unk2;

    UCHAR __pad3[17];

    DWORD unk3;
    DWORD unk4;

    UCHAR __pad4[7];

    UCHAR is_multilayer;
    DWORD unk5;
};
#pragma pack(pop)

ASSERT_SIZE(LAY_BoardHeader, 534);
ASSERT_FIELD_OFFSET(LAY_BoardHeader, size_x, 0x23);
ASSERT_FIELD_OFFSET(LAY_BoardHeader, size_y, 0x27);
ASSERT_FIELD_OFFSET(LAY_BoardHeader, ground_pane, 0x2b);
ASSERT_FIELD_OFFSET(LAY_BoardHeader, active_grid_val_lo, 0x37);
ASSERT_FIELD_OFFSET(LAY_BoardHeader, layer_visible, 0x4e);
ASSERT_FIELD_OFFSET(LAY_BoardHeader, scanned_copy_top_path, 0x57);
ASSERT_FIELD_OFFSET(LAY_BoardHeader, scanned_copy_bottom_path, 0x120);
ASSERT_FIELD_OFFSET(LAY_BoardHeader, unk1, 0x1e9);
