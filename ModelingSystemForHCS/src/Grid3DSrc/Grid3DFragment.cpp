#ifndef GRID_3D_FRAGMENT
#define GRID_3D_FRAGMENT

/// <summary>
/// Трехмерный фрагмент блока расчетной сетки вычислительного устройства
/// </summary>
struct Grid3DFragment
{
	size_t fragmentNx;			///< количество узлов в фрагменте блока устройства вычислительного узла по оси Ox
	size_t fragmentNy;			///< количество узлов в фрагменте блока устройства вычислительного узла по оси Oy
	size_t fragmentNz;			///< количество узлов в фрагменте блока устройства вычислительного узла по оси Oz
	size_t fragmentIndX;			///< индекс фрагмента в блоке устройства вычислительного узла по оси Ox
	size_t fragmentIndY;			///< индекс фрагмента в блоке устройства вычислительного узла по оси Oy
	size_t fragmentIndZ;			///< индекс фрагмента в блоке устройства вычислительного узла по оси Oz
	size_t fragmentOffsetXByDevice;			///< смещение фрагмента относительно блока вычислительного устройства по оси Ox (относительно узла с координатами (0, 0, 0) блока вычислительного устройства)
	size_t fragmentOffsetYByDevice;			///< смещение фрагмента относительно блока вычислительного устройства по оси Oy (относительно узла с координатами (0, 0, 0) блока вычислительного устройства)
	size_t fragmentOffsetZByDevice;			///< смещение фрагмента относительно блока вычислительного устройства по оси Oz (относительно узла с координатами (0, 0, 0) блока вычислительного устройства)
	size_t fragmentOffsetXByNode;			///< смещение фрагмента относительно блока вычислительного узла по оси Ox (относительно узла с координатами (0, 0, 0) блока вычислительного узла)
	size_t fragmentOffsetYByNode;			///< смещение фрагмента относительно блока вычислительного узла по оси Oy (относительно узла с координатами (0, 0, 0) блока вычислительного узла)
	size_t fragmentOffsetZByNode;			///< смещение фрагмента относительно блока вычислительного узла по оси Oz (относительно узла с координатами (0, 0, 0) блока вычислительного узла)
	size_t fragmentOffsetX;			///< смещение фрагмента относительно глобальной расчетной сетки по оси Ox (относительно узла с координатами (0, 0, 0) глобальной расчетной сетки)
	size_t fragmentOffsetY;			///< смещение фрагмента относительно глобальной расчетной сетки по оси Oy (относительно узла с координатами (0, 0, 0) глобальной расчетной сетки)
	size_t fragmentOffsetZ;			///< смещение фрагмента относительно глобальной расчетной сетки по оси Oz (относительно узла с координатами (0, 0, 0) глобальной расчетной сетки)
	Grid3DFragment* fragmentXPrev;		///< указатель на соседний фрагмент по оси Ox (отрицательное направление)
	Grid3DFragment* fragmentXNext;		///< указатель на соседний фрагмент по оси Ox (положительное направление)	
	Grid3DFragment* fragmentYPrev;		///< указатель на соседний фрагмент по оси Oy (отрицательное направление)	
	Grid3DFragment* fragmentYNext;		///< указатель на соседний фрагмент по оси Oy (положительное направление)	
	Grid3DFragment* fragmentZPrev;		///< указатель на соседний фрагмент по оси Oz (отрицательное направление)
	Grid3DFragment* fragmentZNext;		///< указатель на соседний фрагмент по оси Oz (положительное направление)	
	LinearArrays3D linearArrays3D;		///< хранилище данных для фрагмента


