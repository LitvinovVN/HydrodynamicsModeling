#ifndef GRID_BLOCK_3D_BY_DEVICE
#define GRID_BLOCK_3D_BY_DEVICE

/// <summary>
/// Трехмерный блок расчетной сетки вычислительного устройства
/// </summary>
struct GridBlock3DByDevice
{
	size_t blockNx;			///< количество узлов в блоке устройства вычислительного узла по оси Ox
	size_t blockNy;			///< количество узлов в блоке устройства вычислительного узла по оси Oy
	size_t blockNz;			///< количество узлов в блоке устройства вычислительного узла по оси Oz
	size_t deviceIndex;		///< номер вычислительного устройства в вычислительном узле
	std::string deviceName;	///< наименование вычислительного устройства, обрабатывающего блок
	size_t fragmentsNumX;			///< количество фрагментов в блоке устройства по оси Ox 
	size_t fragmentsNumY;			///< количество фрагментов в блоке устройства по оси Oy
	size_t fragmentsNumZ;			///< количество фрагментов в блоке устройства по оси Oz
	size_t deviceOffsetYByNode;		///< смещение блока вычислительного устройства по оси Oy (относительно узла с координатами (0, 0, 0) вычислительного узла)
	size_t deviceOffsetY;			///< смещение блока вычислительного устройства по оси Oy (относительно узла с координатами (0, 0, 0) глобальной расчетной сетки)
	GridBlock3DByDevice* devicePrev;		///< указатель на блок расчетной сетки, обрабатываемый предыдущим вычислительным устройством
	GridBlock3DByDevice* deviceNext;		///< указатель на блок расчетной сетки, обрабатываемый следующим вычислительным устройством

	size_t fragmentNx;			///< количество узлов во фрагментах блока устройства вычислительного узла по оси Ox (кроме последнего)
	size_t fragmentNy;			///< количество узлов во фрагментах блока устройства вычислительного узла по оси Oy (кроме последнего)
	size_t fragmentNz;			///< количество узлов во фрагментах блока устройства вычислительного узла по оси Oz (кроме последнего)
	size_t fragmentNxl;			///< количество узлов во фрагментах блока устройства вычислительного узла по оси Ox (последний)
	size_t fragmentNyl;			///< количество узлов во фрагментах блока устройства вычислительного узла по оси Oy (последний)
	size_t fragmentNzl;			///< количество узлов во фрагментах блока устройства вычислительного узла по оси Oz (последний)

	std::vector< std::vector <std::vector<Grid3DFragment> > > fragments;	///< трехмерный массив фрагментов блока расчетной сетки вычислительного устройства вычислительного узла

	std::vector<Grid2DTransferPlaneXZ> grid2DTransferPlanesXZ; ///< Вектор двумерных плоскостей XZ для передачи данных между вычислителями

