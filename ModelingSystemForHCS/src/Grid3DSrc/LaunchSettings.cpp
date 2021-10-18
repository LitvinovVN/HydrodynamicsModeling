#ifndef LAUNCH_SETTINGS_CPP
#define LAUNCH_SETTINGS_CPP

/// <summary>
/// Параметры запуска приложения
/// </summary>
struct LaunchSettings {
	int numOfCommandLineParameters = 0;///< Число параметров командной строки
	std::vector<std::string> commandLineParameters;///< Параметры командной строки

	bool isMPI = false;
	int mpi_rank = 0;
	int mpi_size = 0;

	/// <summary>
	/// Конструктор. Инициализация из аргументов командной строки
	/// </summary>
	LaunchSettings(int argc, char* argv[])
	{
		numOfCommandLineParameters = argc;

		std::cout << "LaunchSettings constructor argv[] printing: " << std::endl;
		for (int count = 0; count < argc; ++count)
		{			
			commandLineParameters.push_back(argv[count]);
		}

		isMPI = IsMPI();
	}

	bool IsMPI()
	{
		for (int count = 0; count < numOfCommandLineParameters; ++count)
		{
			if (commandLineParameters[count] == "MPI") return true;
			if (commandLineParameters[count] == "mpi") return true;
		}

		return false;
	}

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
		std::cout << "---------------------- " << std::endl;
	}

};

#endif