	Grid3DFragment(size_t FragmentNx, size_t FragmentNy, size_t FragmentNz, size_t FragmentIndX, size_t FragmentIndY, size_t FragmentIndZ,
		size_t FragmentOffsetXByDevice, size_t FragmentOffsetYByDevice, size_t FragmentOffsetZByDevice,
		size_t FragmentOffsetXByNode, size_t FragmentOffsetYByNode, size_t FragmentOffsetZByNode,
		size_t FragmentOffsetX, size_t FragmentOffsetY, size_t FragmentOffsetZ,
		Grid3DFragment* FragmentXPrev = NULL, Grid3DFragment* FragmentXNext = NULL, Grid3DFragment* FragmentYPrev = NULL,
		Grid3DFragment* FragmentYNext = NULL, Grid3DFragment* FragmentZPrev = NULL, Grid3DFragment* FragmentZNext = NULL) :
		fragmentNx(FragmentNx), fragmentNy(FragmentNy), fragmentNz(FragmentNz), fragmentIndX(FragmentIndX), fragmentIndY(FragmentIndY), fragmentIndZ(FragmentIndZ),
		fragmentOffsetXByDevice(FragmentOffsetXByDevice), fragmentOffsetYByDevice(FragmentOffsetYByDevice),
		fragmentOffsetXByNode(FragmentOffsetXByNode), fragmentOffsetYByNode(FragmentOffsetYByNode), fragmentOffsetZByNode(FragmentOffsetZByNode),
		fragmentOffsetX(FragmentOffsetX), fragmentOffsetY(FragmentOffsetY), fragmentOffsetZ(FragmentOffsetZ),
		fragmentXPrev(FragmentXPrev), fragmentXNext(FragmentXNext), fragmentYPrev(FragmentYPrev), fragmentOffsetZByDevice(FragmentOffsetZByDevice),
		fragmentYNext(FragmentYNext), fragmentZPrev(FragmentZPrev), fragmentZNext(FragmentZNext), linearArrays3D{}
	{

	}

	// Методы

	// Указатели на смежные двумерные плоскости для передачи данных

	/// <summary>
	/// Возвращает указатель на первую плоскость XY фрагмента по оси Oz от начала координат по имени массива данных modelDataName
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	/// <returns>Указатель на первую плоскость XY фрагмента по оси Oz от начала координат по имени массива данных modelDataName</returns>
	Grid2DFragmentXY* GetGrid2DFragmentXYFirst(ModelDataName modelDataName)
	{
		Grid2DFragmentXY* grid2DFragmentXY = new Grid2DFragmentXY(fragmentNx, fragmentNy,
			fragmentIndX, fragmentIndY, fragmentIndZ,
			fragmentOffsetXByDevice, fragmentOffsetYByDevice, fragmentOffsetZByDevice,
			fragmentOffsetXByNode, fragmentOffsetYByNode, fragmentOffsetZByNode,
			fragmentOffsetX, fragmentOffsetY, fragmentOffsetZ);

		LinearArrays2D* linearArrays2D = linearArrays3D.GetLinearArrays2DXYFirst(modelDataName);
		grid2DFragmentXY->linearArrays2D = *linearArrays2D;

		return grid2DFragmentXY;
	}

	/// <summary>
	/// Возвращает указатель на последнюю плоскость XY фрагмента по оси Oz от начала координат по имени массива данных modelDataName
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	/// <returns>Указатель на последнюю плоскость XY фрагмента по оси Oz от начала координат по имени массива данных modelDataName</returns>
	Grid2DFragmentXY* GetGrid2DFragmentXYLast(ModelDataName modelDataName)
	{
		Grid2DFragmentXY* grid2DFragmentXY = new Grid2DFragmentXY(fragmentNx, fragmentNy,
			fragmentIndX, fragmentIndY, fragmentIndZ,
			fragmentOffsetXByDevice, fragmentOffsetYByDevice, fragmentOffsetZByDevice,
			fragmentOffsetXByNode, fragmentOffsetYByNode, fragmentOffsetZByNode,
			fragmentOffsetX, fragmentOffsetY, fragmentOffsetZ);

		LinearArrays2D* linearArrays2D = linearArrays3D.GetLinearArrays2DXYLast(modelDataName);
		grid2DFragmentXY->linearArrays2D = *linearArrays2D;

		return grid2DFragmentXY;
	}

