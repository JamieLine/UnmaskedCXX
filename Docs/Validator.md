A planned piece of code

### Architecture 

1. A `Validated<T>` class
	1. Contains just an object of type T passed in from the constructor
	2. The constructor should be `private` and the class should have a `friend` called `Validator`, listed below.
2. A `Validator` class, which just contains a collection of (possibly `static`) methods, all of which take in a `T`, and output an `Optional<Validated<T>>` where the return value exists if and only if the incoming data was valid. `T` should be a type which is produced by the parsing of source, e.g. `ParsedResult<S>` or `ParsedFunction`
3. Replace the direct uses of those `T` types with `Validated<T>` types in code generation.
