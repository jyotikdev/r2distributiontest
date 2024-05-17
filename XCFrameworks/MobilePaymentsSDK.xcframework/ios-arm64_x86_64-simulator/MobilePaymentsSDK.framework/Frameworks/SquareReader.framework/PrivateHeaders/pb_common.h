/* pb_common.h: Common support functions for pb_encode.c and pb_decode.c.
 * These functions are rarely needed by applications directly.
 */

#ifndef PB_COMMON_H_INCLUDED
#define PB_COMMON_H_INCLUDED

#include "pb.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Iterator for pb_field_t list */
struct pb_field_iter_s {
    const pb_msgdesc_t *start;      /* Start of the pb_field_t array */
    union {
        const pb_field_part_t *part;
        const pb_field_t *pos;     /* Current position of the iterator */
    };
    unsigned required_field_index; /* Zero-based index that counts only the required fields */
    unsigned required_fields;      /* Count of required fields once all fields have been seen */
    void *dest_struct;             /* Pointer to start of the structure */
    void *pData;                   /* Pointer to current field value */
    void *pSize;                   /* Pointer to count/has field */
    pb_size_t temp_size;           /* "has" or "count" for this field when decoding to ritual */
};
typedef struct pb_field_iter_s pb_field_iter_t;

/* Initialize the field iterator structure to beginning.
 * Returns false if the message type is empty. */
bool pb_field_iter_begin(pb_field_iter_t *iter, const pb_msgdesc_t *fields, void *dest_struct);
bool pb_field_iter_begin_ritual(pb_field_iter_t *iter, const pb_msgdesc_t *fields, void *dest_struct);

/* Advance the iterator to the next field.
 * Returns false when the iterator wraps back to the first field. */
bool pb_field_iter_next(pb_field_iter_t *iter);
bool pb_field_iter_next_ritual(pb_field_iter_t *iter);

/* Advance the iterator until it points at a field with the given tag.
 * Returns false if no such field exists. */
bool pb_field_iter_find(pb_field_iter_t *iter, uint32_t tag);
bool pb_field_iter_find_ritual(pb_field_iter_t *iter, uint32_t tag);

size_t pb_submessage_get_ritual_size(const pb_msgdesc_t fields[]);
size_t pb_get_ritual_field_size(const pb_field_t *field);
size_t pb_get_ritual_field_count(const pb_field_t *field);


#ifdef __cplusplus
} /* extern "C" */
#endif

#endif

