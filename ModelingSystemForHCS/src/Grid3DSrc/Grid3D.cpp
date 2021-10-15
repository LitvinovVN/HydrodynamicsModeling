#ifndef GRID_3D
#define GRID_3D

/// <summary>
/// Трехмерная расчетная сетка
/// </summary>
struct Grid3D
{
	size_t gridNx;		///< Количество узлов расчетной сетки по оси Ox
	size_t gridNy;		///< Количество узлов расчетной сетки по оси Oy
	size_t gridNz;		///< Количество узлов расчетной сетки по оси Oz
	double hx;			///< Шаг расчетной сетки по оси Ox
	double hy;			///< Шаг расчетной сетки по оси Oy
	double hz;			///< Шаг расчетной сетки по оси Oz
	std::vector<std::string> nodeNames;		///< Вектор идентификаторов вычислительных узлов
	std::map<std::string, GridBlock3DByNode> gridBlock3DByNodes;	///< Хранилище типа ключ-значение объектов GridBlock3DByNode

	Grid3D(size_t GridNx, size_t GridNy, size_t GridNz, double Hx, double Hy, double Hz) :
		gridNx(GridNx), gridNy(GridNy), gridNz(GridNz), hx(Hx), hy(Hy), hz(Hz)
	{

	}

	// Методы

	/// <summary>
	/// Создаёт двумерные плоскости XZ для передачи данных между вычислителями
	/// </summary>
	void CreateGrid2DTransferPlanesXZ()
	{
		// 1. Создаём двумерные плоскости XZ для передачи данных между вычислительными узлами

		// 2. Вызываем методы создания двумерных плоскостей XZ для передачи данных
		//    между вычислительными устройствами и между вычислителями				
		for (auto itByNodes = gridBlock3DByNodes.begin(); itByNodes != gridBlock3DByNodes.end(); itByNodes++)
		{
			auto nodeKey = itByNodes->first;
			GridBlock3DByNode* nodeObj = &(itByNodes->second);
			nodeObj->CreateGrid2DTransferPlanesXZ();
		}
	}

	/// <summary>
	/// Добавляет в объект двумерной плоскости XZ для передачи данных массив требуемого типа
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	void Grid2DTransferPlanesXZLinearArrayCreate(ModelDataName modelDataName)
	{
		// 1. Добавляем массивы для передачи данных между вычислительными узлами

		// 2. Вызываем методы добавления массивов для передачи данных
		//    между вычислительными устройствами и между вычислителями				
		for (auto itByNodes = gridBlock3DByNodes.begin(); itByNodes != gridBlock3DByNodes.end(); itByNodes++)
		{
			auto nodeKey = itByNodes->first;
			GridBlock3DByNode* nodeObj = &(itByNodes->second);
			nodeObj->Grid2DTransferPlanesXZLinearArrayCreate( modelDataName);
		}
	}

	/// <summary>
	/// Выводит в консоль объект двумерной плоскости XZ для передачи данных
	/// </summary>
	/// <param name="nodeName">УИД узла кластера</param>
	/// <param name="deviceName">УИД вычислительного устройства</param>
	/// <param name="transferPlaneXZIndex">Индекс переходного слоя</param>
	/// <param name="fragmentIndX">Индекс фрагмента переходного слоя по OX</param>
	/// <param name="fragmentIndZ">Индекс фрагмента переходного слоя по OZ</param>
	/// <param name="modelDataName">Тип массива данных</param>
	void Grid2DTransferPlanesXZLinearArrayPrint(std::string nodeName, std::string deviceName,
		size_t transferPlaneXZIndex, size_t fragmentIndX, size_t fragmentIndZ,  ModelDataName modelDataName)
	{
		auto* node = GetGridBlock3DByNode(nodeName);
		node->Grid2DTransferPlanesXZLinearArrayPrint(deviceName,
			transferPlaneXZIndex, fragmentIndX, fragmentIndZ, modelDataName);
	}

