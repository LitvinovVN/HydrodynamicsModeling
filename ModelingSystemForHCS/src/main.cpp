#include <iostream>
#include "tests/testFunctions.h"
#include "Grid3DSrc/Classes.h"


int main(int argc, char* argv[])
{
	LaunchSettings* settings = app_init(argc, argv);
	settings->Print();

	runTests(settings);
		
	if (settings->isMPI) MPI_Finalize();
	return 0;
}
