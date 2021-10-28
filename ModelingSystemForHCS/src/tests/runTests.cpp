#include "testFunctions.h"

/// <summary>
/// ���������� ������������� ������� �����.
/// ���� ������ ������ ����, ���������� true
/// (��� ������� ���� ������)
/// </summary>
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
	// ������ ����� matmAlgsTesting(). ������ �����: 1 
	if( isRun(settings, 1) )
		matmAlgsTesting();
	if (isRun(settings, 2) && settings->isCUDA)
		CUDA_Devices_Info();

}