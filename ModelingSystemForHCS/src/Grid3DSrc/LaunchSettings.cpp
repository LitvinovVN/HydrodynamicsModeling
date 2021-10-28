#ifndef LAUNCH_SETTINGS_CPP
#define LAUNCH_SETTINGS_CPP

#include <vector>
#include <thread>
#include <mpi.h>
#include <string>

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
	bool isCUDA = false;///< Флаг использования CUDA
	int mpi_rank = 0;  ///< Ранг процесса MPI
	int mpi_size = 0;  ///< Количество процессов MPI
	int mpi_thread_mode = 0;///< Режим многопоточности MPI
	int pid = 0;///< УИД процесса
	int hardwareConcurrency = 0;///< Количество аппаратных потоков ЦПУ

	std::vector<int> testNumbersToLaunch;///< Вектор номеров тестов для запуска
	int numberOfTestLaunches = 10; ///< Число прогонов одного теста

	int gridSizeX = 0; ///< Размер расчетной сетки по оси Ox
	int gridSizeY = 0; ///< Размер расчетной сетки по оси Oy
	int gridSizeZ = 0; ///< Размер расчетной сетки по оси Oz

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
		
		isCUDA = IsCUDA();
		/*if (isCUDA)
		{
			#ifndef CUDA
			#define CUDA
			#endif
		}*/

		pid = getpid();
		hardwareConcurrency = std::thread::hardware_concurrency();

		//////// Определение запускаемых тестов //////////
		for (size_t i = 0; i < numOfCommandLineParameters; i++)
		{
			auto parameterString = commandLineParameters[i];
			if (parameterString.find("--tests=") != std::string::npos)
			{				
				int parameterStringSize = parameterString.size();
				for (size_t i = 8; i < parameterStringSize; i++)
				{
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

					// Проверка на диапазон
					if(subString.find("-") != std::string::npos)
					{
						int j = 0;
						std::string firstNumStr = "";
						curChar = std::string{ subString[0] };
						while (curChar != "-" && j < subString.size())
						{
							firstNumStr.append(curChar);
							j++;
							curChar = std::string{ subString[j] };
						}
						int firstNum = stoi(firstNumStr);						

						j++;
						std::string lastNumStr = "";
						curChar = std::string{ subString[j] };
						while (j < subString.size())
						{
							lastNumStr.append(curChar);
							j++;
							curChar = std::string{ subString[j] };
						}
						int lastNum = stoi(lastNumStr);						

						for (size_t i = firstNum; i <= lastNum; i++)
						{
							testNumbersToLaunch.emplace_back(i);
						}
					}
					else
					{
						int testNumber = stoi(subString);
						testNumbersToLaunch.emplace_back(testNumber);
					}
				}
			}			
		}


		//////// Размеры расчетной сетки
		for (auto parameterString : commandLineParameters)
		{
			if (parameterString.find("--gridSize=") != std::string::npos)
			{				
				auto gridSizeSubstring = parameterString.substr(11);
				//std::cout << "--gridSize parameter: " << gridSizeSubstring << std::endl;

				std::vector<int> gridSizeVector;
				std::string valueStr;
				for (int i = 0; i <= gridSizeSubstring.size(); i++)
				{
					auto curChar = gridSizeSubstring[i];
					if (curChar == 'x' || i == gridSizeSubstring.size())
					{
						int gridSizeByAxis = stoi(valueStr);
						gridSizeVector.emplace_back(gridSizeByAxis);
						valueStr.clear();
					}
					else
					{
						valueStr.append(1, curChar);
					}					
				}

				auto gridSizeVectorSize = gridSizeVector.size();
				if (gridSizeVectorSize > 0)
					gridSizeX = gridSizeVector[0];
				if (gridSizeVectorSize > 1)
					gridSizeY = gridSizeVector[1];
				if (gridSizeVectorSize > 2)
					gridSizeZ = gridSizeVector[2];
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
	/// Проверяет наличие параметра CUDA в аргументах командной строки.
	/// </summary>
	bool IsCUDA()
	{
		for (int count = 0; count < numOfCommandLineParameters; ++count)
		{
			if (commandLineParameters[count] == "CUDA") return true;
			if (commandLineParameters[count] == "cuda") return true;
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
		std::cout << "isCUDA: "			 << isCUDA			 << std::endl;
		std::cout << "mpi_rank: "		 << mpi_rank		<< std::endl;
		std::cout << "mpi_size: "		 << mpi_size		<< std::endl;
		std::cout << "mpi_thread_mode: " << mpi_thread_mode << std::endl;

		std::cout << "pid: " << pid << std::endl;
		std::cout << "hardwareConcurrency: " << hardwareConcurrency << std::endl;

		std::cout << "testNumbersToLaunch: ";
		for (size_t i = 0; i < testNumbersToLaunch.size(); i++)
		{
			std::cout << testNumbersToLaunch[i] << "; ";
		}
		std::cout << std::endl;

		std::cout << "numberOfTestLaunches: " << numberOfTestLaunches << std::endl;

		std::cout << "gridSizeX: " << gridSizeX << std::endl;
		std::cout << "gridSizeY: " << gridSizeY << std::endl;
		std::cout << "gridSizeZ: " << gridSizeZ << std::endl;

		std::cout << "---------------------- " << std::endl;
	}

};

#endif