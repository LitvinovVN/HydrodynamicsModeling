#ifndef GRID_2D_FRAGMENT_YZ
#define GRID_2D_FRAGMENT_YZ

/// <summary>
/// Описывает двумерный фрагмент расчетной сетки - плоскость YZ(грань трехмерного фрагмента)
/// </summary>
struct Grid2DFragmentYZ
{
	size_t fragmentNy;			// количество узлов в фрагменте блока устройства вычислительного узла по оси Oy
	size_t fragmentNz;			// количество узлов в фрагменте блока устройства вычислительного узла по оси Oz
	size_t fragmentIndX;			// индекс 3D-фрагмента в блоке устройства вычислительного узла по оси Ox, которому принадлежит данный 2D-фрагмент
	size_t fragmentIndY;			// индекс 3D-фрагмента в блоке устройства вычислительного узла по оси Oy, которому принадлежит данный 2D-фрагмент
	size_t fragmentIndZ;			// индекс 3D-фрагмента в блоке устройства вычислительного узла по оси Oz, которому принадлежит данный 2D-фрагмент
	size_t fragmentOffsetXByDevice;			// смещение фрагмента относительно блока вычислительного устройства по оси Ox (относительно узла с координатами (0, 0, 0) блока вычислительного устройства)
	size_t fragmentOffsetYByDevice;			// смещение фрагмента относительно блока вычислительного устройства по оси Oy (относительно узла с координатами (0, 0, 0) блока вычислительного устройства)
	size_t fragmentOffsetZByDevice;			// смещение фрагмента относительно блока вычислительного устройства по оси Oz (относительно узла с координатами (0, 0, 0) блока вычислительного устройства)
	size_t fragmentOffsetXByNode;			// смещение фрагмента относительно блока вычислительного устройства по оси Ox (относительно узла с координатами (0, 0, 0) блока вычислительного узла)
	size_t fragmentOffsetYByNode;			// смещение фрагмента относительно блока вычислительного устройства по оси Oy (относительно узла с координатами (0, 0, 0) блока вычислительного узла)
	size_t fragmentOffsetZByNode;			// смещение фрагмента относительно блока вычислительного устройства по оси Oz (относительно узла с координатами (0, 0, 0) блока вычислительного узла)
	size_t fragmentOffsetX;			// смещение фрагмента относительно блока вычислительного устройства по оси Ox (относительно узла с координатами (0, 0, 0) глобальной расчетной сетки)
	size_t fragmentOffsetY;			// смещение фрагмента относительно блока вычислительного устройства по оси Oy (относительно узла с координатами (0, 0, 0) глобальной расчетной сетки)
	size_t fragmentOffsetZ;			// смещение фрагмента относительно блока вычислительного устройства по оси Oz (относительно узла с координатами (0, 0, 0) глобальной расчетной сетки)
	LinearArrays2D linearArrays2D;		// хранилище данных для фрагмента

	Grid2DFragmentYZ(size_t FragmentNy, size_t FragmentNz, size_t FragmentIndX, size_t FragmentIndY, size_t FragmentIndZ,
		size_t FragmentOffsetXByDevice, size_t FragmentOffsetYByDevice, size_t FragmentOffsetZByDevice,
		size_t FragmentOffsetXByNode, size_t FragmentOffsetYByNode, size_t FragmentOffsetZByNode,
		size_t FragmentOffsetX, size_t FragmentOffsetY, size_t FragmentOffsetZ) :
		fragmentNy(FragmentNy), fragmentNz(FragmentNz), fragmentIndX(FragmentIndX), fragmentIndY(FragmentIndY), fragmentIndZ(FragmentIndZ),
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
		size_t nodesNumber = fragmentNy * fragmentNz;
		return nodesNumber;
	}

	/// <summary>
	/// Выделяет память и создает двумерный массив данных modelDataName размером (fragmentNy, fragmentNz)
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	void LinearArrayCreate(ModelDataName modelDataName)
	{
		linearArrays2D.LinearArrayCreate(fragmentNy, fragmentNz, modelDataName);
	}

	/// <summary>
	/// Возвращает указатель на объект linearArray2D по имени массива данных modelDataName
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	/// <returns>Указатель на объект linearArray2D по имени массива данных modelDataName</returns>
	LinearArray2D* GetLinearArray2D(ModelDataName modelDataName)
	{
		LinearArray2D* linearArray2D = &(linearArrays2D.linearArrays.at(modelDataName));
		return linearArray2D;
	}

	/// <summary>
	/// $$$$Возвращает значение элемента массива с указанным именем массива данных modelDataName по указанным координатам IndX и IndZ
	/// </summary>
	/// <param name="IndX">Индекс элемента массива в фрагменте по оси OX</param>
	/// <param name="IndZ">Индекс элемента массива в фрагменте по оси OZ</param>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	/// <returns>Значение элемента массива с указанным именем массива данных modelDataName по указанным координатам IndX и IndZ</returns>
	double GetElement(size_t IndX, size_t IndZ, ModelDataName modelDataName)
	{
		LinearArray2D* linearArray2D = GetLinearArray2D(modelDataName);
		return linearArray2D->GetElement(IndX, IndZ);
	}

	/// <summary>
	/// $$$$Сохраняет значение элемента массива с указанным именем массива данных modelDataName по указанным координатам IndX и IndZ
	/// </summary>
	/// <param name="IndX">Индекс элемента массива в фрагменте по оси OX</param>
	/// <param name="IndZ">Индекс элемента массива в фрагменте по оси OZ</param>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	/// <param name="Value">Значение элемента массива данных data</param>
	/// <returns>Значение элемента массива с указанным именем массива данных modelDataName по указанным координатам IndX и IndZ</returns>
	double SetElement(size_t IndX, size_t IndZ, ModelDataName modelDataName, double Value)
	{
		LinearArray2D* linearArray2D = GetLinearArray2D(modelDataName);
		linearArray2D->SetElement(IndX, IndZ, Value);
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
	/// $$$$Выводит в консоль двумерный фрагмент расчетной сетки (плоскость YZ – грань трехмерного фрагмента)
	/// </summary>
	void PrintFragment()
	{
		std::cout << "-----------------2D-FRAGMENT YZ-------------------" << std::endl;
		std::cout << "fragmentNy = " << fragmentNy << "; fragmentNz = " << fragmentNz << std::endl;
		std::cout << "fragmentOffsetXByDevice = " << fragmentOffsetXByDevice << "; fragmentOffsetYByDevice = " << fragmentOffsetYByDevice << "; fragmentOffsetZByDevice = " << fragmentOffsetZByDevice << std::endl;
		std::cout << "fragmentOffsetXByNode = " << fragmentOffsetXByNode << "; fragmentOffsetYByNode = " << fragmentOffsetYByNode << "; fragmentOffsetZByNode = " << fragmentOffsetZByNode << std::endl;
		std::cout << "fragmentOffsetX = " << fragmentOffsetX << "; fragmentOffsetY = " << fragmentOffsetY << "; fragmentOffsetZ = " << fragmentOffsetZ << std::endl;

		double dataSizeInMb = GetDataSizeInMb();
		std::cout << "dataSizeInMb = " << dataSizeInMb << " Mb" << std::endl;

		linearArrays2D.Print();
	}
};

#endif