	/// <summary>
	/// Возвращает указатель на первую плоскость YZ фрагмента по оси Ox от начала координат по имени массива данных modelDataName
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	/// <returns>Указатель на первую плоскость YZ фрагмента по оси Ox от начала координат по имени массива данных modelDataName</returns>
	Grid2DFragmentYZ* GetGrid2DFragmentYZFirst(ModelDataName modelDataName)
	{
		Grid2DFragmentYZ* grid2DFragmentYZ = new Grid2DFragmentYZ(fragmentNy, fragmentNz,
			fragmentIndX, fragmentIndY, fragmentIndZ,
			fragmentOffsetXByDevice, fragmentOffsetYByDevice, fragmentOffsetZByDevice,
			fragmentOffsetXByNode, fragmentOffsetYByNode, fragmentOffsetZByNode,
			fragmentOffsetX, fragmentOffsetY, fragmentOffsetZ);

		LinearArrays2D* linearArrays2D = linearArrays3D.GetLinearArrays2DYZFirst(modelDataName);
		grid2DFragmentYZ->linearArrays2D = *linearArrays2D;

		return grid2DFragmentYZ;
	}

	/// <summary>
	/// Возвращает указатель на последнюю плоскость YZ фрагмента по оси Ox от начала координат по имени массива данных modelDataName
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	/// <returns>Указатель на последнюю плоскость YZ фрагмента по оси Ox от начала координат по имени массива данных modelDataName</returns>
	Grid2DFragmentYZ* GetGrid2DFragmentYZLast(ModelDataName modelDataName)
	{
		Grid2DFragmentYZ* grid2DFragmentYZ = new Grid2DFragmentYZ(fragmentNy, fragmentNz,
			fragmentIndX, fragmentIndY, fragmentIndZ,
			fragmentOffsetXByDevice, fragmentOffsetYByDevice, fragmentOffsetZByDevice,
			fragmentOffsetXByNode, fragmentOffsetYByNode, fragmentOffsetZByNode,
			fragmentOffsetX, fragmentOffsetY, fragmentOffsetZ);

		LinearArrays2D* linearArrays2D = linearArrays3D.GetLinearArrays2DYZLast(modelDataName);
		grid2DFragmentYZ->linearArrays2D = *linearArrays2D;

		return grid2DFragmentYZ;
	}

	/// <summary>
	/// Возвращает указатель на первую плоскость XZ фрагмента по оси Oy от начала координат по имени массива данных modelDataName
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	/// <returns>Указатель на первую плоскость XZ фрагмента по оси Oy от начала координат по имени массива данных modelDataName</returns>
	Grid2DFragmentXZ* GetGrid2DFragmentXZFirst(ModelDataName modelDataName)
	{
		Grid2DFragmentXZ* grid2DFragmentXZ = new Grid2DFragmentXZ(fragmentNx, fragmentNz,
			fragmentIndX, fragmentIndY, fragmentIndZ,
			fragmentOffsetXByDevice, fragmentOffsetYByDevice, fragmentOffsetZByDevice,
			fragmentOffsetXByNode, fragmentOffsetYByNode, fragmentOffsetZByNode,
			fragmentOffsetX, fragmentOffsetY, fragmentOffsetZ);

		LinearArrays2D* linearArrays2D = linearArrays3D.GetLinearArrays2DXZFirst(modelDataName);
		grid2DFragmentXZ->linearArrays2D = *linearArrays2D;

		return grid2DFragmentXZ;
	}

