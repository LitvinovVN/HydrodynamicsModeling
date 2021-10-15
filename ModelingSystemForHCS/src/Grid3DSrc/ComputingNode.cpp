#ifndef COMPUTING_NODE
#define COMPUTING_NODE

/// <summary>
/// Описывает характеристики вычислительного узла
/// </summary>
struct ComputingNode
{
	std::string nodeId;			///< уид вычислительного узла
	double ramMemorySize;		///< размер оперативной памяти вычислительного узла
	double nodePerfomance;		///< производительность вычислительного узла
	std::map<std::string, ComputingDevice&> computingDevices;		///< массив объектов ComputingDevice

	ComputingNode(std::string NodeId, double RamMemorySize, double NodePerfomance) :
		nodeId(NodeId), ramMemorySize(RamMemorySize), nodePerfomance(NodePerfomance)
	{

	}

	// Методы

	/// <summary>
	/// Возвращает количество устройств device на вычислительном узле
	/// </summary>
	/// <returns>Количество устройств device на вычислительном узле</returns>
	int CountDevices()
	{
		int countDevices = 0;
		for (auto itByDevices = computingDevices.begin(); itByDevices != computingDevices.end(); itByDevices++)
		{
			auto nodeKey = itByDevices->first;
			ComputingDevice* nodeObj = &(itByDevices->second);
			countDevices += 1;
		}
		//std::cout << "-------------CountDevices   " << "\tcountDevices = " << countDevices << std::endl;
		return countDevices;
	}

	/// <summary>
	/// Возвращает суммарную производительность всех устройств на вычислительном узле
	/// </summary>
	/// <returns>Суммарную производительность всех устройств на вычислительном узле</returns>
	double GetSumDevicePerformance()
	{
		double sumDevicePerfomance = 0;

		for (auto itByDevices = computingDevices.begin(); itByDevices != computingDevices.end(); itByDevices++)
		{
			auto deviceKey = itByDevices->first;
			auto deviceObj = itByDevices->second;
			sumDevicePerfomance += deviceObj.devicePerfomance;
		}

		return sumDevicePerfomance;
	}


};

#endif