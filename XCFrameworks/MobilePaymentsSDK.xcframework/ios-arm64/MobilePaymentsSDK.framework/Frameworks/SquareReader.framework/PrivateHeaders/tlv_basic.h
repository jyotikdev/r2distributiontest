// Copyright 2017 Square, Inc.

#pragma once

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#define TAG_NOT_FOUND ((uint16_t)(-1))

typedef uint32_t tlv_tag_t;

typedef enum {
  TLV_FLAG_NONE = 0x00,
  TLV_FLAG_DISCOVER_TAG9F80  = 0x01,                                                    // Legacy Discover cards have tag 9F80 which violates TLV rule so do special handling if 9F80 is present
  TLV_FLAG_REMOVE_PADDING_FF = 0x02,                                                    // Skip FF paddings
  TLV_FLAG_REMOVE_PADDING_00 = 0x04,                                                    // Skip 00 paddings
  TLV_FLAG_REMOVE_PADDING    = TLV_FLAG_REMOVE_PADDING_FF | TLV_FLAG_REMOVE_PADDING_00, // Skip both 00 and FF paddings
  TLV_FLAG_NON_RECURSIVE     = 0x08,                                                    // Search tlv non-recursively (not checking within composite tags)
  TLV_FLAG_FOR_DOL           = 0x10,                                                    // Set if the buffer has list of Tag and Length only
  TLV_FLAG_TAG_LIST_IN_PARAM = 0x20,                                                    // Set if the 'proc_param' parameter to tlv_map is of type tlv_data_array_t.
} tlv_flags_t;

typedef struct {
    tlv_tag_t tag;
    size_t    length;
    uint8_t   *data;
    size_t    offset_to_tag;
} tlv_tag_data_t;

typedef struct
{
    tlv_tag_t tag;
    size_t data_len;
    size_t data_offset;
} tlv_info_t;

typedef struct {
    tlv_tag_data_t **tags;
    size_t         len;
} tlv_tag_data_array_t;

typedef struct {
    tlv_tag_t  *tags;
    size_t     len;
} tlv_tag_array_t;

#define  MASK_COMPOSITE_TAG 0x20

#define TAGBYTES_MAX 4
#define TAGBITS_MAX (TAGBYTES_MAX * CHAR_BIT)

#define LENBYTES_MAX 2
#define LEN_MAX ((1 << (LENBYTES_MAX*CHAR_BIT))-1)

#define TAG1_HAS_MORE_BYTES(tag) (((tag) & 0x1F) == 0x1F)
#define TAGN_HAS_MORE_BYTES(tag) ((tag) & 0x80)
#define MULTI_BYTE_LEN(num) ((num) | 0x80)

#define LEN_SHORTFORM_MAX 127
#define LEN_LONGFORM_MAX 65535

#define CLASS_SHIFT 6
#define CAT_SHIFT   5

#define CLASS_BITS(x) ((0x3 & (x)) << CLASS_SHIFT)
#define CAT_BIT(x) ((0x1 & (x)) << CAT_SHIFT)
#ifndef CHAR_BIT
    #define CHAR_BIT    8
#endif
#define TAG_9F80    0x9F80

// callback fucntion to process the tlv values
typedef bool (*tlv_processing_proc_t)(tlv_tag_t tag, const uint8_t *data, size_t data_len, size_t data_offset, void *additional_param);

//--TLV buffer reading and searching--

// Get the tag, length and value present at first position in tlv buffer. return false if tag is invalid format. val_offset_out is optional, set to null if not required.
bool tlv_get_first_tlv(const uint8_t *buf, size_t buf_size, tlv_tag_t *tag_out, size_t *len_out, uint8_t **val_out, size_t *val_offset_out, tlv_flags_t tlv_flags);
// Searching a tag in tlv array including the constructed tlv’s
bool tlv_search(tlv_tag_t tag, const uint8_t *buf, size_t buf_size, size_t *len_out, size_t *offset_to_tag, uint8_t **val_out, tlv_flags_t tlv_flags);
// Processing the tlv data through the procedure 'proc'.
bool tlv_map(const uint8_t *buf, size_t buf_size, tlv_flags_t tlv_flags, tlv_processing_proc_t proc, void *proc_param);
// Searching list of tags in the tlv array. tag_list is in/out parameter passed in to 'proc', input tags to search and outputs the lenght & reference to data in the same structure.
bool tlv_search_multiple(tlv_tag_data_array_t *tag_list, const uint8_t *buf, size_t buf_size, tlv_flags_t tlv_flags);
// Check tlv consistency
bool tlv_is_tlv_well_formed(const uint8_t *buf, size_t buf_size, tlv_flags_t tlv_flags);
// Find the template tag for a premitive tag
bool tlv_find_template(tlv_tag_t tag, const uint8_t *buf, size_t buf_size, tlv_flags_t tlv_flags, tlv_tag_t *template_tag);

//--TLV construct and modifications--

// Prepare a tlv using the tag and value (appending to an existing tlv buffer), returns buffer length after adding tlv.
size_t tlv_prepare(uint8_t *buf, size_t offset, size_t buf_size, tlv_tag_t tag, size_t len, const uint8_t* value);
// Remove a tlv from tlv array (remove first instance of tag), returns new length.
bool tlv_remove(tlv_tag_t tag, uint8_t *buf, size_t buf_size, tlv_flags_t tlv_flags, size_t *length_out);

// --TLV compute primitive size--

// Compute number of bytes in a Tag
size_t tlv_get_encoded_tag_size(tlv_tag_t tag);
// Compute number of bytes required for encoded length value
size_t tlv_get_encoded_len_size(size_t len);
// Encoding a length value in ASN.1 DER notation (e.g. len=300 will be encoded to 0x81C0), will return the size of encoded length if buf_out is null otherwise fill the encoded length value in buf_out.
size_t tlv_get_encoded_len_value(size_t len, uint8_t *buf_out, size_t buf_out_size);
// Compute size of buffer required to form a tlv for a tag and length.
size_t tlv_get_encoded_tlv_size(tlv_tag_t tag, size_t len);
// Get the valid TLV size in the buffer
size_t tlv_get_total_tlv_size(const uint8_t *tlv_buf, size_t buf_size);

//--TLV checks and decoding dol's--

// Check if tag is primitive
bool tlv_is_tag_primitive(tlv_tag_t tag);
// Decode DOL (list of tag and length)
bool tlv_dol_map(const uint8_t *buf, size_t buf_size, tlv_processing_proc_t proc, void *proc_param);
