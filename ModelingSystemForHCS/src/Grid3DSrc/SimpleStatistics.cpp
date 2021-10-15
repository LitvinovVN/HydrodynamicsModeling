#ifndef SIMPLE_STATISTICS
#define SIMPLE_STATISTICS

/// <summary>
/// Структура для сохранения результатов измерений и вычисления простых статистик
/// </summary>
struct SimpleStatistics
{
	bool isSorted;///< Признак отсортированности массивов
	std::vector<double> data;///< Массив с результатами экспериментов

	/// <summary>
	/// Добавляет новый элемент в массив данных
	/// </summary>
	void add(double value)
	{
		data.push_back(value);
		isSorted = false;
	}

	/// <summary>
	/// Сортирует массив данных
	/// </summary>
	void sort()
	{
		std::sort(data.begin(), data.end());
	}

	/// <summary>
	/// Возвращает минимальное значение
	/// </summary>
	/// <returns></returns>
	double getMin()
	{
		if (!isSorted) sort();
		return data[0];
	}

	/// <summary>
	/// Возвращает максимальное значение
	/// </summary>
	double getMax()
	{
		if (!isSorted) sort();
		return data[data.size() - 1];
	}

	/// <summary>
	/// Возвращает медиану
	/// </summary>	
	double getMedian()
	{
		auto size = data.size();
		if (size == 0)
			throw std::domain_error("median of an empty vector");

		if (!isSorted) sort();
		auto mid = size / 2;
		return size % 2 == 0 ? (data[mid] + data[mid - 1]) / 2 : data[mid];
	}

	/// <summary>
	/// Возвращает указанный процентиль
	/// </summary>
	/// <param name="percentile"></param>
	/// <returns></returns>
	double getPercentile(double percentile)
	{
		auto size = data.size();
		if (size == 0)
			throw std::domain_error("percentile of an empty vector");

		auto index = int(round(percentile * size / 100)) - 1;
		double result = data[index];
		return result;
	}

	/// <summary>
	/// Выводит в стандартный поток вывода элементы массива данных, разделённые пробелами.
	/// </summary>
	void printData()
	{
		if (!isSorted) sort();
		for (size_t i = 0; i < data.size(); i++)
		{
			std::cout << data[i] << " ";
		}
		std::cout << std::endl;
	}

	void print()
	{
		std::cout << "data: ";
		printData();
		std::cout << "data.size() = " << data.size() << "; ";
		std::cout << "min = " << getMin() << "; ";
		std::cout << "max = " << getMax() << "; ";
		std::cout << "median = " << getMedian() << "; ";
		std::cout << "90 perc = " << getPercentile(90) << std::endl;
	}
};

#endif