/*
 * coreJSON v3.2.0
 * Copyright (C) 2023 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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

#ifndef CORE_JSON_CONTRACTS_C_
#define CORE_JSON_CONTRACTS_C_

#include "core_json_contracts.h"

bool JSON_SearchConst_requires( char * buf,
                                size_t max,
                                char * query,
                                size_t queryLength,
                                char ** outValue,
                                size_t * outValueLength,
                                JSONTypes_t * outType )
{
    bool valid_max = ( max < CBMC_MAX_BUFSIZE );
    bool valid_queryLength = ( queryLength < CBMC_MAX_QUERYKEYLENGTH );
    bool valid_buf = ( buf == NULL || allocates( buf, max ) );
    bool valid_query = ( query == NULL || allocates( query, queryLength ) );
    bool valid_outValue = ( outValue == NULL || allocates( outValue, sizeof( *outValue ) ) );
    bool valid_outValueLength = ( outValueLength == NULL || allocates( outValueLength, sizeof( *outValueLength ) ) );
    bool valid_outType = ( outType == NULL || allocates( outType, sizeof( *outType ) ) );

    return valid_max && valid_queryLength && valid_buf && valid_query && valid_outValue && valid_outValueLength && valid_outType;
}

bool JSON_SearchConst_ensures( JSONStatus_t result,
                               char * buf,
                               char ** outValue,
                               size_t * outValueLength,
                               size_t max )
{
    if( result == JSONSuccess )
    {
        /*pointer_in_range( buf,  *outValue , buf + max)*/
        char * endOfValue = *outValue + *outValueLength;
        char * endOfBuf = buf + max;
        return pointer_in_range( buf, endOfValue, endOfBuf );
    }

    return true;
}

bool JSON_Iterate_requires( char * buf,
                            size_t max,
                            size_t * start,
                            size_t * next,
                            JSONPair_t * outPair )
{
    bool valid_max = ( 0 < max && max < CBMC_MAX_BUFSIZE );
    bool valid_buf = ( buf == NULL || allocates( buf, max ) );
    bool valid_start = ( start == NULL || allocates( start, sizeof( *start ) ) );
    bool valid_next = ( next == NULL || allocates( next, sizeof( *next ) ) );
    bool valid_outPair = ( outPair == NULL || allocates( outPair, sizeof( *outPair ) ) );

/* *INDENT-OFF* */
    bool valid_key = ( outPair != NULL ==> ( ( outPair->keyLength == 0 && outPair->key == NULL ) || allocates( outPair->key, outPair->keyLength ) ) );
    bool valid_value = ( outPair != NULL ==> ( ( outPair->valueLength == 0 && outPair->value == NULL ) || allocates( outPair->value, outPair->valueLength ) ) );
/* *INDENT-ON* */

    return valid_max && valid_buf && valid_start && valid_next && valid_outPair && valid_key && valid_value;
}

bool JSON_Iterate_ensures( JSONStatus_t result,
                           char * buf,
                           size_t max,
                           JSONPair_t * outPair )
{
    bool valid_result = isJSONIterateEnum( result );

    if( valid_result && ( result == JSONSuccess ) )
    {
/* *INDENT-OFF* */
        bool valid_key = ( ( outPair->key != NULL ) ==> ( ( outPair->key > buf ) && ( ( outPair->key + outPair->keyLength ) < ( buf + max ) ) ) );
        bool valid_keyLength = ( ( outPair->key != NULL ) ==> ( ( outPair->key + outPair->keyLength ) < outPair->value ) );
/* *INDENT-ON* */
        bool valid_value = ( ( outPair->value > buf ) && ( ( outPair->value + outPair->valueLength ) <= ( buf + max ) ) );
        bool valid_jsonType = isJSONTypesEnum( outPair->jsonType );
        valid_result = valid_key && valid_keyLength && valid_value && valid_jsonType;
    }

    return valid_result;
}

