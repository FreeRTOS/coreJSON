/*
 * coreJSON v3.2.0
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#ifndef CORE_JSON_CONTRACTS_H_
#define CORE_JSON_CONTRACTS_H_

#include <stdint.h>

#include "core_json.h"

#define isBool( x )             ( ( x == true ) || ( x == false ) )

/* Parameter check fail values for JSON API functions. */
#define isParameterEnum( x )    ( ( x == JSONNullParameter ) || ( x == JSONBadParameter ) )

/* These 3 enums represent all the ways skipCollection() can fail. */
#define isSkipCollectionFailEnum( x ) \
    ( ( x == JSONPartial ) || ( x == JSONIllegalDocument ) || ( x == JSONMaxDepthExceeded ) )

/* All possible return values for skipCollection() */
#define isSkipCollectionEnum( x )    ( isSkipCollectionFailEnum( x ) || ( x == JSONSuccess ) )

/* All possible return values for JSON_Validate() */
#define isJSONValidateEnum( x )      ( isSkipCollectionEnum( x ) || isParameterEnum( x ) )

/* All possible return values for JSON_Search() */
#define isJSONSearchEnum( x )        ( isJSONValidateEnum( x ) || ( x == JSONNotFound ) )

/* All possible return values for JSON_Iterate() */
#define isJSONIterateEnum( x )                                \
    ( isParameterEnum( x ) || ( x == JSONIllegalDocument ) || \
      ( x == JSONNotFound ) || ( x == JSONSuccess ) )

/* All possible type values output from JSON_SearchT() */
#define isJSONTypesEnum( x ) \
    ( ( x == JSONString ) || \
      ( x == JSONNumber ) || \
      ( x == JSONTrue ) ||   \
      ( x == JSONFalse ) ||  \
      ( x == JSONNull ) ||   \
      ( x == JSONObject ) || \
      ( x == JSONArray ) )

/**
 * Renaming all contract functions from CBMC for readability.
 * For more information about contracts in CBMC, see
 * https://diffblue.github.io/cbmc/contracts-user.html
 */
#define requires            __CPROVER_requires
#define ensures             __CPROVER_ensures
#define assigns             __CPROVER_assigns
#define allocates           __CPROVER_is_fresh
#define old                 __CPROVER_old
#define result              __CPROVER_return_value
#define pointer_in_range    __CPROVER_pointer_in_range_dfcc

/**
 * These are declarations for the (normally) static functions from core_json.c.
 * Please see core_json.c for documentation.
 */

JSONStatus_t JSON_SearchConst( const char * buf,
                               size_t max,
                               const char * query,
                               size_t queryLength,
                               const char ** outValue,
                               size_t * outValueLength,
                               JSONTypes_t * outType )
requires( JSON_SearchConst_requires( buf, max, query, queryLength, outValue, outValueLength, outType ) )
assigns( *outValue, *outValueLength, *outType )
ensures( JSON_SearchConst_ensures( result, buf, outValue, outValueLength, max ) )
;

JSONStatus_t JSON_Iterate( const char * buf,
                           size_t max,
                           size_t * start,
                           size_t * next,
                           JSONPair_t * outPair )
requires( JSON_Iterate_requires( buf, max, start, next, outPair ) )
assigns( *start, *next, *outPair )
ensures( JSON_Iterate_ensures( result, buf, max, outPair ) )
;

JSONStatus_t JSON_Validate( const char * buf,
                            size_t max )
requires( max < CBMC_MAX_BUFSIZE )
requires( buf == NULL || allocates( buf, max ) )
ensures( isJSONValidateEnum( result ) )
;

bool arraySearch( const char * buf,
                  size_t max,
                  uint32_t queryIndex,
                  size_t * outValue,
                  size_t * outValueLength )
requires( arraySearch_requires( buf, max, outValue, outValueLength ) )
assigns( *outValue, *outValueLength )
ensures( arraySearch_ensures( result, buf, max, outValue, outValueLength, old( *outValue ), old( *outValueLength ) ) )
;

bool objectSearch( const char * buf,
                   size_t max,
                   const char * query,
                   size_t queryLength,
                   size_t * outValue,
                   size_t * outValueLength )
requires( objectSearch_requires( buf, max, query, queryLength, outValue, outValueLength ) )
assigns( *outValue, *outValueLength )
ensures( objectSearch_ensures( result, buf, max, outValue, outValueLength, old( *outValue ), old( *outValueLength ) ) )
;

JSONStatus_t skipCollection( const char * buf,
                             size_t * start,
                             size_t max )
requires( skipCollection_requires( buf, start, max ) )
assigns( *start )
ensures( skipCollection_ensures( result, buf, start, old( *start ), max ) )
;

void skipScalars( const char * buf,
                  size_t * start,
                  size_t max,
                  char mode )
requires( skipScalars_requires( buf, start, max, mode ) )
assigns( *start )
ensures( skipScalars_ensures( start, old( *start ), max ) )
;

void skipObjectScalars( const char * buf,
                        size_t * start,
                        size_t max )
requires( skipObjectScalars_requires( buf, start, max ) )
assigns( *start )
ensures( skipObjectScalars_ensures( start, old( *start ), max ) )
;

bool skipAnyScalar( const char * buf,
                    size_t * start,
                    size_t max )
requires( skipAnyScalar_requires( buf, start, max ) )
assigns( *start )
ensures( skipAnyScalar_ensures( result, buf, start, old( *start ), max ) )
;

void skipSpace( const char * buf,
                size_t * start,
                size_t max )
requires( skipSpace_requires( buf, start, max ) )
assigns( *start )
ensures( skipSpace_ensures( start, old( *start ), max ) )
;

bool skipString( const char * buf,
                 size_t * start,
                 size_t max )
requires( skipString_requires( buf, start, max ) )
assigns( *start )
ensures( skipString_ensures( result, start, old( *start ), max ) )
;

bool skipEscape( const char * buf,
                 size_t * start,
                 size_t max )
requires( skipEscape_requires( buf, start, max ) )
assigns( *start )
ensures( skipEscape_ensures( result, start, old( *start ), max ) )
;

bool skipUTF8( const char * buf,
               size_t * start,
               size_t max )
requires( skipEscape_requires( buf, start, max ) )
assigns( *start )
ensures( skipEscape_ensures( result, start, old( *start ), max ) )
;

bool skipDigits( const char * buf,
                 size_t * start,
                 size_t max,
                 int32_t * outValue )
requires( skipDigits_requires( buf, start, max, outValue ) )
assigns( *start )
assigns( outValue != NULL: *outValue )
ensures( skipDigits_ensures( result, buf, start, old( *start ), max ) )
;

#endif /* ifndef CORE_JSON_CONTRACTS_H_ */
