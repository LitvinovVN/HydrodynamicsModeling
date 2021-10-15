#ifndef GRID_BLOCK_3D_BY_NODE
#define GRID_BLOCK_3D_BY_NODE

/// <summary>
/// Трехмерный блок расчетной сетки вычислительного узла
/// </summary>
struct GridBlock3DByNode
{
	size_t nodeNx;			///< количество узлов в блоке вычислительного узла по оси Ox
	size_t nodeNy;			///< количество узлов в блоке вычислительного узла по оси Oy
	size_t nodeNz;			///< количество узлов в блоке вычислительного узла по оси Oz
	size_t nodeIndex;		///< номер блока вычислительного узла
	std::string nodeName;	///< наименование вычислительного узла, обрабатывающего блок
	size_t nodeOffsetY;			///< смещение блока вычислительного узла по оси Oy (относительно узла с координатами (0, 0, 0) глобальной расчетной сетки)
	GridBlock3DByNode* nodePrev;		///< указатель на блок расчетной сетки, обрабатываемый предыдущим вычислительным узлом
	GridBlock3DByNode* nodeNext;		///< указатель на блок расчетной сетки, обрабатываемый следующим вычислительным узлом
	std::vector<std::string> deviceNames;		///< вектор идентификаторов вычислительных устройств
	std::map<std::string, GridBlock3DByDevice> gridBlock3DByDevices;		///< хранилище типа ключ-значение объектов GridBlock3DByDevice

	GridBlock3DByNode(size_t NodeNx, size_t NodeNy, size_t NodeNz, size_t NodeIndex, std::string NodeName, size_t NodeOffsetY,
		GridBlock3DByNode* NodePrev = NULL, GridBlock3DByNode* NodeNext = NULL) :
		nodeNx(NodeNx), nodeNy(NodeNy), nodeNz(NodeNz), nodeIndex(NodeIndex), nodeName(NodeName), nodeOffsetY(NodeOffsetY), nodePrev(NodePrev), nodeNext(NodeNext)
	{

	}

	// Методы

	/// <summary>
	/// Создаёт двумерные плоскости XZ для передачи данных между вычислительными устройствами
	/// и вызывает функцию создания двумерных плоскостей XZ для передачи данных между вычислителями
	/// </summary>
	void CreateGrid2DTransferPlanesXZ()
	{
		// 1. Создаём двумерные плоскости XZ для передачи данных между вычислительными устройствами


		// 2. Вызываем методы создания двумерных плоскостей XZ для передачи данных между вычислителями
		for (auto itByDevices = gridBlock3DByDevices.begin(); itByDevices != gridBlock3DByDevices.end(); itByDevices++)
		{
			auto deviceKey = itByDevices->first;
			GridBlock3DByDevice* deviceObj = &(itByDevices->second);
			deviceObj->CreateGrid2DTransferPlanesXZ();
		}
	}

	/// <summary>
	/// Добавляет в объект двумерной плоскости XZ для передачи данных массив требуемого типа modelDataName
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	void Grid2DTransferPlanesXZLinearArrayCreate(ModelDataName modelDataName)
	{
		// 1. Добавляем массивы для передачи данных между вычислительными устройствами

		// 2. Вызываем методы добавления массивов для передачи данных между вычислителями				
		for (auto itByDevices = gridBlock3DByDevices.begin(); itByDevices != gridBlock3DByDevices.end(); itByDevices++)
		{
			auto deviceKey = itByDevices->first;
			GridBlock3DByDevice* deviceObj = &(itByDevices->second);
			deviceObj->Grid2DTransferPlanesXZLinearArrayCreate(modelDataName);
		}
	}

	/// <summary>
	/// Выводит в консоль объект двумерной плоскости XZ для передачи данных
	/// </summary>	
	/// <param name="deviceName">УИД вычислительного устройства</param>
	/// <param name="transferPlaneXZIndex">Индекс переходного слоя</param>
	/// <param name="fragmentIndX">Индекс фрагмента переходного слоя по OX</param>
	/// <param name="fragmentIndZ">Индекс фрагмента переходного слоя по OZ</param>
	/// <param name="modelDataName">Тип массива данных</param>
	void Grid2DTransferPlanesXZLinearArrayPrint(std::string deviceName,
		size_t transferPlaneXZIndex, size_t fragmentIndX, size_t fragmentIndZ, ModelDataName modelDataName)
	{
		auto* device = GetGridBlock3DByDevice(deviceName);
		device->Grid2DTransferPlanesXZLinearArrayPrint(transferPlaneXZIndex, fragmentIndX, fragmentIndZ, modelDataName);
	}

