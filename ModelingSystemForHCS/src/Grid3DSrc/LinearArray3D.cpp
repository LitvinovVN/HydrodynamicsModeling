#ifndef LINEAR_ARRAY_3D
#define LINEAR_ARRAY_3D

/// <summary>
/// Линейный массив, описывающий трёхмерную область
/// </summary>
struct LinearArray3D
{
	size_t nx;             ///< число узлов в фрагменте по оси Ox
	size_t ny;             ///< число узлов в фрагменте по оси Oy
	size_t nz;             ///< число узлов в фрагменте по оси Oz
	double* data;          ///< указатель на массив данных

	/// Конструктор
	LinearArray3D(size_t Nx, size_t Ny, size_t Nz) :
		nx(Nx), ny(Ny), nz(Nz)
	{
		size_t n = nx * ny * nz;
		size_t dataSize = n * sizeof(double);
		data = (double*)malloc(dataSize);

		for (int i = 0; i < n; i++)
		{
			data[i] = 0;
		}
	}

	/// Деструктор
	~LinearArray3D()
	{
		free(data);
	}

	// Методы

	/// <summary>
	/// Инициализирует массив data указанным значением
	/// </summary>
	void InitLinearArray3DByValue(double value)
	{
		for (int gi = 0; gi < nx * ny * nz; gi++)
			data[gi] = value;
	}

	/// <summary>
	/// Инициализирует массив data глобальными индексами
	/// </summary>
	void InitLinearArray3DByGlobalIndexes()
	{
		for (int gi = 0; gi < nx * ny * nz; gi++)
			data[gi] = gi;
	}

	/// <summary>
	/// Проверяет массивы на равенство (поэлементно)
	/// </summary>
	/// <param name="linAr">Указатель на объект класса LinearArray3D</param>
	/// <returns>Результат сравнения двух массивов (true - массивы равны, false - массивы не равны)</returns>
	bool IsEqual(LinearArray3D* linAr)
	{
		for (size_t i = 0; i < nx * ny * nz; i++)
		{
			if ((data[i] - linAr->data[i]) > 0.000001)
				return false;
		}

		return true;
	}

	/// <summary>
	/// Возвращает индекс элемента массива данных по индексам элемента в фрагменте
	/// </summary>
	/// <param name="IndX">Первый индекс элемента в фрагменте</param>
	/// <param name="IndY">Второй индекс элемента в фрагменте</param>
	/// <param name="IndZ">Третий индекс элемента в фрагменте</param>
	/// <returns>Индекс элемента массива данных по индексам элемента в фрагменте</returns>
	size_t GetIndex(size_t IndX, size_t IndY, size_t IndZ)
	{
		return IndX + IndY * nx + IndZ * nx * ny;
	}

	/// <summary>
	/// Возвращает значение элемента массива с индексом, вычисленным по индексам элемента в фрагменте
	/// </summary>
	/// <param name="IndX">Первый индекс элемента в фрагменте</param>
	/// <param name="IndY">Второй индекс элемента в фрагменте</param>
	/// <param name="IndZ">Третий индекс элемента в фрагменте</param>
	/// <returns>Значение элемента массива с индексом, вычисленным по индексам элемента в фрагменте</returns>
	double GetElement(size_t IndX, size_t IndY, size_t IndZ)
	{
#ifndef REMOVE_PROTECTIVE_CHECKS
		if ((IndX > nx) || (IndY > ny) || (IndZ > nz))
		{
			throw - 1;
		}
#endif // !REMOVE_PROTECTIVE_CHECKS
				
		size_t indx = GetIndex(IndX, IndY, IndZ);

		return data[indx];
	}