	/// <summary>
	/// Возвращает указатель на последнюю плоскость XZ фрагмента по оси Oy от начала координат по имени массива данных modelDataName
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	/// <returns>Указатель на последнюю плоскость XZ фрагмента по оси Oy от начала координат по имени массива данных modelDataName</returns>
	Grid2DFragmentXZ* GetGrid2DFragmentXZLast(ModelDataName modelDataName)
	{
		Grid2DFragmentXZ* grid2DFragmentXZ = new Grid2DFragmentXZ(fragmentNx, fragmentNz,
			fragmentIndX, fragmentIndY, fragmentIndZ,
			fragmentOffsetXByDevice, fragmentOffsetYByDevice, fragmentOffsetZByDevice,
			fragmentOffsetXByNode, fragmentOffsetYByNode, fragmentOffsetZByNode,
			fragmentOffsetX, fragmentOffsetY, fragmentOffsetZ);

		LinearArrays2D* linearArrays2D = linearArrays3D.GetLinearArrays2DXZLast(modelDataName);
		grid2DFragmentXZ->linearArrays2D = *linearArrays2D;

		return grid2DFragmentXZ;
	}

	/// <summary>
	/// Возвращает число узлов в фрагменте расчетной сетки
	/// </summary>
	/// <returns>Число узлов в фрагменте расчетной сетки</returns>
	size_t GetNodesNumber()
	{
		size_t nodesNumber = fragmentNx * fragmentNy * fragmentNz;
		return nodesNumber;
	}

	/// <summary>
	/// Возвращает число узлов в плоскости XY фрагмента расчетной сетки
	/// </summary>
	/// <returns>Число узлов в плоскости XY фрагмента расчетной сетки</returns>
	size_t GetNodesNumberXY()
	{
		size_t nodesNumber = fragmentNx * fragmentNy;
		return nodesNumber;
	}

	/// <summary>
	/// Возвращает число узлов в плоскости XZ фрагмента расчетной сетки
	/// </summary>
	/// <returns>Число узлов в плоскости XZ фрагмента расчетной сетки</returns>
	size_t GetNodesNumberXZ()
	{
		size_t nodesNumber = fragmentNx * fragmentNz;
		return nodesNumber;
	}

	/// <summary>
	/// Возвращает число узлов в плоскости YZ фрагмента расчетной сетки
	/// </summary>
	/// <returns>Число узлов в плоскости YZ фрагмента расчетной сетки</returns>
	size_t GetNodesNumberYZ()
	{
		size_t nodesNumber = fragmentNy * fragmentNz;
		return nodesNumber;
	}

	/// <summary>
	/// $$$$Выделяет память и создает трехмерный массив LinearArray3D по указаннному имени массива данных modelDataName размером (fragmentNx, fragmentNy, fragmentNz)
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	void LinearArrayCreate(ModelDataName modelDataName)
	{
		linearArrays3D.LinearArrayCreate(fragmentNx, fragmentNy, fragmentNz, modelDataName);
	}

	/// <summary>
	/// $$$$Возвращает указатель на трехмерный массив LinearArray3D по указанному имени массива данных modelDataName
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	/// <returns>Указатель на трехмерный массив LinearArray3D по указанному имени массива данных modelDataName</returns>
	LinearArray3D* GetLinearArray3D(ModelDataName modelDataName)
	{
		LinearArray3D* linearArray3D = &(linearArrays3D.linearArrays.at(modelDataName));
		return linearArray3D;
	}

	/// <summary>
	/// $$$$Заполняет трехмерный массив данных фрагмента размером (nx, ny, nz) элементами исходного одномерного массива data по указанному имени массива данных modelDataName
	/// </summary>
	/// <param name="data">Массив исходных данных data</param>
	/// <param name="nx">Число узлов расчетной сетки по оси OX</param>
	/// <param name="ny">Число узлов расчетной сетки по оси OY</param>
	/// <param name="nz">Число узлов расчетной сетки по оси OZ</param>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	void AssignData(double* data, size_t nx, size_t ny, size_t nz, ModelDataName modelDataName)
	{
		for (size_t k = 0; k < fragmentNz; k++)
		{
			for (size_t j = 0; j < fragmentNy; j++)
			{
				for (size_t i = 0; i < fragmentNx; i++)
				{
					size_t globalElementIndexX = fragmentOffsetX + i;
					size_t globalElementIndexY = fragmentOffsetY + j;
					size_t globalElementIndexZ = fragmentOffsetZ + k;
					size_t globalElementIndex = globalElementIndexX + globalElementIndexY * nx + globalElementIndexZ * nx * ny;
					LinearArray3D* curArray = GetLinearArray3D(modelDataName);
					curArray->SetElement(i, j, k, data[globalElementIndex]);
				}
			}
		}
	}

