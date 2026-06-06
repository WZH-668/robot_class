// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from lslidar_msgs:msg/LslidarScan.idl
// generated code does not contain a copyright notice

#include "lslidar_msgs/msg/detail/lslidar_scan__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_lslidar_msgs
const rosidl_type_hash_t *
lslidar_msgs__msg__LslidarScan__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x2e, 0xcb, 0xb9, 0x15, 0x43, 0x3b, 0xc8, 0xe3,
      0x9b, 0x38, 0xff, 0x57, 0x51, 0xf6, 0x46, 0xdb,
      0x8e, 0xd5, 0x18, 0xb0, 0x7b, 0x15, 0x55, 0x2e,
      0x30, 0x51, 0xe0, 0x2c, 0xc0, 0x26, 0x96, 0xdd,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "lslidar_msgs/msg/detail/lslidar_point__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t lslidar_msgs__msg__LslidarPoint__EXPECTED_HASH = {1, {
    0xb8, 0xf1, 0x25, 0x11, 0xca, 0x75, 0x6f, 0x45,
    0xc4, 0xdb, 0xa5, 0xca, 0x5e, 0xb3, 0x2a, 0xf7,
    0x50, 0x4e, 0x11, 0xee, 0x8c, 0x8a, 0x9d, 0x15,
    0xbd, 0x0e, 0x2e, 0x88, 0x63, 0x79, 0xab, 0x04,
  }};
#endif

static char lslidar_msgs__msg__LslidarScan__TYPE_NAME[] = "lslidar_msgs/msg/LslidarScan";
static char lslidar_msgs__msg__LslidarPoint__TYPE_NAME[] = "lslidar_msgs/msg/LslidarPoint";

// Define type names, field names, and default values
static char lslidar_msgs__msg__LslidarScan__FIELD_NAME__altitude[] = "altitude";
static char lslidar_msgs__msg__LslidarScan__FIELD_NAME__points[] = "points";

static rosidl_runtime_c__type_description__Field lslidar_msgs__msg__LslidarScan__FIELDS[] = {
  {
    {lslidar_msgs__msg__LslidarScan__FIELD_NAME__altitude, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {lslidar_msgs__msg__LslidarScan__FIELD_NAME__points, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_UNBOUNDED_SEQUENCE,
      0,
      0,
      {lslidar_msgs__msg__LslidarPoint__TYPE_NAME, 29, 29},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription lslidar_msgs__msg__LslidarScan__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {lslidar_msgs__msg__LslidarPoint__TYPE_NAME, 29, 29},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
lslidar_msgs__msg__LslidarScan__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {lslidar_msgs__msg__LslidarScan__TYPE_NAME, 28, 28},
      {lslidar_msgs__msg__LslidarScan__FIELDS, 2, 2},
    },
    {lslidar_msgs__msg__LslidarScan__REFERENCED_TYPE_DESCRIPTIONS, 1, 1},
  };
  if (!constructed) {
    assert(0 == memcmp(&lslidar_msgs__msg__LslidarPoint__EXPECTED_HASH, lslidar_msgs__msg__LslidarPoint__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = lslidar_msgs__msg__LslidarPoint__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "# Altitude of all the points within this scan\n"
  "float64 altitude\n"
  "\n"
  "# The valid points in this scan sorted by azimuth\n"
  "# from 0 to 359.99\n"
  "LslidarPoint[] points";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
lslidar_msgs__msg__LslidarScan__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {lslidar_msgs__msg__LslidarScan__TYPE_NAME, 28, 28},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 155, 155},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
lslidar_msgs__msg__LslidarScan__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[2];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 2, 2};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *lslidar_msgs__msg__LslidarScan__get_individual_type_description_source(NULL),
    sources[1] = *lslidar_msgs__msg__LslidarPoint__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