	GridBlock3DByDevice(size_t BlockNx, size_t BlockNy, size_t BlockNz, size_t DeviceIndex, std::string DeviceName,
		size_t FragmentsNumX, size_t FragmentsNumY, size_t FragmentsNumZ,
		size_t DeviceOffsetYByNode, size_t DeviceOffsetY, GridBlock3DByDevice* DevicePrev = NULL, GridBlock3DByDevice* DeviceNext = NULL) :
		blockNx(BlockNx), blockNy(BlockNy), blockNz(BlockNz), deviceIndex(DeviceIndex), deviceName(DeviceName),
		fragmentsNumX(FragmentsNumX), fragmentsNumY(FragmentsNumY), fragmentsNumZ(FragmentsNumZ),
		deviceOffsetYByNode(DeviceOffsetYByNode), deviceOffsetY(DeviceOffsetY), devicePrev(DevicePrev), deviceNext(DeviceNext)
	{
		size_t _nx;
		size_t _nxl;
		size_t _ny;
		size_t _nyl;
		size_t _nz;
		size_t _nzl;

		if (blockNx % fragmentsNumX == 0)
		{
			_nx = blockNx / fragmentsNumX;
			_nxl = _nx;
		}
		else
		{
			double _nxd = (double)blockNx / fragmentsNumX;
			if ((_nxd - blockNx / fragmentsNumX) < 0.5)
			{
				_nx = blockNx / fragmentsNumX;
				_nxl = blockNx - _nx * (fragmentsNumX - 1);
			}
			else
			{
				_nx = blockNx / fragmentsNumX + 1;
				_nxl = blockNx - _nx * (fragmentsNumX - 1);
			}
		}

		if (blockNy % fragmentsNumY == 0)
		{
			_ny = blockNy / fragmentsNumY;
			_nyl = _ny;
		}
		else
		{
			double _nyd = (double)blockNy / fragmentsNumY;
			if ((_nyd - blockNy / fragmentsNumY) < 0.5)
			{
				_ny = blockNy / fragmentsNumY;
				_nyl = blockNy - _ny * (fragmentsNumY - 1);
			}
			else
			{
				_ny = blockNy / fragmentsNumY + 1;
				_nyl = blockNy - _ny * (fragmentsNumY - 1);
			}
		}

		if (blockNz % fragmentsNumZ == 0)
		{
			_nz = blockNz / fragmentsNumZ;
			_nzl = _nz;
		}
		else
		{
			double _nzd = (double)blockNz / fragmentsNumZ;
			if ((_nzd - blockNz / fragmentsNumZ) < 0.5)
			{
				_nz = blockNz / fragmentsNumZ;
				_nzl = blockNz - _nz * (fragmentsNumZ - 1);
			}
			else
			{
				_nz = blockNz / fragmentsNumZ + 1;
				_nzl = blockNz - _nz * (fragmentsNumZ - 1);
			}
		}

		fragmentNx = _nx;
		fragmentNy = _ny;
		fragmentNz = _nz;
		fragmentNxl = _nxl;
		fragmentNyl = _nyl;
		fragmentNzl = _nzl;


		size_t fragmentOffsetXByDevice = 0;
		size_t fragmentOffsetYByDevice = 0;
		size_t fragmentOffsetZByDevice = 0;

		for (size_t k = 0; k < fragmentsNumZ; k++)
		{
			std::vector <std::vector <Grid3DFragment>> fragmentsByXY;
			for (size_t j = 0; j < fragmentsNumY; j++)
			{
				std::vector <Grid3DFragment> fragmentsByX;
				for (size_t i = 0; i < fragmentsNumX; i++)
				{
					size_t __nx = _nx;
					size_t __ny = _ny;
					size_t __nz = _nz;

					if (k == fragmentsNumZ - 1)
					{
						__nz = _nzl;
					}

					if (j == fragmentsNumY - 1)
					{
						__ny = _nyl;
					}

					if (i == fragmentsNumX - 1)
					{
						__nx = _nxl;
					}

					Grid3DFragment newFragment(__nx, __ny, __nz, i, j, k,
						fragmentOffsetXByDevice, fragmentOffsetYByDevice, fragmentOffsetZByDevice,
						fragmentOffsetXByDevice, fragmentOffsetYByDevice + deviceOffsetYByNode, fragmentOffsetZByDevice,
						fragmentOffsetXByDevice, fragmentOffsetYByDevice + deviceOffsetY, fragmentOffsetZByDevice);
					fragmentsByX.push_back(newFragment);
					fragmentOffsetXByDevice += fragmentNx;
				}
				fragmentsByXY.push_back(fragmentsByX);
				fragmentOffsetXByDevice = 0;
				fragmentOffsetYByDevice += fragmentNy;
			}
			fragments.push_back(fragmentsByXY);
			fragmentOffsetYByDevice = 0;
			fragmentOffsetZByDevice += fragmentNz;
		}

		for (size_t k = 0; k < fragmentsNumZ; k++)
		{
			for (size_t j = 0; j < fragmentsNumY; j++)
			{
				for (size_t i = 0; i < fragmentsNumX; i++)
				{
					Grid3DFragment* fragmentXNext = NULL;
					Grid3DFragment* fragmentXPrev = NULL;
					Grid3DFragment* fragmentYNext = NULL;
					Grid3DFragment* fragmentYPrev = NULL;
					Grid3DFragment* fragmentZNext = NULL;
					Grid3DFragment* fragmentZPrev = NULL;

					if (i != 0)
					{
						fragmentXPrev = &fragments[k][j][i - 1];
					}

					if (i < fragmentsNumX - 1)
					{
						fragmentXNext = &fragments[k][j][i + 1];
					}

					if (j != 0)
					{
						fragmentYPrev = &fragments[k][j - 1][i];
					}

					if (j < fragmentsNumY - 1)
					{
						fragmentYNext = &fragments[k][j + 1][i];
					}

					if (k != 0)
					{
						fragmentZPrev = &fragments[k - 1][j][i];
					}

					if (k < fragmentsNumZ - 1)
					{
						fragmentZNext = &fragments[k + 1][j][i];
					}

					Grid3DFragment* curFragment = &fragments[k][j][i];
					curFragment->fragmentXNext = fragmentXNext;
					curFragment->fragmentXPrev = fragmentXPrev;
					curFragment->fragmentYNext = fragmentYNext;
					curFragment->fragmentYPrev = fragmentYPrev;
					curFragment->fragmentZNext = fragmentZNext;
					curFragment->fragmentZPrev = fragmentZPrev;
				}
			}
		}
	}