	/// <summary>
	/// $$$$Заполняет одномерный массив исходных данных data данными из трехмерных массивов данных фрагментов по указанному имени массива данных modelDataName
	/// </summary>
	/// <param name="data">Массив исходных данных data</param>
	/// <param name="nx">Число узлов расчетной сетки по оси OX</param>
	/// <param name="ny">Число узлов расчетной сетки по оси OY</param>
	/// <param name="nz">Число узлов расчетной сетки по оси OZ</param>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	void Compose(double* data, size_t nx, size_t ny, size_t nz, ModelDataName modelDataName)
	{
		for (size_t k = 0; k < fragmentNz; k++)
		{
			for (size_t j = 0; j < fragmentNy; j++)
			{
				for (size_t i = 0; i < fragmentNx; i++)
				{
					size_t globalElementIndexX = fragmentOffsetX + i;
					size_t globalElementIndexY = fragmentOffsetY + j;
					size_t globalElementIndexZ = fragmentOffsetZ + k;
					size_t globalElementIndex = globalElementIndexX + globalElementIndexY * nx + globalElementIndexZ * nx * ny;
					LinearArray3D* curArray = GetLinearArray3D(modelDataName);
					data[globalElementIndex] = curArray->GetElement(i, j, k);
				}
			}
		}
	}

	/// <summary>
	/// $$$$Выводит в консоль индексы фрагмента в блоке устройства вычислительного узла
	/// </summary>
	void PrintGrid3DFragmentIndex()
	{
		std::cout << "(" << fragmentIndX << ", " << fragmentIndY << ", " << fragmentIndZ << ")";
	}

	/// <summary>
	/// $$$$Выводит в консоль фрагмент по индексам фрагмента в блоке устройства вычислительного узла
	/// </summary>
	/// <param name="fragment">Фрагмент</param>
	void PrintGrid3DFragmentIndex(Grid3DFragment* fragment)
	{
		if (fragment == NULL)
		{
			std::cout << "(NULL)";
		}
		else
		{
			std::cout << "(" << fragment->fragmentIndX << ", " << fragment->fragmentIndY << ", " << fragment->fragmentIndZ << ")";
		}
	}

	/// <summary>
	/// Возвращает объём оперативной памяти, занимаемый фрагментом
	/// </summary>
	/// <returns>Объём оперативной памяти, занимаемый фрагментом</returns>
	double GetDataSizeInMb()
	{
		double result = linearArrays3D.GetDataSizeInMb();
		return result;
	}