bool arraySearch_requires( char * buf,
                           size_t max,
                           size_t * outValue,
                           size_t * outValueLength )
{
    bool valid_max = ( 0 < max && max < CBMC_MAX_BUFSIZE );
    bool valid_buf = ( allocates( buf, max ) );
    bool valid_outValue = ( allocates( outValue, sizeof( *outValue ) ) );
    bool valid_outValueLength = ( allocates( outValueLength, sizeof( *outValueLength ) ) && *outValueLength <= max );

    return valid_max && valid_buf && valid_outValue && valid_outValueLength;
}

bool arraySearch_ensures( JSONStatus_t result,
                          char * buf,
                          size_t max,
                          size_t * outValue,
                          size_t * outValueLength,
                          size_t old_outValue,
                          size_t old_outValueLength )
{
    bool valid_result = isBool( result );
    bool valid_outValue = false;
    bool valid_outValueLength = false;

    if( result )
    {
        valid_outValue = ( 0 <= *outValue && *outValue < max );
        valid_outValueLength = ( 0 < *outValueLength && *outValueLength <= max - *outValue );
/* *INDENT-OFF* */
        valid_outValueLength = valid_outValueLength && ( valid_outValueLength && ( buf[ *outValue ] == '"' ==> ( 2 <= *outValueLength && *outValueLength <= max - *outValue ) ) );
/* *INDENT-ON* */
    }
    else
    {
        valid_outValue = ( *outValue == old_outValue );
        valid_outValueLength = ( *outValueLength == old_outValueLength );
    }

    return valid_result && valid_outValue && valid_outValueLength;
}

bool objectSearch_requires( char * buf,
                            size_t max,
                            char * query,
                            size_t queryLength,
                            size_t * outValue,
                            size_t * outValueLength )
{
    bool valid_queryLength = ( queryLength < CBMC_MAX_QUERYKEYLENGTH );
    bool valid_query = ( allocates( query, queryLength ) );

    return valid_queryLength && valid_query && arraySearch_requires( buf, max, outValue, outValueLength );
}

bool objectSearch_ensures( JSONStatus_t result,
                           char * buf,
                           size_t max,
                           size_t * outValue,
                           size_t * outValueLength,
                           size_t old_outValue,
                           size_t old_outValueLength )
{
    return arraySearch_ensures( result, buf, max, outValue, outValueLength, old_outValue, old_outValueLength );
}

bool skipCollection_requires( char * buf,
                              size_t * start,
                              size_t max )
{
    bool valid_max = ( 0 < max && max < CBMC_MAX_BUFSIZE );
    bool valid_buf = ( allocates( buf, max ) );
    bool valid_start = ( allocates( start, sizeof( *start ) ) );

    return valid_max && valid_buf && valid_start;
}

bool skipCollection_ensures( JSONStatus_t result,
                             char * buf,
                             size_t * start,
                             size_t old_start,
                             size_t max )
{
    bool valid_result = isSkipCollectionEnum( result );
    bool valid_start = ( *start >= old_start );

    if( old_start < max )
    {
        valid_start = valid_start && ( *start <= max );
    }
    else
    {
        valid_start = valid_start && ( *start == old_start );
    }

/* *INDENT-OFF* */
    valid_start = valid_start && ( ( result == JSONSuccess ) ==> ( old_start < max ) && ( *start >= old_start + 2 ) );
/* *INDENT-ON* */

    return valid_result && valid_start;
}

bool skipScalars_requires( char * buf,
                           size_t * start,
                           size_t max,
                           char mode )
{
    bool valid_mode = ( ( mode == '{' ) || ( mode == '[' ) );

    return valid_mode && skipCollection_requires( buf, start, max );
}

bool skipScalars_ensures( size_t * start,
                          size_t old_start,
                          size_t max )
{
    bool valid_start = ( *start >= old_start );

    if( old_start < max )
    {
        valid_start = valid_start && ( *start <= max );
    }
    else
    {
        valid_start = valid_start && ( *start == old_start );
    }

    return valid_start;
}

