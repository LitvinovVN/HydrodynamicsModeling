#ifndef LAUNCH_SETTINGS_CPP
#define LAUNCH_SETTINGS_CPP

#include <vector>
#include <thread>
#include <mpi.h>

/// <summary>
/// Параметры запуска приложения
/// </summary>
struct LaunchSettings {
	int numOfCommandLineParameters = 0;///< Число параметров командной строки
	std::vector<std::string> commandLineParameters;///< Параметры командной строки

	bool isMPI = false;
	int mpi_rank = 0;
	int mpi_size = 0;
	int mpi_thread_mode = 0;
	int pid = 0;///< УИД процесса
	int hardwareConcurrency = 0;///< Количество аппаратных потоков ЦПУ

	/// <summary>
	/// Конструктор. Инициализация из аргументов командной строки
	/// </summary>
	LaunchSettings(int argc, char* argv[])
	{
		numOfCommandLineParameters = argc;
				
		for (int count = 0; count < argc; ++count)
		{			
			commandLineParameters.push_back(argv[count]);
		}

		isMPI = IsMPI();

		//////////
		if (isMPI)
		{
			// Инициализация MPI			
			MPI_Init_thread(&argc, &argv, MPI_THREAD_FUNNELED, &mpi_thread_mode);
			MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
			MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);			
			MPI_Barrier(MPI_COMM_WORLD);
		}
		//////////

		pid = getpid();
		hardwareConcurrency = std::thread::hardware_concurrency();
	}

	/// <summary>
	/// Проверяет наличие параметра MPI в аргументах командной строки.
	/// </summary>
	bool IsMPI()
	{
		for (int count = 0; count < numOfCommandLineParameters; ++count)
		{
			if (commandLineParameters[count] == "MPI") return true;
			if (commandLineParameters[count] == "mpi") return true;
		}

		return false;
	}

	/// <summary>
	/// Выводит в консоль подробный отчет о содержимом объекта
	/// </summary>
	void Print()
	{
		std::cout << "--- LaunchSettings --- " << std::endl;
		std::cout << "numOfCommandLineParameters: " << numOfCommandLineParameters << std::endl;

		std::cout << "commandLineParameters: " << std::endl;
		for (int count = 0; count < numOfCommandLineParameters; ++count)
		{			
			std::cout << "  " << count << ": " << commandLineParameters[count] << '\n';
		}

		std::cout << "isMPI: " << isMPI << std::endl;
		std::cout << "pid: " << pid << std::endl;
		std::cout << "hardwareConcurrency: " << hardwareConcurrency << std::endl;
		std::cout << "---------------------- " << std::endl;
	}

};

#endif