// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from lslidar_msgs:msg/LslidarPacket.idl
// generated code does not contain a copyright notice

#include "lslidar_msgs/msg/detail/lslidar_packet__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_lslidar_msgs
const rosidl_type_hash_t *
lslidar_msgs__msg__LslidarPacket__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x94, 0xdf, 0xde, 0x1f, 0x06, 0x41, 0x33, 0xcf,
      0xa9, 0xb1, 0x52, 0x73, 0x56, 0x7c, 0xfb, 0x68,
      0xd5, 0x1c, 0xcd, 0x06, 0x9e, 0x31, 0x9e, 0x8a,
      0xfb, 0xdc, 0xe1, 0x6b, 0xd8, 0x00, 0x34, 0x71,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "builtin_interfaces/msg/detail/time__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t builtin_interfaces__msg__Time__EXPECTED_HASH = {1, {
    0xb1, 0x06, 0x23, 0x5e, 0x25, 0xa4, 0xc5, 0xed,
    0x35, 0x09, 0x8a, 0xa0, 0xa6, 0x1a, 0x3e, 0xe9,
    0xc9, 0xb1, 0x8d, 0x19, 0x7f, 0x39, 0x8b, 0x0e,
    0x42, 0x06, 0xce, 0xa9, 0xac, 0xf9, 0xc1, 0x97,
  }};
#endif

static char lslidar_msgs__msg__LslidarPacket__TYPE_NAME[] = "lslidar_msgs/msg/LslidarPacket";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";

// Define type names, field names, and default values
static char lslidar_msgs__msg__LslidarPacket__FIELD_NAME__stamp[] = "stamp";
static char lslidar_msgs__msg__LslidarPacket__FIELD_NAME__data[] = "data";

static rosidl_runtime_c__type_description__Field lslidar_msgs__msg__LslidarPacket__FIELDS[] = {
  {
    {lslidar_msgs__msg__LslidarPacket__FIELD_NAME__stamp, 5, 5},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    },
    {NULL, 0, 0},
  },
  {
    {lslidar_msgs__msg__LslidarPacket__FIELD_NAME__data, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_UINT8_ARRAY,
      2000,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription lslidar_msgs__msg__LslidarPacket__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
lslidar_msgs__msg__LslidarPacket__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {lslidar_msgs__msg__LslidarPacket__TYPE_NAME, 30, 30},
      {lslidar_msgs__msg__LslidarPacket__FIELDS, 2, 2},
    },
    {lslidar_msgs__msg__LslidarPacket__REFERENCED_TYPE_DESCRIPTIONS, 1, 1},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "# Raw Leishen LIDAR packet.\n"
  "\n"
  "builtin_interfaces/Time stamp              # packet timestamp\n"
  "uint8[2000] data        # packet contents\n"
  "";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
lslidar_msgs__msg__LslidarPacket__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {lslidar_msgs__msg__LslidarPacket__TYPE_NAME, 30, 30},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 134, 134},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
lslidar_msgs__msg__LslidarPacket__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[2];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 2, 2};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *lslidar_msgs__msg__LslidarPacket__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
