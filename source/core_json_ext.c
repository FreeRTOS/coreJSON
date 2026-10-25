#include <errno.h>
#include <limits.h>
#include <math.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "core_json.h"
#include "core_json_ext.h"

#define EXT_MAX_NUMBER_LENGTH    ( 63U )

static bool isWs( char c )
{
    return ( c == ' ' ) || ( c == '\t' ) || ( c == '\n' ) || ( c == '\r' );
}

/* ---------------- names ---------------- */

const char * JSON_StatusToString( JSONStatus_t status )
{
    const char * s = "Unknown";

    switch( status )
    {
        case JSONPartial:           s = "JSONPartial";           break;
        case JSONSuccess:           s = "JSONSuccess";           break;
        case JSONIllegalDocument:   s = "JSONIllegalDocument";   break;
        case JSONMaxDepthExceeded:  s = "JSONMaxDepthExceeded";  break;
        case JSONNotFound:          s = "JSONNotFound";          break;
        case JSONNullParameter:     s = "JSONNullParameter";     break;
        case JSONBadParameter:      s = "JSONBadParameter";      break;
        default:                    break;
    }

    return s;
}

const char * JSON_TypeToString( JSONTypes_t type )
{
    const char * s = "unknown";

    switch( type )
    {
        case JSONInvalid: s = "invalid"; break;
        case JSONString:  s = "string";  break;
        case JSONNumber:  s = "number";  break;
        case JSONTrue:    s = "true";    break;
        case JSONFalse:   s = "false";   break;
        case JSONNull:    s = "null";    break;
        case JSONObject:  s = "object";  break;
        case JSONArray:   s = "array";   break;
        default:          break;
    }

    return s;
}

/* ---------------- internal: locate a value ---------------- */

static JSONStatus_t resolve( const char * buf, size_t max,
                             const char * query, size_t queryLength,
                             const char ** value, size_t * valueLength,
                             JSONTypes_t * type )
{
    JSONStatus_t ret;

    if( ( buf == NULL ) || ( value == NULL ) || ( valueLength == NULL ) || ( type == NULL ) )
    {
        return JSONNullParameter;
    }

    if( queryLength > 0U )
    {
        return JSON_SearchConst( buf, max, query, queryLength, value, valueLength, type );
    }

    ret = JSON_Validate( buf, max );

    if( ret == JSONSuccess )
    {
        size_t start = 0U;
        size_t end = max;

        while( ( start < max ) && isWs( buf[ start ] ) )
        {
            start++;
        }

        while( ( end > start ) && isWs( buf[ end - 1U ] ) )
        {
            end--;
        }

        switch( buf[ start ] )
        {
            case '"': *type = JSONString; break;
            case '{': *type = JSONObject; break;
            case '[': *type = JSONArray;  break;
            case 't': *type = JSONTrue;   break;
            case 'f': *type = JSONFalse;  break;
            case 'n': *type = JSONNull;   break;
            default:  *type = JSONNumber; break;
        }

        if( *type == JSONString )
        {
            start++;
            end--;
        }

        *value = &buf[ start ];
        *valueLength = end - start;
    }

    return ret;
}

/* ---------------- typed getters ---------------- */

bool JSON_Exists( const char * buf, size_t max,
                  const char * query, size_t queryLength )
{
    const char * v = NULL;
    size_t vl = 0U;
    JSONTypes_t t = JSONInvalid;

    return ( queryLength > 0U ) &&
           ( JSON_SearchConst( buf, max, query, queryLength, &v, &vl, &t ) == JSONSuccess );
}

JSONStatus_t JSON_GetType( const char * buf, size_t max,
                           const char * query, size_t queryLength,
                           JSONTypes_t * outType )
{
    const char * v = NULL;
    size_t vl = 0U;
    JSONTypes_t t = JSONInvalid;
    JSONStatus_t ret;

    if( outType == NULL )
    {
        return JSONNullParameter;
    }

    ret = resolve( buf, max, query, queryLength, &v, &vl, &t );

    if( ret == JSONSuccess )
    {
        *outType = t;
    }

    return ret;
}

