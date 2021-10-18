#ifndef LAUNCH_SETTINGS_CPP
#define LAUNCH_SETTINGS_CPP

/// <summary>
/// ��������� ������� ����������
/// </summary>
struct LaunchSettings {
	int numOfCommandLineParameters = 0;///< ����� ���������� ��������� ������
	std::vector<std::string> commandLineParameters;///< ��������� ��������� ������

	/// <summary>
	/// �����������. ������������� �� ���������� ��������� ������
	/// </summary>
	LaunchSettings(int argc, char* argv[])
	{
		numOfCommandLineParameters = argc;

		std::cout << "LaunchSettings constructor argv[] printing: " << std::endl;
		for (int count = 0; count < argc; ++count)
		{			
			commandLineParameters.push_back(argv[count]);
		}
			
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
	}

};

#endif