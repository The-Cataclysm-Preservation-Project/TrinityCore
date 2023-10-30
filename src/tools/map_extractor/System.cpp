/*
 * This file is part of the TrinityCore Project. See AUTHORS file for Copyright information
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include "StringFormat.h"
#include "MapDefines.h"
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <bitset>
#include <cstdio>
#include <deque>
#include <fstream>
#include <set>
#include <unordered_map>
#include <cstdlib>
#include <cstring>

#include "Common.h"
#ifdef PLATFORM_WINDOWS
#undef PLATFORM_WINDOWS
#endif

#ifdef _WIN32
#include "direct.h"
#else
#include <sys/stat.h>
#include <unistd.h>
#define ERROR_PATH_NOT_FOUND ERROR_FILE_NOT_FOUND
#endif

#include "StormLib.h"
#include "dbcfile.h"
#include "Banner.h"
#include "adt.h"
#include "wdt.h"
#include <fcntl.h>

#if defined( __GNUC__ )
    #define _open   open
    #define _close close
    #ifndef O_BINARY
        #define O_BINARY 0
    #endif
#else
    #include <io.h>
#endif

#ifdef O_LARGEFILE
    #define OPEN_FLAGS (O_RDONLY | O_BINARY | O_LARGEFILE)
#else
    #define OPEN_FLAGS (O_RDONLY | O_BINARY)
#endif

#include <G3D/Plane.h>

HANDLE WorldMpq = nullptr;
HANDLE LocaleMpq = nullptr;

typedef struct
{
    char name[64];
    uint32 id;
} map_id;

struct LiquidMaterialEntry
{
    int8 LVF;
};

struct LiquidObjectEntry
{
    int16 LiquidTypeID;
};

struct LiquidTypeEntry
{
    uint8 SoundBank;
    uint8 MaterialID;
};

std::vector<map_id> map_ids;
std::unordered_map<uint32, LiquidMaterialEntry> LiquidMaterials;
std::unordered_map<uint32, LiquidObjectEntry> LiquidObjects;
std::unordered_map<uint32, LiquidTypeEntry> LiquidTypes;
bool PrintProgress = true;
boost::filesystem::path input_path;
boost::filesystem::path output_path;

// **************************************************
// Extractor options
// **************************************************
enum Extract : uint8
{
    EXTRACT_MAP     = 0x1,
    EXTRACT_DBC     = 0x2,
    EXTRACT_CAMERA  = 0x4,

    EXTRACT_ALL = EXTRACT_MAP | EXTRACT_DBC | EXTRACT_CAMERA
};

// Select data for extract
int   CONF_extract = EXTRACT_MAP | EXTRACT_DBC | EXTRACT_CAMERA;

// This option allow limit minimum height to some value (Allow save some memory)
bool  CONF_allow_height_limit = true;
float CONF_use_minHeight = -2000.0f;

// This option allow use float to int conversion
bool  CONF_allow_float_to_int   = true;
float CONF_float_to_int8_limit  = 2.0f;      // Max accuracy = val/256
float CONF_float_to_int16_limit = 2048.0f;   // Max accuracy = val/65536
float CONF_flat_height_delta_limit = 0.005f; // If max - min less this value - surface is flat
float CONF_flat_liquid_delta_limit = 0.001f; // If max - min less this value - liquid surface is flat

// List MPQ for extract maps from
char const* CONF_mpq_list[]=
{
    "world.MPQ",
    "art.MPQ",
    "world2.MPQ",
    "expansion1.MPQ",
    "expansion2.MPQ",
    "expansion3.MPQ",
};

uint32 const Builds[] = {13164, 13205, 13287, 13329, 13596, 13623, 13914, 14007, 14333, 14480, 14545, 15005, 15050, 15211, 15354, 15595, 0};
#define LAST_DBC_IN_DATA_BUILD 13623    // after this build mpqs with dbc are back to locale folder
#define NEW_BASE_SET_BUILD  15211
#define CLIENT_BUILD 15595

#define LOCALES_COUNT 15

char const* Locales[LOCALES_COUNT] =
{
    "enGB", "enUS",
    "deDE", "esES",
    "frFR", "koKR",
    "zhCN", "zhTW",
    "enCN", "enTW",
    "esMX", "ruRU",
    "ptBR", "ptPT",
    "itIT"
};

uint8 const MpqToWowLocale[LOCALES_COUNT] =
{
    LOCALE_enUS,
    LOCALE_enUS,
    LOCALE_deDE,
    LOCALE_esES,
    LOCALE_frFR,
    LOCALE_koKR,
    LOCALE_zhCN,
    LOCALE_zhTW,
    LOCALE_zhCN,
    LOCALE_zhTW,
    LOCALE_esMX,
    LOCALE_ruRU,
    LOCALE_ptBR,
    LOCALE_ptBR,
    LOCALE_itIT
};

void CreateDir(boost::filesystem::path const& path)
{
    namespace fs = boost::filesystem;
    if (fs::exists(path))
        return;

    boost::system::error_code err;
    if (!fs::create_directory(path, err) || err)
        throw std::runtime_error("Unable to create directory" + path.string());
}

void Usage(char const* prg)
{
    printf(
        "Usage:\n"\
        "%s -[var] [value]\n"\
        "-i set input path\n"\
        "-o set output path\n"\
        "-e extract only MAP(1)/DBC(2)/Camera(4) - standard: all(7)\n"\
        "-f height stored as int (less map size but lost some accuracy) 1 by default\n"\
        "Example: %s -f 0 -i \"c:\\games\\game\"", prg, prg);
    exit(1);
}

void HandleArgs(int argc, char* arg[])
{
    for (int c = 1; c < argc; ++c)
    {
        // i - input path
        // o - output path
        // e - extract only MAP(1)/DBC(2)/Camera(4) - standard: all(7)
        // f - use float to int conversion
        // h - limit minimum height
        // b - target client build
        if (arg[c][0] != '-')
            Usage(arg[0]);

        switch (arg[c][1])
        {
            case 'i':
                if (c + 1 < argc && strlen(arg[c + 1])) // all ok
                    input_path = boost::filesystem::path(arg[c++ + 1]);
                else
                    Usage(arg[0]);
                break;
            case 'o':
                if (c + 1 < argc && strlen(arg[c + 1])) // all ok
                    output_path = boost::filesystem::path(arg[c++ + 1]);
                else
                    Usage(arg[0]);
                break;
            case 'f':
                if (c + 1 < argc)                            // all ok
                    CONF_allow_float_to_int = atoi(arg[c++ + 1]) != 0;
                else
                    Usage(arg[0]);
                break;
            case 'e':
                if (c + 1 < argc)                            // all ok
                {
                    CONF_extract = atoi(arg[c++ + 1]);
                    if (!(CONF_extract > 0 && CONF_extract <= EXTRACT_ALL))
                        Usage(arg[0]);
                }
                else
                    Usage(arg[0]);
                break;
            default:
                break;
        }
    }
}

uint32 ReadBuild(int locale)
{
    // include build info file also
    std::string filename  = std::string("component.wow-") + Locales[locale] + ".txt";
    //printf("Read %s file... ", filename.c_str());

    HANDLE dbcFile;
    if (!SFileOpenFileEx(LocaleMpq, filename.c_str(), SFILE_OPEN_PATCHED_FILE, &dbcFile))
    {
        printf("Fatal error: Not found %s file!\n", filename.c_str());
        exit(1);
    }

    char buff[512];
    DWORD readBytes = 0;
    SFileReadFile(dbcFile, buff, 512, &readBytes, nullptr);
    if (!readBytes)
    {
        printf("Fatal error: Not found %s file!\n", filename.c_str());
        exit(1);
    }

    std::string text = buff;
    SFileCloseFile(dbcFile);

    size_t pos = text.find("version=\"");
    size_t pos1 = pos + strlen("version=\"");
    size_t pos2 = text.find("\"", pos1);
    if (pos == text.npos || pos2 == text.npos || pos1 >= pos2)
    {
        printf("Fatal error: Invalid  %s file format!\n", filename.c_str());
        exit(1);
    }

    std::string build_str = text.substr(pos1,pos2-pos1);

    int build = atoi(build_str.c_str());
    if (build <= 0)
    {
        printf("Fatal error: Invalid  %s file format!\n", filename.c_str());
        exit(1);
    }

    return build;
}

void ReadMapDBC()
{
    printf("Read Map.dbc file... ");

    HANDLE dbcFile;
    if (!SFileOpenFileEx(LocaleMpq, "DBFilesClient\\Map.dbc", SFILE_OPEN_PATCHED_FILE, &dbcFile))
    {
        printf("Fatal error: Cannot find Map.dbc in archive!\n");
        exit(1);
    }

    DBCFile dbc(dbcFile);
    if (!dbc.open())
    {
        printf("Fatal error: Invalid Map.dbc file format!\n");
        exit(1);
    }

    size_t map_count = dbc.getRecordCount();
    map_ids.resize(map_count);
    for(uint32 x = 0; x < map_count; ++x)
    {
        map_ids[x].id = dbc.getRecord(x).getUInt(0);
        strcpy(map_ids[x].name, dbc.getRecord(x).getString(1));
    }

    SFileCloseFile(dbcFile);
    printf("Done! (%u maps loaded)\n", uint32(map_count));
}

void ReadLiquidMaterialTable()
{
    printf("Read LiquidMaterial.dbc file...\n");

    HANDLE dbcFile;
    if (!SFileOpenFileEx(LocaleMpq, "DBFilesClient\\LiquidMaterial.dbc", SFILE_OPEN_PATCHED_FILE, &dbcFile))
    {
        printf("Fatal error: Cannot find LiquidMaterial.dbc in archive!\n");
        exit(1);
    }

    DBCFile dbc(dbcFile);
    if (!dbc.open())
    {
        printf("Fatal error: Invalid LiquidMaterial.dbc file format!\n");
        exit(1);
    }

    for (uint32 x = 0; x < dbc.getRecordCount(); ++x)
    {
        LiquidMaterialEntry& liquidType = LiquidMaterials[dbc.getRecord(x).getUInt(0)];
        liquidType.LVF = dbc.getRecord(x).getUInt(1);
    }

    printf("Done! (" SZFMTD " LiquidMaterials loaded)\n", LiquidMaterials.size());
}

void ReadLiquidObjectTable()
{
    printf("Read LiquidObject.dbc file...\n");

    HANDLE dbcFile;
    if (!SFileOpenFileEx(LocaleMpq, "DBFilesClient\\LiquidObject.dbc", SFILE_OPEN_PATCHED_FILE, &dbcFile))
    {
        printf("Fatal error: Cannot find LiquidObject.dbc in archive!\n");
        exit(1);
    }

    DBCFile dbc(dbcFile);
    if (!dbc.open())
    {
        printf("Fatal error: Invalid LiquidObject.dbc file format!\n");
        exit(1);
    }

    for (uint32 x = 0; x < dbc.getRecordCount(); ++x)
    {
        LiquidObjectEntry& liquidType = LiquidObjects[dbc.getRecord(x).getUInt(0)];
        liquidType.LiquidTypeID = dbc.getRecord(x).getUInt(3);
    }

    printf("Done! (" SZFMTD " LiquidObjects loaded)\n", LiquidObjects.size());
}

void ReadLiquidTypeTable()
{
    printf("Read LiquidType.dbc file...");
    HANDLE dbcFile;
    if (!SFileOpenFileEx(LocaleMpq, "DBFilesClient\\LiquidType.dbc", SFILE_OPEN_PATCHED_FILE, &dbcFile))
    {
        printf("Fatal error: Cannot find LiquidType.dbc in archive!\n");
        exit(1);
    }

    DBCFile dbc(dbcFile);
    if (!dbc.open())
    {
        printf("Fatal error: Invalid LiquidType.dbc file format!\n");
        exit(1);
    }

    for (uint32 x = 0; x < dbc.getRecordCount(); ++x)
    {
        LiquidTypeEntry& liquidType = LiquidTypes[dbc.getRecord(x).getUInt(0)];
        liquidType.SoundBank = dbc.getRecord(x).getUInt(3);
        liquidType.MaterialID = dbc.getRecord(x).getUInt(14);
    }

    SFileCloseFile(dbcFile);
    printf("Done! (" SZFMTD " LiquidTypes loaded)\n", LiquidTypes.size());
}

//
// Adt file convertor function and data
//

float selectUInt8StepStore(float maxDiff)
{
    return 255 / maxDiff;
}

float selectUInt16StepStore(float maxDiff)
{
    return 65535 / maxDiff;
}
// Temporary grid data store
uint16 area_ids[ADT_CELLS_PER_GRID][ADT_CELLS_PER_GRID];

float V8[ADT_GRID_SIZE][ADT_GRID_SIZE];
float V9[ADT_GRID_SIZE+1][ADT_GRID_SIZE+1];
uint16 uint16_V8[ADT_GRID_SIZE][ADT_GRID_SIZE];
uint16 uint16_V9[ADT_GRID_SIZE+1][ADT_GRID_SIZE+1];
uint8  uint8_V8[ADT_GRID_SIZE][ADT_GRID_SIZE];
uint8  uint8_V9[ADT_GRID_SIZE+1][ADT_GRID_SIZE+1];

uint16 liquid_entry[ADT_CELLS_PER_GRID][ADT_CELLS_PER_GRID];
map_liquidHeaderTypeFlags liquid_flags[ADT_CELLS_PER_GRID][ADT_CELLS_PER_GRID];
bool  liquid_show[ADT_GRID_SIZE][ADT_GRID_SIZE];
float liquid_height[ADT_GRID_SIZE+1][ADT_GRID_SIZE+1];
uint16 holes[ADT_CELLS_PER_GRID][ADT_CELLS_PER_GRID];

int16 flight_box_max[3][3];
int16 flight_box_min[3][3];

LiquidVertexFormatType adt_MH2O::GetLiquidVertexFormat(adt_liquid_instance const* liquidInstance) const
{
    if (liquidInstance->LiquidVertexFormat < 42)
        return static_cast<LiquidVertexFormatType>(liquidInstance->LiquidVertexFormat);

    if (liquidInstance->LiquidType == 2)
        return LiquidVertexFormatType::Depth;

    auto liquidType = LiquidTypes.find(liquidInstance->LiquidType);
    if (liquidType != LiquidTypes.end())
    {
        auto liquidMaterial = LiquidMaterials.find(liquidType->second.MaterialID);
        if (liquidMaterial != LiquidMaterials.end())
            return static_cast<LiquidVertexFormatType>(liquidMaterial->second.LVF);
    }

    return static_cast<LiquidVertexFormatType>(-1);
}

bool ConvertADT(std::string const& inputPath, std::string const& outputPath, int /*cell_y*/, int /*cell_x*/, uint32 build, bool ignoreDeepWater)
{
    ChunkedFile adt;

    if (!adt.loadFile(WorldMpq, inputPath))
        return false;

    // Prepare map header
    map_fileheader map{ };
    map.mapMagic = MapMagic;
    map.versionMagic = MapVersionMagic;
    map.buildMagic = build;

    // Get area flags data
    memset(area_ids, 0, sizeof(area_ids));
    memset(V9, 0, sizeof(V9));
    memset(V8, 0, sizeof(V8));

    memset(liquid_show, 0, sizeof(liquid_show));
    memset(liquid_flags, 0, sizeof(liquid_flags));
    memset(liquid_entry, 0, sizeof(liquid_entry));

    memset(holes, 0, sizeof(holes));

    bool hasHoles = false;
    bool hasFlightBox = false;

    for (std::multimap<std::string, FileChunk*>::const_iterator itr = adt.chunks.lower_bound("MCNK"); itr != adt.chunks.upper_bound("MCNK"); ++itr)
    {
        adt_MCNK* mcnk = itr->second->As<adt_MCNK>();

        // Area data
        area_ids[mcnk->iy][mcnk->ix] = mcnk->areaid;

        // Height
        // Height values for triangles stored in order:
        // 1     2     3     4     5     6     7     8     9
        //    10    11    12    13    14    15    16    17
        // 18    19    20    21    22    23    24    25    26
        //    27    28    29    30    31    32    33    34
        // . . . . . . . .
        // For better get height values merge it to V9 and V8 map
        // V9 height map:
        // 1     2     3     4     5     6     7     8     9
        // 18    19    20    21    22    23    24    25    26
        // . . . . . . . .
        // V8 height map:
        //    10    11    12    13    14    15    16    17
        //    27    28    29    30    31    32    33    34
        // . . . . . . . .

        // Set map height as grid height
        for (int y = 0; y <= ADT_CELL_SIZE; y++)
        {
            int cy = mcnk->iy * ADT_CELL_SIZE + y;
            for (int x = 0; x <= ADT_CELL_SIZE; x++)
            {
                int cx = mcnk->ix * ADT_CELL_SIZE + x;
                V9[cy][cx] = mcnk->ypos;
            }
        }

        for (int y = 0; y < ADT_CELL_SIZE; y++)
        {
            int cy = mcnk->iy * ADT_CELL_SIZE + y;
            for (int x = 0; x < ADT_CELL_SIZE; x++)
            {
                int cx = mcnk->ix * ADT_CELL_SIZE + x;
                V8[cy][cx] = mcnk->ypos;
            }
        }

        // Get custom height
        if (FileChunk* chunk = itr->second->GetSubChunk("MCVT"))
        {
            adt_MCVT* mcvt = chunk->As<adt_MCVT>();
            // get V9 height map
            for (int y = 0; y <= ADT_CELL_SIZE; y++)
            {
                int cy = mcnk->iy * ADT_CELL_SIZE + y;
                for (int x = 0; x <= ADT_CELL_SIZE; x++)
                {
                    int cx = mcnk->ix * ADT_CELL_SIZE + x;
                    V9[cy][cx] += mcvt->height_map[y * (ADT_CELL_SIZE * 2 + 1) + x];
                }
            }
            // get V8 height map
            for (int y = 0; y < ADT_CELL_SIZE; y++)
            {
                int cy = mcnk->iy * ADT_CELL_SIZE + y;
                for (int x = 0; x < ADT_CELL_SIZE; x++)
                {
                    int cx = mcnk->ix * ADT_CELL_SIZE + x;
                    V8[cy][cx] += mcvt->height_map[y * (ADT_CELL_SIZE * 2 + 1) + ADT_CELL_SIZE + 1 + x];
                }
            }
        }

        // Liquid data
        if (mcnk->sizeMCLQ > 8)
        {
            if (FileChunk* chunk = itr->second->GetSubChunk("MCLQ"))
            {
                adt_MCLQ* liquid = chunk->As<adt_MCLQ>();
                int count = 0;
                for (int y = 0; y < ADT_CELL_SIZE; ++y)
                {
                    int cy = mcnk->iy * ADT_CELL_SIZE + y;
                    for (int x = 0; x < ADT_CELL_SIZE; ++x)
                    {
                        int cx = mcnk->ix * ADT_CELL_SIZE + x;
                        if (liquid->flags[y][x] != 0x0F)
                        {
                            liquid_show[cy][cx] = true;
                            if (!ignoreDeepWater && liquid->flags[y][x] & (1 << 7))
                                liquid_flags[mcnk->iy][mcnk->ix] |= map_liquidHeaderTypeFlags::DarkWater;;
                            ++count;
                        }
                    }
                }

                uint32 c_flag = mcnk->flags;
                if (c_flag & (1 << 2))
                {
                    liquid_entry[mcnk->iy][mcnk->ix] = 1;
                    liquid_flags[mcnk->iy][mcnk->ix] |= map_liquidHeaderTypeFlags::Water; // water
                }
                if (c_flag & (1 << 3))
                {
                    liquid_entry[mcnk->iy][mcnk->ix] = 2;
                    liquid_flags[mcnk->iy][mcnk->ix] |= map_liquidHeaderTypeFlags::Ocean; // ocean
                }
                if (c_flag & (1 << 4))
                {
                    liquid_entry[mcnk->iy][mcnk->ix] = 3;
                    liquid_flags[mcnk->iy][mcnk->ix] |= map_liquidHeaderTypeFlags::Magma; // magma/slime
                }

                if (!count && liquid_flags[mcnk->iy][mcnk->ix] != map_liquidHeaderTypeFlags::NoWater)
                    fprintf(stderr, "Wrong liquid detect in MCLQ chunk");

                for (int y = 0; y <= ADT_CELL_SIZE; ++y)
                {
                    int cy = mcnk->iy * ADT_CELL_SIZE + y;
                    for (int x = 0; x <= ADT_CELL_SIZE; ++x)
                    {
                        int cx = mcnk->ix * ADT_CELL_SIZE + x;
                        liquid_height[cy][cx] = liquid->liquid[y][x].height;
                    }
                }
            }
        }

        // Hole data
        holes[mcnk->iy][mcnk->ix] = mcnk->holes;
        if (!hasHoles && mcnk->holes != 0)
            hasHoles = true;
    }

    // Get liquid map for grid (in WOTLK used MH2O chunk)
    if (FileChunk* chunk = adt.GetChunk("MH2O"))
    {
        adt_MH2O* h2o = chunk->As<adt_MH2O>();
        for (int32 i = 0; i < ADT_CELLS_PER_GRID; i++)
        {
            for (int32 j = 0; j < ADT_CELLS_PER_GRID; j++)
            {
                adt_liquid_instance const* h = h2o->GetLiquidInstance(i, j);
                if (!h)
                    continue;

                adt_liquid_attributes attrs = h2o->GetLiquidAttributes(i, j);

                int32 count = 0;
                uint64 existsMask = h2o->GetLiquidExistsBitmap(h);
                for (int32 y = 0; y < h->GetHeight(); y++)
                {
                    int32 cy = i * ADT_CELL_SIZE + y + h->GetOffsetY();
                    for (int32 x = 0; x < h->GetWidth(); x++)
                    {
                        int32 cx = j * ADT_CELL_SIZE + x + h->GetOffsetX();
                        if (existsMask & 1)
                        {
                            liquid_show[cy][cx] = true;
                            ++count;
                        }
                        existsMask >>= 1;
                    }
                }

                liquid_entry[i][j] = h2o->GetLiquidType(h);
                switch (LiquidTypes.at(liquid_entry[i][j]).SoundBank)
                {
                    case LIQUID_TYPE_WATER: liquid_flags[i][j] |= map_liquidHeaderTypeFlags::Water; break;
                    case LIQUID_TYPE_OCEAN: liquid_flags[i][j] |= map_liquidHeaderTypeFlags::Ocean; if (!ignoreDeepWater && attrs.Deep) liquid_flags[i][j] |= map_liquidHeaderTypeFlags::DarkWater; break;
                    case LIQUID_TYPE_MAGMA: liquid_flags[i][j] |= map_liquidHeaderTypeFlags::Magma; break;
                    case LIQUID_TYPE_SLIME: liquid_flags[i][j] |= map_liquidHeaderTypeFlags::Slime; break;
                    default:
                        printf("\nCan't find Liquid type %u for map %s\nchunk %d,%d\n", h->LiquidType, inputPath.c_str(), i, j);
                        break;
                }

                if (!count && liquid_flags[i][j] != map_liquidHeaderTypeFlags::NoWater)
                    printf("Wrong liquid detect in MH2O chunk");

                int32 pos = 0;
                for (int32 y = 0; y <= h->GetHeight(); y++)
                {
                    int32 cy = i * ADT_CELL_SIZE + y + h->GetOffsetY();
                    for (int32 x = 0; x <= h->GetWidth(); x++)
                    {
                        int32 cx = j * ADT_CELL_SIZE + x + h->GetOffsetX();
                        liquid_height[cy][cx] = h2o->GetLiquidHeight(h, pos);
                        pos++;
                    }
                }
            }
        }
    }

    if (FileChunk* chunk = adt.GetChunk("MFBO"))
    {
        adt_MFBO* mfbo = chunk->As<adt_MFBO>();
        memcpy(flight_box_max, &mfbo->max, sizeof(flight_box_max));
        memcpy(flight_box_min, &mfbo->min, sizeof(flight_box_min));
        hasFlightBox = true;
    }

    //============================================
    // Try pack area data
    //============================================
    bool fullAreaData = false;
    uint32 areaId = area_ids[0][0];
    for (int y = 0; y < ADT_CELLS_PER_GRID; ++y)
    {
        for (int x = 0; x < ADT_CELLS_PER_GRID; ++x)
        {
            if (area_ids[y][x] != areaId)
            {
                fullAreaData = true;
                break;
            }
        }
    }

    map.areaMapOffset = sizeof(map);
    map.areaMapSize = sizeof(map_areaHeader);

    map_areaHeader areaHeader;
    areaHeader.areaMagic = MapAreaMagic;
    areaHeader.flags = map_areaHeaderFlags::None;
    if (fullAreaData)
    {
        areaHeader.gridArea = 0;
        map.areaMapSize += sizeof(area_ids);
    }
    else
    {
        areaHeader.flags |= map_areaHeaderFlags::NoArea;
        areaHeader.gridArea = static_cast<uint16>(areaId);
    }


    //============================================
    // Try pack height data
    //============================================
    float maxHeight = -20000;
    float minHeight =  20000;
    for (int y=0; y<ADT_GRID_SIZE; y++)
    {
        for(int x=0;x<ADT_GRID_SIZE;x++)
        {
            float h = V8[y][x];
            if (maxHeight < h) maxHeight = h;
            if (minHeight > h) minHeight = h;
        }
    }
    for (int y=0; y<=ADT_GRID_SIZE; y++)
    {
        for(int x=0;x<=ADT_GRID_SIZE;x++)
        {
            float h = V9[y][x];
            if (maxHeight < h) maxHeight = h;
            if (minHeight > h) minHeight = h;
        }
    }

    // Check for allow limit minimum height (not store height in deep ochean - allow save some memory)
    if (CONF_allow_height_limit && minHeight < CONF_use_minHeight)
    {
        for (int y=0; y<ADT_GRID_SIZE; y++)
            for(int x=0;x<ADT_GRID_SIZE;x++)
                if (V8[y][x] < CONF_use_minHeight)
                    V8[y][x] = CONF_use_minHeight;
        for (int y=0; y<=ADT_GRID_SIZE; y++)
            for(int x=0;x<=ADT_GRID_SIZE;x++)
                if (V9[y][x] < CONF_use_minHeight)
                    V9[y][x] = CONF_use_minHeight;
        if (minHeight < CONF_use_minHeight)
            minHeight = CONF_use_minHeight;
        if (maxHeight < CONF_use_minHeight)
            maxHeight = CONF_use_minHeight;
    }

    map.heightMapOffset = map.areaMapOffset + map.areaMapSize;
    map.heightMapSize = sizeof(map_heightHeader);

    map_heightHeader heightHeader;
    heightHeader.heightMagic = MapHeightMagic;
    heightHeader.flags = map_heightHeaderFlags::None;
    heightHeader.gridHeight = minHeight;
    heightHeader.gridMaxHeight = maxHeight;

    if (maxHeight == minHeight)
        heightHeader.flags |= map_heightHeaderFlags::NoHeight;

    // Not need store if flat surface
    if (CONF_allow_float_to_int && (maxHeight - minHeight) < CONF_flat_height_delta_limit)
        heightHeader.flags |= map_heightHeaderFlags::NoHeight;

    if (hasFlightBox)
    {
        heightHeader.flags |= map_heightHeaderFlags::HasFlightBounds;
        map.heightMapSize += sizeof(flight_box_max) + sizeof(flight_box_min);
    }

        // Try store as packed in uint16 or uint8 values
    if (!heightHeader.flags.HasFlag(map_heightHeaderFlags::NoHeight))
    {
        float step = 0;
        // Try Store as uint values
        if (CONF_allow_float_to_int)
        {
            float diff = maxHeight - minHeight;
            if (diff < CONF_float_to_int8_limit)      // As uint8 (max accuracy = CONF_float_to_int8_limit/256)
            {
                heightHeader.flags |= map_heightHeaderFlags::HeightAsInt8;
                step = selectUInt8StepStore(diff);
            }
            else if (diff<CONF_float_to_int16_limit)  // As uint16 (max accuracy = CONF_float_to_int16_limit/65536)
            {
                heightHeader.flags |= map_heightHeaderFlags::HeightAsInt16;
                step = selectUInt16StepStore(diff);
            }
        }

        // Pack it to int values if need
        if (heightHeader.flags.HasFlag(map_heightHeaderFlags::HeightAsInt8))
        {
            for (int y=0; y<ADT_GRID_SIZE; y++)
                for(int x=0;x<ADT_GRID_SIZE;x++)
                    uint8_V8[y][x] = uint8((V8[y][x] - minHeight) * step + 0.5f);
            for (int y=0; y<=ADT_GRID_SIZE; y++)
                for(int x=0;x<=ADT_GRID_SIZE;x++)
                    uint8_V9[y][x] = uint8((V9[y][x] - minHeight) * step + 0.5f);
            map.heightMapSize+= sizeof(uint8_V9) + sizeof(uint8_V8);
        }
        else if (heightHeader.flags.HasFlag(map_heightHeaderFlags::HeightAsInt16))
        {
            for (int y=0; y<ADT_GRID_SIZE; y++)
                for(int x=0;x<ADT_GRID_SIZE;x++)
                    uint16_V8[y][x] = uint16((V8[y][x] - minHeight) * step + 0.5f);
            for (int y=0; y<=ADT_GRID_SIZE; y++)
                for(int x=0;x<=ADT_GRID_SIZE;x++)
                    uint16_V9[y][x] = uint16((V9[y][x] - minHeight) * step + 0.5f);
            map.heightMapSize+= sizeof(uint16_V9) + sizeof(uint16_V8);
        }
        else
            map.heightMapSize+= sizeof(V9) + sizeof(V8);
    }

    //============================================
    // Pack liquid data
    //============================================
    uint16 firstLiquidType = liquid_entry[0][0];
    map_liquidHeaderTypeFlags firstLiquidFlag = liquid_flags[0][0];
    bool fullType = false;
    for (int y = 0; y < ADT_CELLS_PER_GRID; y++)
    {
        for (int x = 0; x < ADT_CELLS_PER_GRID; x++)
        {
            if (liquid_entry[y][x] != firstLiquidType || liquid_flags[y][x] != firstLiquidFlag)
            {
                fullType = true;
                y = ADT_CELLS_PER_GRID;
                break;
            }
        }
    }

    map_liquidHeader liquidHeader;

    // no water data (if all grid have 0 liquid type)
    if (firstLiquidFlag == map_liquidHeaderTypeFlags::NoWater && !fullType)
    {
        // No liquid data
        map.liquidMapOffset = 0;
        map.liquidMapSize   = 0;
    }
    else
    {
        int minX = 255, minY = 255;
        int maxX = 0, maxY = 0;
        maxHeight = -20000;
        minHeight = 20000;
        for (int y=0; y<ADT_GRID_SIZE; y++)
        {
            for(int x=0; x<ADT_GRID_SIZE; x++)
            {
                if (liquid_show[y][x])
                {
                    if (minX > x) minX = x;
                    if (maxX < x) maxX = x;
                    if (minY > y) minY = y;
                    if (maxY < y) maxY = y;
                    float h = liquid_height[y][x];
                    if (maxHeight < h) maxHeight = h;
                    if (minHeight > h) minHeight = h;
                }
                else
                {
                    liquid_height[y][x] = CONF_use_minHeight;
                    if (minHeight > CONF_use_minHeight) minHeight = CONF_use_minHeight;
                }
            }
        }
        map.liquidMapOffset = map.heightMapOffset + map.heightMapSize;
        map.liquidMapSize = sizeof(map_liquidHeader);
        liquidHeader.liquidMagic = MapLiquidMagic;
        liquidHeader.flags = map_liquidHeaderFlags::None;
        liquidHeader.liquidType = 0;
        liquidHeader.offsetX = minX;
        liquidHeader.offsetY = minY;
        liquidHeader.width   = maxX - minX + 1 + 1;
        liquidHeader.height  = maxY - minY + 1 + 1;
        liquidHeader.liquidLevel = minHeight;

        if (maxHeight == minHeight)
            liquidHeader.flags |= map_liquidHeaderFlags::NoHeight;

        // Not need store if flat surface
        if (CONF_allow_float_to_int && (maxHeight - minHeight) < CONF_flat_liquid_delta_limit)
            liquidHeader.flags |= map_liquidHeaderFlags::NoHeight;

        if (!fullType)
            liquidHeader.flags |= map_liquidHeaderFlags::NoType;

        if (liquidHeader.flags.HasFlag(map_liquidHeaderFlags::NoType))
        {
            liquidHeader.liquidFlags = firstLiquidFlag;
            liquidHeader.liquidType = firstLiquidType;
        }
        else
            map.liquidMapSize += sizeof(liquid_entry) + sizeof(liquid_flags);

        if (!liquidHeader.flags.HasFlag(map_liquidHeaderFlags::NoHeight))
            map.liquidMapSize += sizeof(float)*liquidHeader.width*liquidHeader.height;
    }

    if (hasHoles)
    {
        if (map.liquidMapOffset)
            map.holesOffset = map.liquidMapOffset + map.liquidMapSize;
        else
            map.holesOffset = map.heightMapOffset + map.heightMapSize;

        map.holesSize = sizeof(holes);
    }
    else
    {
        map.holesOffset = 0;
        map.holesSize = 0;
    }

    // Ok all data prepared - store it
    std::ofstream outFile(outputPath, std::ofstream::out | std::ofstream::binary);
    if (!outFile)
    {
        printf("Can't create the output file '%s'\n", outputPath.c_str());
        return false;
    }

    outFile.write(reinterpret_cast<const char*>(&map), sizeof(map));
    // Store area data
    outFile.write(reinterpret_cast<const char*>(&areaHeader), sizeof(areaHeader));
    if (!areaHeader.flags.HasFlag(map_areaHeaderFlags::NoArea))
        outFile.write(reinterpret_cast<const char*>(area_ids), sizeof(area_ids));

    // Store height data
    outFile.write(reinterpret_cast<const char*>(&heightHeader), sizeof(heightHeader));
    if (!heightHeader.flags.HasFlag(map_heightHeaderFlags::NoHeight))
    {
        if (heightHeader.flags.HasFlag(map_heightHeaderFlags::HeightAsInt16))
        {
            outFile.write(reinterpret_cast<const char*>(uint16_V9), sizeof(uint16_V9));
            outFile.write(reinterpret_cast<const char*>(uint16_V8), sizeof(uint16_V8));
        }
        else if (heightHeader.flags.HasFlag(map_heightHeaderFlags::HeightAsInt8))
        {
            outFile.write(reinterpret_cast<const char*>(uint8_V9), sizeof(uint8_V9));
            outFile.write(reinterpret_cast<const char*>(uint8_V8), sizeof(uint8_V8));
        }
        else
        {
            outFile.write(reinterpret_cast<const char*>(V9), sizeof(V9));
            outFile.write(reinterpret_cast<const char*>(V8), sizeof(V8));
        }
    }

    if (heightHeader.flags.HasFlag(map_heightHeaderFlags::HasFlightBounds))
    {
        outFile.write(reinterpret_cast<char*>(flight_box_max), sizeof(flight_box_max));
        outFile.write(reinterpret_cast<char*>(flight_box_min), sizeof(flight_box_min));
    }

    // Store liquid data if need
    if (map.liquidMapOffset)
    {
        outFile.write(reinterpret_cast<const char*>(&liquidHeader), sizeof(liquidHeader));
        if (!liquidHeader.flags.HasFlag(map_liquidHeaderFlags::NoType))
        {
            outFile.write(reinterpret_cast<const char*>(liquid_entry), sizeof(liquid_entry));
            outFile.write(reinterpret_cast<const char*>(liquid_flags), sizeof(liquid_flags));
        }

        if (!liquidHeader.flags.HasFlag(map_liquidHeaderFlags::NoHeight))
        {
            for (int y = 0; y < liquidHeader.height; y++)
                outFile.write(reinterpret_cast<const char*>(&liquid_height[y + liquidHeader.offsetY][liquidHeader.offsetX]), sizeof(float) * liquidHeader.width);
        }
    }

    // store hole data
    if (hasHoles)
        outFile.write(reinterpret_cast<const char*>(holes), map.holesSize);

    outFile.close();

    return true;
}

