# MISRA Compliance

The coreJSON library files conform to the [MISRA C:2012](https://www.misra.org.uk)
guidelines, with some noted exceptions. Compliance is checked with Coverity static analysis.
The specific deviations, suppressed inline, are listed below.

Additionally, [MISRA configuration file](https://github.com/FreeRTOS/coreJSON/blob/main/tools/coverity/misra.config) contains the project wide deviations.

### Suppressed with Coverity Comments
To find the violation references in the source files run grep on the source code
with ( Assuming rule 11.4 violation; with justification in point 2 ):
```
grep 'MISRA Ref 11.4.2' . -rI

#### Rule 11.3
_Ref 11.3.1_

- MISRA C-2012 Rule 11.3 prohibits casting a pointer to a different type.
        This instance is a false positive, as the rule permits the
        addition of a type qualifier.

#### Rule 14.3
_Ref 14.3.1_

- MISRA C-2012 Rule 14.3 False positive as the value might be changed
        depending on the conditionally compiled code