	/// <summary>
	/// $$$$-----------
	/// </summary>
	/// <param name="nodeIndex">$$$$</param>
	/// <returns>$$$$</returns>
	GridBlock3DByNode* GetGridBlock3DByNode(size_t nodeIndex)
	{
		auto nodeName = nodeNames[nodeIndex];
		GridBlock3DByNode* gridBlock3DByNode = GetGridBlock3DByNode(nodeName);
		return gridBlock3DByNode;
	}

	/// <summary>
	/// $$$$----------------
	/// </summary>
	/// <param name="nodeName">$$$$</param>
	/// <returns>$$$$</returns>
	GridBlock3DByNode* GetGridBlock3DByNode(std::string nodeName)
	{
		GridBlock3DByNode* gridBlock3DByNode = &(gridBlock3DByNodes.at(nodeName));
		return gridBlock3DByNode;
	}

	/// <summary>
	/// Выполняет декомпозицию расчетной сетки: делит сетку Grid на блоки для выч.устройств кластера (Nodes)
	/// </summary>
	/// <param name="cluster">$$$$</param>
	/// <param name="fragmentsNumX">$$$$</param>
	/// <param name="fragmentsNumZ">$$$$</param>
	void Decompose(ComputingCluster cluster, size_t fragmentsNumX, size_t fragmentsNumZ)
	{
		double sumNyNode = 0;
		double sumPerfomance = cluster.GetClusterPerfomance();
		size_t nodeOffsetY = 0;	

		// Распределение по узлам кластера
		auto itByNodes = cluster.computingNodes.begin();
		int i = 0;
		for (auto itByNodes = cluster.computingNodes.begin(); itByNodes != cluster.computingNodes.end(); itByNodes++)
		{
			auto nodeKey = itByNodes->first;
			auto nodeObj = itByNodes->second;
			if (i < cluster.CountNodes() - 1)
			{
				size_t nodeNx = gridNx;
				size_t nodeNy = gridNy * nodeObj.nodePerfomance / sumPerfomance;
				size_t nodeNz = gridNz;
				sumNyNode += nodeNy;
				GridBlock3DByNode gridBlock3DByNode(nodeNx, nodeNy, nodeNz, i, nodeKey, nodeOffsetY);
				gridBlock3DByNode.Decompose(nodeObj, fragmentsNumX, fragmentsNumZ);
				nodeNames.emplace_back(nodeKey);
				gridBlock3DByNodes.emplace(nodeKey, std::ref(gridBlock3DByNode));
				nodeOffsetY = nodeOffsetY + nodeNy;
				i += 1;
			}
			else
			{
				size_t nodeNx = gridNx;
				size_t nodeNy = gridNy - sumNyNode;
				size_t nodeNz = gridNz;
				GridBlock3DByNode gridBlock3DByNode(nodeNx, nodeNy, nodeNz, i, nodeKey, nodeOffsetY);
				gridBlock3DByNode.Decompose(nodeObj, fragmentsNumX, fragmentsNumZ);
				nodeNames.emplace_back(nodeKey);
				gridBlock3DByNodes.emplace(nodeKey, std::ref(gridBlock3DByNode));

			}

		}

		// Устанавливаем указатели на соседние блоки
		SetPrevNextPointers();
	}
	
	// Устанавливает указатели на соседние блоки
	void SetPrevNextPointers()
	{
		GridBlock3DByNode* prevGridBlock3DByNode = NULL;
		GridBlock3DByNode* curGridBlock3DByNode = NULL;
		for (auto nodeName : nodeNames)
		{			
			curGridBlock3DByNode = GetGridBlock3DByNode(nodeName);
			curGridBlock3DByNode->nodePrev = prevGridBlock3DByNode;
			prevGridBlock3DByNode = curGridBlock3DByNode;
		}

		GridBlock3DByNode* nextGridBlock3DByNode = NULL;
		for (int nodeIndex = nodeNames.size() - 1; nodeIndex >= 0 ; nodeIndex--)
		{			
			curGridBlock3DByNode = GetGridBlock3DByNode(nodeIndex);
			curGridBlock3DByNode->nodeNext = nextGridBlock3DByNode;
			nextGridBlock3DByNode = curGridBlock3DByNode;
		}

		// Устанавливаем указатели на соседние блоки, рассчитываемые различными устройствами
		SetPrevNextPointersForDeviceBlocks();
	}
	