bool IsDeepWaterIgnored(uint32 mapId, uint32 x, uint32 y)
{
    if (mapId == 0)
    {
        //                                                                                                GRID(39, 24) || GRID(39, 25) || GRID(39, 26) ||
        //                                                                                                GRID(40, 24) || GRID(40, 25) || GRID(40, 26) ||
        //GRID(41, 18) || GRID(41, 19) || GRID(41, 20) || GRID(41, 21) || GRID(41, 22) || GRID(41, 23) || GRID(41, 24) || GRID(41, 25) || GRID(41, 26) ||
        //GRID(42, 18) || GRID(42, 19) || GRID(42, 20) || GRID(42, 21) || GRID(42, 22) || GRID(42, 23) || GRID(42, 24) || GRID(42, 25) || GRID(42, 26) ||
        //GRID(43, 18) || GRID(43, 19) || GRID(43, 20) || GRID(43, 21) || GRID(43, 22) || GRID(43, 23) || GRID(43, 24) || GRID(43, 25) || GRID(43, 26) ||
        //GRID(44, 18) || GRID(44, 19) || GRID(44, 20) || GRID(44, 21) || GRID(44, 22) || GRID(44, 23) || GRID(44, 24) || GRID(44, 25) || GRID(44, 26) ||
        //GRID(45, 18) || GRID(45, 19) || GRID(45, 20) || GRID(45, 21) || GRID(45, 22) || GRID(45, 23) || GRID(45, 24) || GRID(45, 25) || GRID(45, 26) ||
        //GRID(46, 18) || GRID(46, 19) || GRID(46, 20) || GRID(46, 21) || GRID(46, 22) || GRID(46, 23) || GRID(46, 24) || GRID(46, 25) || GRID(46, 26)

        // Vashj'ir grids completely ignore fatigue
        return (x >= 39 && x <= 40 && y >= 24 && y <= 26) || (x >= 41 && x <= 46 && y >= 18 && y <= 26);
    }

    if (mapId == 1)
    {
        // GRID(43, 39) || GRID(43, 40)
        // Thousand Needles
        return x == 43 && (y == 39 || y == 40);
    }

    return false;
}

