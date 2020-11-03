# Change Log for coreJSON Library

## v2.0.0 (November 2020)

### Updates
 - [#53](https://github.com/FreeRTOS/coreJSON/pull/53) Update the `JSON_Search` function to support searching JSON arrays. This change is not backwards compatible.

### Other
 - [#35](https://github.com/FreeRTOS/coreJSON/pull/35), [#36](https://github.com/FreeRTOS/coreJSONpull/36), [#39](https://github.com/FreeRTOS/coreJSON/pull/39), [#51](https://github.com/FreeRTOS/coreJSON/pull/51), [#52](https://github.com/FreeRTOS/coreJSON/pull/52), [#54](https://github.com/FreeRTOS/coreJSON/pull/54) Minor documentation updates.
 - [#40](https://github.com/FreeRTOS/coreJSON/pull/40) Build the unit tests with Unity instead of CMock.
 - [#44](https://github.com/FreeRTOS/coreJSON/pull/44) Add 100% branch coverage to the unit tests.
 - [#46](https://github.com/FreeRTOS/coreJSON/pull/46), [#49](https://github.com/FreeRTOS/coreJSON/pull/49) Fix warnings in the source code.

## v1.0.0 (September 2020)

This is the first release of the coreJSON library, a parser that strictly enforces the [ECMA-404 JSON standard](https://www.json.org/json-en.html) and is suitable for low memory footprint embedded devices.