	/// <summary>
	/// Выполняет декомпозицию блока расчетной сетки вычислительного узла
	/// </summary>
	/// <param name="computingNode">Вычислительный узел</param>
	/// <param name="fragmentsNumX">Число фрагментов по оси OX</param>
	/// <param name="fragmentsNumZ">Число фрагментов по оси OY</param>
	void Decompose(ComputingNode computingNode, size_t fragmentsNumX, size_t fragmentsNumZ)
	{
		size_t indexStart = 0;
		double sumDevicePerfomance = computingNode.GetSumDevicePerformance();
		size_t curDeviceOffsetY = 0;

		int i = 0;
		for (auto itByDevices = computingNode.computingDevices.begin(); itByDevices != computingNode.computingDevices.end(); itByDevices++)
		{
			auto deviceKey = itByDevices->first;
			auto& deviceObj = itByDevices->second;
			if (i < computingNode.CountDevices() - 1)
			{
				size_t deviceNx = nodeNx;
				size_t deviceNy = nodeNy * deviceObj.devicePerfomance / sumDevicePerfomance;
				size_t deviceNz = nodeNz;
				size_t deviceOffsetY = nodeOffsetY + curDeviceOffsetY;
				size_t fragmentsNumY = deviceObj.GetNumThreads();
				GridBlock3DByDevice* gridBlock3DByDevice = new GridBlock3DByDevice(deviceNx, deviceNy, deviceNz, i, deviceKey,
					fragmentsNumX, fragmentsNumY, fragmentsNumZ, curDeviceOffsetY, deviceOffsetY);
				deviceNames.emplace_back(deviceKey);
				gridBlock3DByDevices.emplace(deviceKey, *gridBlock3DByDevice);
				curDeviceOffsetY += deviceNy;
				i += 1;
			}
			else
			{
				size_t deviceNx = nodeNx;
				size_t deviceNy = nodeNy - curDeviceOffsetY;
				size_t deviceNz = nodeNz;
				size_t deviceOffsetY = nodeOffsetY + curDeviceOffsetY;
				size_t fragmentsNumY = deviceObj.GetNumThreads();
				GridBlock3DByDevice* gridBlock3DByDevice = new GridBlock3DByDevice(deviceNx, deviceNy, deviceNz, i, deviceKey,
					fragmentsNumX, fragmentsNumY, fragmentsNumZ, curDeviceOffsetY, deviceOffsetY);
				deviceNames.emplace_back(deviceKey);
				gridBlock3DByDevices.emplace(deviceKey, *gridBlock3DByDevice);
			}
		}
	}

	/// <summary>
	/// $$$$--------------
	/// </summary>
	/// <param name="deviceIndex">$$$$</param>
	/// <returns></returns>
	GridBlock3DByDevice* GetGridBlock3DByDevice(size_t deviceIndex)
	{
		auto deviceName = deviceNames[deviceIndex];
		auto gridBlock3DByDevice = GetGridBlock3DByDevice(deviceName);
		return gridBlock3DByDevice;
	}

	/// <summary>
	/// $$$$--------------
	/// </summary>
	/// <param name="deviceName">$$$$</param>
	/// <returns></returns>
	GridBlock3DByDevice* GetGridBlock3DByDevice(std::string deviceName)
	{
		GridBlock3DByDevice* gridBlock3DByDevice = &(gridBlock3DByDevices.at(deviceName));
		return gridBlock3DByDevice;
	}

	/// <summary>
	/// Устанавливает указатели на соседние блоки
	/// </summary>
	void SetPrevNextPointers()
	{
		GridBlock3DByDevice* prevGridBlock3DByDevice = NULL;
		GridBlock3DByDevice* curGridBlock3DByDevice = NULL;
		for (auto deviceName : deviceNames)
		{
			curGridBlock3DByDevice = GetGridBlock3DByDevice(deviceName);
			curGridBlock3DByDevice->devicePrev = prevGridBlock3DByDevice;
			prevGridBlock3DByDevice = curGridBlock3DByDevice;
		}

		GridBlock3DByDevice* nextGridBlock3DByDevice = NULL;
		for (int nodeIndex = deviceNames.size() - 1; nodeIndex >= 0; nodeIndex--)
		{
			curGridBlock3DByDevice = GetGridBlock3DByDevice(nodeIndex);
			curGridBlock3DByDevice->deviceNext = nextGridBlock3DByDevice;
			nextGridBlock3DByDevice = curGridBlock3DByDevice;
		}

		// Устанавливаем указатели на соседние блоки, рассчитываемые различными устройствами
		//SetPrevNextPointersForDeviceBlocks();
	}