void ExtractMapsFromMpq(uint32 build)
{
    std::string mpqFileName;
    std::string mpqMapName;
    std::string outputFileName;

    printf("Extracting maps...\n");

    ReadMapDBC();
    ReadLiquidMaterialTable();
    ReadLiquidObjectTable();
    ReadLiquidTypeTable();

    CreateDir(output_path / "maps");

    printf("Convert map files\n");
    for (std::size_t z = 0; z < map_ids.size(); ++z)
    {
        printf("Extract %s (" SZFMTD "/" SZFMTD ")                  \n", map_ids[z].name, z + 1, map_ids.size());
        mpqMapName = Trinity::StringFormat("World\\Maps\\%s\\%s.wdt", map_ids[z].name, map_ids[z].name);
        ChunkedFile wdt;
        std::bitset<(WDT_MAP_SIZE)* (WDT_MAP_SIZE)> existingTiles;
        if (wdt.loadFile(WorldMpq, mpqMapName, false))
        {
            FileChunk* main = wdt.GetChunk("MAIN");
            for (uint32 y = 0; y < WDT_MAP_SIZE; ++y)
            {
                for (uint32 x = 0; x < WDT_MAP_SIZE; ++x)
                {
                    if (!(main->As<wdt_MAIN>()->adt_list[y][x].flag & 0x1))
                        continue;

                    mpqFileName = Trinity::StringFormat("World\\Maps\\%s\\%s_%u_%u.adt", map_ids[z].name, map_ids[z].name, x, y);
                    outputFileName = Trinity::StringFormat("%s/maps/%03u%02u%02u.map", output_path.string(), map_ids[z].id, y, x);
                    bool ignoreDeepWater = IsDeepWaterIgnored(map_ids[z].id, y, x);
                    existingTiles[y * WDT_MAP_SIZE + x] = ConvertADT(mpqFileName, outputFileName, y, x, build, ignoreDeepWater);
                }

                // draw progress bar
                if (PrintProgress)
                    printf("Processing........................%d%%\r", (100 * (y + 1)) / WDT_MAP_SIZE);
            }
        }

        if (FILE* tileList = fopen(Trinity::StringFormat("%s/maps/%03u.tilelist", output_path.string(), map_ids[z].id).c_str(), "wb"))
        {
            fwrite(MapMagic.data(), 1, MapMagic.size(), tileList);
            fwrite(&MapVersionMagic, 1, sizeof(MapVersionMagic), tileList);
            fwrite(&build, sizeof(build), 1, tileList);
            fwrite(existingTiles.to_string().c_str(), 1, existingTiles.size(), tileList);
            fclose(tileList);
        }
    }

    printf("\n");
}

