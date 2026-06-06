// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from lslidar_msgs:msg/LslidarSweep.idl
// generated code does not contain a copyright notice

#include "lslidar_msgs/msg/detail/lslidar_sweep__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_lslidar_msgs
const rosidl_type_hash_t *
lslidar_msgs__msg__LslidarSweep__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0x08, 0x39, 0x66, 0xef, 0xc1, 0xd7, 0xb2, 0x01,
      0x75, 0xd5, 0x1c, 0x26, 0xec, 0x31, 0x76, 0xd9,
      0x49, 0x70, 0x27, 0xd0, 0x16, 0x8c, 0xa3, 0x14,
      0x7c, 0xb8, 0x85, 0xef, 0xd0, 0x34, 0x89, 0xa4,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types
#include "lslidar_msgs/msg/detail/lslidar_scan__functions.h"
#include "builtin_interfaces/msg/detail/time__functions.h"
#include "std_msgs/msg/detail/header__functions.h"
#include "lslidar_msgs/msg/detail/lslidar_point__functions.h"

// Hashes for external referenced types
#ifndef NDEBUG
static const rosidl_type_hash_t builtin_interfaces__msg__Time__EXPECTED_HASH = {1, {
    0xb1, 0x06, 0x23, 0x5e, 0x25, 0xa4, 0xc5, 0xed,
    0x35, 0x09, 0x8a, 0xa0, 0xa6, 0x1a, 0x3e, 0xe9,
    0xc9, 0xb1, 0x8d, 0x19, 0x7f, 0x39, 0x8b, 0x0e,
    0x42, 0x06, 0xce, 0xa9, 0xac, 0xf9, 0xc1, 0x97,
  }};
static const rosidl_type_hash_t lslidar_msgs__msg__LslidarPoint__EXPECTED_HASH = {1, {
    0xb8, 0xf1, 0x25, 0x11, 0xca, 0x75, 0x6f, 0x45,
    0xc4, 0xdb, 0xa5, 0xca, 0x5e, 0xb3, 0x2a, 0xf7,
    0x50, 0x4e, 0x11, 0xee, 0x8c, 0x8a, 0x9d, 0x15,
    0xbd, 0x0e, 0x2e, 0x88, 0x63, 0x79, 0xab, 0x04,
  }};
static const rosidl_type_hash_t lslidar_msgs__msg__LslidarScan__EXPECTED_HASH = {1, {
    0x2e, 0xcb, 0xb9, 0x15, 0x43, 0x3b, 0xc8, 0xe3,
    0x9b, 0x38, 0xff, 0x57, 0x51, 0xf6, 0x46, 0xdb,
    0x8e, 0xd5, 0x18, 0xb0, 0x7b, 0x15, 0x55, 0x2e,
    0x30, 0x51, 0xe0, 0x2c, 0xc0, 0x26, 0x96, 0xdd,
  }};
static const rosidl_type_hash_t std_msgs__msg__Header__EXPECTED_HASH = {1, {
    0xf4, 0x9f, 0xb3, 0xae, 0x2c, 0xf0, 0x70, 0xf7,
    0x93, 0x64, 0x5f, 0xf7, 0x49, 0x68, 0x3a, 0xc6,
    0xb0, 0x62, 0x03, 0xe4, 0x1c, 0x89, 0x1e, 0x17,
    0x70, 0x1b, 0x1c, 0xb5, 0x97, 0xce, 0x6a, 0x01,
  }};
#endif

static char lslidar_msgs__msg__LslidarSweep__TYPE_NAME[] = "lslidar_msgs/msg/LslidarSweep";
static char builtin_interfaces__msg__Time__TYPE_NAME[] = "builtin_interfaces/msg/Time";
static char lslidar_msgs__msg__LslidarPoint__TYPE_NAME[] = "lslidar_msgs/msg/LslidarPoint";
static char lslidar_msgs__msg__LslidarScan__TYPE_NAME[] = "lslidar_msgs/msg/LslidarScan";
static char std_msgs__msg__Header__TYPE_NAME[] = "std_msgs/msg/Header";