	/// <summary>
	/// $$$$Выводит в консоль сведения о фрагменте в блоке устройства вычислительного узла по указанному уровню детализации вывода grid3DPrintDetalisation
	/// </summary>
	/// <param name="grid3DPrintDetalisation">Уровень детализации вывода grid3DPrintDetalisation</param>
	void PrintFragment(Grid3DPrintDetalisation grid3DPrintDetalisation)
	{
		std::cout << " \t\t\t-----------------FRAGMENT ";
		PrintGrid3DFragmentIndex();
		std::cout << "-------------------" << std::endl;
		std::cout << " \t\t\tfragmentNx = " << fragmentNx << "; fragmentNy = " << fragmentNy << "; fragmentNz = " << fragmentNz << std::endl;
		std::cout << " \t\t\tfragmentOffsetXByDevice = " << fragmentOffsetXByDevice << "; fragmentOffsetYByDevice = " << fragmentOffsetYByDevice << "; fragmentOffsetZByDevice = " << fragmentOffsetZByDevice << std::endl;
		std::cout << " \t\t\tfragmentOffsetXByNode = " << fragmentOffsetXByNode << "; fragmentOffsetYByNode = " << fragmentOffsetYByNode << "; fragmentOffsetZByNode = " << fragmentOffsetZByNode << std::endl;
		std::cout << " \t\t\tfragmentOffsetX = " << fragmentOffsetX << "; fragmentOffsetY = " << fragmentOffsetY << "; fragmentOffsetZ = " << fragmentOffsetZ << std::endl;

		double dataSizeInMb = GetDataSizeInMb();
		std::cout << "\t\t\tdataSizeInMb = " << dataSizeInMb << " Mb" << std::endl;

		std::cout << " \t\t\tfragmentXPrev = ";
		PrintGrid3DFragmentIndex(fragmentXPrev);
		std::cout << "; fragmentXNext = ";
		PrintGrid3DFragmentIndex(fragmentXNext);
		std::cout << std::endl;

		std::cout << " \t\t\tfragmentYPrev = ";
		PrintGrid3DFragmentIndex(fragmentYPrev);
		std::cout << "; fragmentYNext = ";
		PrintGrid3DFragmentIndex(fragmentYNext);
		std::cout << std::endl;

		std::cout << " \t\t\tfragmentZPrev = ";
		PrintGrid3DFragmentIndex(fragmentZPrev);
		std::cout << "; fragmentZNext = ";
		PrintGrid3DFragmentIndex(fragmentZNext);
		std::cout << std::endl;

	}

	/// <summary>
	/// Выводит в консоль все элементы плоскости Fragment3DPlane фрагмента для указанного типа ModelDataName
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	/// <param name="fragment3DPlane">Плоскость для передачи данных между фрагментами</param>
	void PrintFragmentPlane(ModelDataName modelDataName, Fragment3DPlane fragment3DPlane)
	{
		if (fragment3DPlane == XOZ_Prev || fragment3DPlane == XOZ_Next)
		{
			PrintFragmentPlane(modelDataName, fragment3DPlane, 0, 0, fragmentNx - 1, fragmentNz - 1);
		}

		if (fragment3DPlane == XOY_Prev || fragment3DPlane == XOY_Next)
		{
			PrintFragmentPlane(modelDataName, fragment3DPlane, 0, 0, fragmentNx - 1, fragmentNy - 1);
		}

		if (fragment3DPlane == YOZ_Prev || fragment3DPlane == YOZ_Next)
		{
			PrintFragmentPlane(modelDataName, fragment3DPlane, 0, 0, fragmentNy - 1, fragmentNz - 1);
		}
	}

	/// <summary>
	/// $$$$Выводит в консоль части элементов плоскости Fragment3DPlane фрагмента для указанного типа ModelDataName, часть размером (Ind1_1, Ind1_2) на (Ind2_1, Ind2_2)
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	/// <param name="fragment3DPlane">Плоскость для передачи данных между фрагментами</param>
	/// <param name="Ind1_1">Первый индекс по первой оси</param>
	/// <param name="Ind1_2">Второй индекс по первой оси</param>
	/// <param name="Ind2_1">Первый индекс по второй оси</param>
	/// <param name="Ind2_2">Второй индекс по второй оси</param>
	void PrintFragmentPlane(ModelDataName modelDataName, Fragment3DPlane fragment3DPlane, size_t Ind1_1, size_t Ind1_2, size_t Ind2_1, size_t Ind2_2)
	{
		LinearArray3D* linearArray3D = GetLinearArray3D(modelDataName);
		linearArray3D->Print(fragment3DPlane, Ind1_1, Ind1_2, Ind2_1, Ind2_2);
	}
};

#endif