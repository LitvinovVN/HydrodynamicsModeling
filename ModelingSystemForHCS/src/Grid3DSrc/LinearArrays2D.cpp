#ifndef LINEAR_ARRAYS_2D
#define LINEAR_ARRAYS_2D


/// <summary>
/// Реализует хранение объектов класса LinearArray2D с доступом к данным по имени массива данных ModelDataName
/// </summary>
struct LinearArrays2D
{
	std::vector<ModelDataName> modelDataNames;				// вектор элементов modelDataName
	std::map<ModelDataName, LinearArray2D> linearArrays;	// контейнер, устанавливающий соотвествие между объектом LinearArray2D и именем ModelDataName

	// Методы

	/// <summary>
	/// Создает указатель на объект LinearArray2D размером (nx, ny) по указанному имени массива данных modelDataName
	/// </summary>
	/// <param name="nx">Число узлов по первой оси</param>
	/// <param name="ny">Число узлов по второй оси</param>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	void LinearArrayCreate(size_t nx, size_t ny, ModelDataName modelDataName)
	{
		LinearArray2D* linearArray2D = new LinearArray2D(nx, ny);
		linearArrays.emplace(modelDataName, std::ref(*linearArray2D));
		modelDataNames.push_back(modelDataName);
	}

	/// <summary>
	/// Возвращает объём оперативной памяти, занимаемый объектом LinearArrays
	/// </summary>
	/// <returns>Объём оперативной памяти, занимаемый объектом LinearArrays</returns>
	double GetDataSizeInMb()
	{
		double result = 0;

		for (auto itByLinearArrays = linearArrays.begin(); itByLinearArrays != linearArrays.end(); itByLinearArrays++)
		{
			auto linearArrayKey = itByLinearArrays->first;
			LinearArray2D* linearArrayObj = &(itByLinearArrays->second);
			result += linearArrayObj->GetDataSizeInMb();
		}

		return result;
	}

	/// <summary>
	/// Выводит в консоль все объекты класса LinearArray2D
	/// </summary>
	void Print()
	{
		for (auto itByLinearArrays = linearArrays.begin(); itByLinearArrays != linearArrays.end(); itByLinearArrays++)
		{
			auto linearArrayKey = itByLinearArrays->first;
			LinearArray2D* linearArrayObj = &(itByLinearArrays->second);
			std::cout << linearArrayKey << ": " << std::endl;
			linearArrayObj->Print();
		}
	}

	/// <summary>
	/// Выводит в консоль объект класса LinearArray2D по указанному имени массива данных modelDataName
	/// </summary>
	/// <param name="modelDataName">Имя массива данных modelDataName</param>
	void Print(ModelDataName modelDataName)
	{
		LinearArray2D& linearArrayObj = linearArrays.at(modelDataName);
		linearArrayObj.Print();
	}

};

#endif