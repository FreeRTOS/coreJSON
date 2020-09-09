## JSON Client Library

This repository contains an ISO C90-compliant JSON library that is able to both validate and parse JSON documents.

## Reference Examples

```c
// Variables used in this example.
JSONStatus_t result;
char * buf = "{\"foo\":\"abc\",\"bar\":{\"foo\":\"xyz\"}";
size_t bufLength = sizeof( buf ) - 1;
char * queryKey = "bar.foo";
size_t queryKeyLength = sizeof( queryKey ) - 1;
char * value;
size_t valueLength;
 
// Calling JSON_Validate() is not necessary if the document is guaranteed to be valid.
result = JSON_Validate( buf, bufLength );
 
if( result == JSONSuccess )
{
    result = JSON_Search( buf, bufLength, queryKey, queryKeyLength, '.',
                          &value, &valueLength );
}
 
if( result == JSONSuccess )
{
    char save = value[ valueLength ];
    value[ valueLength ] = '\0';
    // "Found: bar.foo -> xyz" will be printed.
    printf( "Found: %s -> %s\n", queryKey, value );
    value[ valueLength ] = save;
}
```
A search may descend through nested objects when the `queryKey` contains matching key strings joined by a separator (e.g. `.`). In the example above, `bar` has the value `{"foo":"xyz"}`. Therefore, a search for query key `bar.foo` would output `xyz`.


## Building Unit Tests

### Platform Prerequisites

- For building the library, **CMake 3.13.0 or later** and a **C90 compiler**.
- For running unit tests, Ruby 2.0.0 or later is additionally required for the CMock test framework (that we use).
- For running the coverage target, gcov is additionally required.

### Steps to build **Library** and **Unit Tests**

1. Go to the root directory of this repository.

1. Create build directory: `mkdir build && cd build`

1. Run *cmake* while inside build directory: `cmake -S ../test/unit-test `

1. Run this command to build the library and unit tests: `make all`

1. The generated test executables will be present in `build/bin/tests` folder.

1. Run `ctest` to execute all tests and view the test run summary.

## Generating documentation

The Doxygen references were created using Doxygen version 1.8.20. To generate the
Doxygen pages, please run the following command from the root of this repository:

```shell
doxygen docs/doxygen/config.doxyfile
```