	/// <summary>
	/// Сохраняет значение элемента массива с индексом, вычисленным по индексам элемента в фрагменте
	/// </summary>
	/// <param name="IndX">Первый индекс элемента в фрагменте</param>
	/// <param name="IndY">Второй индекс элемента в фрагменте</param>
	/// <param name="IndZ">Третий индекс элемента в фрагменте</param>
	/// <param name="Value">Значение элемента массива</param>
	/// <returns>Значение элемента массива с индексом, вычисленным по индексам элемента в фрагменте</returns>
	double SetElement(size_t IndX, size_t IndY, size_t IndZ, double Value)
	{
#ifndef REMOVE_PROTECTIVE_CHECKS
		if ((IndX > nx) || (IndY > ny) || (IndZ > nz))
		{
			throw - 1;
		}
#endif // !REMOVE_PROTECTIVE_CHECKS
		
		size_t indx = GetIndex(IndX, IndY, IndZ);

		data[indx] = Value;
	}

	/// <summary>
	/// Заполняет объект LinearArray1D данными, расположенными вдоль оси Ox по указанным координатам Oy, Oz
	/// </summary>
	/// <param name="IndY">Индекс по оси OY</param>
	/// <param name="IndZ">Индекс по оси OZ</param>
	/// <param name="linAr1D">Указатель на объект класса LinearArray1D</param>
	void GetLineX(size_t IndY, size_t IndZ, LinearArray1D* linAr1D)
	{
		size_t indStart = GetIndex(0, IndY, IndZ);

		size_t cnt = 0;
		for (size_t i = indStart; i < indStart + nx; i++)
		{
			linAr1D->data[cnt++] = data[i];
		}
	}

	/// <summary>
	/// Заполняет массив данными, расположенными вдоль оси Ox по указанным координатам Oy, Oz
	/// </summary>
	/// <param name="IndY">Индекс по оси OY</param>
	/// <param name="IndZ">Индекс по оси OZ</param>
	/// <param name="linArray">Указатель на массив типа double</param>
	void GetLineX(size_t IndY, size_t IndZ, double* linArray)
	{
		size_t indStart = GetIndex(0, IndY, IndZ);

		size_t cnt = 0;
		for (size_t i = indStart; i < indStart + nx; i++)
		{
			linArray[cnt++] = data[i];
		}
	}

	/// <summary>
	/// Заполняет элементы объекта, расположенные вдоль оси Ox по указанным координатам Oy, Oz, данными из LinearArray1D
	/// </summary>
	/// <param name="IndY">Индекс по оси OY</param>
	/// <param name="IndZ">Индекс по оси OZ</param>
	/// <param name="linAr1D">Указатель на объект класса LinearArray1D</param>
	void SetLineX(size_t IndY, size_t IndZ, LinearArray1D* linAr1D)
	{
		size_t indStart = GetIndex(0, IndY, IndZ);

		size_t cnt = 0;
		for (size_t i = indStart; i < indStart + nx; i++)
		{
			data[i] = linAr1D->data[cnt++];
		}
	}

	/// <summary>
	/// Заполняет элементы объекта, расположенные вдоль оси Ox по указанным координатам Oy, Oz, данными из LinearArray1D
	/// </summary>
	/// <param name="IndY">Индекс по оси OY</param>
	/// <param name="IndZ">Индекс по оси OZ</param>
	/// <param name="linAr1D">Указатель на массив типа double</param>
	void SetLineX(size_t IndY, size_t IndZ, double* linArray)
	{
		size_t indStart = GetIndex(0, IndY, IndZ);

		size_t cnt = 0;
		for (size_t i = indStart; i < indStart + nx; i++)
		{
			data[i] = linArray[cnt++];
		}
	}

	/// <summary>
	/// Заполняет двумерный массив layerZ значениями указанного слоя z
	/// </summary>
	/// <param name="IndZ">Индекс по оси OZ</param>
	/// <param name="layerZ">Указатель на массив типа double</param>
	void GetLayerZ(size_t IndZ, double* layerZ)
	{
		size_t indStart = GetIndex(0, 0, IndZ);

		size_t cnt = 0;
		for (size_t i = indStart; i < indStart + nx * ny; i++)
		{
			layerZ[cnt++] = data[i];
		}

	}

