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

/*
 * These are declarations for the (normally) static functions from core_json.c.
 * Please see core_json.c for documentation.
 */

/* *INDENT-OFF* */

JSONStatus_t JSON_SearchConst( const char * buf,
                               size_t max,
                               const char * query,
                               size_t queryLength,
                               const char ** outValue,
                               size_t * outValueLength,
                               JSONTypes_t * outType )
__CPROVER_requires( max < CBMC_MAX_BUFSIZE )
__CPROVER_requires( queryLength < CBMC_MAX_QUERYKEYLENGTH )
__CPROVER_requires( buf == NULL || __CPROVER_is_fresh( buf, max ) )
__CPROVER_requires( query == NULL || __CPROVER_is_fresh( query, queryLength ) )
__CPROVER_requires( outValue == NULL || __CPROVER_is_fresh( outValue, sizeof( *outValue ) ) )
__CPROVER_requires( outValueLength == NULL || __CPROVER_is_fresh( outValueLength, sizeof( *outValueLength ) ) )
__CPROVER_requires( outType == NULL || __CPROVER_is_fresh( outType, sizeof( *outType ) ) )
__CPROVER_assigns( *outValue )
__CPROVER_assigns( *outValueLength )
__CPROVER_assigns( *outType )
__CPROVER_ensures( isJSONSearchEnum( __CPROVER_return_value ) )
__CPROVER_ensures( __CPROVER_return_value == JSONSuccess ==> ( ( *outValue >= buf ) && ( ( *outValue + *outValueLength ) <= ( buf + max ) ) ) );

JSONStatus_t JSON_Iterate( const char * buf,
                           size_t max,
                           size_t * start,
                           size_t * next,
                           JSONPair_t * outPair )
__CPROVER_requires( 0 < max && max < CBMC_MAX_BUFSIZE )
__CPROVER_requires( buf == NULL || __CPROVER_is_fresh( buf, max ) )
__CPROVER_requires( start == NULL || __CPROVER_is_fresh( start, sizeof( *start ) ) )
__CPROVER_requires( next == NULL || __CPROVER_is_fresh( next, sizeof( *next ) ) )
__CPROVER_requires( outPair == NULL || __CPROVER_is_fresh( outPair, sizeof( *outPair ) ) )
__CPROVER_requires( outPair != NULL ==> ( ( outPair->keyLength == 0 && outPair->key == NULL ) || __CPROVER_is_fresh( outPair->key, outPair->keyLength ) ) )
__CPROVER_requires( outPair != NULL ==> ( ( outPair->keyLength == 0 && outPair->key == NULL ) || __CPROVER_is_fresh( outPair->value, outPair->valueLength ) ) )
__CPROVER_assigns( *start )
__CPROVER_assigns( *next )
__CPROVER_assigns( *outPair )
__CPROVER_ensures( isJSONIterateEnum( __CPROVER_return_value ) )
__CPROVER_ensures( ( __CPROVER_return_value == JSONSuccess && outPair->key != NULL ) ==> ( ( outPair->key > buf ) && ( ( outPair->key + outPair->keyLength ) < ( buf + max ) ) ) )
__CPROVER_ensures( ( __CPROVER_return_value == JSONSuccess && outPair->key != NULL ) ==> ( ( outPair->key + outPair->keyLength ) < outPair->value ) )
__CPROVER_ensures( __CPROVER_return_value == JSONSuccess ==> ( ( outPair->value > buf ) && ( ( outPair->value + outPair->valueLength ) <= ( buf + max ) ) ) )
__CPROVER_ensures( __CPROVER_return_value == JSONSuccess ==> isJSONTypesEnum( outPair->jsonType ) );

JSONStatus_t JSON_Validate( const char * buf,
                            size_t max )
__CPROVER_requires( max < CBMC_MAX_BUFSIZE )
__CPROVER_requires( buf == NULL || __CPROVER_is_fresh( buf, max ) )
__CPROVER_ensures( isJSONValidateEnum( __CPROVER_return_value ) );

bool arraySearch( const char * buf,
                  size_t max,
                  uint32_t queryIndex,
                  size_t * outValue,
                  size_t * outValueLength )
__CPROVER_requires( 0 < max && max < CBMC_MAX_BUFSIZE )
__CPROVER_requires( __CPROVER_is_fresh( buf, max ) )
__CPROVER_requires( __CPROVER_is_fresh( outValue, sizeof( *outValue ) ) )
__CPROVER_requires( __CPROVER_is_fresh( outValueLength, sizeof( *outValueLength ) ) )
__CPROVER_requires( *outValueLength <= max )
__CPROVER_assigns( *outValue )
__CPROVER_assigns( *outValueLength )
__CPROVER_ensures( isBool( __CPROVER_return_value ) )
__CPROVER_ensures( __CPROVER_return_value ? ( 0 <= *outValue && *outValue < max ) : ( *outValue == __CPROVER_old( *outValue ) ) )
__CPROVER_ensures( __CPROVER_return_value ? ( 0 < *outValueLength && *outValueLength <= max - *outValue ) : ( *outValueLength == __CPROVER_old( *outValueLength ) ) )
__CPROVER_ensures( __CPROVER_return_value && buf[ *outValue ] == '"' ==> ( 2 <= *outValueLength && *outValueLength <= max - *outValue ) );