bool ExtractFile(HANDLE fileInArchive, std::string const& fileName)
{
    FILE* output = fopen(fileName.c_str(), "wb");
    if (!output)
    {
        printf("Can't create the output file '%s'\n", fileName.c_str());
        return false;
    }

    char  buffer[0x10000];
    DWORD readBytes = 1;

    while (readBytes > 0)
    {
        SFileReadFile(fileInArchive, buffer, sizeof(buffer), &readBytes, nullptr);
        if (readBytes > 0)
            fwrite(buffer, 1, readBytes, output);
    }

    fclose(output);
    return true;
}

void ExtractDBCFiles(int locale)
{
    printf("Extracting dbc files...\n");

    SFILE_FIND_DATA foundFile;
    memset(&foundFile, 0, sizeof(foundFile));
    HANDLE listFile = SFileFindFirstFile(LocaleMpq, "DBFilesClient\\*dbc", &foundFile, nullptr);
    HANDLE dbcFile = nullptr;
    uint32 count = 0;
    if (listFile)
    {
        boost::filesystem::path localePath = output_path / "dbc" / localeNames[MpqToWowLocale[locale]];

        CreateDir(output_path / "dbc");
        CreateDir(localePath);

        printf("locale %s output path %s\n", localeNames[MpqToWowLocale[locale]], localePath.string().c_str());

        std::string fileName;
        do
        {
            if (!SFileOpenFileEx(LocaleMpq, foundFile.cFileName, SFILE_OPEN_PATCHED_FILE, &dbcFile))
            {
                printf("Unable to open file %s in the archive\n", foundFile.cFileName);
                continue;
            }

            fileName = foundFile.cFileName;
            fileName = fileName.substr(fileName.rfind('\\') + 1);

            boost::filesystem::path filePath = localePath / boost::filesystem::path(fileName).filename();

            if (!boost::filesystem::exists(filePath))
                if (ExtractFile(dbcFile, filePath.string()))
                    ++count;

            SFileCloseFile(dbcFile);
        } while (SFileFindNextFile(listFile, &foundFile));

        SFileFindClose(listFile);
    }

    printf("Extracted %u DBC files\n\n", count);
}

