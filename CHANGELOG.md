# Change Log for coreJSON Library

## v3.3.1 (January 2026)
- [#184](https://github.com/FreeRTOS/coreJSON/pull/184) Add write permissions to doxygen generation workflow on main.
- [#182](https://github.com/FreeRTOS/coreJSON/pull/182) Update manifest to list unity as a test dependency.
- [#181](https://github.com/FreeRTOS/coreJSON/pull/181) Add CMakeLists.txt for coreJSON library.
- [#180](https://github.com/FreeRTOS/coreJSON/pull/180) Remove version numbers from headers. Add library version define.
- [#179](https://github.com/FreeRTOS/coreJSON/pull/179) Fix JSON validation issues concerning missing commas, escaped control characters, and hex escaped 0 value.
- [#174](https://github.com/FreeRTOS/coreJSON/pull/174) Remove formatting bot workflow.
- [#173](https://github.com/FreeRTOS/coreJSON/pull/173) Follow Security Guide to update YAML files.
- [#172](https://github.com/FreeRTOS/coreJSON/pull/172) Update GitHub Actions workflows as per security guidelines.
- [#171](https://github.com/FreeRTOS/coreJSON/pull/171) Fix link.
- [#170](https://github.com/FreeRTOS/coreJSON/pull/170) Adjust proof tooling to support CBMC v6.
- [#169](https://github.com/FreeRTOS/coreJSON/pull/169) Treat empty JSON object in key-value pair as valid.
- [#167](https://github.com/FreeRTOS/coreJSON/pull/167) Correctly detect missing key in a key-value pair.
- [#166](https://github.com/FreeRTOS/coreJSON/pull/166) Update LTS 202406 information.
- [#164](https://github.com/FreeRTOS/coreJSON/pull/164) Use CBMC version 5.95.1.

## v3.3.0 (May 2024)
- [#157](https://github.com/FreeRTOS/coreJSON/pull/157) MISRA C:2012 compliance check.
- [#154](https://github.com/FreeRTOS/coreJSON/pull/154) Add ClusterFuzzLite setup.
- [#144](https://github.com/FreeRTOS/coreJSON/pull/144) Upgrade to CBMC Starter Kit 2.10.
- [#134](https://github.com/FreeRTOS/coreJSON/pull/134) Update doxygen version for documentation.
- [#133](https://github.com/FreeRTOS/coreJSON/pull/133) Update Memory safety proofs to use function contracts.

## v3.2.0 (October 2022)
- [#121](https://github.com/FreeRTOS/coreJSON/pull/121) MISRA C:2012 compliance updates.
- [#119](https://github.com/FreeRTOS/coreJSON/pull/119) Update CBMC Starter Kit.
- [#115](https://github.com/FreeRTOS/coreJSON/pull/115) Fix JSON validation for mismatched brackets.
- [#109](https://github.com/FreeRTOS/coreJSON/pull/109) Remove non-ASCII characters

## v3.1.0 (November 2021)
- [#106](https://github.com/FreeRTOS/coreJSON/pull/106) Update doxygen version for documentation.

## v3.0.2 (July 2021)
- [#100](https://github.com/FreeRTOS/coreJSON/pull/100) Fix overflow in skipOneHexEscape().
- [#95](https://github.com/FreeRTOS/coreJSON/pull/95) Eliminate warnings when base char type is unsigned.
- [#93](https://github.com/FreeRTOS/coreJSON/pull/93) Wrap query key separator macro with ifndef.

## v3.0.1 (February 2021)
 - [#86](https://github.com/FreeRTOS/coreJSON/pull/86) Fix MISRA 9.1 violation.
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
