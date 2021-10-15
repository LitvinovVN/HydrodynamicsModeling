#include <iostream>
#include "tests/testFunctions.h"

int main()
{
	std::cout << "Hello CMake." << std::endl;
	
	testFunction1(2, 5);
	
	testFunction4();

	matmAlgsTesting();

	return 0;
}
