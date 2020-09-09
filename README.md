## coreJSON Library

A JSON parser written in C compliant with the official ECMA-404 - JSON Data Interchange Standard.

## Reference example

```c
#include <stdio.h>
#include "core_json.h"

int main()
{
    // Variables used in this example.
    JSONStatus_t result;
    char buffer[] = "{\"foo\":\"abc\",\"bar\":{\"foo\":\"xyz\"}}";
    size_t bufferLength = sizeof( buffer ) - 1;
    char queryKey[] = "bar.foo";
    size_t queryKeyLength = sizeof( queryKey ) - 1;
    char * value;
    size_t valueLength;
    
    // Calling JSON_Validate() is not necessary if the document is guaranteed to be valid.
    result = JSON_Validate( buffer, bufferLength );
    
    if( result == JSONSuccess )
    {
        result = JSON_Search( buffer, bufferLength, queryKey, queryKeyLength, '.',
                              &value, &valueLength );
    }
    
    if( result == JSONSuccess )
    {
        // The pointer "value" will point to a location in the "buffer".
        char save = value[ valueLength ];
        // After saving the character, set it to a null byte for printing.
        value[ valueLength ] = '\0';
        // "Found: bar.foo -> xyz" will be printed.
        printf( "Found: %s -> %s\n", queryKey, value );
        // Restore the original character.
        value[ valueLength ] = save;
    }

    return 0;
}
```
A search may descend through nested objects when the `queryKey` contains matching key strings joined by a separator (e.g. `.`). In the example above, `bar` has the value `{"foo":"xyz"}`. Therefore, a search for query key `bar.foo` would output `xyz`.

## Building coreJSON

A compiler that supports **C89 or later** such as *gcc* is required to build the library.

For instance, if the example above is copied to a file named `example.c`, *gcc* can be used like so:
```bash
gcc -I source/include example.c source/core_json.c -o example
./example
```

*gcc* can also produce an output file to be linked:
```bash
gcc -I source/include -c source/core_json.c
```

## Generating documentation

The Doxygen references were created using Doxygen version 1.8.20. To generate the
Doxygen pages, please run the following command from the root of this repository:

```shell
doxygen docs/doxygen/config.doxyfile
```

## Building unit tests

### Platform Prerequisites

- For running unit tests
    - C90 compiler like gcc
    - CMake 3.13.0 or later
    - Ruby 2.0.0 or later is additionally required for the CMock test framework (that we use).
- For running the coverage target, gcov is additionally required.

### Steps to build Unit Tests

1. Go to the root directory of this repository.

1. Create build directory: `mkdir build && cd build`

1. Run *cmake* while inside build directory: `cmake -S ../test/unit-test `

1. Run this command to build the library and unit tests: `make all`

1. The generated test executables will be present in `build/bin/tests` folder.

1. Run `ctest` to execute all tests and view the test run summary.