void ExtractDB2Files(int locale)
{
    printf("Extracting db2 files...\n");

    SFILE_FIND_DATA foundFile;
    memset(&foundFile, 0, sizeof(foundFile));
    HANDLE listFile = SFileFindFirstFile(LocaleMpq, "DBFilesClient\\*db2", &foundFile, nullptr);
    HANDLE dbcFile = nullptr;
    uint32 count = 0;
    if (listFile)
    {
        boost::filesystem::path localePath = output_path / "dbc" / localeNames[MpqToWowLocale[locale]];

        printf("locale %s output path %s\n", localeNames[MpqToWowLocale[locale]], localePath.string().c_str());

        std::string fileName;
        do
        {
            if (!SFileOpenFileEx(LocaleMpq, foundFile.cFileName, SFILE_OPEN_PATCHED_FILE, &dbcFile))
            {
                printf("Unable to open file %s in the archive\n", foundFile.cFileName);
                continue;
            }

            fileName = foundFile.cFileName;
            fileName = fileName.substr(fileName.rfind('\\') + 1);

            boost::filesystem::path filePath = localePath / boost::filesystem::path(fileName).filename();

            if (!boost::filesystem::exists(filePath))
                if (ExtractFile(dbcFile, filePath.string()))
                ++count;

            SFileCloseFile(dbcFile);
        } while (SFileFindNextFile(listFile, &foundFile));

        SFileFindClose(listFile);
    }

    printf("Extracted %u DB2 files\n\n", count);
}

