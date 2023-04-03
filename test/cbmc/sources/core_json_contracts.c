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
    CHECK( max < CBMC_MAX_BUFSIZE );
    CHECK( queryLength < CBMC_MAX_QUERYKEYLENGTH );
    CHECK( buf == NULL || allocates( buf, max ) );
    CHECK( query == NULL || allocates( query, queryLength ) );
    CHECK( outValue == NULL || allocates( outValue, sizeof( *outValue ) ) );
    CHECK( outValueLength == NULL || allocates( outValueLength, sizeof( *outValueLength ) ) );
    CHECK( outType == NULL || allocates( outType, sizeof( *outType ) ) );

    return true;
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
    CHECK( 0 < max && max < CBMC_MAX_BUFSIZE );
    CHECK( buf == NULL || allocates( buf, max ) );
    CHECK( start == NULL || allocates( start, sizeof( *start ) ) );
    CHECK( next == NULL || allocates( next, sizeof( *next ) ) );
    CHECK( outPair == NULL || allocates( outPair, sizeof( *outPair ) ) );
/* *INDENT-OFF* */
    CHECK( outPair != NULL ==> ( ( outPair->keyLength == 0 && outPair->key == NULL ) || allocates( outPair->key, outPair->keyLength ) ) );
    CHECK( outPair != NULL ==> ( ( outPair->valueLength == 0 && outPair->value == NULL ) || allocates( outPair->value, outPair->valueLength ) ) );
/* *INDENT-ON* */

    return true;
}

bool JSON_Iterate_ensures( JSONStatus_t result,
                           char * buf,
                           size_t max,
                           JSONPair_t * outPair )
{
    CHECK( isJSONIterateEnum( result ) );

    if( result == JSONSuccess )
    {
/* *INDENT-OFF* */
        CHECK( ( outPair->key != NULL ) ==> ( ( outPair->key > buf ) && ( ( outPair->key + outPair->keyLength ) < ( buf + max ) ) ) );
        CHECK( ( outPair->key != NULL ) ==> ( ( outPair->key + outPair->keyLength ) < outPair->value ) );
/* *INDENT-ON* */
        CHECK( ( outPair->value > buf ) && ( ( outPair->value + outPair->valueLength ) <= ( buf + max ) ) );
        CHECK( isJSONTypesEnum( outPair->jsonType ) );
    }

    return true;
}

bool arraySearch_requires( char * buf,
                           size_t max,
                           size_t * outValue,
                           size_t * outValueLength )
{
    CHECK( 0 < max && max < CBMC_MAX_BUFSIZE );
    CHECK( allocates( buf, max ) );
    CHECK( allocates( outValue, sizeof( *outValue ) ) );
    CHECK( allocates( outValueLength, sizeof( *outValueLength ) ) && *outValueLength <= max );

    return true;
}

bool arraySearch_ensures( bool result,
                          char * buf,
                          size_t max,
                          size_t * outValue,
                          size_t * outValueLength,
                          size_t old_outValue,
                          size_t old_outValueLength )
{
    if( result )
    {
        CHECK( 0 <= *outValue && *outValue < max );
        CHECK( 0 < *outValueLength && *outValueLength <= max - *outValue );
/* *INDENT-OFF* */
        CHECK( buf[ *outValue ] == '"' ==> ( 2 <= *outValueLength && *outValueLength <= max - *outValue ) );
/* *INDENT-ON* */
    }
    else
    {
        CHECK( *outValue == old_outValue );
        CHECK( *outValueLength == old_outValueLength );
    }

    return true;
}

bool skipCollection_requires( char * buf,
                              size_t * start,
                              size_t max )
{
    CHECK( 0 < max && max < CBMC_MAX_BUFSIZE );
    CHECK( allocates( buf, max ) );
    CHECK( allocates( start, sizeof( *start ) ) );

    return true;
}

bool skipCollection_ensures( JSONStatus_t result,
                             char * buf,
                             size_t * start,
                             size_t old_start,
                             size_t max )
{
    CHECK( isSkipCollectionEnum( result ) );
    CHECK( *start >= old_start );

    if( old_start < max )
    {
        CHECK( *start <= max );
    }
    else
    {
        CHECK( *start == old_start );
    }

/* *INDENT-OFF* */
    CHECK( ( result == JSONSuccess ) ==> ( old_start < max ) && ( *start >= old_start + 2 ) );
/* *INDENT-ON* */

    return true;
}

bool skipScalars_requires( char * buf,
                           size_t * start,
                           size_t max,
                           char mode )
{
    CHECK( ( mode == '{' ) || ( mode == '[' ) );

    return skipCollection_requires( buf, start, max );
}

bool skipScalars_ensures( size_t * start,
                          size_t old_start,
                          size_t max )
{
    CHECK( *start >= old_start );

    if( old_start < max )
    {
        CHECK( *start <= max );
    }
    else
    {
        CHECK( *start == old_start );
    }

    return true;
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
    CHECK( isBool( result ) );
    CHECK( *start >= old_start );

    if( old_start < max )
    {
        CHECK( *start <= max );
    }
    else
    {
        CHECK( *start == old_start );
    }

/* *INDENT-OFF* */
    CHECK( result ==> old_start < max && *start > old_start );
    CHECK( ( result && ( buf[ old_start ] == '"' ) ) ==> *start >= old_start + 2 );
/* *INDENT-ON* */

    return true;
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
    CHECK( isBool( result ) );
    CHECK( *start >= old_start );

    if( old_start < max )
    {
        CHECK( *start <= max );
    }
    else
    {
        CHECK( *start == old_start );
    }

/* *INDENT-OFF* */
    CHECK( result ==> ( old_start < max ) && ( *start >= old_start + 2 ) );
/* *INDENT-ON* */

    return true;
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
    CHECK( isBool( result ) );
    CHECK( *start >= old_start );

    if( old_start < max )
    {
        CHECK( *start <= max );
    }
    else
    {
        CHECK( *start == old_start );
    }

/* *INDENT-OFF* */
    CHECK( result ==> ( old_start < max ) && ( *start >= old_start + 1 ) );
/* *INDENT-ON* */

    return true;
}

bool skipDigits_requires( char * buf,
                          size_t * start,
                          size_t max,
                          int32_t * outValue )
{
    CHECK( outValue == NULL || allocates( outValue, sizeof( *outValue ) ) );

    return skipCollection_requires( buf, start, max );
}

bool skipDigits_ensures( bool result,
                         char * buf,
                         size_t * start,
                         size_t old_start,
                         size_t max )
{
    CHECK( isBool( result ) );
    CHECK( *start >= old_start );

    if( old_start < max )
    {
        CHECK( *start <= max );
    }
    else
    {
        CHECK( *start == old_start );
    }

/* *INDENT-OFF* */
    CHECK( result ==> ( old_start < max ) && ( *start > old_start ) && ( ( ( buf[ old_start ] ) >= '0' ) && ( ( buf[ old_start ] ) <= '9' ) ) );
/* *INDENT-ON* */

    return true;
}

#endif /* ifndef CORE_JSON_CONTRACTS_C_ */
