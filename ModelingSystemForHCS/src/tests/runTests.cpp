#include "testFunctions.h"

bool isRun(LaunchSettings* settings, int testNum)
{
	auto tests = settings->testNumbersToLaunch;
	if(tests.size() == 0)
		return true;

	for (auto el : tests)
	{
		if (el == testNum)
			return true;
	}

	return false;
}

/// <summary>
/// ������ ������
/// </summary>
void runTests(LaunchSettings* settings)
{
	if( isRun(settings, 1) )
		matmAlgsTesting();
}