#ifndef LINEAR_ARRAYS_3D
#define LINEAR_ARRAYS_3D

/// <summary>
/// Реализует хранение объектов класса LinearArray3D с доступом к данным по имени массива данных modelDataName
/// </summary>
struct LinearArrays3D
{
	std::vector<ModelDataName> modelDataNames;				// вектор элементов ModelDataName
	std::map<ModelDataName, LinearArray3D> linearArrays;	// контейнер, устанавливающий соотвествие между объектом LinearArray3D и именем ModelDataName

	~LinearArrays3D()
	{
		//std::cout << "~LinearArrays3D() invoked" << std::endl;
	}

	// Методы

	/// <summary>
	/// Проверяет: содержит ли объект массив данных с именем modelDataName
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	/// <returns>Результат проверки наличия массива данных с именем modelDataName (true - в наличии, false - отсутствует)</returns>
	bool IsContain(ModelDataName modelDataName)
	{
		for (auto curModelDataName : modelDataNames)
		{
			if (curModelDataName == modelDataName)
				return true;
		}
		return false;
	}

	/// <summary>
	/// $$$$Создаёт в памяти и возвращает указатель на двумерный массив данных типа LinearArray2D
	/// по имени массива данных ModelDataName (плоскость XOY для z=0)
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	/// <returns>$$$$Указатель на двумерный массив данных типа LinearArray2D</returns>
	LinearArrays2D* GetLinearArrays2DXYFirst(ModelDataName modelDataName)
	{
		if (!IsContain(modelDataName))
		{
			return NULL;
		}

		LinearArrays2D* linearArrays2D = new LinearArrays2D();
		linearArrays2D->modelDataNames.emplace_back(modelDataName);

		LinearArray3D* linearArray3D = GetLinearArray3D(modelDataName);
		LinearArray2D* linearArray2D = new LinearArray2D(linearArray3D->nx, linearArray3D->ny);

		auto k = 0;
		for (auto j = 0; j < linearArray3D->ny; j++)
			for (auto i = 0; i < linearArray3D->nx; i++)
			{
				auto value = linearArray3D->GetElement(i, j, k);
				linearArray2D->SetElement(i, j, value);
			}


		linearArrays2D->linearArrays.emplace(modelDataName, std::ref(*linearArray2D));

		return linearArrays2D;
	}

	/// <summary>
	/// $$$$Создаёт в памяти и возвращает указатель на двумерный массив данных типа LinearArray2D
	/// по имени массива данных modelDataName (плоскость XOY для z=nz - 1)
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	/// <returns>$$$$Указатель на двумерный массив данных типа LinearArray2D</returns>
	LinearArrays2D* GetLinearArrays2DXYLast(ModelDataName modelDataName)
	{
		if (!IsContain(modelDataName))
		{
			return NULL;
		}

		LinearArrays2D* linearArrays2D = new LinearArrays2D();
		linearArrays2D->modelDataNames.emplace_back(modelDataName);

		LinearArray3D* linearArray3D = GetLinearArray3D(modelDataName);
		LinearArray2D* linearArray2D = new LinearArray2D(linearArray3D->nx, linearArray3D->ny);

		auto k = linearArray3D->nz - 1;
		for (auto j = 0; j < linearArray3D->ny; j++)
			for (auto i = 0; i < linearArray3D->nx; i++)
			{
				auto value = linearArray3D->GetElement(i, j, k);
				linearArray2D->SetElement(i, j, value);
			}


		linearArrays2D->linearArrays.emplace(modelDataName, std::ref(*linearArray2D));

		return linearArrays2D;
	}

	/// <summary>
	/// $$$$Создаёт в памяти и возвращает указатель на двумерный массив данных типа LinearArray2D
	/// по имени массива данных modelDataName (плоскость YOZ для x=0)
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	/// <returns>$$$$Указатель на двумерный массив данных типа LinearArray2D</returns>
	LinearArrays2D* GetLinearArrays2DYZFirst(ModelDataName modelDataName)
	{
		if (!IsContain(modelDataName))
		{
			return NULL;
		}

		LinearArrays2D* linearArrays2D = new LinearArrays2D();
		linearArrays2D->modelDataNames.emplace_back(modelDataName);

		LinearArray3D* linearArray3D = GetLinearArray3D(modelDataName);
		LinearArray2D* linearArray2D = new LinearArray2D(linearArray3D->ny, linearArray3D->nz);

		auto i = 0;
		for (auto k = 0; k < linearArray3D->nz; k++)
			for (auto j = 0; j < linearArray3D->ny; j++)
			{
				auto value = linearArray3D->GetElement(i, j, k);
				linearArray2D->SetElement(j, k, value);
			}


		linearArrays2D->linearArrays.emplace(modelDataName, std::ref(*linearArray2D));

		return linearArrays2D;
	}

	/// <summary>
	/// $$$$Создаёт в памяти и возвращает указатель на двумерный массив данных типа LinearArray2D
	/// по имени массива данных modelDataName (плоскость YOZ для x=nx - 1)
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	/// <returns>$$$$Указатель на двумерный массив данных типа LinearArray2D</returns>
	LinearArrays2D* GetLinearArrays2DYZLast(ModelDataName modelDataName)
	{
		if (!IsContain(modelDataName))
		{
			return NULL;
		}

		LinearArrays2D* linearArrays2D = new LinearArrays2D();
		linearArrays2D->modelDataNames.emplace_back(modelDataName);

		LinearArray3D* linearArray3D = GetLinearArray3D(modelDataName);
		LinearArray2D* linearArray2D = new LinearArray2D(linearArray3D->ny, linearArray3D->nz);

		auto i = linearArray3D->nx - 1;
		for (auto k = 0; k < linearArray3D->nz; k++)
			for (auto j = 0; j < linearArray3D->ny; j++)
			{
				auto value = linearArray3D->GetElement(i, j, k);
				linearArray2D->SetElement(j, k, value);
			}


		linearArrays2D->linearArrays.emplace(modelDataName, std::ref(*linearArray2D));

		return linearArrays2D;
	}