void ExtractCameraFiles()
{
    printf("Extracting camera files...\n");

    HANDLE dbcFile;
    if (!SFileOpenFileEx(LocaleMpq, "DBFilesClient\\CinematicCamera.dbc", SFILE_OPEN_PATCHED_FILE, &dbcFile))
    {
        printf("Fatal error: Cannot find Map.dbc in archive!\n");
        exit(1);
    }

    DBCFile camdbc(dbcFile);

    if (!camdbc.open())
    {
        printf("Unable to open CinematicCamera.dbc. Camera extract aborted.\n");
        return;
    }


    // get camera file list from DBC
    std::vector<std::string> camerafiles;
    size_t cam_count = camdbc.getRecordCount();

    for (size_t i = 0; i < cam_count; ++i)
    {
        std::string camFile(camdbc.getRecord(i).getString(1));
        size_t loc = camFile.find(".mdx");
        if (loc != std::string::npos)
            camFile.replace(loc, 4, ".m2");
        camerafiles.push_back(std::string(camFile));
    }
    SFileCloseFile(dbcFile);

    boost::filesystem::path outputPath = output_path / "Cameras";

    CreateDir(outputPath);

    printf("output path %s\n", outputPath.string().c_str());

    // extract M2s
    uint32 count = 0;
    for (std::string const& thisFile : camerafiles)
    {
        boost::filesystem::path filePath = outputPath / (thisFile.c_str() + strlen("Cameras\\"));

        if (boost::filesystem::exists(filePath))
            continue;

        HANDLE dbcFile = nullptr;

        if (!SFileOpenFileEx(WorldMpq, thisFile.c_str(), SFILE_OPEN_PATCHED_FILE, &dbcFile))
        {
            printf("Unable to open file %s in the archive\n", thisFile.c_str());
            continue;
        }

        if (ExtractFile(dbcFile, filePath.string()))
            ++count;

        SFileCloseFile(dbcFile);
    }
    printf("Extracted %u camera files\n", count);
}