bool objectSearch( const char * buf,
                   size_t max,
                   const char * query,
                   size_t queryLength,
                   size_t * outValue,
                   size_t * outValueLength )
__CPROVER_requires( 0 < max && max < CBMC_MAX_BUFSIZE )
__CPROVER_requires( queryLength < CBMC_MAX_QUERYKEYLENGTH )
__CPROVER_requires( __CPROVER_is_fresh( buf, max ) )
__CPROVER_requires( __CPROVER_is_fresh( query, queryLength ) )
__CPROVER_requires( __CPROVER_is_fresh( outValue, sizeof( *outValue ) ) )
__CPROVER_requires( __CPROVER_is_fresh( outValueLength, sizeof( *outValueLength ) ) )
__CPROVER_assigns( *outValue )
__CPROVER_assigns( *outValueLength )
__CPROVER_ensures( isBool( __CPROVER_return_value ) )
__CPROVER_ensures( __CPROVER_return_value ? ( 0 <= *outValue && *outValue < max ) : ( *outValue == __CPROVER_old( *outValue ) ) )
__CPROVER_ensures( __CPROVER_return_value ? ( 0 < *outValueLength && *outValueLength <= max - *outValue ) : ( *outValueLength == __CPROVER_old( *outValueLength ) ) )
__CPROVER_ensures( __CPROVER_return_value && buf[ *outValue ] == '"' ==> ( 2 <= *outValueLength && *outValueLength <= max - *outValue ) );

JSONStatus_t skipCollection( const char * buf,
                             size_t * start,
                             size_t max )
__CPROVER_requires( 0 < max && max < CBMC_MAX_BUFSIZE )
__CPROVER_requires( __CPROVER_is_fresh( buf, max ) )
__CPROVER_requires( __CPROVER_is_fresh( start, sizeof( *start ) ) )
__CPROVER_assigns( *start )
__CPROVER_ensures( isSkipCollectionEnum( __CPROVER_return_value ) )
__CPROVER_ensures( *start >= __CPROVER_old( *start ) )
__CPROVER_ensures( ( __CPROVER_old( *start ) < max ) ? ( ( *start <= max ) ) : ( *start == __CPROVER_old( *start ) ) )
__CPROVER_ensures( ( __CPROVER_return_value == JSONSuccess ) ==> ( __CPROVER_old( *start ) < max ) && ( *start >= __CPROVER_old( *start ) + 2 ) );

void skipScalars( const char * buf,
                  size_t * start,
                  size_t max,
                  char mode )
__CPROVER_requires( 0 < max && max < CBMC_MAX_BUFSIZE )
__CPROVER_requires( __CPROVER_is_fresh( buf, max ) )
__CPROVER_requires( __CPROVER_is_fresh( start, sizeof( *start ) ) )
__CPROVER_requires( ( mode == '{' ) || ( mode == '[' ) )
__CPROVER_assigns( *start )
__CPROVER_ensures( *start >= __CPROVER_old( *start ) )
__CPROVER_ensures( ( __CPROVER_old( *start ) < max ) ? ( ( *start <= max ) ) : ( *start == __CPROVER_old( *start ) ) );

void skipObjectScalars( const char * buf,
                        size_t * start,
                        size_t max )
__CPROVER_requires( 0 < max && max < CBMC_MAX_BUFSIZE )
__CPROVER_requires( __CPROVER_is_fresh( buf, max ) )
__CPROVER_requires( __CPROVER_is_fresh( start, sizeof( *start ) ) )
__CPROVER_assigns( *start )
__CPROVER_ensures( *start >= __CPROVER_old( *start ) )
__CPROVER_ensures( ( __CPROVER_old( *start ) < max ) ? ( ( *start <= max ) ) : ( *start == __CPROVER_old( *start ) ) );

bool skipAnyScalar( const char * buf,
                    size_t * start,
                    size_t max )
__CPROVER_requires( 0 < max && max < CBMC_MAX_BUFSIZE )
__CPROVER_requires( __CPROVER_is_fresh( buf, max ) )
__CPROVER_requires( __CPROVER_is_fresh( start, sizeof( *start ) ) )
__CPROVER_assigns( *start )
__CPROVER_ensures( isBool( __CPROVER_return_value ) )
__CPROVER_ensures( *start >= __CPROVER_old( *start ) )
__CPROVER_ensures( ( __CPROVER_old( *start ) < max ) ? ( ( *start <= max ) ) : ( *start == __CPROVER_old( *start ) ) )
__CPROVER_ensures( __CPROVER_return_value ==> __CPROVER_old( *start ) < max && *start > __CPROVER_old( *start ) )
__CPROVER_ensures( __CPROVER_return_value && buf[ __CPROVER_old( *start ) ] == '"' ==> *start >= __CPROVER_old( *start ) + 2 );

