# Change Log for coreJSON Library

## v3.0.1 (December 2020)
 - [#86]([#84](https://github.com/FreeRTOS/coreJSON/pull/86) Fix MISRA 9.1 volation.
 - [#84](https://github.com/FreeRTOS/coreJSON/pull/84), [#82](https://github.com/FreeRTOS/coreJSON/pull/82) and [#80](https://github.com/FreeRTOS/coreJSON/pull/80) Documentation updates and fixes.

## v3.0.0 (December 2020)
 - [#74](https://github.com/FreeRTOS/coreJSON/pull/74) Add `JSON_Iterate` function to iterate over items in a JSON collection.
 - [#74](https://github.com/FreeRTOS/coreJSON/pull/74) Add `JSONInvalid` enum with the value 0 to `JSONTypes_t`. This change is not backwards compatible.

## v2.0.0 (November 2020)

### Updates
 - [#53](https://github.com/FreeRTOS/coreJSON/pull/53) Update the `JSON_Search` function to support searching JSON arrays. This change is not backwards compatible.

### Other
 - [#35](https://github.com/FreeRTOS/coreJSON/pull/35), [#36](https://github.com/FreeRTOS/coreJSON/pull/36), [#39](https://github.com/FreeRTOS/coreJSON/pull/39), [#51](https://github.com/FreeRTOS/coreJSON/pull/51), [#52](https://github.com/FreeRTOS/coreJSON/pull/52), [#54](https://github.com/FreeRTOS/coreJSON/pull/54) Minor documentation updates.
 - [#40](https://github.com/FreeRTOS/coreJSON/pull/40) Build the unit tests with Unity instead of CMock.
 - [#44](https://github.com/FreeRTOS/coreJSON/pull/44) Add 100% branch coverage to the unit tests.
 - [#46](https://github.com/FreeRTOS/coreJSON/pull/46), [#49](https://github.com/FreeRTOS/coreJSON/pull/49) Fix warnings in the source code.

## v1.0.0 (September 2020)

This is the first release of the coreJSON library, a parser that strictly enforces the [ECMA-404 JSON standard](https://www.json.org/json-en.html) and is suitable for low memory footprint embedded devices.
