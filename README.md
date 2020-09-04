## JSON Client Library

This repository contains an ISO C90-compliant JSON library that is able to both validate and parse JSON documents.

## Reference examples

#### Validating a JSON document
```c
// Variables used in this example.
JSONStatus_t result;
char * buf = "{\"foo\":\"abc\",\"bar\":{\"foo\":\"xyz\"}";
size_t bufLength = sizeof( buf ) - 1;
 
result = JSON_Validate( buf, bufLength );
 
if( result == JSONSuccess )
{
    // JSON document is valid.
}
else
{
    // JSON document is invalid.
}
```

#### Finding a value from a JSON document given a query key
```c
// Variables used in this example.
JSONStatus_t result;
char * buf = "{\"foo\":\"abc\",\"bar\":{\"foo\":\"xyz\"}";
size_t bufLength = sizeof( buf ) - 1;
char * key = "bar.foo";
size_t keyLength = sizeof( key ) - 1;
char * value;
size_t valueLength;
 
// If you know the JSON document is valid, this call is not necessary.
result = JSON_Validate( buf, bufLength );
 
if( result == JSONSuccess )
{
    result = JSON_Search( buf, bufLength, key, keyLength, '.',
                          &value, &valueLength );
}
 
if( result == JSONSuccess )
{
    char save = value[ valueLength ];
    value[ valueLength ] = '\0';
    // "Found: bar.foo -> xyz\n" will be printed.
    printf( "Found: %s -> %s\n", key, value );
    value[ valueLength ] = save;
}
```


## Building Unit Tests

### Platform Prerequisites

- For building the library, CMake 3.13.0 or later and a C90 compiler.
- For running unit tests, Ruby 2.0.0 or later is additionally required for the CMock test framework (that we use).
- For running the coverage target, gcov is additionally required.

### Steps to build Unit Tests

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