	/// <summary>
	/// Заполняет двумерный массив layerZ значениями указанного слоя z
	/// </summary>
	/// <param name="IndZ">Индекс по оси OZ</param>
	/// <param name="layerZ">Указатель на объект класса LinearArray2D</param>
	void GetLayerZ(size_t IndZ, LinearArray2D* layerZ)
	{
		size_t indStart = GetIndex(0, 0, IndZ);

		size_t cnt = 0;
		for (size_t i = indStart; i < indStart + nx * ny; i++)
		{
			layerZ->data[cnt++] = data[i];
		}

	}

	/// <summary>
	/// Заполняет указанный слой z значениями двумерного массива layerZ
	/// </summary>
	/// <param name="IndZ">Индекс по оси OZ</param>
	/// <param name="layerZ">Указатель на массив типа double</param>
	void SetLayerZ(size_t IndZ, double* layerZ)
	{
		size_t indStart = GetIndex(0, 0, IndZ);

		size_t cnt = 0;
		for (size_t i = indStart; i < indStart + nx * ny; i++)
		{
			data[i] = layerZ[cnt++];
		}
	}

	/// <summary>
	/// Заполняет указанный слой z значениями двумерного массива layerZ
	/// </summary>
	/// <param name="IndZ">Индекс по оси OZ</param>
	/// <param name="layerZ">Указатель на объект класса LinearArray2D</param>
	void SetLayerZ(size_t IndZ, LinearArray2D* layerZ)
	{
		size_t indStart = GetIndex(0, 0, IndZ);

		size_t cnt = 0;
		for (size_t i = indStart; i < indStart + nx * ny; i++)
		{
			data[i] = layerZ->data[cnt++];
		}
	}

	/// <summary>
	/// Передает массив данных (плоскость XOY) между фрагментами вперед
	/// </summary>
	/// <param name="linearArray3DNext">Указатель на объект класса LinearArray3D</param>
	void LinearArray3DTrasferXOYForward(LinearArray3D* linearArray3DNext)
	{
		for (int i = 0; i < nx; i++)
		{
			for (int j = 0; j < ny; j++)
			{
				linearArray3DNext->data[GetIndex(i, j, 0)] = data[GetIndex(i, j, nz - 1)];
			}
		}
	}

	/// <summary>
	/// Передает массив данных (плоскость XOY) между фрагментами назад
	/// </summary>
	/// <param name="linearArray3DPrev">Указатель на объект класса LinearArray3D</param>
	void LinearArray3DTrasferXOYBack(LinearArray3D* linearArray3DPrev)
	{
		for (int i = 0; i < nx; i++)
		{
			for (int j = 0; j < ny; j++)
			{
				linearArray3DPrev->data[GetIndex(i, j, nz - 1)] = data[GetIndex(i, j, 0)];
			}
		}
	}

	/// <summary>
	/// Передает массив данных (плоскость XOZ) между фрагментами вперед
	/// </summary>
	/// <param name="linearArray3DNext">Указатель на объект класса LinearArray3D</param>
	void LinearArray3DTrasferXOZForward(LinearArray3D* linearArray3DNext)
	{
		for (int k = 0; k < nz; k++)
		{
			for (int i = 0; i < nx; i++)
			{
				linearArray3DNext->data[GetIndex(i, 0, k)] = data[GetIndex(i, ny - 1, k)];
			}
		}
	}

	/// <summary>
	/// Передает массив данных (плоскость XOZ) между фрагментами назад
	/// </summary>
	/// <param name="linearArray3DPrev">Указатель на объект класса LinearArray3D</param>
	void LinearArray3DTrasferXOZBack(LinearArray3D* linearArray3DPrev)
	{
		for (int k = 0; k < nz; k++)
		{
			for (int i = 0; i < nx; i++)
			{
				linearArray3DPrev->data[GetIndex(i, ny - 1, k)] = data[GetIndex(i, 0, k)];
			}
		}
	}

