#ifndef GRID_2D_FRAGMENT_XY
#define GRID_2D_FRAGMENT_XY

/// <summary>
/// Описывает двумерный фрагмент расчетной сетки - плоскость XY(грань трехмерного фрагмента)
/// </summary>
struct Grid2DFragmentXY
{
	size_t fragmentNx;			// количество узлов в фрагменте блока устройства вычислительного узла по оси Ox
	size_t fragmentNy;			// количество узлов в фрагменте блока устройства вычислительного узла по оси Oy
	size_t fragmentIndX;			// индекс 3D-фрагмента в блоке устройства вычислительного узла по оси Ox, которому принадлежит данный 2D-фрагмент
	size_t fragmentIndY;			// индекс 3D-фрагмента в блоке устройства вычислительного узла по оси Oy, которому принадлежит данный 2D-фрагмент
	size_t fragmentIndZ;			// индекс 3D-фрагмента в блоке устройства вычислительного узла по оси Oz, которому принадлежит данный 2D-фрагмент
	size_t fragmentOffsetXByDevice;			// смещение фрагмента относительно блока вычислительного устройства по оси Ox (относительно узла с координатами (0, 0, 0) блока вычислительного устройства)
	size_t fragmentOffsetYByDevice;			// смещение фрагмента относительно блока вычислительного устройства по оси Oy (относительно узла с координатами (0, 0, 0) блока вычислительного устройства)
	size_t fragmentOffsetZByDevice;			// смещение фрагмента относительно блока вычислительного устройства по оси Oz (относительно узла с координатами (0, 0, 0) блока вычислительного устройства)
	size_t fragmentOffsetXByNode;			// смещение фрагмента относительно блока вычислительного узла по оси Ox (относительно узла с координатами (0, 0, 0) блока вычислительного узла)
	size_t fragmentOffsetYByNode;			// смещение фрагмента относительно блока вычислительного узла по оси Oy (относительно узла с координатами (0, 0, 0) блока вычислительного узла)
	size_t fragmentOffsetZByNode;			// смещение фрагмента относительно блока вычислительного узла по оси Oz (относительно узла с координатами (0, 0, 0) блока вычислительного узла)
	size_t fragmentOffsetX;			// смещение фрагмента относительно глобальной расчетной сетки по оси Ox (относительно узла с координатами (0, 0, 0) глобальной расчетной сетки)
	size_t fragmentOffsetY;			// смещение фрагмента относительно глобальной расчетной сетки по оси Oy (относительно узла с координатами (0, 0, 0) глобальной расчетной сетки)
	size_t fragmentOffsetZ;			// смещение фрагмента относительно глобальной расчетной сетки по оси Oz (относительно узла с координатами (0, 0, 0) глобальной расчетной сетки)
	LinearArrays2D linearArrays2D;		// хранилище данных для фрагмента

	Grid2DFragmentXY(size_t FragmentNx, size_t FragmentNy, size_t FragmentIndX, size_t FragmentIndY, size_t FragmentIndZ,
		size_t FragmentOffsetXByDevice, size_t FragmentOffsetYByDevice, size_t FragmentOffsetZByDevice,
		size_t FragmentOffsetXByNode, size_t FragmentOffsetYByNode, size_t FragmentOffsetZByNode,
		size_t FragmentOffsetX, size_t FragmentOffsetY, size_t FragmentOffsetZ) :
		fragmentNx(FragmentNx), fragmentNy(FragmentNy), fragmentIndX(FragmentIndX), fragmentIndY(FragmentIndY), fragmentIndZ(FragmentIndZ),
		fragmentOffsetXByDevice(FragmentOffsetXByDevice), fragmentOffsetYByDevice(FragmentOffsetYByDevice), fragmentOffsetZByDevice(FragmentOffsetZByDevice),
		fragmentOffsetXByNode(FragmentOffsetXByNode), fragmentOffsetYByNode(FragmentOffsetYByNode), fragmentOffsetZByNode(FragmentOffsetZByNode),
		fragmentOffsetX(FragmentOffsetX), fragmentOffsetY(FragmentOffsetY), fragmentOffsetZ(FragmentOffsetZ),
		linearArrays2D{}
	{

	}

	/// <summary>
	/// Возвращает число узлов в фрагменте расчетной сетки
	/// </summary>
	/// <returns>Число узлов в фрагменте расчетной сетки</returns>
	size_t GetNodesNumber()
	{
		size_t nodesNumber = fragmentNx * fragmentNy;
		return nodesNumber;
	}

