#include <iostream>
#include <thread>

void hello()
{
	std::cout << "Hello threads!" << std::endl;
}

/// <summary>
/// ������������ ����������� ������� ������� ������ ���� ��� � ������������� ������
/// </summary>
void matmAlgsTestingMultithreading()
{
	int threads = 2;
	std::thread t1(hello);
	std::thread t2(hello);
	t1.join();
	t2.join();
}