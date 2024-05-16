// Copyright 2017 Square, Inc.

#pragma once

#include <stdint.h>

// Defines symbolic names, mapping tag macro to value of tag
#define X(_mandatory, _numeric, _tagname, _tagstr, _tagfmt, _minlen, _maxlen, __autoinit, _comment) \
 TAG_ ## _tagname = _numeric,
typedef enum {
#include "l2tag/tags_icc.def"
#include "l2tag/tags_issuer.def"
#include "l2tag/tags_terminal.def"
#include "l2tag/tags_nostorage.def"
} tag_e;

typedef uint32_t tag_t;
#undef X
