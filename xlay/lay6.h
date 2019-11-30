#pragma once

#include <windows.h>
#include <atlstr.h>
#include <atlcoll.h>
#include <math.h>
#include <assert.h>

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
    double active_grid_val;
    double zoom;
    DWORD viewport_offset_x;
    DWORD viewport_offset_y;

    UCHAR active_layer;
    UCHAR __pad2[3]; // DWORD?

    UCHAR layer_visible[7];
    UCHAR show_scanned_copy_top;
    UCHAR show_scanned_copy_bottom;

    LAY_String<200> scanned_copy_top_path;
    LAY_String<200> scanned_copy_bottom_path;

    DWORD unk1;
    DWORD unk2;

    UCHAR __pad3[16];

    DWORD unk3;
    DWORD unk4;

    INT32 center_x; // mm/10000
    INT32 center_y; // mm/10000

    UCHAR is_multilayer;

    DWORD num_objects;
};
#pragma pack(pop)

ASSERT_SIZE(LAY_BoardHeader, 534);
ASSERT_FIELD_OFFSET(LAY_BoardHeader, size_x, 0x23);
ASSERT_FIELD_OFFSET(LAY_BoardHeader, size_y, 0x27);
ASSERT_FIELD_OFFSET(LAY_BoardHeader, ground_pane, 0x2b);
ASSERT_FIELD_OFFSET(LAY_BoardHeader, active_grid_val, 0x32);
ASSERT_FIELD_OFFSET(LAY_BoardHeader, zoom, 0x3a);
ASSERT_FIELD_OFFSET(LAY_BoardHeader, layer_visible, 0x4e);
ASSERT_FIELD_OFFSET(LAY_BoardHeader, scanned_copy_top_path, 0x57);
ASSERT_FIELD_OFFSET(LAY_BoardHeader, scanned_copy_bottom_path, 0x120);
ASSERT_FIELD_OFFSET(LAY_BoardHeader, center_x, 0x209);
ASSERT_FIELD_OFFSET(LAY_BoardHeader, center_y, 0x20d);
ASSERT_FIELD_OFFSET(LAY_BoardHeader, unk1, 0x1e9);

enum
{
    OBJ_THT_PAD = 2,
    OBJ_POLY = 4,
    OBJ_CIRCLE = 5,
    OBJ_LINE = 6,
    OBJ_TEXT = 7,
    OBJ_SMD_PAD = 8,
};

enum
{
    THT_SHAPE_CIRCLE = 1,
    THT_SHAPE_OCT = 2,
    THT_SHAPE_SQUARE = 3,
};

enum
{
    TH_STYLE_C1,
    TH_STYLE_C2,
    TH_STYLE_I1,
    TH_STYLE_I2,
};

#pragma pack(push, 1)
struct LAY_Object
{
    UCHAR type;
    float x;
    float y;
    float out; // rin (for circles)
    float in; // rout (for circles)

    DWORD line_width; // end angle (for circles)
    UCHAR __pad1;
    UCHAR layer;
    UCHAR tht_shape;

    UCHAR __pad_1_1[4];
    UINT16 component_id;
    UCHAR unk1;

    union {
        UCHAR th_style[4];
        DWORD start_angle;
    };

    UCHAR __pad1_2[5];
    UCHAR th_style_custom; // also fill

    UCHAR __pad2[9];
    UCHAR thermobarier;
    UCHAR flip_vertical;
    UCHAR cutoff;
    DWORD thzise; // rotation
    UCHAR metalisation;
    UCHAR soldermask;

    UCHAR __pad4[18];
};
#pragma pack(pop)

ASSERT_FIELD_OFFSET(LAY_Object, tht_shape, 0x17);
ASSERT_FIELD_OFFSET(LAY_Object, th_style, 0x1f);
ASSERT_FIELD_OFFSET(LAY_Object, th_style_custom, 0x28);
ASSERT_FIELD_OFFSET(LAY_Object, thermobarier, 0x32);
ASSERT_FIELD_OFFSET(LAY_Object, metalisation, 0x39);

#pragma pack(push, 1)
struct LAY_Point
{
    float x;
    float y;
};
#pragma pack(pop)

#pragma pack(push, 1)
struct LAY_Component
{
    float off_x;
    float off_y;
    UCHAR center_mode;
    double rotation;
};
#pragma pack(pop)


class CLayFileHeader : public LAY_FileHeader
{
public:
    CLayFileHeader()
    {}

    void Read(FILE *file)
    {
        fread_s((LAY_FileHeader *) this, sizeof(LAY_FileHeader), sizeof(LAY_FileHeader), 1, file);
    }
};

class CLayBoardHeader : public LAY_BoardHeader
{
public:
    CLayBoardHeader()
    {}

    void Read(FILE *file)
    {
        fread_s((LAY_BoardHeader *) this, sizeof(LAY_BoardHeader), sizeof(LAY_BoardHeader), 1, file);
    }
};

class CLayComponent : public LAY_Component
{
public:
    bool valid;
    CStringA package;
    CStringA comment;
    UCHAR use;

    CLayComponent()
        : valid(false)
    {}

    void Read(FILE *file)
    {
        DWORD len;

        fread_s((LAY_Component *) this, sizeof(LAY_Component), sizeof(LAY_Component), 1, file);

        fread_s(&len, sizeof(len), sizeof(len), 1, file);
        fread_s(package.GetBuffer(len), len, 1, len, file);
        package.ReleaseBuffer(len);

        fread_s(&len, sizeof(len), sizeof(len), 1, file);
        fread_s(comment.GetBuffer(len), len, 1, len, file);
        comment.ReleaseBuffer(len);

        fread_s(&use, sizeof(use), sizeof(use), 1, file);
        valid = true;
    }
};

class CLayObject : public LAY_Object
{
public:
    CStringA text;
    CStringA marker;
    CAtlArray<DWORD> groups;
    CAtlArray<LAY_Point> poly_points; //??
    CAtlArray<CLayObject> text_objects;
    CLayComponent component;

    CLayObject()
    {}

    void Read(FILE *file, bool textchild = false)
    {
        DWORD len;

        fread_s((LAY_Object *) this, sizeof(LAY_Object), sizeof(LAY_Object), 1, file);

        if (!textchild)
        {
            // ???

            fread_s(&len, sizeof(len), sizeof(len), 1, file);
            fread_s(text.GetBuffer(len), len, 1, len, file);
            text.ReleaseBuffer(len);

            fread_s(&len, sizeof(len), sizeof(len), 1, file);
            fread_s(marker.GetBuffer(len), len, 1, len, file);
            marker.ReleaseBuffer(len);

            fread_s(&len, sizeof(len), sizeof(len), 1, file);
            groups.SetCount(len);
            fread_s(groups.GetData(), sizeof(DWORD) * len, sizeof(DWORD), len, file);
        }

        if (type == OBJ_CIRCLE)
            return;

        if (type == OBJ_TEXT)
        {
            fread_s(&len, sizeof(len), sizeof(len), 1, file);
            for (int i = 0; i < len; i++)
            {
                text_objects.Add();
                text_objects[text_objects.GetCount() - 1].Read(file, true);
            }

            if (tht_shape == 1)
            {
                component.Read(file);
            }

            return;
        }

        fread_s(&len, sizeof(len), sizeof(len), 1, file);
        poly_points.SetCount(len);
        fread_s(poly_points.GetData(), sizeof(LAY_Point) * len, sizeof(LAY_Point), len, file);
    }
};
