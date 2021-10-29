#ifndef LINEAR_ARRAY_2D
#define LINEAR_ARRAY_2D

/// <summary>
/// Класс для работы с двумерным массивом, сохраняемым как одномерный
/// </summary>
struct LinearArray2D
{
	size_t n1;             // число узлов в фрагменте по первой оси
	size_t n2;             // число узлов в фрагменте по второй оси	
	double* data;          // указатель на массив данных

	// Конструктор
	LinearArray2D(size_t N1, size_t N2) :
		n1(N1), n2(N2)
	{
		size_t n = n1 * n2;
		size_t dataSize = n * sizeof(double);
		data = (double*)malloc(dataSize);

		for (int i = 0; i < n; i++)
		{
			data[i] = 0;
		}
	}

	// Деструктор
	~LinearArray2D()
	{
		free(data);
	}

	// Методы

	/// <summary>
	/// Возвращает индекс элемента массива данных по индексам элемента в фрагменте
	/// </summary>
	/// <param name="Ind1">Первый индекс элемента в фрагменте</param>
	/// <param name="Ind2">Второй индекс элемента в фрагменте</param>
	/// <returns>Индекс элемента массива данных по индексам элемента в фрагменте</returns>
	size_t GetIndex(size_t Ind1, size_t Ind2)
	{
		return Ind1 + Ind2 * n1;
	}

	/// <summary>
	/// Возвращает значение элемента массива с индексом, вычисленным по индексам элемента в фрагменте
	/// </summary>
	/// <param name="Ind1">Первый индекс элемента в фрагменте</param>
	/// <param name="Ind2">Второй индекс элемента в фрагменте</param>
	/// <returns>Значение элемента массива с индексом, вычисленным по индексам элемента в фрагменте</returns>
	double GetElement(size_t Ind1, size_t Ind2)
	{		
		size_t indx = GetIndex(Ind1, Ind2);

		return data[indx];
	}

	/// <summary>
	/// Сохраняет значение элемента массива с индексом, вычисленным по индексам элемента в фрагменте
	/// </summary>
	/// <param name="Ind1">Первый индекс элемента в фрагменте</param>
	/// <param name="Ind2">Второй индекс элемента в фрагменте</param>
	/// <param name="Value">Значения элемента массива</param>
	/// <returns></returns>
	void SetElement(size_t Ind1, size_t Ind2, double Value)
	{		
		size_t indx = GetIndex(Ind1, Ind2);

		data[indx] = Value;
	}

	/// <summary>
	/// Заполняет объект LinearArray1D данными, расположенными вдоль оси Ox по указанной координате Oy
	/// </summary>
	/// <param name="IndY">Индекс по оси OY</param>	
	/// <param name="linAr1D">Указатель на объект класса LinearArray1D</param>
	void GetLineX(size_t IndY, LinearArray1D* linAr1D)
	{
		size_t indStart = GetIndex(0, IndY);

		size_t cnt = 0;
		for (size_t i = indStart; i < indStart + n1; i++)
		{
			linAr1D->data[cnt++] = data[i];
		}
	}

	/// <summary>
	/// Заполняет массив данными, расположенными вдоль оси Ox по указанной координате Oy
	/// </summary>
	/// <param name="IndY">Индекс по оси OY</param>
	/// <param name="linArray">Указатель на массив типа double</param>
	void GetLineX(size_t IndY, double* linArray)
	{
		size_t indStart = GetIndex(0, IndY);

		size_t cnt = 0;
		for (size_t i = indStart; i < indStart + n1; i++)
		{
			linArray[cnt++] = data[i];
		}
	}

	/// <summary>
	/// Заполняет элементы объекта, расположенные вдоль оси Ox по указанной координате Oy, данными из LinearArray1D
	/// </summary>
	/// <param name="IndY">Индекс по оси OY</param>
	/// <param name="linAr1D">Указатель на объект класса LinearArray1D</param>
	void SetLineX(size_t IndY, LinearArray1D* linAr1D)
	{
		size_t indStart = GetIndex(0, IndY);

		size_t cnt = 0;
		for (size_t i = indStart; i < indStart + n1; i++)
		{
			data[i] = linAr1D->data[cnt++];
		}
	}

	/// <summary>
	/// Заполняет элементы объекта, расположенные вдоль оси Ox по указанной координате Oy, данными из LinearArray1D
	/// </summary>
	/// <param name="IndY">Индекс по оси OY</param>
	/// <param name="linAr1D">Указатель на массив типа double</param>
	void SetLineX(size_t IndY, double* linArray)
	{
		size_t indStart = GetIndex(0, IndY);

		size_t cnt = 0;
		for (size_t i = indStart; i < indStart + n1; i++)
		{
			data[i] = linArray[cnt++];
		}
	}

	/// <summary>
	/// Возвращает объём оперативной памяти, занимаемый объектом LinearArray2D
	/// </summary>
	/// <returns>Объём оперативной памяти, занимаемый объектом LinearArray2D</returns>
	double GetDataSizeInMb()
	{
		double result = (double)n1 * n2 * sizeof(double) / 1024 / 1024;
		return result;
	}

	/// <summary>
	/// Выводит элементы фрагмента по слоям XY в консоль
	/// </summary>
	void Print()
	{
		std::cout << std::fixed;
		std::cout << "=================PrintArray=================" << std::endl;
		for (int j = 0; j < n2; j++)
		{
			std::cout << j << ": ";
			for (int i = 0; i < n1; i++)
			{
				std::cout << data[GetIndex(i, j)] << " ";
			}
			std::cout << std::endl;
		}
	}

};

#endif