bool LoadLocaleMPQFile(int locale)
{
    std::string fileName = Trinity::StringFormat("%s/Data/%s/locale-%s.MPQ", input_path.string(), Locales[locale], Locales[locale]);

    if (!SFileOpenArchive(fileName.c_str(), 0, MPQ_OPEN_READ_ONLY, &LocaleMpq))
    {
        if (GetLastError() != ERROR_PATH_NOT_FOUND)
        {
            printf("\nLoading %s locale MPQs\n", Locales[locale]);
            printf("Cannot open archive %s\n", fileName.c_str());
        }
        return false;
    }

    printf("\nLoading %s locale MPQs\n", Locales[locale]);
    char const* prefix = nullptr;
    for (int i = 0; Builds[i] && Builds[i] <= CLIENT_BUILD; ++i)
    {
        // Do not attempt to read older MPQ patch archives past this build, they were merged with base
        // and trying to read them together with new base will not end well
        if (CLIENT_BUILD >= NEW_BASE_SET_BUILD && Builds[i] < NEW_BASE_SET_BUILD)
            continue;

        fileName.clear();

        if (Builds[i] > LAST_DBC_IN_DATA_BUILD)
        {
            prefix = "";
            fileName = Trinity::StringFormat("%s/Data/%s/wow-update-%s-%u.MPQ", input_path.string(), Locales[locale], Locales[locale], Builds[i]);
        }
        else
        {
            prefix = Locales[locale];
            fileName = Trinity::StringFormat("%s/Data/wow-update-%u.MPQ", input_path.string(), Builds[i]);
        }

        if (!SFileOpenPatchArchive(LocaleMpq, fileName.c_str(), prefix, 0))
        {
            if (GetLastError() != ERROR_FILE_NOT_FOUND)
                printf("Cannot open patch archive %s\n", fileName.c_str());
            continue;
        }
        else
            printf("Loaded %s\n", fileName.c_str());
    }

    printf("\n");
    return true;
}

