// Substitution values
// LinearCombination: The name of the tested CPP function.
// int: The return type of the function LinearCombination
// int,int,int,int: The types of each argument of LinearCombination in order.
// 10: The number of test iterations to run

#include "../SampleCXXProject/Calculations.h"
#include "../Generators/AllGenerators.h"
#include <iostream>
#include <tuple>
#include <algorithm>

// Intentionally missing the comma before OTHER_ARGS
// Returns true if and only if the contained test passed.
bool SampleCXXProject_Calculations3() {
	int ReturnedValues[10]{};

	std::vector<std::tuple<int,int,int,int>> GeneratedArguments; 

	for (int i = 0; i < 10; i++) {
		// Generate Inputs

		static const size_t NumberOfGenerators = 4;
		//void* Generators[NumberOfGenerators] = { Generator<ArgumentTypes...>() };


		// Generate a value for every argument
		//void* GeneratedValues[NumberOfGenerators] = { Generator<ArgumentTypes>::GenerateValue()... };
		std::tuple<Generator_int,Generator_int,Generator_int,Generator_int> Generators;
		//GeneratedArguments[i] = std::tuple<int,int,int,int>(GENERATE_ARGUMENTS);
		GeneratedArguments.push_back(std::make_tuple(std::get<0>(Generators).GenerateValue(),std::get<1>(Generators).GenerateValue(),std::get<2>(Generators).GenerateValue(),std::get<3>(Generators).GenerateValue()));

		// Replace the named arguments when we call the function.
		
		ReturnedValues[i] = LinearCombination(std::get<0>(GeneratedArguments[i]),0,std::get<2>(GeneratedArguments[i]),0);
	}

	// Check if all values are equal

	if (std::all_of(ReturnedValues, ReturnedValues+10, [ReturnedValues](int x){ return x == ReturnedValues[0];})) {
		std::cout << "All values are equal to each other" << std::endl;
		return true;
	}

	else {
		std::cout << "Test failed\n";
		std::cout << "Printing results in format\n";
		std::cout << "TEST_NUMBER | INPUT_ARGS_AS_TUPLE | RETURN_VALUE\n";
		for (int i = 0; i < 10; i++) {
			std::cout << i;
			std::cout << " | ";
			// Intentionally de-indented this token to make generation easier.

			std::cout << std::get<0>(GeneratedArguments[i]); 
 			std::cout << ",";
			std::cout << 0;
 			std::cout << ",";
			std::cout << std::get<2>(GeneratedArguments[i]); 
 			std::cout << ",";
			std::cout << 0;
 			std::cout << ",";

			//std::cout << GeneratedArguments[i];
			std::cout << " | ";
			std::cout << ReturnedValues[i];
			std::cout << "\n";
		}

		// Force a buffer flush.
		std::cout << std::endl;
		return false;
	}
}

/*int main(int argc, char** argv) {
	SampleCXXProject_Calculations3();
}*/