	/// <summary>
	/// $$$$------------------
	/// </summary>
	void SetPrevNextPointersForDeviceBlocks()
	{
		GridBlock3DByNode* curGridBlock3DByNode = NULL;
		for (auto nodeName : nodeNames)
		{
			curGridBlock3DByNode = GetGridBlock3DByNode(nodeName);
			curGridBlock3DByNode->SetPrevNextPointers();
		}
	}

	/// <summary>
	/// $$$$-----------------
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	void LinearArrayCreate(ModelDataName modelDataName)
	{
		for (auto itByNodes = gridBlock3DByNodes.begin(); itByNodes != gridBlock3DByNodes.end(); itByNodes++)
		{
			auto nodeKey = itByNodes->first;
			GridBlock3DByNode* nodeObj = &(itByNodes->second);
			nodeObj->LinearArrayCreate(modelDataName);
		}
	}

	/// <summary>
	/// $$$$--------------
	/// </summary>
	/// <param name="data">Массив исходных данных data</param>
	/// <param name="nx">Число узлов расчетной сетки по оси OX</param>
	/// <param name="ny">Число узлов расчетной сетки по оси OY</param>
	/// <param name="nz">Число узлов расчетной сетки по оси OZ</param>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	void AssignData(double* data, size_t nx, size_t ny, size_t nz, ModelDataName modelDataName)
	{
		for (auto itByNodes = gridBlock3DByNodes.begin(); itByNodes != gridBlock3DByNodes.end(); itByNodes++)
		{
			auto nodeKey = itByNodes->first;
			GridBlock3DByNode* nodeObj = &(itByNodes->second);
			nodeObj->AssignData(data, nx, ny, nz, modelDataName);
		}
	}

	/// <summary>
	/// Выполняет сборку элементов массива, разбросанных по узлам расчетной сетки
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	/// <returns>Одномерный массив исходных данных data</returns>
	double* Compose(ModelDataName modelDataName)	
	{
		long ram = sizeof(double) * gridNx * gridNy * gridNz;
		double* data = (double*)malloc(ram);
		
		for (auto itByNodes = gridBlock3DByNodes.begin(); itByNodes != gridBlock3DByNodes.end(); itByNodes++)
		{
			auto nodeKey = itByNodes->first;
			GridBlock3DByNode* nodeObj = &(itByNodes->second);
			nodeObj->Compose(data, gridNx, gridNy, gridNz, modelDataName);
		}

		return data;
	}

	/// <summary>
	/// Возвращает объём оперативной памяти, занимаемый объектом Grid3D
	/// </summary>
	/// <returns>Объём оперативной памяти, занимаемый объектом Grid3D</returns>
	double GetDataSizeInMb()
	{
		double result = 0;

		for (auto itByNodes = gridBlock3DByNodes.begin(); itByNodes != gridBlock3DByNodes.end(); itByNodes++)
		{
			auto nodeKey = itByNodes->first;
			GridBlock3DByNode* nodeObj = &(itByNodes->second);
			result += nodeObj->GetDataSizeInMb();
		}

		return result;
	}

	/// <summary>
	/// Выводит в консоль параметры расчетной сетки
	/// </summary>
	/// <param name="grid3DPrintDetalisation">Уровень детализации вывода grid3DPrintDetalisation</param>
	void Print(Grid3DPrintDetalisation grid3DPrintDetalisation)
	{
		std::cout << "-----------------GRID-------------------" << std::endl;
		std::cout << "gridNx = " << gridNx << "; gridNy = " << gridNy << "; gridNz = " << gridNz << std::endl;
		std::cout << "hx = " << hx << "; hy = " << hy << "; hz = " << hz << std::endl;		

		double dataSizeInMb = GetDataSizeInMb();
		std::cout << "dataSizeInMb = " << dataSizeInMb << " Mb" << std::endl;

		if (grid3DPrintDetalisation > GRID)
		{
			for (auto nodeName : nodeNames)
			{
				GridBlock3DByNode* gridBlock3DByNode = GetGridBlock3DByNode(nodeName);
				gridBlock3DByNode->PrintNode(grid3DPrintDetalisation);
			}
		}

	}
};

#endif