JSONStatus_t JSON_GetBool( const char * buf, size_t max,
                           const char * query, size_t queryLength,
                           bool * outValue )
{
    const char * v = NULL;
    size_t vl = 0U;
    JSONTypes_t t = JSONInvalid;
    JSONStatus_t ret;

    if( outValue == NULL )
    {
        return JSONNullParameter;
    }

    ret = resolve( buf, max, query, queryLength, &v, &vl, &t );

    if( ret == JSONSuccess )
    {
        if( t == JSONTrue )
        {
            *outValue = true;
        }
        else if( t == JSONFalse )
        {
            *outValue = false;
        }
        else
        {
            ret = JSONBadParameter;
        }
    }

    return ret;
}

JSONStatus_t JSON_GetInt( const char * buf, size_t max,
                          const char * query, size_t queryLength,
                          long long * outValue )
{
    const char * v = NULL;
    size_t vl = 0U;
    JSONTypes_t t = JSONInvalid;
    JSONStatus_t ret;

    if( outValue == NULL )
    {
        return JSONNullParameter;
    }

    ret = resolve( buf, max, query, queryLength, &v, &vl, &t );

    if( ret != JSONSuccess )
    {
        return ret;
    }

    if( ( t != JSONNumber ) || ( vl == 0U ) )
    {
        return JSONBadParameter;
    }

    {
        size_t i = 0U;
        bool negative = false;
        long long result = 0;

        if( v[ 0 ] == '-' )
        {
            negative = true;
            i = 1U;
        }

        if( i >= vl )
        {
            return JSONBadParameter;
        }

        /* Accumulate as a NEGATIVE number so LLONG_MIN is representable. */
        for( ; i < vl; i++ )
        {
            int digit;

            if( ( v[ i ] < '0' ) || ( v[ i ] > '9' ) )
            {
                return JSONBadParameter; /* '.', 'e' or 'E' => not an integer */
            }

            digit = v[ i ] - '0';

            if( result < ( ( LLONG_MIN + digit ) / 10 ) )
            {
                return JSONBadParameter; /* overflow */
            }

            result = ( result * 10 ) - digit;
        }

        if( negative )
        {
            *outValue = result;
        }
        else
        {
            if( result == LLONG_MIN )
            {
                return JSONBadParameter;
            }

            *outValue = -result;
        }
    }

    return JSONSuccess;
}

JSONStatus_t JSON_GetDouble( const char * buf, size_t max,
                             const char * query, size_t queryLength,
                             double * outValue )
{
    const char * v = NULL;
    size_t vl = 0U;
    JSONTypes_t t = JSONInvalid;
    JSONStatus_t ret;

    if( outValue == NULL )
    {
        return JSONNullParameter;
    }

    ret = resolve( buf, max, query, queryLength, &v, &vl, &t );

    if( ret != JSONSuccess )
    {
        return ret;
    }

    if( ( t != JSONNumber ) || ( vl == 0U ) || ( vl > EXT_MAX_NUMBER_LENGTH ) )
    {
        return JSONBadParameter;
    }

    {
        char tmp[ EXT_MAX_NUMBER_LENGTH + 1U ];
        char * endPtr = NULL;
        double d;

        memcpy( tmp, v, vl );
        tmp[ vl ] = '\0';

        errno = 0;
        d = strtod( tmp, &endPtr );

        if( ( endPtr != &tmp[ vl ] ) || ( errno == ERANGE ) || isinf( d ) || isnan( d ) )
        {
            return JSONBadParameter;
        }

        *outValue = d;
    }

    return JSONSuccess;
}

/* ---------------- string decoding ---------------- */

static int hexVal( char c )
{
    int r = -1;

    if( ( c >= '0' ) && ( c <= '9' ) )
    {
        r = c - '0';
    }
    else if( ( c >= 'a' ) && ( c <= 'f' ) )
    {
        r = ( c - 'a' ) + 10;
    }
    else if( ( c >= 'A' ) && ( c <= 'F' ) )
    {
        r = ( c - 'A' ) + 10;
    }
    else
    {
        /* not hex */
    }

    return r;
}

