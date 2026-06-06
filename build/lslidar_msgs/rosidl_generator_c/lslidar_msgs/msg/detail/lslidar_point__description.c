// generated from rosidl_generator_c/resource/idl__description.c.em
// with input from lslidar_msgs:msg/LslidarPoint.idl
// generated code does not contain a copyright notice

#include "lslidar_msgs/msg/detail/lslidar_point__functions.h"

ROSIDL_GENERATOR_C_PUBLIC_lslidar_msgs
const rosidl_type_hash_t *
lslidar_msgs__msg__LslidarPoint__get_type_hash(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_type_hash_t hash = {1, {
      0xb8, 0xf1, 0x25, 0x11, 0xca, 0x75, 0x6f, 0x45,
      0xc4, 0xdb, 0xa5, 0xca, 0x5e, 0xb3, 0x2a, 0xf7,
      0x50, 0x4e, 0x11, 0xee, 0x8c, 0x8a, 0x9d, 0x15,
      0xbd, 0x0e, 0x2e, 0x88, 0x63, 0x79, 0xab, 0x04,
    }};
  return &hash;
}

#include <assert.h>
#include <string.h>

// Include directives for referenced types

// Hashes for external referenced types
#ifndef NDEBUG
#endif

static char lslidar_msgs__msg__LslidarPoint__TYPE_NAME[] = "lslidar_msgs/msg/LslidarPoint";

// Define type names, field names, and default values
static char lslidar_msgs__msg__LslidarPoint__FIELD_NAME__time[] = "time";
static char lslidar_msgs__msg__LslidarPoint__FIELD_NAME__x[] = "x";
static char lslidar_msgs__msg__LslidarPoint__FIELD_NAME__y[] = "y";
static char lslidar_msgs__msg__LslidarPoint__FIELD_NAME__z[] = "z";
static char lslidar_msgs__msg__LslidarPoint__FIELD_NAME__azimuth[] = "azimuth";
static char lslidar_msgs__msg__LslidarPoint__FIELD_NAME__distance[] = "distance";
static char lslidar_msgs__msg__LslidarPoint__FIELD_NAME__intensity[] = "intensity";

static rosidl_runtime_c__type_description__Field lslidar_msgs__msg__LslidarPoint__FIELDS[] = {
  {
    {lslidar_msgs__msg__LslidarPoint__FIELD_NAME__time, 4, 4},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_FLOAT,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {lslidar_msgs__msg__LslidarPoint__FIELD_NAME__x, 1, 1},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {lslidar_msgs__msg__LslidarPoint__FIELD_NAME__y, 1, 1},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {lslidar_msgs__msg__LslidarPoint__FIELD_NAME__z, 1, 1},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {lslidar_msgs__msg__LslidarPoint__FIELD_NAME__azimuth, 7, 7},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {lslidar_msgs__msg__LslidarPoint__FIELD_NAME__distance, 8, 8},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
  {
    {lslidar_msgs__msg__LslidarPoint__FIELD_NAME__intensity, 9, 9},
    {
      rosidl_runtime_c__type_description__FieldType__FIELD_TYPE_DOUBLE,
      0,
      0,
      {NULL, 0, 0},
    },
    {NULL, 0, 0},
  },
};

const rosidl_runtime_c__type_description__TypeDescription *
lslidar_msgs__msg__LslidarPoint__get_type_description(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static bool constructed = false;
  static const rosidl_runtime_c__type_description__TypeDescription description = {
    {
      {lslidar_msgs__msg__LslidarPoint__TYPE_NAME, 29, 29},
      {lslidar_msgs__msg__LslidarPoint__FIELDS, 7, 7},
    },
    {NULL, 0, 0},
  };
  if (!constructed) {
    constructed = true;
  }
  return &description;
}

static char toplevel_type_raw_source[] =
  "# Time when the point is captured\n"
  "float32 time\n"
  "\n"
  "# Converted distance in the sensor frame\n"
  "float64 x\n"
  "float64 y\n"
  "float64 z\n"
  "\n"
  "# Raw measurement from Leishen M10\n"
  "float64 azimuth\n"
  "float64 distance\n"
  "float64 intensity";

static char msg_encoding[] = "msg";

// Define all individual source functions

const rosidl_runtime_c__type_description__TypeSource *
lslidar_msgs__msg__LslidarPoint__get_individual_type_description_source(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static const rosidl_runtime_c__type_description__TypeSource source = {
    {lslidar_msgs__msg__LslidarPoint__TYPE_NAME, 29, 29},
    {msg_encoding, 3, 3},
    {toplevel_type_raw_source, 206, 206},
  };
  return &source;
}

const rosidl_runtime_c__type_description__TypeSource__Sequence *
lslidar_msgs__msg__LslidarPoint__get_type_description_sources(
  const rosidl_message_type_support_t * type_support)
{
  (void)type_support;
  static rosidl_runtime_c__type_description__TypeSource sources[1];
  static const rosidl_runtime_c__type_description__TypeSource__Sequence source_sequence = {sources, 1, 1};
  static bool constructed = false;
  if (!constructed) {
    sources[0] = *lslidar_msgs__msg__LslidarPoint__get_individual_type_description_source(NULL),
    constructed = true;
  }
  return &source_sequence;
}