	/// <summary>
	/// Передает массив данных (плоскость YOZ) между фрагментами вперед
	/// </summary>
	/// <param name="linearArray3DNext">Указатель на объект класса LinearArray3D</param>
	void LinearArray3DTrasferYOZForward(LinearArray3D* linearArray3DNext)
	{
		for (int j = 0; j < ny; j++)
		{
			for (int k = 0; k < nz; k++)
			{
				linearArray3DNext->data[GetIndex(0, j, k)] = data[GetIndex(nx - 1, j, k)];
			}
		}
	}

	/// <summary>
	/// Передает массив данных (плоскость YOZ) между фрагментами назад
	/// </summary>
	/// <param name="linearArray3DPrev">Указатель на объект класса LinearArray3D</param>
	void LinearArray3DTrasferYOZBack(LinearArray3D* linearArray3DPrev)
	{
		for (int j = 0; j < ny; j++)
		{
			for (int k = 0; k < nz; k++)
			{
				linearArray3DPrev->data[GetIndex(nx - 1, j, k)] = data[GetIndex(0, j, k)];
			}
		}
	}

	/// <summary>
	/// Передает массив данных фрагмента (последняя по оси OZ плоскость XOY) плоскости planeXOY
	/// </summary>
	/// <param name="planeXOY">Указатель на объект класса LinearArray2D</param>
	void SetPlaneXOYLast(LinearArray2D* planeXOY)
	{
		for (int i = 0; i < nx; i++)
		{
			for (int j = 0; j < ny; j++)
			{
				planeXOY->data[planeXOY->GetIndex(i, j)] = data[GetIndex(i, j, nz - 1)];
			}
		}
	}

	/// <summary>
	/// Передает массив данных фрагмента (первая по оси OZ плоскость XOY) плоскости planeXOY
	/// </summary>
	/// <param name="planeXOY">Указатель на объект класса LinearArray2D</param>
	void SetPlaneXOYFirst(LinearArray2D* planeXOY)
	{
		for (int i = 0; i < nx; i++)
		{
			for (int j = 0; j < ny; j++)
			{
				planeXOY->data[planeXOY->GetIndex(i, j)] = data[GetIndex(i, j, 0)];
			}
		}
	}

	/// <summary>
	/// Передает массив данных фрагмента (последняя по оси OY плоскость XOZ) плоскости planeXOZ
	/// </summary>
	/// <param name="planeXOZ">Указатель на объект класса LinearArray2D</param>
	void SetPlaneXOZLast(LinearArray2D* planeXOZ)
	{
		for (int k = 0; k < nz; k++)
		{
			for (int i = 0; i < nx; i++)
			{
				planeXOZ->data[planeXOZ->GetIndex(i, k)] = data[GetIndex(i, ny - 1, k)];
			}
		}
	}

	/// <summary>
	/// Передает массив данных фрагмента (первая по оси OY плоскость XOZ) плоскости planeXOZ
	/// </summary>
	/// <param name="planeXOZ">Указатель на объект класса LinearArray2D</param>
	void SetPlaneXOZFirst(LinearArray2D* planeXOZ)
	{
		for (int i = 0; i < nx; i++)
		{
			for (int k = 0; k < nz; k++)
			{
				planeXOZ->data[planeXOZ->GetIndex(i, k)] = data[GetIndex(i, 0, k)];
			}
		}
	}

	/// <summary>
	/// Передает массив данных фрагмента (последняя по оси OX плоскость YOZ) плоскости planeYOZ
	/// </summary>
	/// <param name="planeYOZ">Указатель на объект класса LinearArray2D</param>
	void SetPlaneYOZLast(LinearArray2D* planeYOZ)
	{
		for (int j = 0; j < ny; j++)
		{
			for (int k = 0; k < nz; k++)
			{
				planeYOZ->data[planeYOZ->GetIndex(j, k)] = data[GetIndex(nx - 1, j, k)];
			}
		}
	}

	/// <summary>
	/// Передает массив данных фрагмента (первая по оси OX плоскость YOZ) плоскости planeYOZ
	/// </summary>
	/// <param name="planeYOZ">Указатель на объект класса LinearArray2D</param>
	void SetPlaneYOZFirst(LinearArray2D* planeYOZ)
	{
		for (int j = 0; j < ny; j++)
		{
			for (int k = 0; k < nz; k++)
			{
				planeYOZ->data[planeYOZ->GetIndex(j, k)] = data[GetIndex(0, j, k)];
			}
		}
	}

