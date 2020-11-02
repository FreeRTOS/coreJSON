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
 * @brief Find a key in a JSON object and output the pointer @p outValue
 * to its value.
 *
 * The JSON document must contain an object (e.g., <code>{"key":"value"}</code>).
 * Any value may also be an object and so forth to a maximum depth.  A search
 * may descend through nested objects when the queryKey contains matching
 * key strings joined by a separator.
 *
 * For example, if buf contains <code>{"foo":"abc","bar":{"foo":"xyz"}}</code>,
 * then a search for 'foo' would output <code>abc</code>, 'bar' would output
 * <code>{"foo":"xyz"}</code>, and a search for 'bar.foo' would output
 * <code>xyz</code> (given separator is specified as '.').
 *
 * On success, the pointer @p outValue points to a location in buf.  No null
 * termination is done for the value.  For valid JSON it is safe to place
 * a null character at the end of the value, so long as the character
 * replaced is put back before running another search.
 *
 * @param[in] buf  The buffer to search.
 * @param[in] max  size of the buffer.
 * @param[in] queryKey  The key to search for.
 * @param[in] queryKeyLength  Length of the key.
 * @param[in] separator  A character between a key and a sub-key in queryKey.
 * @param[out] outValue  A pointer to receive the address of the value found.
 * @param[out] outValueLength  A pointer to receive the length of the value found.
 *
 * @note The maximum nesting depth may be specified by defining the macro
 * JSON_MAX_DEPTH.  The default is 32 of sizeof(char).
 *
 * @note JSON_Search() performs validation, but stops upon finding a matching
 * key and its value. To validate the entire JSON document, use JSON_Validate().
 *
 * @return #JSONSuccess if the queryKey is found and the value output;
 * #JSONNullParameter if any pointer parameters are NULL;
 * #JSONBadParameter if the queryKey is empty, or any subpart is empty, or max is 0;
 * #JSONIllegalDocument if the buffer contents are NOT valid JSON;
 * #JSONMaxDepthExceeded if object and array nesting exceeds a threshold;
 * #JSONNotFound if the queryKey is NOT found.
 *
 * <b>Example</b>
 * @code{c}
 *     // Variables used in this example.
 *     JSONStatus_t result;
 *     char buffer[] = "{\"foo\":\"abc\",\"bar\":{\"foo\":\"xyz\"}}";
 *     size_t bufferLength = sizeof( buffer ) - 1;
 *     char queryKey[] = "bar.foo";
 *     size_t queryKeyLength = sizeof( queryKey ) - 1;
 *     char * value;
 *     size_t valueLength;
 *
 *     // Calling JSON_Validate() is not necessary if the document is guaranteed to be valid.
 *     result = JSON_Validate( buffer, bufferLength );
 *
 *     if( result == JSONSuccess )
 *     {
 *         result = JSON_Search( buffer, bufferLength, queryKey, queryKeyLength, '.',
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
 *         printf( "Found: %s -> %s\n", queryKey, value );
 *         // Restore the original character.
 *         value[ valueLength ] = save;
 *     }
 * @endcode
 */
/* @[declare_json_search] */
JSONStatus_t JSON_Search( char * buf,
                          size_t max,
                          const char * queryKey,
                          size_t queryKeyLength,
                          char separator,
                          char ** outValue,
                          size_t * outValueLength );
/* @[declare_json_search] */

#endif /* ifndef CORE_JSON_H_ */