	// Методы

	/// <summary>
	/// Создаёт двумерные плоскости XZ для передачи данных между вычислителями
	/// </summary>
	void CreateGrid2DTransferPlanesXZ()
	{
		for (size_t j = 0; j < fragmentsNumY - 1; j++)
		{
			Grid2DTransferPlaneXZ newPlaneXZ(fragmentsNumX, fragmentsNumZ,
				fragmentNx, fragmentNz,
				fragmentNxl, fragmentNzl);
			grid2DTransferPlanesXZ.push_back(newPlaneXZ);
		}
	}

	/// <summary>
	/// Добавляет в объект двумерной плоскости XZ для передачи данных массив требуемого типа
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	void Grid2DTransferPlanesXZLinearArrayCreate(ModelDataName modelDataName)
	{
		for (size_t j = 0; j < fragmentsNumY - 1; j++)
		{
			auto& curPlaneXZ = grid2DTransferPlanesXZ.at(j);
			curPlaneXZ.LinearArrayCreate(modelDataName);
		}

	}

	/// <summary>
	/// $$$$Возвращает указатель на трехмерный фрагмент типа Grid3DFragment по указанным индексам indX, indY, indZ
	/// </summary>
	/// <param name="indX">Индекс фрагмента по оси OX</param>
	/// <param name="indY">Индекс фрагмента по оси OX</param>
	/// <param name="indZ">Индекс фрагмента по оси OX</param>
	/// <returns>Указатель на трехмерный фрагмент типа Grid3DFragment по указанным индексам indX, indY, indZ</returns>
	Grid3DFragment* GetGrid3DFragment(size_t indX, size_t indY, size_t indZ)
	{
		Grid3DFragment* fragment = &fragments[indZ][indY][indX];
		return fragment;
	}

	/// <summary>
	/// Выделяет память и создает трехмерный массив данных размером (fragmentsNumX, fragmentsNumY, fragmentsNumZ) по указанному имени массива данных modelDataName 
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	void LinearArrayCreate(ModelDataName modelDataName)
	{
		for (size_t k = 0; k < fragmentsNumZ; k++)
		{
			for (size_t j = 0; j < fragmentsNumY; j++)
			{
				for (size_t i = 0; i < fragmentsNumX; i++)
				{
					Grid3DFragment* fragment = GetGrid3DFragment(i, j, k);
					fragment->LinearArrayCreate(modelDataName);
				}
			}
		}
	}

	/// <summary>
	/// Выводит в консоль объект двумерной плоскости XZ для передачи данных
	/// </summary>	
	/// <param name="transferPlaneXZIndex">Индекс переходного слоя</param>
	/// <param name="fragmentIndX">Индекс фрагмента переходного слоя по OX</param>
	/// <param name="fragmentIndZ">Индекс фрагмента переходного слоя по OZ</param>
	/// <param name="modelDataName">Тип массива данных</param>
	void Grid2DTransferPlanesXZLinearArrayPrint(size_t transferPlaneXZIndex,
		size_t fragmentIndX, size_t fragmentIndZ, ModelDataName modelDataName)
	{
		Grid2DTransferPlaneXZ& grid2DTransferPlaneXZ = grid2DTransferPlanesXZ.at(transferPlaneXZIndex);
		grid2DTransferPlaneXZ.PrintGrid2DTransferPlaneXZ(fragmentIndX, fragmentIndZ, modelDataName);
	}

	/// <summary>
	/// $$$$----------------------
	/// </summary>
	/// <param name="data">Массив исходных данных data</param>
	/// <param name="nx">Число узлов расчетной сетки по оси OX</param>
	/// <param name="ny">Число узлов расчетной сетки по оси OY</param>
	/// <param name="nz">Число узлов расчетной сетки по оси OZ</param>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	void AssignData(double* data, size_t nx, size_t ny, size_t nz, ModelDataName modelDataName)
	{
		for (size_t k = 0; k < fragmentsNumZ; k++)
		{
			for (size_t j = 0; j < fragmentsNumY; j++)
			{
				for (size_t i = 0; i < fragmentsNumX; i++)
				{
					Grid3DFragment* fragment = GetGrid3DFragment(i, j, k);
					fragment->AssignData(data, nx, ny, nz, modelDataName);
				}
			}
		}
	}