	/// <summary>
	/// $$$$Выделяет память и создает двумерный массив данных modelDataName размером (fragmentNx, fragmentNy)
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	void LinearArrayCreate(ModelDataName modelDataName)
	{
		linearArrays2D.LinearArrayCreate(fragmentNx, fragmentNy, modelDataName);
	}

	/// <summary>
	/// $$$$Возвращает указатель на двумерный массив LinearArray2D по указанному имени массива данных modelDataName
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	/// <returns>$$$$Указатель на двумерный массив LinearArray2D по указанному имени массива данных modelDataName</returns>
	LinearArray2D* GetLinearArray2D(ModelDataName modelDataName)
	{
		LinearArray2D* linearArray2D = &(linearArrays2D.linearArrays.at(modelDataName));
		return linearArray2D;
	}

	/// <summary>
	/// $$$$Заполняет двумерный массив данных фрагмента размером (nx, ny) элементами исходного одномерного массива data по указанному имени массива данных modelDataName
	/// </summary>
	/// <param name="data">Массив исходных данных data</param>
	/// <param name="nx">Число узлов расчетной сетки по оси OX</param>
	/// <param name="ny">Число узлов расчетной сетки по оси OY</param>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	void AssignData(double* data, size_t nx, size_t ny, ModelDataName modelDataName)
	{
		for (size_t j = 0; j < fragmentNy; j++)
		{
			for (size_t i = 0; i < fragmentNx; i++)
			{
				size_t globalElementIndexX = fragmentOffsetX + i;
				size_t globalElementIndexY = fragmentOffsetY + j;
				size_t globalElementIndexZ = fragmentOffsetZ;
				size_t globalElementIndex = globalElementIndexX + globalElementIndexY * nx + globalElementIndexZ * nx * ny;
				LinearArray2D* curArray = GetLinearArray2D(modelDataName);
				curArray->SetElement(i, j, data[globalElementIndex]);
			}
		}
	}

	/// <summary>
	/// $$$$Заполняет одномерный массив исходных данных data данными из двумерных массивов данных фрагментов по указанному имени массива данных modelDataName
	/// </summary>
	/// <param name="data">Массив исходных данных data</param>
	/// <param name="nx">Число узлов расчетной сетки по оси OX</param>
	/// <param name="ny">Число узлов расчетной сетки по оси OY</param>
	/// <param name="nz">Число узлов расчетной сетки по оси OZ</param>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	void Compose(double* data, size_t nx, size_t ny, size_t nz, ModelDataName modelDataName)
	{
		for (size_t j = 0; j < fragmentNy; j++)
		{
			for (size_t i = 0; i < fragmentNx; i++)
			{
				size_t globalElementIndexX = fragmentOffsetX + i;
				size_t globalElementIndexY = fragmentOffsetY + j;
				size_t globalElementIndexZ = fragmentOffsetZ;
				size_t globalElementIndex = globalElementIndexX + globalElementIndexY * nx + globalElementIndexZ * nx * ny;
				LinearArray2D* curArray = GetLinearArray2D(modelDataName);
				data[globalElementIndex] = curArray->GetElement(i, j);
			}
		}
	}

	/// <summary>
	/// Возвращает объём оперативной памяти, занимаемый фрагментом
	/// </summary>
	/// <returns>Объём оперативной памяти, занимаемый фрагментом</returns>
	double GetDataSizeInMb()
	{
		double result = linearArrays2D.GetDataSizeInMb();
		return result;
	}

	/// <summary>
	/// $$$$Выводит в консоль двумерный фрагмент расчетной сетки (плоскость XY – грань трехмерного фрагмента)
	/// </summary>
	void PrintFragment()
	{
		std::cout << "-----------------2D-FRAGMENT XY-------------------" << std::endl;
		std::cout << "fragmentNx = " << fragmentNx << "; fragmentNy = " << fragmentNy << std::endl;
		std::cout << "fragmentOffsetXByDevice = " << fragmentOffsetXByDevice << "; fragmentOffsetYByDevice = " << fragmentOffsetYByDevice << "; fragmentOffsetZByDevice = " << fragmentOffsetZByDevice << std::endl;
		std::cout << "fragmentOffsetXByNode = " << fragmentOffsetXByNode << "; fragmentOffsetYByNode = " << fragmentOffsetYByNode << "; fragmentOffsetZByNode = " << fragmentOffsetZByNode << std::endl;
		std::cout << "fragmentOffsetX = " << fragmentOffsetX << "; fragmentOffsetY = " << fragmentOffsetY << "; fragmentOffsetZ = " << fragmentOffsetZ << std::endl;

		double dataSizeInMb = GetDataSizeInMb();
		std::cout << "dataSizeInMb = " << dataSizeInMb << " Mb" << std::endl;

		linearArrays2D.Print();
	}
};

#endif