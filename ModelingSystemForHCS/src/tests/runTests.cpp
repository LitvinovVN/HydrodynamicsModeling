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
	// Запуск теста CUDA_Devices_Info(). Индекс теста: 2
	if (isRun(settings, 2) && settings->isCUDA)
		CUDA_Devices_Info();
	// Запуск теста transferFragmentXOZTesting02(). Индекс теста: 3.
	// Тест скорости передачи данных, расположенных в смежной плоскости между двумя трёхмерными фрагментами, напрямую
	if (isRun(settings, 3))
		transferFragmentXOZTesting02();
	// Запуск теста transferFragmentPlaneXOZTesting03(). Индекс теста: 4.
	// Тест скорости передачи данных, расположенных в смежной плоскости между двумя трёхмерными фрагментами, через плоскость
	if (isRun(settings, 4))
		transferFragmentPlaneXOZTesting03();
	// Запуск теста transferFragmentXOYTesting04(). Индекс теста: 5.
	// Тест скорости передачи данных, расположенных в смежной плоскости между двумя трёхмерными фрагментами, напрямую
	if (isRun(settings, 5))
		transferFragmentXOYTesting04();
	// Запуск теста transferFragmentPlaneXOYTesting05(). Индекс теста: 6.
	// Тест скорости передачи данных, расположенных в смежной плоскости между двумя трёхмерными фрагментами, через плоскость
	if (isRun(settings, 6))
		transferFragmentPlaneXOYTesting05();
	// Запуск теста transferFragmentYOZTesting06(). Индекс теста: 7.
	// Тест скорости передачи данных, расположенных в смежной плоскости между двумя трёхмерными фрагментами, напрямую
	if (isRun(settings, 7))
		transferFragmentYOZTesting06();
	// Запуск теста transferFragmentPlaneXOYTesting05(). Индекс теста: 8.
	// Тест скорости передачи данных, расположенных в смежной плоскости между двумя трёхмерными фрагментами, через плоскость
	if (isRun(settings, 8))
		transferFragmentPlaneYOZTesting07();
}