
# MISRA Compliance

The coreJSON library files conform to the [MISRA C:2012](https://www.misra.org.uk/MISRAHome/MISRAC2012/tabid/196/Default.aspx)
guidelines, with some noted exceptions. Compliance is checked with Coverity static analysis.
Deviations from the MISRA standard are listed below:

### Ignored by [Coverity Configuration](tools/coverity/misra.config)
| Deviation | Category | Justification |
| :-: | :-: | :-: |
| Directive 4.9 | Advisory | Allow inclusion of function like macros. |
| Rule 3.1 | Required | Allow nested comments. C++ style `//` comments are used in example code within Doxygen documentation blocks. |
| Rule 15.4 | Advisory | Allow more then one `break` statement to terminate a loop. |
| Rule 19.2 | Advisory | Allow a `union` of a signed and unsigned type of identical sizes. |
| Rule 20.12 | Required | Allow use of `assert()`, which uses a parameter in both expanded and raw forms. |

### Flagged by Coverity
| Deviation | Category | Justification |
| :-: | :-: | :-: |
| Rule 8.7 | Advisory | API functions are not used by the library; however, they must be externally visible in order to be used by an application. |

### Suppressed with Coverity Comments
*None.*
