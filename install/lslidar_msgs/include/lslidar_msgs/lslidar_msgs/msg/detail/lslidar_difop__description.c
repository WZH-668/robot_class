// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from lslidar_msgs:msg/LslidarDifop.idl
// generated code does not contain a copyright notice

#include "lslidar_msgs/msg/detail/lslidar_difop__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_lslidar_msgs
const rosidl_type_hash_t *
lslidar_msgs__msg__LslidarDifop__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x39, 0x1a, 0xe8, 0xfe, 0x9b, 0x25, 0x66, 0xa8,
      0xf3, 0x25, 0xc6, 0xb2, 0x46, 0x65, 0xc6, 0x49,
      0x92, 0xee, 0x93, 0x45, 0xe4, 0x5c, 0xbf, 0x98,
      0x49, 0xef, 0x66, 0xa9, 0x62, 0x72, 0x50, 0x5b,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char lslidar_msgs__msg__LslidarDifop__TYPE_NAME[] = "lslidar_msgs/msg/LslidarDifop";

// Define type names, field names, and default values
static char lslidar_msgs__msg__LslidarDifop__FIELD_NAME__temperature[] = "temperature";
static char lslidar_msgs__msg__LslidarDifop__FIELD_NAME__rpm[] = "rpm";

static rosidl_runtime_c__type_description__Field lslidar_msgs__msg__LslidarDifop__FIELDS[] = {
  {
    {lslidar_msgs__msg__LslidarDifop__FIELD_NAME__temperature, 11, 11},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT64,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {lslidar_msgs__msg__LslidarDifop__FIELD_NAME__rpm, 3, 3},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_INT64,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
lslidar_msgs__msg__LslidarDifop__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {lslidar_msgs__msg__LslidarDifop__TYPE_NAME, 29, 29},
      {lslidar_msgs__msg__LslidarDifop__FIELDS, 2, 2},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "int64 temperature\n"
  "int64 rpm";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
lslidar_msgs__msg__LslidarDifop__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {lslidar_msgs__msg__LslidarDifop__TYPE_NAME, 29, 29},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 28, 28},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
lslidar_msgs__msg__LslidarDifop__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *lslidar_msgs__msg__LslidarDifop__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
