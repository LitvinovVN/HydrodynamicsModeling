#ifndef COMPUTING_CLUSTER
#define COMPUTING_CLUSTER

/// <summary>
/// Описывает характеристики вычислительного кластера
/// </summary>
struct ComputingCluster
{
	std::map<std::string, ComputingNode> computingNodes;	// контейнер объектов computingNodes

	// ћетоды

	/// <summary>
	/// Возвращает количество вычислительных узлов computingNode на кластере
	/// </summary>
	/// <returns> оличество вычислительных узлов computingNode на кластере</returns>
	int CountNodes()
	{
		int countNodes = 0;
		for (auto itByNodes = computingNodes.begin(); itByNodes != computingNodes.end(); itByNodes++)
		{
			auto nodeKey = itByNodes->first;
			ComputingNode* nodeObj = &(itByNodes->second);
			countNodes += 1;
		}
		return countNodes;
	}

	/// <summary>
	/// Возвращает производительность кластера
	/// </summary>
	/// <returns>Производительность кластера</returns>
	double GetClusterPerfomance()
	{
		double sumPerfomance = 0;
		for (auto itByNodes = computingNodes.begin(); itByNodes != computingNodes.end(); itByNodes++)
		{
			auto nodeKey = itByNodes->first;
			auto nodeObj = itByNodes->second;
			sumPerfomance += nodeObj.nodePerfomance;
		}
		return sumPerfomance;
	}

	/// <summary>
	/// Выводит в консоль характеристики кластера (характеристики вычислительных узлов и их устройств)
	/// </summary>
	void Print()
	{
		for (auto itByNodes = computingNodes.begin(); itByNodes != computingNodes.end(); itByNodes++) {	// выводим nodes
			auto nodeKey = itByNodes->first;
			ComputingNode* nodeObj = &(itByNodes->second);
			std::cout << nodeKey << ": " << "\tramMemorySize=" << nodeObj->ramMemorySize
				<< "\tnodePerfomance=" << nodeObj->nodePerfomance << std::endl;

			for (auto itByDevices = nodeObj->computingDevices.begin(); itByDevices != nodeObj->computingDevices.end(); itByDevices++) {	// выводим devices
				auto deviceKey = itByDevices->first;
				ComputingDevice& deviceObj = itByDevices->second;
				deviceObj.PrintDevice();
			}
		}
	}
};

#endif