	/// <summary>
	/// Передает массив данных от плоскости planeXOY последней по оси OZ плоскости фрагмента
	/// </summary>
	/// <param name="planeXOY">Указатель на объект класса LinearArray2D</param>
	void GetPlaneXOYLast(LinearArray2D* planeXOY)
	{
		for (int i = 0; i < nx; i++)
		{
			for (int j = 0; j < ny; j++)
			{
				data[GetIndex(i, j, nz - 1)] = planeXOY->data[planeXOY->GetIndex(i, j)];
			}
		}
	}

	/// <summary>
	/// Передает массив данных от плоскости planeXOY первой по оси OZ плоскости фрагмента
	/// </summary>
	/// <param name="planeXOY">Указатель на объект класса LinearArray2D</param>
	void GetPlaneXOYFirst(LinearArray2D* planeXOY)
	{
		for (int i = 0; i < nx; i++)
		{
			for (int j = 0; j < ny; j++)
			{
				data[GetIndex(i, j, 0)] = planeXOY->data[planeXOY->GetIndex(i, j)];
			}
		}
	}

	/// <summary>
	/// Передает массив данных от плоскости плоскости planeXOZ последней по оси OY плоскости фрагмента
	/// </summary>
	/// <param name="planeXOZ">Указатель на объект класса LinearArray2D</param>
	void GetPlaneXOZLast(LinearArray2D* planeXOZ)
	{
		for (int i = 0; i < nx; i++)
		{
			for (int k = 0; k < nz; k++)
			{
				data[GetIndex(i, ny - 1, k)] = planeXOZ->data[planeXOZ->GetIndex(i, k)];
			}
		}
	}

	/// <summary>
	/// Передает массив данных от плоскости planeXOZ первой по оси OY плоскости фрагмента
	/// </summary>
	/// <param name="planeXOZ">Указатель на объект класса LinearArray2D</param>
	void GetPlaneXOZFirst(LinearArray2D* planeXOZ)
	{
		for (int k = 0; k < nz; k++)
		{
			for (int i = 0; i < nx; i++)
			{
				data[GetIndex(i, 0, k)] = planeXOZ->data[planeXOZ->GetIndex(i, k)];
			}
		}
	}

	/// <summary>
	/// Передает массив данных от плоскости planeYOZ последней по оси OX плоскости фрагмента
	/// </summary>
	/// <param name="planeYOZ">Указатель на объект класса LinearArray2D</param>
	void GetPlaneYOZLast(LinearArray2D* planeYOZ)
	{
		for (int j = 0; j < ny; j++)
		{
			for (int k = 0; k < nz; k++)
			{
				data[GetIndex(nx - 1, j, k)] = planeYOZ->data[planeYOZ->GetIndex(j, k)];
			}
		}
	}

	/// <summary>
	/// Передает массив данных от плоскости planeYOZ первой по оси OX плоскости фрагмента
	/// </summary>
	/// <param name="planeYOZ">Указатель на объект класса LinearArray2D</param>
	void GetPlaneYOZFirst(LinearArray2D* planeYOZ)
	{
		for (int j = 0; j < ny; j++)
		{
			for (int k = 0; k < nz; k++)
			{
				data[GetIndex(0, j, k)] = planeYOZ->data[planeYOZ->GetIndex(j, k)];
			}
		}
	}

	/// <summary>
	/// Возвращает объём оперативной памяти, занимаемый объектом LinearArray3D
	/// </summary>
	/// <returns>Объём оперативной памяти, занимаемый объектом LinearArray3D</returns>
	double GetDataSizeInMb()
	{
		double result = (double)nx * ny * nz * sizeof(double) / 1024 / 1024;
		return result;
	}