static long read4Hex( const char * s )
{
    long value = 0;
    size_t i;

    for( i = 0U; i < 4U; i++ )
    {
        int h = hexVal( s[ i ] );

        if( h < 0 )
        {
            return -1;
        }

        value = ( value * 16 ) + h;
    }

    return value;
}

typedef struct
{
    char * out;
    size_t capacity;
    size_t length;
} Writer_t;

static void putc_( Writer_t * w, char c )
{
    if( ( w->out != NULL ) && ( ( w->length + 1U ) < w->capacity ) )
    {
        w->out[ w->length ] = c;
    }

    w->length++;
}

static void putUtf8( Writer_t * w, unsigned long cp )
{
    if( cp < 0x80UL )
    {
        putc_( w, ( char ) cp );
    }
    else if( cp < 0x800UL )
    {
        putc_( w, ( char ) ( 0xC0UL | ( cp >> 6 ) ) );
        putc_( w, ( char ) ( 0x80UL | ( cp & 0x3FUL ) ) );
    }
    else if( cp < 0x10000UL )
    {
        putc_( w, ( char ) ( 0xE0UL | ( cp >> 12 ) ) );
        putc_( w, ( char ) ( 0x80UL | ( ( cp >> 6 ) & 0x3FUL ) ) );
        putc_( w, ( char ) ( 0x80UL | ( cp & 0x3FUL ) ) );
    }
    else
    {
        putc_( w, ( char ) ( 0xF0UL | ( cp >> 18 ) ) );
        putc_( w, ( char ) ( 0x80UL | ( ( cp >> 12 ) & 0x3FUL ) ) );
        putc_( w, ( char ) ( 0x80UL | ( ( cp >> 6 ) & 0x3FUL ) ) );
        putc_( w, ( char ) ( 0x80UL | ( cp & 0x3FUL ) ) );
    }
}

static JSONStatus_t finish( Writer_t * w, size_t * outLength )
{
    JSONStatus_t ret = JSONSuccess;

    if( outLength != NULL )
    {
        *outLength = w->length;
    }

    if( ( w->out == NULL ) || ( ( w->length + 1U ) > w->capacity ) )
    {
        ret = JSONBadParameter;

        if( ( w->out != NULL ) && ( w->capacity > 0U ) )
        {
            w->out[ 0 ] = '\0';
        }
    }
    else
    {
        w->out[ w->length ] = '\0';
    }

    return ret;
}

JSONStatus_t JSON_GetString( const char * buf, size_t max,
                             const char * query, size_t queryLength,
                             char * out, size_t outCapacity,
                             size_t * outLength )
{
    const char * v = NULL;
    size_t vl = 0U;
    JSONTypes_t t = JSONInvalid;
    JSONStatus_t ret;
    Writer_t w;
    size_t i = 0U;

    ret = resolve( buf, max, query, queryLength, &v, &vl, &t );

    if( ret != JSONSuccess )
    {
        return ret;
    }

    if( t != JSONString )
    {
        return JSONBadParameter;
    }

    w.out = out;
    w.capacity = outCapacity;
    w.length = 0U;

    while( i < vl )
    {
        char c = v[ i ];

        if( c != '\\' )
        {
            putc_( &w, c );
            i++;
            continue;
        }

        i++;

        if( i >= vl )
        {
            return JSONIllegalDocument;
        }

        switch( v[ i ] )
        {
            case '"':  putc_( &w, '"' );  i++; break;
            case '\\': putc_( &w, '\\' ); i++; break;
            case '/':  putc_( &w, '/' );  i++; break;
            case 'b':  putc_( &w, '\b' ); i++; break;
            case 'f':  putc_( &w, '\f' ); i++; break;
            case 'n':  putc_( &w, '\n' ); i++; break;
            case 'r':  putc_( &w, '\r' ); i++; break;
            case 't':  putc_( &w, '\t' ); i++; break;

            case 'u':
            {
                long hi;
                unsigned long cp;

                if( ( i + 5U ) > vl )
                {
                    return JSONIllegalDocument;
                }

                hi = read4Hex( &v[ i + 1U ] );

                if( hi < 0 )
                {
                    return JSONIllegalDocument;
                }

                i += 5U;
                cp = ( unsigned long ) hi;

                if( ( cp >= 0xD800UL ) && ( cp <= 0xDBFFUL ) )
                {
                    long lo;

                    if( ( ( i + 6U ) > vl ) || ( v[ i ] != '\\' ) || ( v[ i + 1U ] != 'u' ) )
                    {
                        return JSONIllegalDocument;
                    }

                    lo = read4Hex( &v[ i + 2U ] );

                    if( ( lo < 0xDC00L ) || ( lo > 0xDFFFL ) )
                    {
                        return JSONIllegalDocument;
                    }

                    i += 6U;
                    cp = 0x10000UL + ( ( cp - 0xD800UL ) << 10 ) + ( ( unsigned long ) lo - 0xDC00UL );
                }

                if( cp == 0UL )
                {
                    return JSONBadParameter; /* embedded NUL not allowed */
                }

                putUtf8( &w, cp );
                break;
            }

            default:
                return JSONIllegalDocument;
        }
    }

    return finish( &w, outLength );
}

