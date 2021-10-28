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
	// ������ ����� CUDA_Devices_Info(). ������ �����: 2
	if (isRun(settings, 2) && settings->isCUDA)
		CUDA_Devices_Info();
	// ������ ����� transferFragmentXOZTesting02(). ������ �����: 3.
	// ���� �������� �������� ������, ������������� � ������� ��������� ����� ����� ���������� �����������, ��������
	if (isRun(settings, 3))
		transferFragmentXOZTesting02();
	// ������ ����� transferFragmentPlaneXOZTesting03(). ������ �����: 4.
	// ���� �������� �������� ������, ������������� � ������� ��������� ����� ����� ���������� �����������, ����� ���������
	if (isRun(settings, 4))
		transferFragmentPlaneXOZTesting03();
	// ������ ����� transferFragmentXOYTesting04(). ������ �����: 5.
	// ���� �������� �������� ������, ������������� � ������� ��������� ����� ����� ���������� �����������, ��������
	if (isRun(settings, 5))
		transferFragmentXOYTesting04();
	// ������ ����� transferFragmentPlaneXOYTesting05(). ������ �����: 6.
	// ���� �������� �������� ������, ������������� � ������� ��������� ����� ����� ���������� �����������, ����� ���������
	if (isRun(settings, 6))
		transferFragmentPlaneXOYTesting05();
	// ������ ����� transferFragmentYOZTesting06(). ������ �����: 7.
	// ���� �������� �������� ������, ������������� � ������� ��������� ����� ����� ���������� �����������, ��������
	if (isRun(settings, 7))
		transferFragmentYOZTesting06();
	// ������ ����� transferFragmentPlaneXOYTesting05(). ������ �����: 8.
	// ���� �������� �������� ������, ������������� � ������� ��������� ����� ����� ���������� �����������, ����� ���������
	if (isRun(settings, 8))
		transferFragmentPlaneYOZTesting07();
}