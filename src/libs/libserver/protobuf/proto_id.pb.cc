// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: proto_id.proto

#include "proto_id.pb.h"

#include <algorithm>

#include <google/protobuf/stubs/common.h>
#include <google/protobuf/io/coded_stream.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/wire_format_lite.h>
#include <google/protobuf/descriptor.h>
#include <google/protobuf/generated_message_reflection.h>
#include <google/protobuf/reflection_ops.h>
#include <google/protobuf/wire_format.h>
// @@protoc_insertion_point(includes)
#include <google/protobuf/port_def.inc>
namespace Proto {
}  // namespace Proto
static constexpr ::PROTOBUF_NAMESPACE_ID::Metadata* file_level_metadata_proto_5fid_2eproto = nullptr;
static const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* file_level_enum_descriptors_proto_5fid_2eproto[1];
static constexpr ::PROTOBUF_NAMESPACE_ID::ServiceDescriptor const** file_level_service_descriptors_proto_5fid_2eproto = nullptr;
const ::PROTOBUF_NAMESPACE_ID::uint32 TableStruct_proto_5fid_2eproto::offsets[1] = {};
static constexpr ::PROTOBUF_NAMESPACE_ID::internal::MigrationSchema* schemas = nullptr;
static constexpr ::PROTOBUF_NAMESPACE_ID::Message* const* file_default_instances = nullptr;

const char descriptor_table_protodef_proto_5fid_2eproto[] PROTOBUF_SECTION_VARIABLE(protodesc_cold) =
  "\n\016proto_id.proto\022\005Proto*\254\003\n\005MsgId\022\010\n\004Non"
  "e\020\000\022\025\n\021MI_NetworkConnect\020\001\022\024\n\020MI_Network"
  "Listen\020\002\022\030\n\024MI_NetworkDisconnect\020\003\022\032\n\026MI"
  "_NetworkDisconnectEx\020\004\022\035\n\031MI_NetworkDisc"
  "onnectToNet\020\005\022\026\n\022MI_RemoveComponent\0202\022\026\n"
  "\022MI_CreateComponent\0203\022\013\n\007MI_Ping\020e\022\025\n\020C2"
  "L_AccountCheck\020\351\007\022\027\n\022C2L_AccountCheckRs\020"
  "\352\007\022!\n\034MI_AccountQueryOnlineToRedis\020\353\007\022#\n"
  "\036MI_AccountQueryOnlineToRedisRs\020\354\007\022\034\n\027MI"
  "_AccountCheckToHttpRs\020\355\007\022\026\n\021MI_RobotSync"
  "State\020\211\'\022\026\n\021MI_RobotTestBegin\020\354\'\022\024\n\017MI_R"
  "obotTestEnd\020\355\'b\006proto3"
  ;
static const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable*const descriptor_table_proto_5fid_2eproto_deps[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::SCCInfoBase*const descriptor_table_proto_5fid_2eproto_sccs[1] = {
};
static ::PROTOBUF_NAMESPACE_ID::internal::once_flag descriptor_table_proto_5fid_2eproto_once;
static bool descriptor_table_proto_5fid_2eproto_initialized = false;
const ::PROTOBUF_NAMESPACE_ID::internal::DescriptorTable descriptor_table_proto_5fid_2eproto = {
  &descriptor_table_proto_5fid_2eproto_initialized, descriptor_table_protodef_proto_5fid_2eproto, "proto_id.proto", 462,
  &descriptor_table_proto_5fid_2eproto_once, descriptor_table_proto_5fid_2eproto_sccs, descriptor_table_proto_5fid_2eproto_deps, 0, 0,
  schemas, file_default_instances, TableStruct_proto_5fid_2eproto::offsets,
  file_level_metadata_proto_5fid_2eproto, 0, file_level_enum_descriptors_proto_5fid_2eproto, file_level_service_descriptors_proto_5fid_2eproto,
};

// Force running AddDescriptors() at dynamic initialization time.
static bool dynamic_init_dummy_proto_5fid_2eproto = (  ::PROTOBUF_NAMESPACE_ID::internal::AddDescriptors(&descriptor_table_proto_5fid_2eproto), true);
namespace Proto {
const ::PROTOBUF_NAMESPACE_ID::EnumDescriptor* MsgId_descriptor() {
  ::PROTOBUF_NAMESPACE_ID::internal::AssignDescriptors(&descriptor_table_proto_5fid_2eproto);
  return file_level_enum_descriptors_proto_5fid_2eproto[0];
}
bool MsgId_IsValid(int value) {
  switch (value) {
    case 0:
    case 1:
    case 2:
    case 3:
    case 4:
    case 5:
    case 50:
    case 51:
    case 101:
    case 1001:
    case 1002:
    case 1003:
    case 1004:
    case 1005:
    case 5001:
    case 5100:
    case 5101:
      return true;
    default:
      return false;
  }
}


// @@protoc_insertion_point(namespace_scope)
}  // namespace Proto
PROTOBUF_NAMESPACE_OPEN
PROTOBUF_NAMESPACE_CLOSE

// @@protoc_insertion_point(global_scope)
#include <google/protobuf/port_undef.inc>
