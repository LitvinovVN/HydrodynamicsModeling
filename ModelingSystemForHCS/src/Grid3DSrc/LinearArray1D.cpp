#ifndef LINEAR_ARRAY_1D
#define LINEAR_ARRAY_1D

/// <summary>
/// Линейный одномерный массив
/// </summary>
struct LinearArray1D
{
	size_t nx;             ///< число узлов в фрагменте по оси Ox	
	double* data;          ///< указатель на массив данных

	/// <summary>
	/// Конструктор
	/// </summary>
	LinearArray1D(size_t Nx) :
		nx(Nx)
	{
		size_t n = nx;
		size_t dataSize = n * sizeof(double);
		data = (double*)malloc(dataSize);

		for (int i = 0; i < n; i++)
		{
			data[i] = 0;
		}
	}

	/// <summary>
	/// Деструктор
	/// </summary>
	~LinearArray1D()
	{
		free(data);
	}

	// Методы	

	/// <summary>
	/// Возвращает значение элемента массива по указанному индексу
	/// </summary>
	/// <param name="IndX">Индекс элемента массива</param>
	/// <returns>Значение элемента массива по указанному индексу</returns>
	double GetElement(size_t IndX)
	{
		return data[IndX];
	}

	/// <summary>
	/// Сохраняет значение элемента массива с индексом, вычисленным по индексам элемента в фрагменте
	/// </summary>
	/// <param name="IndX">Индекс элемента массива</param>	
	/// <param name="Value">Значение элемента массива</param>	
	void SetElement(size_t IndX, double Value)
	{
		data[IndX] = Value;
	}

	/// <summary>
	/// Возвращает объём оперативной памяти, занимаемый объектом LinearArray3D
	/// </summary>
	/// <returns></returns>
	double GetDataSizeInMb()
	{
		double result = (double)nx * sizeof(double) / 1024 / 1024;
		return result;
	}

	/// <summary>
	/// Вывод элементов фрагмента по слоям XY в консоль
	/// </summary>
	void Print()
	{
		std::cout << "=================PrintArray=================" << std::endl;
		for (int i = 0; i < nx; i++)
		{
			std::cout << data[i] << " ";
		}
		std::cout << std::endl;
	}

};

#endif