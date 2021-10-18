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
	
	//////////
	int rank, size, provided;
	mpi_init(argc, argv, MPI_THREAD_FUNNELED, provided, rank, size);
	MPI_Barrier(MPI_COMM_WORLD);		
	//////////



	app_init(argc, argv);
		
	printTime(); std::cout << std::endl;

	matmAlgsTesting();


	MPI_Finalize();
	return 0;
}