void skipSpace( const char * buf,
                size_t * start,
                size_t max )
__CPROVER_requires( 0 < max && max < CBMC_MAX_BUFSIZE )
__CPROVER_requires( __CPROVER_is_fresh( buf, max ) )
__CPROVER_requires( __CPROVER_is_fresh( start, sizeof( *start ) ) )
__CPROVER_assigns( *start )
__CPROVER_ensures( ( __CPROVER_old( *start ) < max ) ? ( ( *start <= max ) && ( *start >= __CPROVER_old( *start ) ) ) : ( *start == __CPROVER_old( *start ) ) );

bool skipString( const char * buf,
                 size_t * start,
                 size_t max )
__CPROVER_requires( 0 < max && max < CBMC_MAX_BUFSIZE )
__CPROVER_requires( __CPROVER_is_fresh( buf, max ) )
__CPROVER_requires( __CPROVER_is_fresh( start, sizeof( *start ) ) )
__CPROVER_assigns( *start )
__CPROVER_ensures( isBool( __CPROVER_return_value ) )
__CPROVER_ensures( *start >= __CPROVER_old( *start ) )
__CPROVER_ensures( ( __CPROVER_old( *start ) < max ) ? ( ( *start <= max ) ) : ( *start == __CPROVER_old( *start ) ) )
__CPROVER_ensures( __CPROVER_return_value ==> ( __CPROVER_old( *start ) < max ) && ( *start >= __CPROVER_old( *start ) + 2 ) );

bool skipEscape( const char * buf,
                 size_t * start,
                 size_t max )
__CPROVER_requires( 0 < max && max < CBMC_MAX_BUFSIZE )
__CPROVER_requires( __CPROVER_is_fresh( buf, max ) )
__CPROVER_requires( __CPROVER_is_fresh( start, sizeof( *start ) ) )
__CPROVER_assigns( *start )
__CPROVER_ensures( isBool( __CPROVER_return_value ) )
__CPROVER_ensures( *start >= __CPROVER_old( *start ) )
__CPROVER_ensures( ( __CPROVER_old( *start ) < max ) ? ( ( *start <= max ) ) : ( *start == __CPROVER_old( *start ) ) )
__CPROVER_ensures( __CPROVER_return_value ==> ( __CPROVER_old( *start ) < max ) && ( *start >= __CPROVER_old( *start ) + 1 ) );

bool skipUTF8( const char * buf,
               size_t * start,
               size_t max )
__CPROVER_requires( 0 < max && max < CBMC_MAX_BUFSIZE )
__CPROVER_requires( __CPROVER_is_fresh( buf, max ) )
__CPROVER_requires( __CPROVER_is_fresh( start, sizeof( *start ) ) )
__CPROVER_assigns( *start )
__CPROVER_ensures( isBool( __CPROVER_return_value ) )
__CPROVER_ensures( *start >= __CPROVER_old( *start ) )
__CPROVER_ensures( ( __CPROVER_old( *start ) < max ) ? ( ( *start <= max ) ) : ( *start == __CPROVER_old( *start ) ) )
__CPROVER_ensures( __CPROVER_return_value ==> ( __CPROVER_old( *start ) < max ) && ( *start >= __CPROVER_old( *start ) + 1 ) );

bool skipDigits( const char * buf,
                 size_t * start,
                 size_t max,
                 int32_t * outValue )
__CPROVER_requires( 0 < max && max < CBMC_MAX_BUFSIZE )
__CPROVER_requires( __CPROVER_is_fresh( buf, max ) )
__CPROVER_requires( __CPROVER_is_fresh( start, sizeof( *start ) ) )
__CPROVER_requires( outValue == NULL || __CPROVER_is_fresh( outValue, sizeof( *outValue ) ) )
__CPROVER_assigns( *start )
__CPROVER_assigns( outValue != NULL: *outValue )
__CPROVER_ensures( isBool( __CPROVER_return_value ) )
__CPROVER_ensures( __CPROVER_old( *start ) <= *start )
__CPROVER_ensures( __CPROVER_old( *start ) < max ? *start <= max : *start == __CPROVER_old( *start ) )
__CPROVER_ensures( __CPROVER_return_value ==> ( __CPROVER_old( *start ) < max ) && ( *start > __CPROVER_old( *start ) ) && ( ( ( buf[ __CPROVER_old( *start ) ] ) >= '0' ) && ( ( buf[ __CPROVER_old( *start ) ] ) <= '9' ) ) );

/* *INDENT-ON* */

#endif /* ifndef CORE_JSON_CONTRACTS_H_ */