	/// <summary>
	/// Выводит в консоль УИД и наименование вычислительного узла
	/// </summary>
	/// <param name="gridBlock3DByNode">Вычислительный узел</param>
	void PrintNodeName(GridBlock3DByNode* gridBlock3DByNode)
	{
		if (gridBlock3DByNode == NULL)
		{
			std::cout << "(NULL)";
		}
		else
		{
			std::cout << "(" << gridBlock3DByNode->nodeIndex;
			std::cout << ": " << gridBlock3DByNode->nodeName;
			std::cout << ")";
		}
	}

	/// <summary>
	/// $$$$Создание массива данных по имени массива данных modelDataName
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	void LinearArrayCreate(ModelDataName modelDataName)
	{
		for (auto itByDevices = gridBlock3DByDevices.begin(); itByDevices != gridBlock3DByDevices.end(); itByDevices++)
		{
			auto deviceKey = itByDevices->first;
			GridBlock3DByDevice* deviceObj = &(itByDevices->second);
			deviceObj->LinearArrayCreate(modelDataName);
		}
	}

	/// <summary>
	/// $$$$-----------------
	/// </summary>
	/// <param name="data"></param>
	/// <param name="nx"></param>
	/// <param name="ny"></param>
	/// <param name="nz"></param>
	/// <param name="modelDataName"></param>
	void AssignData(double* data, size_t nx, size_t ny, size_t nz, ModelDataName modelDataName)
	{
		for (auto itByDevices = gridBlock3DByDevices.begin(); itByDevices != gridBlock3DByDevices.end(); itByDevices++)
		{
			auto deviceKey = itByDevices->first;
			GridBlock3DByDevice* deviceObj = &(itByDevices->second);
			deviceObj->AssignData(data, nx, ny, nz, modelDataName);
		}
	}

	/// <summary>
	/// $$$$-----------------
	/// </summary>
	/// <param name="data">Массив исходных данных data</param>
	/// <param name="nx">Число узлов расчетной сетки по оси OX</param>
	/// <param name="ny">Число узлов расчетной сетки по оси OY</param>
	/// <param name="nz">Число узлов расчетной сетки по оси OZ</param>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	void Compose(double* data, size_t nx, size_t ny, size_t nz, ModelDataName modelDataName)
	{
		for (auto itByDevices = gridBlock3DByDevices.begin(); itByDevices != gridBlock3DByDevices.end(); itByDevices++)
		{
			auto deviceKey = itByDevices->first;
			GridBlock3DByDevice* deviceObj = &(itByDevices->second);
			deviceObj->Compose(data, nx, ny, nz, modelDataName);
		}
	}

	/// <summary>
	/// Возвращает объём оперативной памяти, занимаемый объектом GridBlock3DByNode
	/// </summary>
	/// <returns>Объём оперативной памяти, занимаемый объектом GridBlock3DByNode</returns>
	double GetDataSizeInMb()
	{
		double result = 0;

		for (auto itByDevices = gridBlock3DByDevices.begin(); itByDevices != gridBlock3DByDevices.end(); itByDevices++)
		{
			auto deviceKey = itByDevices->first;
			GridBlock3DByDevice* deviceObj = &(itByDevices->second);
			result += deviceObj->GetDataSizeInMb();
		}

		return result;
	}

	/// <summary>
	/// $$$$Выводит в консоль сведения о вычислительных узлах по указанному уровню детализации вывода grid3DPrintDetalisation
	/// </summary>
	/// <param name="grid3DPrintDetalisation">Уровень детализации вывода grid3DPrintDetalisation</param>
	void PrintNode(Grid3DPrintDetalisation grid3DPrintDetalisation)
	{
		std::cout << " \t-----------------NODE " << nodeIndex << ": " << nodeName << "-------------------" << std::endl;
		std::cout << " \tnodeNx = " << nodeNx << "; nodeNy = " << nodeNy << "; nodeNz = " << nodeNz << std::endl;
		std::cout << " \tnodeOffsetY = " << nodeOffsetY << std::endl;

		std::cout << "\tnodePrev = ";
		PrintNodeName(nodePrev);
		std::cout << "; nodeNext = ";
		PrintNodeName(nodeNext);
		std::cout << std::endl;

		std::cout << "\tdeviceNames contains " << deviceNames.capacity() << " elements" << std::endl;

		double dataSizeInMb = GetDataSizeInMb();
		std::cout << "\tdataSizeInMb = " << dataSizeInMb << " Mb" << std::endl;

		if (grid3DPrintDetalisation > NODES)
		{
			for (auto deviceName : deviceNames)
			{
				GridBlock3DByDevice* gridBlock3DByDevice = GetGridBlock3DByDevice(deviceName);
				gridBlock3DByDevice->PrintDevice(grid3DPrintDetalisation);
			}
		}
	}
};

#endif