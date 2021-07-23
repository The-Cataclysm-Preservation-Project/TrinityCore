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

#ifndef WARDENCHECK_H_
#define WARDENCHECK_H_

#include "WardenFwd.h"
#include "Field.h"
#include "ByteBuffer.h"

class Warden;

struct WardenCheck
{
    enum class Type : uint32
    {
        Memory,                      // { byte ModuleNameIndex; uint32 Address; uint8 Length; }
        Module,                      // { byte Seed[4]; SHA1Digest Hash; }
        PageCheckA,                  // { byte Seed[4]; SHA1Digest Hash; uint32 Address; uint8 Length; }
        PageCheckB,                  // { byte Seed[4]; SHA1Digest Hash; uint32 Address; uint8 Length; }
        Driver,                      // { byte Seed[4]; SHA1Digest Hash; uint8 DriverNameIndex; }
        LuaString,                   // { byte LuaNameIndex; }
        LuaStringAdvanced,           // { byte CodeToExecuteIndex; byte LuaNameIndex; }
        MPQ,                         // { byte FileNameIndex; }
        Proc,                        // { byte Seed[4]; SHA1Digest Hash; byte ModuleNameIndex; byte ProcNameIndex; uint32 Address; uint8 Length; }
        Timing,                      // { }
        WardenInterop,               // { }

        MAX,                         // Limit checks loaded from DB
        Scripted
    };

    //! The order of member declaration in this enum must match the order or columns retrieved by WardenMgr::LoadWardenChecks.
    enum class DatabaseColumn : uint32
    {
        ID,
        Type,
        Flags,
        Data0,
        Data1,
        Address,
        Length,
        Result,
        Comment
    };

    // Defines which type corresponds to which column
    template <DatabaseColumn Column> struct DatabaseColumnType { };
    template <> struct DatabaseColumnType<DatabaseColumn::ID>       { using type = uint32; };
    template <> struct DatabaseColumnType<DatabaseColumn::Type>     { using type = uint8; };
    template <> struct DatabaseColumnType<DatabaseColumn::Flags>    { using type = uint32; };
    template <> struct DatabaseColumnType<DatabaseColumn::Data0>    { using type = std::string; };
    template <> struct DatabaseColumnType<DatabaseColumn::Data1>    { using type = std::string; };
    template <> struct DatabaseColumnType<DatabaseColumn::Address>  { using type = uint32; };
    template <> struct DatabaseColumnType<DatabaseColumn::Result>   { using type = std::string; };
    template <> struct DatabaseColumnType<DatabaseColumn::Length>   { using type = uint8; };
    template <> struct DatabaseColumnType<DatabaseColumn::Comment>  { using type = std::string; };

protected:
    WardenCheck(Type scanType, Field* fields);
    WardenCheck(Type scanType);
    virtual ~WardenCheck() { }

public:
    virtual bool TryWriteRequest(Warden* warden, WardenCheatChecksRequest& request, ByteBuffer& requestBuffer) = 0;
    virtual WardenCheckResult ProcessResponse(Warden* warden, ByteBuffer& packet) const = 0;

    /// <summary>
    /// Performs logic checks to validate that this check can be sent as a request to the client.
    /// </summary>
    /// <param name="session">The session to which we try to send this check.</param>
    /// <param name="warden">The session's associated warden.</param>
    /// <returns>true when able; false otherwise.</returns>
    virtual bool TrySelect(WorldSession* session, Warden* warden);

    uint32 GetID() const { return _id; }
    std::string const& GetComment() const { return _comment; }
    WardenCheckFlags GetFlags() const { return _flags; }
    Type GetCheckType() const { return _scanType; }

protected:
    void WritePackedValue(ByteBuffer& buffer, uint64 value) const;
    uint64 ReadPackedValue(ByteBuffer& buffer) const;

    bool TransformResultCode(bool checkFailed) const;

    virtual WardenCheckResult HandleResponse(bool checkFailed) const;

private:
    uint32 _id;
    Type _scanType;
    WardenCheckFlags _flags;
    std::string _comment;
};

// Helpers used to avoid copy-pasting hardcoded offsets when loading various checks from database.
template <typename T> struct field_value_extractor { };

#define MAKE_QUERY_FIELD_EXTRACTOR(TYPE, FN)         \
    template <> struct field_value_extractor<TYPE> { \
        static TYPE read(Field& field) {             \
            return field.FN();                       \
        }                                            \
    }

MAKE_QUERY_FIELD_EXTRACTOR(uint64, GetUInt64);
MAKE_QUERY_FIELD_EXTRACTOR(uint32, GetUInt32);
MAKE_QUERY_FIELD_EXTRACTOR(uint16, GetUInt16);
MAKE_QUERY_FIELD_EXTRACTOR(uint8, GetUInt8);

MAKE_QUERY_FIELD_EXTRACTOR(int64, GetInt64);
MAKE_QUERY_FIELD_EXTRACTOR(int32, GetInt32);
MAKE_QUERY_FIELD_EXTRACTOR(int16, GetInt16);
MAKE_QUERY_FIELD_EXTRACTOR(int8, GetInt8);

MAKE_QUERY_FIELD_EXTRACTOR(float, GetFloat);
MAKE_QUERY_FIELD_EXTRACTOR(double, GetDouble);

MAKE_QUERY_FIELD_EXTRACTOR(std::string, GetString);
MAKE_QUERY_FIELD_EXTRACTOR(const char*, GetCString);

#undef MAKE_QUERY_FIELD_EXTRACTOR

template <WardenCheck::DatabaseColumn Column>
static auto ReadDatabaseField(Field* fields) -> typename WardenCheck::DatabaseColumnType<Column>::type
{
    using ValueType = typename WardenCheck::DatabaseColumnType<Column>::type;

    return field_value_extractor<ValueType>::read(fields[static_cast<std::underlying_type_t<WardenCheck::DatabaseColumn>>(Column)]);
}


#endif // WARDENCHECK_H_
