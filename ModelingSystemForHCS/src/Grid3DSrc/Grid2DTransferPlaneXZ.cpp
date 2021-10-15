#ifndef GRID_2D_TRANSFER_PLANE_XZ
#define GRID_2D_TRANSFER_PLANE_XZ

/// <summary>
/// Описывает смежную двумерную плоскость XZ для передачи данных между устройствами
/// </summary>
struct Grid2DTransferPlaneXZ
{
	size_t fragmentsNumX;		///< Количество фрагментов в плоскости по оси Ox 
	size_t fragmentsNumZ;		///< Количество фрагментов в плоскости по оси Oz

	size_t fragmentNx;			///< Количество узлов во фрагментах блока устройства вычислительного узла по оси Ox (кроме последнего)
	size_t fragmentNz;			///< Количество узлов во фрагментах блока устройства вычислительного узла по оси Oz (кроме последнего)
	size_t fragmentNxl;			///< Количество узлов во фрагментах блока устройства вычислительного узла по оси Ox (последний)
	size_t fragmentNzl;			///< Количество узлов во фрагментах блока устройства вычислительного узла по оси Oz (последний)

	std::vector<std::vector<LinearArrays2D>> linearArrays2D;///< Хранилище двумерных массивов

	Grid2DTransferPlaneXZ(size_t FragmentsNumX, size_t FragmentsNumZ,
		size_t FragmentNx, size_t FragmentNz,
		size_t FragmentNxl, size_t FragmentNzl) :
		fragmentsNumX(FragmentsNumX), fragmentsNumZ(FragmentsNumZ),
		fragmentNx(FragmentNx), fragmentNz(FragmentNz),
		fragmentNxl(FragmentNxl), fragmentNzl(FragmentNzl)

	{
		for (size_t k = 0; k < fragmentsNumZ; k++)
		{
			std::vector<LinearArrays2D> linearArrays2DbyX;
			for (size_t i = 0; i < fragmentsNumX; i++)
			{
				LinearArrays2D linearArrays2D;
				linearArrays2DbyX.push_back(linearArrays2D);
			}
			linearArrays2D.push_back(linearArrays2DbyX);
		}
	}

	// Методы

	/// <summary>
	/// Создание массива данных по имени массива данных modelDataName
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	void LinearArrayCreate(ModelDataName modelDataName)
	{
		for (size_t k = 0; k < fragmentsNumZ; k++)
		{
			for (size_t i = 0; i < fragmentsNumX; i++)
			{
				auto& curLinearArrays2D = linearArrays2D[k][i];
				size_t nx = i == fragmentsNumX - 1 ? fragmentNxl : fragmentNx;
				size_t nz = k == fragmentsNumZ - 1 ? fragmentNzl : fragmentNz;
				curLinearArrays2D.LinearArrayCreate(nx, nz, modelDataName);
			}
		}
	}

	/// <summary>
	/// Вывод в консоль фрагмента переходного слоя заданного типа
	/// </summary>
	/// <param name="fragmentIndX">Индекс фрагмента переходного слоя по OX</param>
	/// <param name="fragmentIndZ">Индекс фрагмента переходного слоя по OZ</param>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	void PrintGrid2DTransferPlaneXZ(size_t fragmentIndX, size_t fragmentIndZ, ModelDataName modelDataName)
	{
		std::cout << std::endl;
		std::cout << "-------PrintGrid2DTransferPlaneXZ()------" << std::endl;
		std::cout << "fragmentsNumX = " << fragmentsNumX << "; fragmentsNumZ = " << fragmentsNumZ << std::endl;
		std::cout << "fragmentNx = " << fragmentNx << "; fragmentNxl = " << fragmentNxl << std::endl;
		std::cout << "fragmentNz = " << fragmentNz << "; fragmentNzl = " << fragmentNzl << std::endl;
		std::cout << "linearArrays2D.size() = " << linearArrays2D.size() << std::endl;

		std::cout << "------------------------" << std::endl;
		std::cout << "fragmentIndX = " << fragmentIndX << "; fragmentIndZ = " << fragmentIndZ << std::endl;
		std::cout << "modelDataName = " << modelDataName << std::endl;

		auto& curLinearArrays2D = linearArrays2D[fragmentIndZ][fragmentIndX];
		curLinearArrays2D.Print(modelDataName);
		std::cout << "------------------------" << std::endl;
	}

};

#endif