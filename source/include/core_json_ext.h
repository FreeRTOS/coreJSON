#ifndef CORE_JSON_EXT_H_
#define CORE_JSON_EXT_H_

#include <stdbool.h>
#include <stddef.h>

#include "core_json.h"

#ifdef __cplusplus
    extern "C" {
#endif

/* queryLength == 0 means "whole document" where stated.
 * Wrong type, number out of range, or output buffer too small
 * returns JSONBadParameter. */

const char * JSON_StatusToString( JSONStatus_t status );
const char * JSON_TypeToString( JSONTypes_t type );

bool JSON_Exists( const char * buf, size_t max,
                  const char * query, size_t queryLength );

JSONStatus_t JSON_GetType( const char * buf, size_t max,
                           const char * query, size_t queryLength,
                           JSONTypes_t * outType );

JSONStatus_t JSON_GetBool( const char * buf, size_t max,
                           const char * query, size_t queryLength,
                           bool * outValue );

JSONStatus_t JSON_GetInt( const char * buf, size_t max,
                          const char * query, size_t queryLength,
                          long long * outValue );

JSONStatus_t JSON_GetDouble( const char * buf, size_t max,
                             const char * query, size_t queryLength,
                             double * outValue );

/* Decodes escapes to UTF-8 and NUL-terminates. *outLength (optional) gets the
 * length without NUL, or the size needed if out is too small. */
JSONStatus_t JSON_GetString( const char * buf, size_t max,
                             const char * query, size_t queryLength,
                             char * out, size_t outCapacity,
                             size_t * outLength );

JSONStatus_t JSON_Count( const char * buf, size_t max,
                         const char * query, size_t queryLength,
                         size_t * outCount );

typedef bool (* JSONVisitor_t)( const JSONPair_t * pair, size_t index, void * context );

JSONStatus_t JSON_ForEach( const char * buf, size_t max,
                           const char * query, size_t queryLength,
                           JSONVisitor_t visitor, void * context );

JSONStatus_t JSON_GetDepth( const char * buf, size_t max, size_t * outDepth );

JSONStatus_t JSON_Minify( const char * buf, size_t max,
                          char * out, size_t outCapacity,
                          size_t * outLength );

JSONStatus_t JSON_Prettify( const char * buf, size_t max,
                            size_t indentWidth,
                            char * out, size_t outCapacity,
                            size_t * outLength );

#ifdef __cplusplus
    }
#endif

#endif /* ifndef CORE_JSON_EXT_H_ */