void LoadCommonMPQFiles(uint32 build)
{
    std::string filename = Trinity::StringFormat("%s/Data/world.MPQ", input_path.string());
    printf("Loading common MPQ files\n");
    if (!SFileOpenArchive(filename.c_str(), 0, MPQ_OPEN_READ_ONLY, &WorldMpq))
    {
        if (GetLastError() != ERROR_PATH_NOT_FOUND)
            printf("Cannot open archive %s\n", filename.c_str());
        return;
    }

    int count = sizeof(CONF_mpq_list) / sizeof(char*);
    for (int i = 1; i < count; ++i)
    {
        if (build < NEW_BASE_SET_BUILD && !strcmp("world2.MPQ", CONF_mpq_list[i]))   // 4.3.2 and higher MPQ
            continue;

        filename = Trinity::StringFormat("%s/Data/%s", input_path.string(), CONF_mpq_list[i]);
        if (!SFileOpenPatchArchive(WorldMpq, filename.c_str(), "", 0))
        {
            if (GetLastError() != ERROR_PATH_NOT_FOUND)
                printf("Cannot open archive %s\n", filename.c_str());
            else
                printf("Not found %s\n", filename.c_str());
        }
        else
            printf("Loaded %s\n", filename.c_str());

    }

    char const* prefix = nullptr;
    for (int i = 0; Builds[i] && Builds[i] <= CLIENT_BUILD; ++i)
    {
        // Do not attempt to read older MPQ patch archives past this build, they were merged with base
        // and trying to read them together with new base will not end well
        if (CLIENT_BUILD >= NEW_BASE_SET_BUILD && Builds[i] < NEW_BASE_SET_BUILD)
            continue;

        filename.clear();
        if (Builds[i] > LAST_DBC_IN_DATA_BUILD)
        {
            prefix = "";
            filename = Trinity::StringFormat("%s/Data/wow-update-base-%u.MPQ", input_path.string(), Builds[i]);
        }
        else
        {
            prefix = "base";
            filename = Trinity::StringFormat("%s/Data/wow-update-%u.MPQ", input_path.string(), Builds[i]);
        }

        if (!SFileOpenPatchArchive(WorldMpq, filename.c_str(), prefix, 0))
        {
            if (GetLastError() != ERROR_PATH_NOT_FOUND)
                printf("Cannot open patch archive %s\n", filename.c_str());
            else
                printf("Not found %s\n", filename.c_str());
            continue;
        }
        else
            printf("Loaded %s\n", filename.c_str());
    }

    printf("\n");
}

int main(int argc, char * arg[])
{
    Trinity::Banner::Show("Map & DBC Extractor", [](char const* text) { printf("%s\n", text); }, nullptr);

    PrintProgress = isatty(fileno(stdout));
    input_path = boost::filesystem::current_path();
    output_path = boost::filesystem::current_path();

    HandleArgs(argc, arg);

    int32 firstInstalledLocale = -1;
    uint32 build = 0;

    for (int i = 0; i < LOCALES_COUNT; ++i)
    {
        std::string filename = Trinity::StringFormat("%s/Data/%s/locale-%s.MPQ", input_path.string(), Locales[i], Locales[i]);
        if (boost::filesystem::exists(filename))
        {
            printf("Detected locale: %s\n", Locales[i]);

            //Open MPQs
            if (!LoadLocaleMPQFile(i))
            {
                if (GetLastError() != ERROR_PATH_NOT_FOUND)
                    printf("Unable to load %s locale archives!\n", Locales[i]);
                continue;
            }

            if ((CONF_extract & EXTRACT_DBC) == 0)
            {
                firstInstalledLocale = i;
                build = ReadBuild(i);
                if (build > CLIENT_BUILD)
                {
                    printf("Base locale-%s.MPQ has build higher than target build (%u > %u), nothing extracted!\n", Locales[i], build, uint32(CLIENT_BUILD));
                    return 0;
                }

                printf("Detected client build: %u\n", build);
                break;
            }

            if (firstInstalledLocale < 0)
            {
                firstInstalledLocale = i;
                build = ReadBuild(i);
                if (build > CLIENT_BUILD)
                {
                    printf("Base locale-%s.MPQ has build higher than target build (%u > %u), nothing extracted!\n", Locales[i], build, uint32(CLIENT_BUILD));
                    return 0;
                }
                printf("Detected client build: %u\n", build);
            }

            ExtractDBCFiles(i);
            ExtractDB2Files(i);

            //Close MPQs
            SFileCloseArchive(LocaleMpq);
        }

    }

    if (firstInstalledLocale < 0)
    {
        printf("No locales detected\n");
        return 0;
    }

    if (CONF_extract & EXTRACT_CAMERA)
    {
        printf("Using locale: %s\n", Locales[firstInstalledLocale]);

        // Open MPQs
        LoadLocaleMPQFile(firstInstalledLocale);
        LoadCommonMPQFiles(build);

        // Extract cameras
        ExtractCameraFiles();

        // Close MPQs
        SFileCloseArchive(WorldMpq);
        SFileCloseArchive(LocaleMpq);
    }

    if (CONF_extract & EXTRACT_MAP)
    {
        printf("Using locale: %s\n", Locales[firstInstalledLocale]);

        // Open MPQs
        LoadLocaleMPQFile(firstInstalledLocale);
        LoadCommonMPQFiles(build);

        // Extract maps
        ExtractMapsFromMpq(build);

        // Close MPQs
        SFileCloseArchive(WorldMpq);
        SFileCloseArchive(LocaleMpq);
    }

    return 0;
}