/* ---------------- collections ---------------- */

static JSONStatus_t resolveCollection( const char * buf, size_t max,
                                       const char * query, size_t queryLength,
                                       const char ** coll, size_t * collLength )
{
    JSONTypes_t t = JSONInvalid;
    JSONStatus_t ret = resolve( buf, max, query, queryLength, coll, collLength, &t );

    if( ret == JSONSuccess )
    {
        if( ( t != JSONObject ) && ( t != JSONArray ) )
        {
            ret = JSONBadParameter;
        }
    }

    return ret;
}

JSONStatus_t JSON_ForEach( const char * buf, size_t max,
                           const char * query, size_t queryLength,
                           JSONVisitor_t visitor, void * context )
{
    const char * coll = NULL;
    size_t collLength = 0U;
    JSONStatus_t ret;
    size_t start = 0U;
    size_t next = 0U;
    size_t index = 0U;
    JSONPair_t pair;

    if( visitor == NULL )
    {
        return JSONNullParameter;
    }

    ret = resolveCollection( buf, max, query, queryLength, &coll, &collLength );

    if( ret != JSONSuccess )
    {
        return ret;
    }

    ret = JSON_Validate( coll, collLength );

    if( ret != JSONSuccess )
    {
        return ret;
    }

    ret = JSON_Iterate( coll, collLength, &start, &next, &pair );

    while( ret == JSONSuccess )
    {
        if( !visitor( &pair, index, context ) )
        {
            return JSONSuccess;
        }

        index++;
        ret = JSON_Iterate( coll, collLength, &start, &next, &pair );
    }

    return ( ret == JSONNotFound ) ? JSONSuccess : ret;
}

static bool countVisitor( const JSONPair_t * pair, size_t index, void * context )
{
    ( void ) pair;
    ( void ) index;
    ( *( size_t * ) context )++;
    return true;
}

JSONStatus_t JSON_Count( const char * buf, size_t max,
                         const char * query, size_t queryLength,
                         size_t * outCount )
{
    size_t count = 0U;
    JSONStatus_t ret;

    if( outCount == NULL )
    {
        return JSONNullParameter;
    }

    ret = JSON_ForEach( buf, max, query, queryLength, countVisitor, &count );

    if( ret == JSONSuccess )
    {
        *outCount = count;
    }

    return ret;
}

