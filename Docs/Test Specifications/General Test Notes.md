If the generator settings were left unspecified, the code generator will choose "sensibly generic" defaults.

For any argument type `A` the function takes, there must exist a generator type `Generator_{A}` unless another type is specified, even if the generated value will not be used. E.g. for an `int`, the associated generator type is `Generator_int`.

