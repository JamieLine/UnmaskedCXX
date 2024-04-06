Before attempting the parse, consider the incoming C++ / Non-UnmaskedCXX code as ignored. 

Non-terminals are enclosed in `<>`
Quotes are used to describe specific target strings
If a quote is in a target string, it will be escaped (`\"`)
If a token is enclosed in `{}`, it refers to a C++ item.
Start with the symbol `<Full Specification>`

+ `<Full Specfication>` ::= `<Specification>` <`List of Specifications`>
+ `<List of Specifications>` ::= `<Specification>` `< List of Specifications>` | `""`
+ `<Specification>` ::= `<Test>` | `<Set Category>` | `<Set Parameter>`
+ `<Test>` ::= `<Predicate Test>` | `<Stabilising Set Test>` | `<Always Return Value Test>`
+ `<Predicate Test>` ::= "`UnmaskedPredicateTest(`"  `{C++ std::function object, with types explicitly specified}` `{C++ lambda which returns bool}` `<Generator Setting List>`
+ `<Stabilising Set Test>` ::= "`UnmaskedStabilisingSetTest(`" `{C++ std::function object, with types explicitly specfied}` `<Generator Setting List>` "`)`"
+ `<Always Return Value Test>` ::= "`UnmaskedAlwaysReturnValueTest(`" `{C++ std::function object, with types explicitly specified}` `<Generator Setting List>` "`)`"
+ `<Set Category>` ::= "`UnmaskedSetCategory(`" `{C++ String Object or Literal}`"`)`"
+ 