bool skipObjectScalars_requires( char * buf,
                                 size_t * start,
                                 size_t max )
{
    return skipCollection_requires( buf, start, max );
}

bool skipObjectScalars_ensures( size_t * start,
                                size_t old_start,
                                size_t max )
{
    return skipScalars_ensures( start, old_start, max );
}

bool skipAnyScalar_requires( char * buf,
                             size_t * start,
                             size_t max )
{
    return skipCollection_requires( buf, start, max );
}

bool skipAnyScalar_ensures( bool result,
                            char * buf,
                            size_t * start,
                            size_t old_start,
                            size_t max )
{
    bool valid_result = isBool( result );
    bool valid_start = ( *start >= old_start );

    if( old_start < max )
    {
        valid_start = valid_start && ( *start <= max );
    }
    else
    {
        valid_start = valid_start && ( *start == old_start );
    }

    if( result )
    {
        valid_start = valid_start && ( old_start < max && *start > old_start );
/* *INDENT-OFF* */
        valid_start = valid_start && ( ( buf[ old_start ] == '"' ) ==> *start >= old_start + 2 );
/* *INDENT-ON* */
    }

    return valid_result && valid_start;
}

bool skipSpace_requires( char * buf,
                         size_t * start,
                         size_t max )
{
    return skipCollection_requires( buf, start, max );
}

bool skipSpace_ensures( size_t * start,
                        size_t old_start,
                        size_t max )
{
    return skipScalars_ensures( start, old_start, max );
}

bool skipString_requires( char * buf,
                          size_t * start,
                          size_t max )
{
    return skipCollection_requires( buf, start, max );
}

bool skipString_ensures( bool result,
                         size_t * start,
                         size_t old_start,
                         size_t max )
{
    bool valid_result = isBool( result );
    bool valid_start = ( *start >= old_start );

    if( old_start < max )
    {
        valid_start = valid_start && ( *start <= max );
    }
    else
    {
        valid_start = valid_start && ( *start == old_start );
    }

/* *INDENT-OFF* */
    valid_start = valid_start && ( result ==> ( old_start < max ) && ( *start >= old_start + 2 ) );
/* *INDENT-ON* */

    return valid_result && valid_start;
}

bool skipEscape_requires( char * buf,
                          size_t * start,
                          size_t max )
{
    return skipCollection_requires( buf, start, max );
}

bool skipEscape_ensures( bool result,
                         size_t * start,
                         size_t old_start,
                         size_t max )
{
    bool valid_result = isBool( result );
    bool valid_start = ( *start >= old_start );

    if( old_start < max )
    {
        valid_start = valid_start && ( *start <= max );
    }
    else
    {
        valid_start = valid_start && ( *start == old_start );
    }

/* *INDENT-OFF* */
    valid_start = valid_start && ( result ==> ( old_start < max ) && ( *start >= old_start + 1 ) );
/* *INDENT-ON* */

    return valid_result && valid_start;
}

bool skipDigits_requires( char * buf,
                          size_t * start,
                          size_t max,
                          int32_t * outValue )
{
    bool valid_outValue = ( outValue == NULL || allocates( outValue, sizeof( *outValue ) ) );

    return valid_outValue && skipCollection_requires( buf, start, max );
}

bool skipDigits_ensures( bool result,
                         char * buf,
                         size_t * start,
                         size_t old_start,
                         size_t max )
{
    bool valid_result = isBool( result );
    bool valid_start = ( *start >= old_start );

    if( old_start < max )
    {
        valid_start = valid_start && ( *start <= max );
    }
    else
    {
        valid_start = valid_start && ( *start == old_start );
    }

/* *INDENT-OFF* */
    valid_start = valid_start && ( result ==> ( old_start < max ) && ( *start > old_start ) && ( ( ( buf[ old_start ] ) >= '0' ) && ( ( buf[ old_start ] ) <= '9' ) ) );
/* *INDENT-ON* */

    return valid_result && valid_start;
}

#endif /* ifndef CORE_JSON_CONTRACTS_C_ */
