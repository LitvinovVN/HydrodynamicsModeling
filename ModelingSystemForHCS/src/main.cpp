#include <iostream>
#include "tests/testFunctions.h"
#include "Grid3DSrc/Classes.h"


int main(int argc, char* argv[])
{
	setlocale(LC_CTYPE, "rus"); // вызов функции настройки локали
	std::cout << "Modeling System for HCS";
	std::cout << " (launched at ";
	printTime();
	std::cout << ")";
	std::cout << std::endl;
	
	LaunchSettings settings{argc, argv};
	settings.Print();

	//////////
	if (settings.isMPI)
	{		
		int rank, size, provided;
		mpi_init(argc, argv, MPI_THREAD_FUNNELED, provided, rank, size);
		MPI_Barrier(MPI_COMM_WORLD);		
	}
	//////////
	
	



	app_init(argc, argv);
		
	printTime(); std::cout << std::endl;

	matmAlgsTesting();


	if (settings.isMPI) MPI_Finalize();
	return 0;
}
