## Specification

### Example
```c++
int Negate(int A) { ... }

UnmaskedPredicateTest(std::function<int(int)>(&Negate),
					 [&] (int X_Input) { return X_Input == -ReturnedValue; },
					 {(GeneratorSettings) { .Index = 0, .Fixed = true, .Value = 4} } );
```

This example creates an `UnmaskedPredicateTest`, for an `int` to `int` function called `Negate`, where the predicate is that the result is the negative of the incoming value, and the generators are told that the first generator (in this case, the only generator) should return a fixed value of 4.