	/// <summary>
	/// Вывод элементов фрагмента по слоям XY в консоль
	/// </summary>
	void Print()
	{
		std::cout << "=================PrintArray=================" << std::endl;
		for (int k = 0; k < nz; k++)
		{
			std::cout << "z = " << k << std::endl;
			for (int j = 0; j < ny; j++)
			{
				std::cout << "y = " << j << ": ";
				for (int i = 0; i < nx; i++)
				{
					std::cout << data[GetIndex(i, j, k)] << " ";
				}
				std::cout << std::endl;
			}
		}
	}

	/// <summary>
	/// Выводит в консоль элементы массива данных data
	/// </summary>
	void PrintData()
	{
		size_t n = nx * ny * nz;
		for (int i = 0; i < n; i++)
		{
			std::cout << data[i] << " ";
		}
		std::cout << std::endl;
	}

	// FIX!!!
	/// <summary>
	/// Выводит в консоль значения массива data для плоскости fragment3DPlane фрагмента, в котором указаны размеры
	/// (координаты двух противоположных вершин прямоугольника) прямоугольной области для вывода
	/// </summary>
	/// <param name="fragment3DPlane">Элемент перечисления Fragment3DPlane</param>
	/// <param name="Ind1_1">Первая координата первой вершины прямоугольника</param>
	/// <param name="Ind1_2">Вторая координата первой вершины прямоугольника</param>
	/// <param name="Ind2_1">Первая координата второй вершины прямоугольника</param>
	/// <param name="Ind2_2">Вторая координата второй вершины прямоугольника</param>
	void Print(Fragment3DPlane fragment3DPlane, size_t Ind1_1, size_t Ind1_2, size_t Ind2_1, size_t Ind2_2)
	{
		if (fragment3DPlane == YOZ_Prev)
		{
			for (int k = Ind1_2; k <= Ind2_2; k++)
			{
				std::cout << "z = " << k << std::endl;
				for (int j = Ind1_1; j <= Ind2_1; j++)
				{
					std::cout << data[GetIndex(nx - 1, j, k)] << " ";
				}
				std::cout << std::endl;
			}
		}

		if (fragment3DPlane == YOZ_Next)
		{
			for (int k = Ind1_2; k <= Ind2_2; k++)
			{
				std::cout << "z = " << k << std::endl;
				for (int j = Ind1_1; j <= Ind2_1; j++)
				{
					std::cout << data[GetIndex(0, j, k)] << " ";
				}
				std::cout << std::endl;
			}
		}

		if (fragment3DPlane == XOZ_Prev)
		{
			for (int k = Ind1_2; k <= Ind2_2; k++)
			{
				std::cout << "z = " << k << std::endl;
				for (int i = Ind1_1; i <= Ind2_1; i++)
				{
					std::cout << data[GetIndex(i, ny - 1, k)] << " ";
				}
				std::cout << std::endl;
			}
		}

		if (fragment3DPlane == XOZ_Next)
		{
			for (int k = Ind1_2; k <= Ind2_2; k++)
			{
				std::cout << "z = " << k << std::endl;
				for (int i = Ind1_1; i <= Ind2_1; i++)
				{
					std::cout << data[GetIndex(i, 0, k)] << " ";
				}
				std::cout << std::endl;
			}
		}

		if (fragment3DPlane == XOY_Prev)
		{
			for (int j = Ind1_2; j <= Ind2_2; j++)
			{
				std::cout << "y = " << j << std::endl;
				for (int i = Ind1_1; i <= Ind2_1; i++)
				{
					std::cout << data[GetIndex(i, j, nz - 1)] << " ";
				}
				std::cout << std::endl;
			}
		}

		if (fragment3DPlane == XOY_Next)
		{
			for (int j = Ind1_2; j <= Ind2_2; j++)
			{
				std::cout << "y = " << j << std::endl;
				for (int i = Ind1_1; i <= Ind2_1; i++)
				{
					std::cout << data[GetIndex(i, j, 0)] << " ";
				}
				std::cout << std::endl;
			}
		}
	}

};

#endif