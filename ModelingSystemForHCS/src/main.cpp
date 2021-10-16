#include <iostream>
#include "tests/testFunctions.h"
#include "Grid3DSrc/Classes.h"

int main(int argc, char* argv[])
{
	setlocale(LC_CTYPE, "rus"); // вызов функции настройки локали
	std::cout << "Modeling System for HCS" << std::endl;
	std::cout << "argc = " << argc << std::endl;
	for (int count = 0; count < argc; ++count)
		std::cout << count << " " << argv[count] << '\n';
	
	
	app_init(argc, argv);
	
	printTime("Комментарии");
	printTime(); std::cout << std::endl;

	//matmAlgsTesting();

	return 0;
}