	/// <summary>
	/// $$$$Создаёт в памяти и возвращает указатель на двумерный массив данных типа LinearArray2D
	/// по имени массива данных modelDataName (плоскость XOZ для y=0)
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	/// <returns>$$$$Указатель на двумерный массив данных типа LinearArray2D</returns>
	LinearArrays2D* GetLinearArrays2DXZFirst(ModelDataName modelDataName)
	{
		if (!IsContain(modelDataName))
		{
			return NULL;
		}

		LinearArrays2D* linearArrays2D = new LinearArrays2D();
		linearArrays2D->modelDataNames.emplace_back(modelDataName);

		LinearArray3D* linearArray3D = GetLinearArray3D(modelDataName);
		LinearArray2D* linearArray2D = new LinearArray2D(linearArray3D->nx, linearArray3D->nz);

		auto j = 0;
		for (auto k = 0; k < linearArray3D->nz; k++)
			for (auto i = 0; i < linearArray3D->nx; i++)
			{
				auto value = linearArray3D->GetElement(i, j, k);
				linearArray2D->SetElement(i, k, value);
			}

		linearArrays2D->linearArrays.emplace(modelDataName, std::ref(*linearArray2D));

		return linearArrays2D;
	}

	/// <summary>
	/// $$$$Создаёт в памяти и возвращает указатель на двумерный массив данных типа LinearArray2D
	/// по имени массива данных modelDataName (плоскость XOZ для y=ny - 1)
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	/// <returns>$$$$Указатель на двумерный массив данных типа LinearArray2D</returns>
	LinearArrays2D* GetLinearArrays2DXZLast(ModelDataName modelDataName)
	{
		if (!IsContain(modelDataName))
		{
			return NULL;
		}

		LinearArrays2D* linearArrays2D = new LinearArrays2D();
		linearArrays2D->modelDataNames.emplace_back(modelDataName);

		LinearArray3D* linearArray3D = GetLinearArray3D(modelDataName);
		LinearArray2D* linearArray2D = new LinearArray2D(linearArray3D->nx, linearArray3D->nz);

		auto j = linearArray3D->ny - 1;
		for (auto k = 0; k < linearArray3D->nz; k++)
			for (auto i = 0; i < linearArray3D->nx; i++)
			{
				auto value = linearArray3D->GetElement(i, j, k);
				linearArray2D->SetElement(i, k, value);
			}

		linearArrays2D->linearArrays.emplace(modelDataName, std::ref(*linearArray2D));

		return linearArrays2D;
	}

	/// <summary>
	/// $$$$Возвращает указатель на трехмерный массив типа LinearArray3D по указанному имени массива данных modelDataName
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	/// <returns>Указатель на трехмерный массив типа LinearArray3D по указанному имени массива данных modelDataName</returns>
	LinearArray3D* GetLinearArray3D(ModelDataName modelDataName)
	{
		LinearArray3D* linearArray3D = &linearArrays.at(modelDataName);
		return linearArray3D;
	}

	/// <summary>
	/// $$$$Создает трехмерный массив данных типа LinearArray3D(nx, ny, nz) для указанного имени массива данных modelDataName
	/// </summary>
	/// <param name="nx">Число узлов по оси OX</param>
	/// <param name="ny">Число узлов по оси OY</param>
	/// <param name="nz">Число узлов по оси OZ</param>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	void LinearArrayCreate(size_t nx, size_t ny, size_t nz, ModelDataName modelDataName)
	{
		LinearArray3D* linearArray3D = new LinearArray3D(nx, ny, nz);
		linearArrays.emplace(modelDataName, std::ref(*linearArray3D));
		modelDataNames.push_back(modelDataName);
	}

	/// <summary>
	/// Возвращает объём оперативной памяти, занимаемый объектом LinearArray3D
	/// </summary>
	/// <returns>Объём оперативной памяти, занимаемый объектом LinearArray3D</returns>
	double GetDataSizeInMb()
	{
		double result = 0;

		for (auto itByLinearArrays = linearArrays.begin(); itByLinearArrays != linearArrays.end(); itByLinearArrays++)
		{
			auto linearArrayKey = itByLinearArrays->first;
			LinearArray3D* linearArrayObj = &(itByLinearArrays->second);
			result += linearArrayObj->GetDataSizeInMb();
		}

		return result;
	}

	/// <summary>
	/// $$$$Выводит в консоль все объекты класса LinearArray3D
	/// </summary>
	void Print()
	{
		for (auto itByLinearArrays = linearArrays.begin(); itByLinearArrays != linearArrays.end(); itByLinearArrays++)
		{
			auto linearArrayKey = itByLinearArrays->first;
			LinearArray3D* linearArrayObj = &(itByLinearArrays->second);
			std::cout << linearArrayKey << ": " << std::endl;
			linearArrayObj->Print();
		}
	}
};

#endif