// Define type names, field names, and default values
static char lslidar_msgs__msg__LslidarSweep__FIELD_NAME__header[] = "header";
static char lslidar_msgs__msg__LslidarSweep__FIELD_NAME__scans[] = "scans";

static rosidl_runtime_c__type_description__Field lslidar_msgs__msg__LslidarSweep__FIELDS[] = {
  {
    {lslidar_msgs__msg__LslidarSweep__FIELD_NAME__header, 6, 6},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE,
      0,
      0,
      {std_msgs__msg__Header__TYPE_NAME, 19, 19},
    },
    {NULL, 0, 0},
  },
  {
    {lslidar_msgs__msg__LslidarSweep__FIELD_NAME__scans, 5, 5},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_NESTED_TYPE_ARRAY,
      16,
      0,
      {lslidar_msgs__msg__LslidarScan__TYPE_NAME, 28, 28},
    },
    {NULL, 0, 0},
  },
};

static rosidl_runtime_c__type_description__IndividualTypeDescription lslidar_msgs__msg__LslidarSweep__REFERENCED_TYPE_DESCRIPTIONS[] = {
  {
    {builtin_interfaces__msg__Time__TYPE_NAME, 27, 27},
    {NULL, 0, 0},
  },
  {
    {lslidar_msgs__msg__LslidarPoint__TYPE_NAME, 29, 29},
    {NULL, 0, 0},
  },
  {
    {lslidar_msgs__msg__LslidarScan__TYPE_NAME, 28, 28},
    {NULL, 0, 0},
  },
  {
    {std_msgs__msg__Header__TYPE_NAME, 19, 19},
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
lslidar_msgs__msg__LslidarSweep__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {lslidar_msgs__msg__LslidarSweep__TYPE_NAME, 29, 29},
      {lslidar_msgs__msg__LslidarSweep__FIELDS, 2, 2},
    },
    {lslidar_msgs__msg__LslidarSweep__REFERENCED_TYPE_DESCRIPTIONS, 4, 4},
  };
  if (!constructed) {
    assert(0 == memcmp(&builtin_interfaces__msg__Time__EXPECTED_HASH, builtin_interfaces__msg__Time__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[0].fields = builtin_interfaces__msg__Time__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&lslidar_msgs__msg__LslidarPoint__EXPECTED_HASH, lslidar_msgs__msg__LslidarPoint__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[1].fields = lslidar_msgs__msg__LslidarPoint__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&lslidar_msgs__msg__LslidarScan__EXPECTED_HASH, lslidar_msgs__msg__LslidarScan__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[2].fields = lslidar_msgs__msg__LslidarScan__get_type_description(NULL)->type_description.fields;
    assert(0 == memcmp(&std_msgs__msg__Header__EXPECTED_HASH, std_msgs__msg__Header__get_type_hash(NULL), sizeof(rosidl_type_hash_t)));
    description.referenced_type_descriptions.data[3].fields = std_msgs__msg__Header__get_type_description(NULL)->type_description.fields;
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "std_msgs/Header header\n"
  "\n"
  "# The 0th scan is at the bottom\n"
  "LslidarScan[16] scans";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
lslidar_msgs__msg__LslidarSweep__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {lslidar_msgs__msg__LslidarSweep__TYPE_NAME, 29, 29},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 78, 78},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
lslidar_msgs__msg__LslidarSweep__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[5];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 5, 5};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *lslidar_msgs__msg__LslidarSweep__get_individual_type_description_source(NULL),
    sources[1] = *builtin_interfaces__msg__Time__get_individual_type_description_source(NULL);
    sources[2] = *lslidar_msgs__msg__LslidarPoint__get_individual_type_description_source(NULL);
    sources[3] = *lslidar_msgs__msg__LslidarScan__get_individual_type_description_source(NULL);
    sources[4] = *std_msgs__msg__Header__get_individual_type_description_source(NULL);
    constructed = true;
  }
  return &source_sequence;
}
