// Substitution values
// FN_NAME: The name of the tested CPP function.
// RETURN_TYPE: The return type of the function FN_NAME
// ARGUMENT_TYPES: The types of each argument of FN_NAME in order.
// NUM_TESTS_TO_RUN: The number of test iterations to run

#include "../FILE_ADDRESS"
#include "../Generators/AllGenerators.h"
#include "../Generators/GeneratorParameterStore.h"
#include <iostream>
#include <tuple>
#include <algorithm>

// Returns true if and only if the contained test passed.
bool TEST_FN_NAME() {
	GeneratorParameterStore Parameters;

	RETURN_TYPE ReturnedValues[NUM_TESTS_TO_RUN]{};

	std::vector<std::tuple<ARGUMENT_TYPES>> GeneratedArguments; 

	for (int i = 0; i < NUM_TESTS_TO_RUN; i++) {
		// Generate a value for every argument
		std::tuple<GENERATOR_TYPES> Generators;

// Intentionally un-indented
PUSH_PARAMETERS_TO_STORE

		GeneratedArguments.push_back(std::make_tuple(GENERATORS_GET_VALUES));

		// Replace the named arguments when we call the function.
		
		ReturnedValues[i] = FN_NAME(GENERATED_ARGUMENTS);

		Parameters.ClearEverything();
	}

	// Check if all values are equal
	if (std::all_of(ReturnedValues, ReturnedValues+NUM_TESTS_TO_RUN, [](RETURN_TYPE x){ return x == TARGET_VALUE;})) {
		std::cout << "All values are equal to the target of TARGET_VALUE" << std::endl;
		return true;
	}

	else {
		std::cout << "Test failed\n";
		std::cout << "Printing results in format\n";
		std::cout << "TEST_NUMBER | INPUT_ARGS_AS_TUPLE | RETURN_VALUE\n";
		for (int i = 0; i < NUM_TESTS_TO_RUN; i++) {
			std::cout << i;
			std::cout << " | ";
			// Intentionally de-indented this token to make generation easier.

COUT_EACH_ARGUMENT_IN_TURN

			std::cout << " | ";
			std::cout << ReturnedValues[i];
			std::cout << "\n";
		}

		// Force a buffer flush.
		std::cout << std::endl;
		return false;
	}
}