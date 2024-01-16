// Substitution values
// FN_NAME: The name of the tested CPP function.
// RETURN_TYPE: The return type of the function FN_NAME
// ARGUMENT_TYPES: The types of each argument of FN_NAME in order.
// NUM_TESTS_TO_RUN: The number of test iterations to run

#include "../FILE_ADDRESS"
#include "../Generators/AllGenerators.h"
#include <iostream>
#include <tuple>
#include <algorithm>

// Intentionally missing the comma before OTHER_ARGS
void Test() {
	RETURN_TYPE ReturnedValues[NUM_TESTS_TO_RUN]{};

	std::vector<std::tuple<ARGUMENT_TYPES>> GeneratedArguments; 

	for (int i = 0; i < NUM_TESTS_TO_RUN; i++) {
		// Generate Inputs

		static const size_t NumberOfGenerators = NUM_ARGUMENT_TYPES;
		//void* Generators[NumberOfGenerators] = { Generator<ArgumentTypes...>() };


		// Generate a value for every argument
		//void* GeneratedValues[NumberOfGenerators] = { Generator<ArgumentTypes>::GenerateValue()... };
		std::tuple<GENERATOR_TYPES> Generators;
		//GeneratedArguments[i] = std::tuple<ARGUMENT_TYPES>(GENERATE_ARGUMENTS);
		GeneratedArguments.push_back(std::make_tuple(GENERATORS_GET_VALUES));

		// Replace the named arguments when we call the function.
		
		ReturnedValues[i] = FN_NAME(GENERATED_ARGUMENTS);
	}

	// Check if all values are equal

	if (std::all_of(ReturnedValues, ReturnedValues+NUM_TESTS_TO_RUN, [ReturnedValues](RETURN_TYPE x){ return x == ReturnedValues[0];})) {
		std::cout << "All values are equal to each other" << std::endl;
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

			//std::cout << GeneratedArguments[i];
			std::cout << " | ";
			std::cout << ReturnedValues[i];
			std::cout << "\n";
		}

		// Force a buffer flush.
		std::cout << std::endl;
	}
}

int main(int argc, char** argv) {
	Test();
}