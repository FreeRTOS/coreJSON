/*
 * coreJSON v1.0.1
 * Copyright (C) 2020 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
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

/**
 * @file core_json.h
 * @brief Include this header file to use coreJSON in your application.
 */

#ifndef CORE_JSON_H_
#define CORE_JSON_H_

#include <stddef.h>

/**
 * @ingroup json_enum_types
 * @brief Return codes from coreJSON library functions.
 */
typedef enum
{
    JSONPartial = 0,      /**< @brief JSON document is valid so far but incomplete. */
    JSONSuccess,          /**< @brief JSON document is valid and complete. */
    JSONIllegalDocument,  /**< @brief JSON document is invalid or malformed. */
    JSONMaxDepthExceeded, /**< @brief JSON document has nesting that exceeds JSON_MAX_DEPTH. */
    JSONNotFound,         /**< @brief Query key could not be found in the JSON document. */
    JSONNullParameter,    /**< @brief Pointer parameter passed to a function is NULL. */
    JSONBadParameter      /**< @brief Query key is empty, or any subpart is empty, or max is 0. */
} JSONStatus_t;

/**
 * @brief Parse a buffer to determine if it contains a valid JSON document.
 *
 * @param[in] buf  The buffer to parse.
 * @param[in] max  The size of the buffer.
 *
 * @note The maximum nesting depth may be specified by defining the macro
 * JSON_MAX_DEPTH.  The default is 32 of sizeof(char).
 *
 * @note By default, a valid JSON document may contain a single element
 * (e.g., string, boolean, number).  To require that a valid document
 * contain an object or array, define JSON_VALIDATE_COLLECTIONS_ONLY.
 *
 * @return #JSONSuccess if the buffer contents are valid JSON;
 * #JSONNullParameter if buf is NULL;
 * #JSONBadParameter if max is 0;
 * #JSONIllegalDocument if the buffer contents are NOT valid JSON;
 * #JSONMaxDepthExceeded if object and array nesting exceeds a threshold;
 * #JSONPartial if the buffer contents are potentially valid but incomplete.
 *
 * <b>Example</b>
 * @code{c}
 *     // Variables used in this example.
 *     JSONStatus_t result;
 *     char buffer[] = "{\"foo\":\"abc\",\"bar\":{\"foo\":\"xyz\"}}";
 *     size_t bufferLength = sizeof( buffer ) - 1;
 *
 *     result = JSON_Validate( buffer, bufferLength );
 *
 *     // JSON document is valid.
 *     assert( result == JSONSuccess );
 * @endcode
 */
/* @[declare_json_validate] */
JSONStatus_t JSON_Validate( const char * buf,
                            size_t max );
/* @[declare_json_validate] */

/**
 * @brief Find a key or array index in a JSON document and output the
 * pointer @p outValue to its value.
 *
 * Any value may also be an object or an array to a maximum depth.  A search
 * may descend through nested objects or arrays when the query contains matching
 * key strings or array indexes joined by a separator.
 *
 * For example, if the provided buffer contains <code>{"foo":"abc","bar":{"foo":"xyz"}}</code>,
 * then a search for 'foo' would output <code>abc</code>, 'bar' would output
 * <code>{"foo":"xyz"}</code>, and a search for 'bar.foo' would output
 * <code>xyz</code>.
 *
 * If the provided buffer contains <code>[123,456,{"foo":"abc","bar":[88,99]}]</code>,
 * then a search for '[1]' would output <code>456</code>, '[2].foo' would output
 * <code>abc</code>, and '[2].bar[0]' would output <code>88</code>.
 *
 * On success, the pointer @p outValue points to a location in buf.  No null
 * termination is done for the value.  For valid JSON it is safe to place
 * a null character at the end of the value, so long as the character
 * replaced is put back before running another search.
 *
 * @param[in] buf  The buffer to search.
 * @param[in] max  size of the buffer.
 * @param[in] query  The object keys and array indexes to search for.
 * @param[in] queryLength  Length of the key.
 * @param[out] outValue  A pointer to receive the address of the value found.
 * @param[out] outValueLength  A pointer to receive the length of the value found.
 *
 * @note The maximum nesting depth may be specified by defining the macro
 * JSON_MAX_DEPTH.  The default is 32 of sizeof(char).
 *
 * @note JSON_Search() performs validation, but stops upon finding a matching
 * key and its value. To validate the entire JSON document, use JSON_Validate().
 *
 * @return #JSONSuccess if the query is matched and the value output;
 * #JSONNullParameter if any pointer parameters are NULL;
 * #JSONBadParameter if the query is empty, or the portion after a separator is empty,
 * or max is 0, or an index is too large to convert to a signed 32-bit integer;
 * #JSONNotFound if the query has no match.
 *
 * <b>Example</b>
 * @code{c}
 *     // Variables used in this example.
 *     JSONStatus_t result;
 *     char buffer[] = "{\"foo\":\"abc\",\"bar\":{\"foo\":\"xyz\"}}";
 *     size_t bufferLength = sizeof( buffer ) - 1;
 *     char query[] = "bar.foo";
 *     size_t queryLength = sizeof( query ) - 1;
 *     char * value;
 *     size_t valueLength;
 *
 *     // Calling JSON_Validate() is not necessary if the document is guaranteed to be valid.
 *     result = JSON_Validate( buffer, bufferLength );
 *
 *     if( result == JSONSuccess )
 *     {
 *         result = JSON_Search( buffer, bufferLength, query, queryLength,
 *                               &value, &valueLength );
 *     }
 *
 *     if( result == JSONSuccess )
 *     {
 *         // The pointer "value" will point to a location in the "buffer".
 *         char save = value[ valueLength ];
 *         // After saving the character, set it to a null byte for printing.
 *         value[ valueLength ] = '\0';
 *         // "Found: bar.foo -> xyz" will be printed.
 *         printf( "Found: %s -> %s\n", query, value );
 *         // Restore the original character.
 *         value[ valueLength ] = save;
 *     }
 * @endcode
 *
 * @note The maximum index value is ~2 billion ( 2^31 - 9 ).
 */
/* @[declare_json_search] */
JSONStatus_t JSON_Search( char * buf,
                          size_t max,
                          const char * query,
                          size_t queryLength,
                          char ** outValue,
                          size_t * outValueLength );
/* @[declare_json_search] */


/**
 * @brief The largest value usable as an array index in a query
 * for JSON_Search(), ~2 billion.
 */
#define MAX_INDEX_VALUE    ( 0x7FFFFFF7 )   /* 2^31 - 9 */

#endif /* ifndef CORE_JSON_H_ */
