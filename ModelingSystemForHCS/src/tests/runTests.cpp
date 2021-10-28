#include "testFunctions.h"

/// <summary>
/// Определяет необходимость запуска теста.
/// Если список тестов пуст, возвращает true
/// (для запуска всех тестов)
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
/// Запуск тестов
/// </summary>
void runTests(LaunchSettings* settings)
{
	// Запуск теста matmAlgsTesting(). Индекс теста: 1 
	if( isRun(settings, 1) )
		matmAlgsTesting();
	if (isRun(settings, 2) && settings->isCUDA)
		CUDA_Devices_Info();

}