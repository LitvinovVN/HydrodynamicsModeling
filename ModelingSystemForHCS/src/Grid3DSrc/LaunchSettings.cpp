#ifndef LAUNCH_SETTINGS_CPP
#define LAUNCH_SETTINGS_CPP

#include <vector>
#include <thread>
#include <mpi.h>

#ifdef _WIN32
#include <Windows.h>
#include <tchar.h>
#elif _WIN64
#include <Windows.h>
#include <tchar.h>
#elif WIN32
#include <Windows.h>
#include <tchar.h>
#elif WIN64
#include <Windows.h>
#include <tchar.h>
#else
// code for linux
#include <unistd.h>// getpid
#endif

/// <summary>
/// Параметры запуска приложения
/// </summary>
struct LaunchSettings {
	int numOfCommandLineParameters = 0;///< Число параметров командной строки
	std::vector<std::string> commandLineParameters;///< Параметры командной строки

	bool isMPI = false;///< Флаг использования MPI
	int mpi_rank = 0;  ///< Ранг процесса MPI
	int mpi_size = 0;  ///< Количество процессов MPI
	int mpi_thread_mode = 0;///< Режим многопоточности MPI
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

		//////// Определение запускаемых тестов //////////
		for (size_t i = 0; i < numOfCommandLineParameters; i++)
		{
			auto parameterString = commandLineParameters[i];
			if (parameterString.find("--tests:") != std::string::npos)
			{
				std::cout << "--tests founded!" << std::endl;
				int parameterStringSize = parameterString.size();
				for (size_t i = 8; i < parameterStringSize; i++)
				{
					std::cout << i << ": " << parameterString[i] << std::endl;

					int start = i;
					int end = i;

					std::string subString = "";
					std::string curChar = std::string{ parameterString[i] };
					while (curChar != "," && i < parameterStringSize)
					{						
						subString.append(curChar);
						i++;
						curChar = std::string{ parameterString[i] };
					}

					std::cout << " subString = " << subString << std::endl;
				}
			}			
		}

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
		if (isMPI && mpi_rank > 0) return;// Не выводим информацию, если MPI и ранг не 0

		std::cout << "--- LaunchSettings --- " << std::endl;
		std::cout << "numOfCommandLineParameters: " << numOfCommandLineParameters << std::endl;

		std::cout << "commandLineParameters: " << std::endl;
		for (int count = 0; count < numOfCommandLineParameters; ++count)
		{			
			std::cout << "  " << count << ": " << commandLineParameters[count] << '\n';
		}

		std::cout << "isMPI: "			 << isMPI			<< std::endl;
		std::cout << "mpi_rank: "		 << mpi_rank		<< std::endl;
		std::cout << "mpi_size: "		 << mpi_size		<< std::endl;
		std::cout << "mpi_thread_mode: " << mpi_thread_mode << std::endl;

		std::cout << "pid: " << pid << std::endl;
		std::cout << "hardwareConcurrency: " << hardwareConcurrency << std::endl;
		std::cout << "---------------------- " << std::endl;
	}

};

#endif