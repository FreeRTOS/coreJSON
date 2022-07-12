# MISRA Compliance

The coreJSON library files conform to the [MISRA C:2012](https://www.misra.org.uk)
guidelines, with some noted exceptions. Compliance is checked with Coverity static analysis.
Deviations from the MISRA standard are listed below:

### Ignored by [Coverity Configuration](tools/coverity/misra.config)
| Deviation | Category | Justification |
| :-: | :-: | :-: |
| Directive 4.9 | Advisory | Allow inclusion of function like macros. |
| Rule 2.5 | Advisory | A macro is not used by the library; however, it exists to be used by an application. |
| Rule 3.1 | Required | Allow nested comments. C++ style `//` comments are used in example code within Doxygen documentation blocks. |
| Rule 8.7 | Advisory | API functions are not used by the library; however, they must be externally visible in order to be used by an application. |
| Rule 8.13 | Advisory | Allow one function to have a char * argument without const qualifier. |
| Rule 12.3 | Advisory | Allow use of `assert()` macro, expansion of which uses comma operator. |
| Rule 15.4 | Advisory | Allow more then one `break` statement to terminate a loop. |
| Rule 15.6 | Required | Allow use of `assert()` macro, expansion of which contains non-compound if statements. |
| Rule 19.2 | Advisory | Allow a `union` of a signed and unsigned type of identical sizes. |
| Rule 20.12 | Required | Allow use of `assert()`, which uses a parameter in both expanded and raw forms. |

### Flagged by Coverity
| Deviation | Category | Justification |
| :-: | :-: | :-: |
| Rule 2.5 | Advisory | A macro is not used by the library; however, it exists to be used by an application. |

### Suppressed with Coverity Comments
| Deviation | Category | Justification |
| :-: | :-: | :-: |
| Rule 11.3 | Required | False positive - the rule permits type qualifiers to be added. |
