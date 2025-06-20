#ifndef _TYPES_H
#define _TYPES_H

#include <stdint.h>
#include <stdbool.h>

#define DATA_SECTION_ALIGN 16
#define DATA_MAGIC 0xDA7A 

typedef enum {
    TYPE_VOID,
    TYPE_INT8,
    TYPE_INT16,
    TYPE_INT32,
    TYPE_INT64,
    TYPE_UINT8,
    TYPE_UINT16,
    TYPE_UINT32,
    TYPE_UINT64,
    TYPE_FLOAT32,
    TYPE_FLOAT64,
    TYPE_BOOL,
} ValueType;

#define TYPE_TAG_VOID     0x00
#define TYPE_TAG_INT8     0x01
#define TYPE_TAG_INT16    0x02
#define TYPE_TAG_INT32    0x03
#define TYPE_TAG_INT64    0x04
#define TYPE_TAG_UINT8    0x05
#define TYPE_TAG_UINT16   0x06
#define TYPE_TAG_UINT32   0x07
#define TYPE_TAG_UINT64   0x08
#define TYPE_TAG_FLOAT32  0x09
#define TYPE_TAG_FLOAT64  0x0A
#define TYPE_TAG_BOOL     0x0B

typedef union {
    int8_t i8;
    int16_t i16;
    int32_t i32;
    int64_t i64;
    uint8_t u8;
    uint16_t u16;
    uint32_t u32;
    uint64_t u64;
    float f32;
    double f64;
    bool b;
} Value;

typedef struct {
    ValueType type;
    Value value;
} TypedValue;

typedef struct {
    uint16_t magic;     
    uint16_t size;     
    uint8_t type_tag; 
    uint8_t* data;     
} DataSection;

#endif