	/// <summary>
	/// $$$$----------------------
	/// </summary>
	/// <param name="data">Массив исходных данных data</param>
	/// <param name="nx">Число узлов расчетной сетки по оси OX</param>
	/// <param name="ny">Число узлов расчетной сетки по оси OY</param>
	/// <param name="nz">Число узлов расчетной сетки по оси OZ</param>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	void Compose(double* data, size_t nx, size_t ny, size_t nz, ModelDataName modelDataName)
	{
		for (size_t k = 0; k < fragmentsNumZ; k++)
		{
			for (size_t j = 0; j < fragmentsNumY; j++)
			{
				for (size_t i = 0; i < fragmentsNumX; i++)
				{
					Grid3DFragment* fragment = GetGrid3DFragment(i, j, k);
					fragment->Compose(data, nx, ny, nz, modelDataName);
				}
			}
		}
	}

	/// <summary>
	/// Возвращает объём оперативной памяти, занимаемый объектом GridBlock3DByDevice
	/// </summary>
	/// <returns>Объём оперативной памяти, занимаемый объектом GridBlock3DByDevice</returns>
	double GetDataSizeInMb()
	{
		double result = 0;
		for (size_t k = 0; k < fragmentsNumZ; k++)
		{
			for (size_t j = 0; j < fragmentsNumY; j++)
			{
				for (size_t i = 0; i < fragmentsNumX; i++)
				{
					Grid3DFragment* fragment = GetGrid3DFragment(i, j, k);
					result += fragment->GetDataSizeInMb();
				}
			}
		}
		return result;
	}

	/// <summary>
	/// Выводит в консоль УИД и наименование вычислительного узла gridBlock3DByDevice
	/// </summary>
	/// <param name="gridBlock3DByDevice">Трехмерный блок расчетной сетки вычислительного узла</param>
	void PrintDeviceName(GridBlock3DByDevice* gridBlock3DByDevice)
	{
		if (gridBlock3DByDevice == NULL)
		{
			std::cout << "(NULL)";
		}
		else
		{
			std::cout << "(" << gridBlock3DByDevice->deviceIndex;
			std::cout << ": " << gridBlock3DByDevice->deviceName;
			std::cout << ")";
		}
	}

	/// <summary>
	/// $$$$Выводит в консоль сведения о блоках устройства вычислительного узла по указанному уровню детализации вывода grid3DPrintDetalisation
	/// </summary>
	/// <param name="grid3DPrintDetalisation">Уровень детализации вывода grid3DPrintDetalisation</param>
	void PrintDevice(Grid3DPrintDetalisation grid3DPrintDetalisation)
	{
		std::cout << " \t\t-----------------DEVICE " << deviceIndex << ": " << deviceName << "-------------------" << std::endl;
		std::cout << " \t\tblockNx = " << blockNx << "; blockNy = " << blockNy << "; blockNz = " << blockNz << std::endl;
		std::cout << " \t\tdeviceOffsetYByNode = " << deviceOffsetYByNode << std::endl;
		std::cout << " \t\tdeviceOffsetY = " << deviceOffsetY << std::endl;

		if (devicePrev == NULL)
		{
			std::cout << " \t\tdevicePrev = NULL" << std::endl;
		}
		else
		{
			std::cout << " \t\tdevicePrev = " << devicePrev->deviceIndex << " (" << devicePrev->deviceName << ")" << std::endl;
		}

		if (deviceNext == NULL)
		{
			std::cout << " \t\tdeviceNext = NULL" << std::endl;
		}
		else
		{
			std::cout << " \t\tdeviceNext = " << deviceNext->deviceIndex << " (" << deviceNext->deviceName << ")" << std::endl;
		}

		double dataSizeInMb = GetDataSizeInMb();
		std::cout << "\t\tdataSizeInMb = " << dataSizeInMb << " Mb" << std::endl;

		if (grid3DPrintDetalisation > DEVICES)
		{
			for (auto& fragmentsXY : fragments)
			{
				for (auto& fragmentsX : fragmentsXY)
				{
					for (auto& fragment : fragmentsX)
					{
						fragment.PrintFragment(grid3DPrintDetalisation);
					}
				}
			}
		}
	}

};

#endif