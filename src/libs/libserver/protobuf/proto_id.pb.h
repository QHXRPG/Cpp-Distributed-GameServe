// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: proto_id.proto

#ifndef GOOGLE_PROTOBUF_INCLUDED_proto_5fid_2eproto
#define GOOGLE_PROTOBUF_INCLUDED_proto_5fid_2eproto

#include <limits>
#include <string>

#include <google/protobuf/port_def.inc>
#if PROTOBUF_VERSION < 3009000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers. Please update
#error your headers.
#endif
#if 3009001 < PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers. Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/port_undef.inc>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/arena.h>
#include <google/protobuf/arenastring.h>
#include <google/protobuf/generated_message_table_driven.h>
#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/inlined_string_field.h>
#include <google/protobuf/metadata.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/repeated_field.h>  // IWYU pragma: export
#include <google/protobuf/extension_set.h>  // IWYU pragma: export
#include <google/protobuf/generated_enum_reflection.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
#define PROTOBUF_INTERNAL_EXPORT_proto_5fid_2eproto
PROTOBUF_NAMESPACE_OPEN
namespace internal {
class AnyMetadata;
}  // namespace internal
PROTOBUF_NAMESPACE_CLOSE

// Internal implementation detail -- do not use these members.
struct TableStruct_proto_5fid_2eproto {
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTableField entries[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::AuxillaryParseTableField aux[]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::ParseTable schema[1]
    PROTOBUF_SECTION_VARIABLE(protodesc_cold);
  static const ::PROTOBUF_NAMESPACE_ID::internal::FieldMetadata field_metadata[];
  static const ::PROTOBUF_NAMESPACE_ID::internal::SerializationTable serialization_table[];
  static const ::PROTOBUF_NAMESPACE_ID::uint32 offsets[];
};
extern const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_proto_5fid_2eproto;
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE
namespace Proto {

enum MsgId : int {
  None = 0,
  MI_NetworkConnect = 1,
  MI_NetworkListen = 2,
  MI_NetworkDisconnect = 3,
  MI_NetworkDisconnectEx = 4,
  MI_NetworkRequestDisconnect = 5,
  MI_RemoveComponent = 50,
  MI_CreateComponent = 51,
  MI_Ping = 101,
  C2L_AccountCheck = 1001,
  C2L_AccountCheckRs = 1002,
  MI_AccountQueryOnlineToRedis = 1003,
  MI_AccountQueryOnlineToRedisRs = 1004,
  MI_AccountCheckToHttpRs = 1005,
  L2DB_QueryPlayerList = 1010,
  L2DB_QueryPlayerListRs = 1011,
  G2DB_QueryPlayer = 1101,
  G2DB_QueryPlayerRs = 1102,
  L2C_PlayerList = 1103,
  C2L_CreatePlayer = 1105,
  C2L_CreatePlayerRs = 1106,
  L2DB_CreatePlayer = 1107,
  L2DB_CreatePlayerRs = 1108,
  G2DB_SavePlayer = 1800,
  MI_RobotSyncState = 5001,
  MI_RobotTestBegin = 5100,
  MI_RobotTestEnd = 5101,
  MsgId_INT_MIN_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::min(),
  MsgId_INT_MAX_SENTINEL_DO_NOT_USE_ = std::numeric_limits<::PROTOBUF_NAMESPACE_ID::int32>::max()
};
bool MsgId_IsValid(int value);
constexpr MsgId MsgId_MIN = None;
constexpr MsgId MsgId_MAX = MI_RobotTestEnd;
constexpr int MsgId_ARRAYSIZE = MsgId_MAX + 1;

const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* MsgId_descriptor();
template<typename T>
inline const std::string& MsgId_Name(T enum_t_value) {
  static_assert(::std::is_same<T, MsgId>::value ||
    ::std::is_integral<T>::value,
    "Incorrect type passed to function MsgId_Name.");
  return ::PROTOBUF_NAMESPACE_ID::internal::NameOfEnum(
    MsgId_descriptor(), enum_t_value);
}
inline bool MsgId_Parse(
    const std::string& name, MsgId* value) {
  return ::PROTOBUF_NAMESPACE_ID::internal::ParseNamedEnum<MsgId>(
    MsgId_descriptor(), name, value);
}
// ===================================================================


// ===================================================================


// ===================================================================

#ifdef __GNUC__
  #pragma GCC diagnostic push
  #pragma GCC diagnostic ignored "-Wstrict-aliasing"
#endif  // __GNUC__
#ifdef __GNUC__
  #pragma GCC diagnostic pop
#endif  // __GNUC__

// @@protoc_insertion_point(namespace_scope)

}  // namespace Proto

PROTOBUF_NAMESPACE_OPEN

template <> struct is_proto_enum< ::Proto::MsgId> : ::std::true_type {};
template <>
inline const EnumDescriptor* GetEnumDescriptor< ::Proto::MsgId>() {
  return ::Proto::MsgId_descriptor();
}

PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)

#include <google/protobuf/port_undef.inc>
#endif  // GOOGLE_PROTOBUF_INCLUDED_GOOGLE_PROTOBUF_INCLUDED_proto_5fid_2eproto