JSONStatus_t JSON_GetDepth( const char * buf, size_t max, size_t * outDepth )
{
    JSONStatus_t ret;
    size_t i;
    size_t depth = 0U;
    size_t best = 0U;
    bool inString = false;

    if( outDepth == NULL )
    {
        return JSONNullParameter;
    }

    ret = JSON_Validate( buf, max );

    if( ret != JSONSuccess )
    {
        return ret;
    }

    for( i = 0U; i < max; i++ )
    {
        char c = buf[ i ];

        if( inString )
        {
            if( c == '\\' )
            {
                i++;
            }
            else if( c == '"' )
            {
                inString = false;
            }
            else
            {
                /* ordinary string character */
            }
        }
        else if( c == '"' )
        {
            inString = true;
        }
        else if( ( c == '{' ) || ( c == '[' ) )
        {
            depth++;

            if( depth > best )
            {
                best = depth;
            }
        }
        else if( ( c == '}' ) || ( c == ']' ) )
        {
            depth--;
        }
        else
        {
            /* other token characters */
        }
    }

    *outDepth = best;
    return JSONSuccess;
}

/* ---------------- formatting ---------------- */

JSONStatus_t JSON_Minify( const char * buf, size_t max,
                          char * out, size_t outCapacity,
                          size_t * outLength )
{
    JSONStatus_t ret = JSON_Validate( buf, max );
    Writer_t w;
    size_t i;
    bool inString = false;

    if( ret != JSONSuccess )
    {
        return ret;
    }

    w.out = out;
    w.capacity = outCapacity;
    w.length = 0U;

    for( i = 0U; i < max; i++ )
    {
        char c = buf[ i ];

        if( inString )
        {
            putc_( &w, c );

            if( c == '\\' )
            {
                i++;
                putc_( &w, buf[ i ] );
            }
            else if( c == '"' )
            {
                inString = false;
            }
            else
            {
                /* ordinary string character */
            }
        }
        else if( c == '"' )
        {
            inString = true;
            putc_( &w, c );
        }
        else if( !isWs( c ) )
        {
            putc_( &w, c );
        }
        else
        {
            /* drop whitespace */
        }
    }

    return finish( &w, outLength );
}

static void putIndent( Writer_t * w, size_t level, size_t width )
{
    size_t n = level * width;
    size_t k;

    putc_( w, '\n' );

    for( k = 0U; k < n; k++ )
    {
        putc_( w, ' ' );
    }
}

JSONStatus_t JSON_Prettify( const char * buf, size_t max,
                            size_t indentWidth,
                            char * out, size_t outCapacity,
                            size_t * outLength )
{
    JSONStatus_t ret = JSON_Validate( buf, max );
    Writer_t w;
    size_t i;
    size_t level = 0U;
    bool inString = false;

    if( ret != JSONSuccess )
    {
        return ret;
    }

    w.out = out;
    w.capacity = outCapacity;
    w.length = 0U;

    for( i = 0U; i < max; i++ )
    {
        char c = buf[ i ];

        if( inString )
        {
            putc_( &w, c );

            if( c == '\\' )
            {
                i++;
                putc_( &w, buf[ i ] );
            }
            else if( c == '"' )
            {
                inString = false;
            }
            else
            {
                /* ordinary string character */
            }

            continue;
        }

        switch( c )
        {
            case '"':
                inString = true;
                putc_( &w, c );
                break;

            case '{':
            case '[':
            {
                size_t j = i + 1U;

                while( ( j < max ) && isWs( buf[ j ] ) )
                {
                    j++;
                }

                putc_( &w, c );

                if( ( j < max ) && ( ( buf[ j ] == '}' ) || ( buf[ j ] == ']' ) ) )
                {
                    putc_( &w, buf[ j ] );
                    i = j;
                }
                else
                {
                    level++;
                    putIndent( &w, level, indentWidth );
                }

                break;
            }

            case '}':
            case ']':
                level--;
                putIndent( &w, level, indentWidth );
                putc_( &w, c );
                break;

            case ',':
                putc_( &w, c );
                putIndent( &w, level, indentWidth );
                break;

            case ':':
                putc_( &w, ':' );
                putc_( &w, ' ' );
                break;

            default:
                if( !isWs( c ) )
                {
                    putc_( &w, c );
                }

                break;
        }
    }

    return finish( &w, outLength );
}