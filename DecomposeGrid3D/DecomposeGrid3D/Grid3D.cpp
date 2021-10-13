#pragma once

#include <iostream>
#include <functional> // std::ref
#include <map>
#include <vector>
#include <chrono>
#include <algorithm>    // std::sort

/// <summary>
/// Вспомогательные вычислительные методы для Grid
/// </summary>
static struct GridHelper
{
	static size_t CountN(size_t nx, size_t ny, size_t nz)
	{
		size_t N = nx * ny * nz;
		return N;
	}
} GridHelper;

/// Перечисление наименований массивов данных модели
enum ModelDataName
{
	U,///< fgsdfg 
	V,
	W,
	P,
	MU,
	NU,
	C0,
	C1,
	C2,
	C3,
	C4,
	C5,
	C6,
	F1,
	F2,
	F3,
	S
};

/// <summary>
/// Перечисление уровней детализации вывода метода Print класса Grid3D
/// </summary>
enum Grid3DPrintDetalisation
{
	GRID,
	NODES,
	DEVICES,
	FRAGMENTS,
	ARRAYS
};

/// <summary>
/// Перечисление плоскостей для передачи между фрагментами
/// </summary>
enum Fragment3DPlane
{
	XOZ_Prev,
	XOZ_Next,
	XOY_Prev,
	XOY_Next,
	YOZ_Prev,
	YOZ_Next	
};



/// <summary>
/// Таймер
/// </summary>
/// <typeparam name="duration">Единица измерения</typeparam>
/// <typeparam name="clock">Настройки часов</typeparam>
template <typename duration = std::chrono::seconds, typename clock = std::chrono::high_resolution_clock>
class timer
{
	/// Старт, стоп
	typename clock::time_point m_start, m_stop;

	typename clock::rep get_time() const
	{
		return std::chrono::duration_cast<duration>(m_stop - m_start).count();
	}

public:
	void         start() { m_start = clock::now(); }
	const timer& stop() { m_stop = clock::now(); return *this; }

	std::ostream& print() const
	{
		return std::cout << "Time running: [" << get_time() << "]";
	}

	double get_time_as_double()
	{
		double time = get_time();
		return time;
	}
};

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




#pragma region LinearArrays
/// <summary>
/// Линейный одномерный массив
/// </summary>
struct LinearArray1D
{
	size_t nx;             // число узлов в фрагменте по оси Ox	
	double* data;          // указатель на массив данных

	// Конструктор
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

	// Деструктор
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
		if ((Ind1 > n1) || (Ind2 > n2))
		{
			throw - 1;
		}
		size_t indx = GetIndex(Ind1, Ind2);

		return data[indx];
	}

	/// <summary>
	/// Сохраняет значения элемента массива с индексом, вычисленным по индексам элемента в фрагменте
	/// </summary>
	/// <param name="Ind1">Первый индекс элемента в фрагменте</param>
	/// <param name="Ind2">Второй индекс элемента в фрагменте</param>
	/// <param name="Value">Значения элемента массива</param>
	/// <returns></returns>
	double SetElement(size_t Ind1, size_t Ind2, double Value)
	{
		if ((Ind1 > n1) || (Ind2 > n2))
		{
			throw - 1;
		}
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
		if ((IndX > nx) || (IndY > ny) || (IndZ > nz))
		{
			throw - 1;
		}
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
		if ((IndX > nx) || (IndY > ny) || (IndZ > nz))
		{
			throw - 1;
		}
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
		for (int i = 0; i < nx; i++)
		{
			for (int k = 0; k < nz; k++)
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
		for (int i = 0; i < nx; i++)
		{
			for (int k = 0; k < nz; k++)
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
		for (int i = 0; i < nx; i++)
		{
			for (int k = 0; k < nz; k++)
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
		for (int i = 0; i < nx; i++)
		{
			for (int k = 0; k < nz; k++)
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
		size_t n = nx*ny*nz;
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
					std::cout << data[GetIndex(nx-1, j, k)] << " ";
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
					std::cout << data[GetIndex(i, ny-1, k)] << " ";
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
					std::cout << data[GetIndex(i, j, nz-1)] << " ";
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

/// <summary>
/// $$$$-----------
/// </summary>
struct LinearArrays2D
{
	std::vector<ModelDataName> modelDataNames;				//$$$$-----------
	std::map<ModelDataName, LinearArray2D> linearArrays;	//$$$$-----------

	// Методы

	/// <summary>
	/// $$$$---------
	/// </summary>
	/// <param name="nx"></param>
	/// <param name="ny"></param>
	/// <param name="modelDataName"></param>
	void LinearArrayCreate(size_t nx, size_t ny, ModelDataName modelDataName)
	{
		LinearArray2D* linearArray2D = new LinearArray2D(nx, ny);
		linearArrays.emplace(modelDataName, std::ref(*linearArray2D));
		modelDataNames.push_back(modelDataName);
	}

	/// <summary>
	/// Возвращает объём оперативной памяти, занимаемый объектом LinearArrays
	/// </summary>
	/// <returns></returns>
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
	/// $$$$---------
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
	/// $$$$------------
	/// </summary>
	/// <param name="modelDataName"></param>
	void Print(ModelDataName modelDataName)
	{		
		LinearArray2D& linearArrayObj = linearArrays.at(modelDataName);
		linearArrayObj.Print();
	}

};

/// <summary>
/// $$$$-----------
/// </summary>
struct LinearArrays3D
{
	std::vector<ModelDataName> modelDataNames;				//$$$$----------
	std::map<ModelDataName, LinearArray3D> linearArrays;	//$$$$----------

	~LinearArrays3D()
	{
		//std::cout << "~LinearArrays3D() invoked" << std::endl;
	}

	// Методы

	/// <summary>
	/// $$$$$------------------
	/// </summary>
	/// <param name="modelDataName"></param>
	/// <returns></returns>
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
	/// $$$$-----------------
	/// </summary>
	/// <param name="modelDataName"></param>
	/// <returns></returns>
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
	/// $$$$--------------
	/// </summary>
	/// <param name="modelDataName"></param>
	/// <returns></returns>
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
	/// $$$$---------------
	/// </summary>
	/// <param name="modelDataName"></param>
	/// <returns></returns>
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
	/// $$$$$--------------
	/// </summary>
	/// <param name="modelDataName"></param>
	/// <returns></returns>
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
	/// $$$$-------------
	/// </summary>
	/// <param name="modelDataName"></param>
	/// <returns></returns>
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
	/// $$$$$-----------
	/// </summary>
	/// <param name="modelDataName"></param>
	/// <returns></returns>
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
	/// $$$$----------
	/// </summary>
	/// <param name="modelDataName"></param>
	/// <returns></returns>
	LinearArray3D* GetLinearArray3D(ModelDataName modelDataName)
	{
		LinearArray3D* linearArray3D = &linearArrays.at(modelDataName);
		return linearArray3D;
	}

	/// <summary>
	/// $$$$-----
	/// </summary>
	/// <param name="nx"></param>
	/// <param name="ny"></param>
	/// <param name="nz"></param>
	/// <param name="modelDataName"></param>
	void LinearArrayCreate(size_t nx, size_t ny, size_t nz, ModelDataName modelDataName)
	{		
		LinearArray3D* linearArray3D = new LinearArray3D(nx, ny, nz);
		linearArrays.emplace(modelDataName, std::ref(*linearArray3D));
		modelDataNames.push_back(modelDataName);
	}

	/// <summary>
	/// Возвращает объём оперативной памяти, занимаемый объектом LinearArrays3D
	/// </summary>
	/// <returns></returns>
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
	/// $$$$------
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

#pragma endregion LinearArrays


#pragma region Devices

struct ComputingDevice
{
	std::string deviceId;			// уид устройства (CPU или GPU)
	double devicePerfomance;		// производительность устройства

	ComputingDevice(std::string DeviceId, double DevicePerfomance) :
		deviceId(DeviceId), devicePerfomance(DevicePerfomance)
	{

	}

	// Методы

	virtual long GetNumThreads()
	{
		return 0;
	}

	virtual void PrintDevice()
	{
		std::cout << "\t" << deviceId << ": " << "\tdevicePerfomance = " << devicePerfomance;
	}
};

struct ComputingDeviceGPU : ComputingDevice
{
	long spNumber;				// количество мультипроцессоров SP
	long sharedMemory;			// объем распределяемой памяти
	long globalMemory;			// объем видеопамяти GPU

	ComputingDeviceGPU(std::string DeviceId, double DevicePerfomance, long SpNumber, long SharedMemory, long GlobalMemory) :
		ComputingDevice(DeviceId, DevicePerfomance), spNumber(SpNumber), sharedMemory(SharedMemory), globalMemory(GlobalMemory)
	{

	}

	// Методы

	long GetNumThreads() override	// Возвращает количество потоков
	{
		return spNumber;
	}

	void PrintDevice() override
	{
		ComputingDevice::PrintDevice();
		std::cout << "\tspNumber = " << spNumber << "\tsharedMemory = " << sharedMemory << "\tglobalMemory = " << globalMemory << std::endl;
	}
};

struct ComputingDeviceCPU : ComputingDevice
{
	int coreNumber;			// число ядер CPU на вычислительном узле

	ComputingDeviceCPU(std::string DeviceId, double DevicePerfomance, int CoreNumber) :
		ComputingDevice(DeviceId, DevicePerfomance), coreNumber(CoreNumber)
	{

	}

	// Методы

	long GetNumThreads() override	// Возвращает количество потоков
	{
		return coreNumber;
	}

	void PrintDevice() override
	{
		ComputingDevice::PrintDevice();
		std::cout << "\tcoreNumber = " << coreNumber << std::endl;
	}
};

#pragma endregion Devices


#pragma region Cluster

struct ComputingNode
{
	std::string nodeId;			// уид вычислительного узла
	double ramMemorySize;		// размер оперативной памяти вычислительного узла
	double nodePerfomance;		// производительность вычислительного узла
	std::map<std::string, ComputingDevice&> computingDevices;		// массив объектов ComputingDevice

	ComputingNode(std::string NodeId, double RamMemorySize, double NodePerfomance) :
		nodeId(NodeId), ramMemorySize(RamMemorySize), nodePerfomance(NodePerfomance)
	{

	}

	// Методы

	int CountDevices()	// вычисляет количество devices
	{
		int countDevices = 0;
		for (auto itByDevices = computingDevices.begin(); itByDevices != computingDevices.end(); itByDevices++)
		{
			auto nodeKey = itByDevices->first;
			ComputingDevice* nodeObj = &(itByDevices->second);
			countDevices += 1;
		}
		//std::cout << "-------------CountDevices   " << "\tcountDevices = " << countDevices << std::endl;
		return countDevices;
	}

	double GetSumDevicePerformance()	// вычисляет суммарную производительность всех устройств на вычислительном узле
	{
		double sumDevicePerfomance = 0;

		for (auto itByDevices = computingDevices.begin(); itByDevices != computingDevices.end(); itByDevices++)
		{
			auto deviceKey = itByDevices->first;
			auto deviceObj = itByDevices->second;
			sumDevicePerfomance += deviceObj.devicePerfomance;
		}

		return sumDevicePerfomance;
	}


};

/// <summary>
/// Computing cluster describing - Вычислительный кластер
/// </summary>
struct ComputingCluster
{
	std::map<std::string, ComputingNode> computingNodes;

	// Методы

	int CountNodes()	// вычисляет количество nodes на кластере
	{
		int countNodes = 0;
		for (auto itByNodes = computingNodes.begin(); itByNodes != computingNodes.end(); itByNodes++)
		{
			auto nodeKey = itByNodes->first;
			ComputingNode* nodeObj = &(itByNodes->second);
			countNodes += 1;
		}

		return countNodes;
	}

	double GetClusterPerfomance()	// вычисляет производительность кластера
	{
		double sumPerfomance = 0;

		for (auto itByNodes = computingNodes.begin(); itByNodes != computingNodes.end(); itByNodes++)
			{
				auto nodeKey = itByNodes->first;
				auto nodeObj = itByNodes->second;
				sumPerfomance += nodeObj.nodePerfomance;
			}

		return sumPerfomance;
	}
	

	void Print()
	{
		for (auto itByNodes = computingNodes.begin(); itByNodes != computingNodes.end(); itByNodes++) {	// выводим nodes
			auto nodeKey = itByNodes->first;
			ComputingNode* nodeObj = &(itByNodes->second);
			std::cout << nodeKey << ": " << "\tramMemorySize=" << nodeObj->ramMemorySize
				<< "\tnodePerfomance=" << nodeObj->nodePerfomance << std::endl;

			for (auto itByDevices = nodeObj->computingDevices.begin(); itByDevices != nodeObj->computingDevices.end(); itByDevices++) {	// выводим devices
				auto deviceKey = itByDevices->first;
				ComputingDevice& deviceObj = itByDevices->second;
				deviceObj.PrintDevice();
			}
		}

	}
};

#pragma endregion Cluster

#pragma region Grid

/// <summary>
/// Двумерный фрагмент расчетной сетки - плоскость XY(грань трехмерного фрагмента)
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
	/// <returns></returns>
	size_t GetNodesNumber()
	{
		size_t nodesNumber = fragmentNx * fragmentNy;
		return nodesNumber;
	}		

	/// <summary>
	/// $$$$--------
	/// </summary>
	/// <param name="modelDataName"></param>
	void LinearArrayCreate(ModelDataName modelDataName)
	{
		linearArrays2D.LinearArrayCreate(fragmentNx, fragmentNy, modelDataName);
	}

	/// <summary>
	/// $$$$--------
	/// </summary>
	/// <param name="modelDataName"></param>
	/// <returns></returns>
	LinearArray2D* GetLinearArray2D(ModelDataName modelDataName)
	{
		LinearArray2D* linearArray2D = &(linearArrays2D.linearArrays.at(modelDataName));
		return linearArray2D;
	}

	/// <summary>
	/// $$$$---------
	/// </summary>
	/// <param name="data"></param>
	/// <param name="nx"></param>
	/// <param name="ny"></param>
	/// <param name="modelDataName"></param>
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
	/// <returns></returns>
	double GetDataSizeInMb()
	{
		double result = linearArrays2D.GetDataSizeInMb();
		return result;
	}

	/// <summary>
	/// $$$$-------
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

/// <summary>
/// Двумерный фрагмент расчетной сетки - плоскость YZ(грань трехмерного фрагмента)
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
	/// <returns></returns>
	size_t GetNodesNumber()
	{
		size_t nodesNumber = fragmentNy * fragmentNz;
		return nodesNumber;
	}

	/// <summary>
	/// Выделяет память и создает двумерный массив данных типа ModelDataName размерами fragmentNy на fragmentNz
	/// </summary>
	/// <param name="modelDataName"></param>
	void LinearArrayCreate(ModelDataName modelDataName)
	{
		linearArrays2D.LinearArrayCreate(fragmentNy, fragmentNz, modelDataName);
	}

	/// <summary>
	/// Возвращает указатель на объект linearArray2D типа ModelDataName
	/// </summary>
	/// <param name="modelDataName"></param>
	/// <returns></returns>
	LinearArray2D* GetLinearArray2D(ModelDataName modelDataName)
	{
		LinearArray2D* linearArray2D = &(linearArrays2D.linearArrays.at(modelDataName));
		return linearArray2D;
	}
	
	/// <summary>
	/// Возвращает элемент массива данных типа ModelDataName по указанным координатам IndX и IndZ
	/// </summary>
	/// <param name="IndX"></param>
	/// <param name="IndZ"></param>
	/// <param name="modelDataName"></param>
	/// <returns></returns>
	double GetElement(size_t IndX, size_t IndZ, ModelDataName modelDataName)
	{
		LinearArray2D* linearArray2D = GetLinearArray2D(modelDataName);
		return linearArray2D->GetElement(IndX, IndZ);
	}

	/// <summary>
	/// 
	/// </summary>
	/// <param name="IndX"></param>
	/// <param name="IndZ"></param>
	/// <param name="modelDataName"></param>
	/// <param name="Value"></param>
	/// <returns></returns>
	double SetElement(size_t IndX, size_t IndZ, ModelDataName modelDataName, double Value)
	{
		LinearArray2D* linearArray2D = GetLinearArray2D(modelDataName);
		linearArray2D->SetElement(IndX, IndZ, Value);
	}

	/// <summary>
	/// Возвращает объём оперативной памяти, занимаемый фрагментом
	/// </summary>
	/// <returns></returns>
	double GetDataSizeInMb()
	{
		double result = linearArrays2D.GetDataSizeInMb();
		return result;
	}

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

/// <summary>
/// Двумерный фрагмент расчетной сетки - плоскость XZ(грань трехмерного фрагмента)
/// </summary>
struct Grid2DFragmentXZ
{
	size_t fragmentNx;			// количество узлов в фрагменте блока устройства вычислительного узла по оси Ox
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

	Grid2DFragmentXZ(size_t FragmentNx, size_t FragmentNz, size_t FragmentIndX, size_t FragmentIndY, size_t FragmentIndZ,
		size_t FragmentOffsetXByDevice, size_t FragmentOffsetYByDevice, size_t FragmentOffsetZByDevice,
		size_t FragmentOffsetXByNode, size_t FragmentOffsetYByNode, size_t FragmentOffsetZByNode,
		size_t FragmentOffsetX, size_t FragmentOffsetY, size_t FragmentOffsetZ) :
		fragmentNx(FragmentNx), fragmentNz(FragmentNz), fragmentIndX(FragmentIndX), fragmentIndY(FragmentIndY), fragmentIndZ(FragmentIndZ),
		fragmentOffsetXByDevice(FragmentOffsetXByDevice), fragmentOffsetYByDevice(FragmentOffsetYByDevice), fragmentOffsetZByDevice(FragmentOffsetZByDevice),
		fragmentOffsetXByNode(FragmentOffsetXByNode), fragmentOffsetYByNode(FragmentOffsetYByNode), fragmentOffsetZByNode(FragmentOffsetZByNode),
		fragmentOffsetX(FragmentOffsetX), fragmentOffsetY(FragmentOffsetY), fragmentOffsetZ(FragmentOffsetZ),
		linearArrays2D{}
	{

	}

	/// <summary>
	/// Возвращает число узлов в фрагменте расчетной сетки
	/// </summary>
	/// <returns></returns>
	size_t GetNodesNumber()
	{
		size_t nodesNumber = fragmentNx * fragmentNz;
		return nodesNumber;
	}

	/// <summary>
	/// Создание массива указанного типа
	/// </summary>
	/// <param name="modelDataName"></param>
	void LinearArrayCreate(ModelDataName modelDataName)
	{
		linearArrays2D.LinearArrayCreate(fragmentNx, fragmentNz, modelDataName);
	}

	/// <summary>
	/// Возвращает указатель на двумерный массив указанного типа ModelDataName
	/// </summary>
	/// <param name="modelDataName"></param>
	/// <returns></returns>
	LinearArray2D* GetLinearArray2D(ModelDataName modelDataName)
	{
		LinearArray2D* linearArray2D = &(linearArrays2D.linearArrays.at(modelDataName));
		return linearArray2D;
	}
		
	/// <summary>
	/// Возвращает объём оперативной памяти, занимаемый фрагментом
	/// </summary>
	/// <returns></returns>
	double GetDataSizeInMb()
	{
		double result = linearArrays2D.GetDataSizeInMb();
		return result;
	}

	void PrintFragment()
	{
		std::cout << "-----------------2D-FRAGMENT XZ-------------------" << std::endl;
		std::cout << "fragmentNx = " << fragmentNx << "; fragmentNz = " << fragmentNz << std::endl;
		std::cout << "fragmentOffsetXByDevice = " << fragmentOffsetXByDevice << "; fragmentOffsetYByDevice = " << fragmentOffsetYByDevice << "; fragmentOffsetZByDevice = " << fragmentOffsetZByDevice << std::endl;
		std::cout << "fragmentOffsetXByNode = " << fragmentOffsetXByNode << "; fragmentOffsetYByNode = " << fragmentOffsetYByNode << "; fragmentOffsetZByNode = " << fragmentOffsetZByNode << std::endl;
		std::cout << "fragmentOffsetX = " << fragmentOffsetX << "; fragmentOffsetY = " << fragmentOffsetY << "; fragmentOffsetZ = " << fragmentOffsetZ << std::endl;

		double dataSizeInMb = GetDataSizeInMb();
		std::cout << "dataSizeInMb = " << dataSizeInMb << " Mb" << std::endl;

		linearArrays2D.Print();
	}
};


/// <summary>
/// Смежная двумерная плоскость XZ для передачи данных между устройствами
/// </summary>
struct Grid2DTransferPlaneXZ
{
	size_t fragmentsNumX;		// количество фрагментов в плоскости по оси Ox 
	size_t fragmentsNumZ;		// количество фрагментов в плоскости по оси Oz

	size_t fragmentNx;			// количество узлов во фрагментах блока устройства вычислительного узла по оси Ox (кроме последнего)
	size_t fragmentNz;			// количество узлов во фрагментах блока устройства вычислительного узла по оси Oz (кроме последнего)
	size_t fragmentNxl;			// количество узлов во фрагментах блока устройства вычислительного узла по оси Ox (последний)
	size_t fragmentNzl;			// количество узлов во фрагментах блока устройства вычислительного узла по оси Oz (последний)
		
	std::vector<std::vector<LinearArrays2D>> linearArrays2D;//Хранилище

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
	/// Создание массива данных указанного типа ModelDataName
	/// </summary>
	/// <param name="modelDataName">Тип создаваемого массива</param>
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
	/// <param name="modelDataName">Тип массива данных</param>
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



/// <summary>
/// Трехмерный фрагмент блока расчетной сетки вычислительного устройства
/// </summary>
struct Grid3DFragment
{
	size_t fragmentNx;			// количество узлов в фрагменте блока устройства вычислительного узла по оси Ox
	size_t fragmentNy;			// количество узлов в фрагменте блока устройства вычислительного узла по оси Oy
	size_t fragmentNz;			// количество узлов в фрагменте блока устройства вычислительного узла по оси Oz
	size_t fragmentIndX;			// индекс фрагмента в блоке устройства вычислительного узла по оси Ox
	size_t fragmentIndY;			// индекс фрагмента в блоке устройства вычислительного узла по оси Oy
	size_t fragmentIndZ;			// индекс фрагмента в блоке устройства вычислительного узла по оси Oz
	size_t fragmentOffsetXByDevice;			// смещение фрагмента относительно блока вычислительного устройства по оси Ox (относительно узла с координатами (0, 0, 0) блока вычислительного устройства)
	size_t fragmentOffsetYByDevice;			// смещение фрагмента относительно блока вычислительного устройства по оси Oy (относительно узла с координатами (0, 0, 0) блока вычислительного устройства)
	size_t fragmentOffsetZByDevice;			// смещение фрагмента относительно блока вычислительного устройства по оси Oz (относительно узла с координатами (0, 0, 0) блока вычислительного устройства)
	size_t fragmentOffsetXByNode;			// смещение фрагмента относительно блока вычислительного узла по оси Ox (относительно узла с координатами (0, 0, 0) блока вычислительного узла)
	size_t fragmentOffsetYByNode;			// смещение фрагмента относительно блока вычислительного узла по оси Oy (относительно узла с координатами (0, 0, 0) блока вычислительного узла)
	size_t fragmentOffsetZByNode;			// смещение фрагмента относительно блока вычислительного узла по оси Oz (относительно узла с координатами (0, 0, 0) блока вычислительного узла)
	size_t fragmentOffsetX;			// смещение фрагмента относительно глобальной расчетной сетки по оси Ox (относительно узла с координатами (0, 0, 0) глобальной расчетной сетки)
	size_t fragmentOffsetY;			// смещение фрагмента относительно глобальной расчетной сетки по оси Oy (относительно узла с координатами (0, 0, 0) глобальной расчетной сетки)
	size_t fragmentOffsetZ;			// смещение фрагмента относительно глобальной расчетной сетки по оси Oz (относительно узла с координатами (0, 0, 0) глобальной расчетной сетки)
	Grid3DFragment* fragmentXPrev;		// указатель на соседний фрагмент по оси Ox (отрицательное направление)
	Grid3DFragment* fragmentXNext;		// указатель на соседний фрагмент по оси Ox (положительное направление)	
	Grid3DFragment* fragmentYPrev;		// указатель на соседний фрагмент по оси Oy (отрицательное направление)	
	Grid3DFragment* fragmentYNext;		// указатель на соседний фрагмент по оси Oy (положительное направление)	
	Grid3DFragment* fragmentZPrev;		// указатель на соседний фрагмент по оси Oz (отрицательное направление)
	Grid3DFragment* fragmentZNext;		// указатель на соседний фрагмент по оси Oz (положительное направление)	
	LinearArrays3D linearArrays3D;		// хранилище данных для фрагмента

	
	Grid3DFragment(size_t FragmentNx, size_t FragmentNy, size_t FragmentNz, size_t FragmentIndX, size_t FragmentIndY, size_t FragmentIndZ,
		size_t FragmentOffsetXByDevice, size_t FragmentOffsetYByDevice, size_t FragmentOffsetZByDevice, 
		size_t FragmentOffsetXByNode, size_t FragmentOffsetYByNode, size_t FragmentOffsetZByNode, 
		size_t FragmentOffsetX, size_t FragmentOffsetY, size_t FragmentOffsetZ, 
		Grid3DFragment* FragmentXPrev = NULL, Grid3DFragment* FragmentXNext = NULL, Grid3DFragment* FragmentYPrev = NULL, 
		Grid3DFragment* FragmentYNext = NULL, Grid3DFragment* FragmentZPrev = NULL, Grid3DFragment* FragmentZNext = NULL ) :
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
	/// Возвращает указатель на первую плоскость XY фрагмента по оси Oz (нижняя плоскость 3D-фрагмента)
	/// </summary>
	/// <returns></returns>
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
	/// Возвращает указатель на последнюю плоскость XY фрагмента по оси Oz (верхняя плоскость 3D-фрагмента)
	/// </summary>
	/// <returns></returns>
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
	/// Возвращает указатель на первую плоскость YZ фрагмента по оси Ox (левая плоскость 3D-фрагмента----)
	/// </summary>
	/// <returns></returns>
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
	/// Возвращает указатель на последнюю плоскость YZ фрагмента по оси Ox (правая плоскость 3D-фрагмента-----)
	/// </summary>
	/// <returns></returns>
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
	/// Возвращает указатель на первую плоскость XZ фрагмента по оси Oy от начала координат
	/// </summary>
	/// <returns></returns>
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
	/// Возвращает указатель на последнюю плоскость XZ фрагмента по оси Oy от начала координат
	/// </summary>
	/// <returns></returns>
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
	/// <returns></returns>
	size_t GetNodesNumber()
	{
		size_t nodesNumber = fragmentNx * fragmentNy * fragmentNz;
		return nodesNumber;
	}

	/// <summary>
	/// Возвращает число узлов в плоскости XY фрагмента расчетной сетки
	/// </summary>
	/// <returns></returns>
	size_t GetNodesNumberXY()
	{
		size_t nodesNumber = fragmentNx * fragmentNy;
		return nodesNumber;
	}

	/// <summary>
	/// Возвращает число узлов в плоскости XZ фрагмента расчетной сетки
	/// </summary>
	/// <returns></returns>
	size_t GetNodesNumberXZ()
	{
		size_t nodesNumber = fragmentNx * fragmentNz;
		return nodesNumber;
	}

	/// <summary>
	/// Возвращает число узлов в плоскости YZ фрагмента расчетной сетки
	/// </summary>
	/// <returns></returns>
	size_t GetNodesNumberYZ()
	{
		size_t nodesNumber = fragmentNy * fragmentNz;
		return nodesNumber;
	}

	/// <summary>
	/// ---------------------
	/// </summary>
	/// <param name="modelDataName"></param>
	void LinearArrayCreate(ModelDataName modelDataName)
	{
		linearArrays3D.LinearArrayCreate(fragmentNx, fragmentNy, fragmentNz, modelDataName);
	}

	/// <summary>
	/// --------------
	/// </summary>
	/// <param name="modelDataName"></param>
	/// <returns></returns>
	LinearArray3D* GetLinearArray3D(ModelDataName modelDataName)
	{
		LinearArray3D* linearArray3D = &(linearArrays3D.linearArrays.at(modelDataName));
		return linearArray3D;
	}

	/// <summary>
	/// ------------------
	/// </summary>
	/// <param name="data"></param>
	/// <param name="nx"></param>
	/// <param name="ny"></param>
	/// <param name="nz"></param>
	/// <param name="modelDataName"></param>
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
	/// ----------------
	/// </summary>
	void PrintGrid3DFragmentIndex()
	{
		std::cout << "(" << fragmentIndX << ", " << fragmentIndY << ", " << fragmentIndZ << ")";
	}

	/// <summary>
	/// -------------------
	/// </summary>
	/// <param name="fragment"></param>
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
	/// <returns></returns>
	double GetDataSizeInMb()
	{
		double result = linearArrays3D.GetDataSizeInMb();
		return result;
	}

	/// <summary>
	/// ------------------
	/// </summary>
	/// <param name="grid3DPrintDetalisation"></param>
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
	/// Вывод на печать всех элементов плоскости Fragment3DPlane фрагмента для указанного типа ModelDataName
	/// </summary>
	/// <param name="modelDataName"></param>
	/// <param name="fragment3DPlane"></param>
	void PrintFragmentPlane(ModelDataName modelDataName, Fragment3DPlane fragment3DPlane)
	{
		if (fragment3DPlane == XOZ_Prev || fragment3DPlane == XOZ_Next)
		{
			PrintFragmentPlane(modelDataName, fragment3DPlane, 0, 0, fragmentNx-1, fragmentNz-1);
		}
		
		if (fragment3DPlane == XOY_Prev || fragment3DPlane == XOY_Next)
		{
			PrintFragmentPlane(modelDataName, fragment3DPlane, 0, 0, fragmentNx-1, fragmentNy-1);
		}
		
		if (fragment3DPlane == YOZ_Prev || fragment3DPlane == YOZ_Next)
		{
			PrintFragmentPlane(modelDataName, fragment3DPlane, 0, 0, fragmentNy-1, fragmentNz-1);
		}
	}

	/// <summary>
	/// Вывод на печать части элементов плоскости Fragment3DPlane фрагмента для указанного типа ModelDataName, часть размером (Ind1_1, Ind1_2) на (Ind2_1, Ind2_2)
	/// </summary>
	/// <param name="modelDataName"></param>
	/// <param name="fragment3DPlane"></param>
	/// <param name="Ind1_1"></param>
	/// <param name="Ind1_2"></param>
	/// <param name="Ind2_1"></param>
	/// <param name="Ind2_2"></param>
	void PrintFragmentPlane(ModelDataName modelDataName, Fragment3DPlane fragment3DPlane, size_t Ind1_1, size_t Ind1_2, size_t Ind2_1, size_t Ind2_2)
	{
		LinearArray3D* linearArray3D = GetLinearArray3D(modelDataName);
		linearArray3D->Print(fragment3DPlane, Ind1_1, Ind1_2, Ind2_1, Ind2_2);
	}

	
};

/// <summary>
/// Трехмерный блок расчетной сетки вычислительного устройства
/// </summary>
struct GridBlock3DByDevice
{
	size_t blockNx;			// количество узлов в блоке устройства вычислительного узла по оси Ox
	size_t blockNy;			// количество узлов в блоке устройства вычислительного узла по оси Oy
	size_t blockNz;			// количество узлов в блоке устройства вычислительного узла по оси Oz
	size_t deviceIndex;		// номер вычислительного устройства в вычислительном узле
	std::string deviceName;	// наименование вычислительного устройства, обрабатывающего блок
	size_t fragmentsNumX;			// количество фрагментов в блоке устройства по оси Ox 
	size_t fragmentsNumY;			// количество фрагментов в блоке устройства по оси Oy
	size_t fragmentsNumZ;			// количество фрагментов в блоке устройства по оси Oz
	size_t deviceOffsetYByNode;		// смещение блока вычислительного устройства по оси Oy (относительно узла с координатами (0, 0, 0) вычислительного узла)
	size_t deviceOffsetY;			// смещение блока вычислительного устройства по оси Oy (относительно узла с координатами (0, 0, 0) глобальной расчетной сетки)
	GridBlock3DByDevice* devicePrev;		// указатель на блок расчетной сетки, обрабатываемый предыдущим вычислительным устройством
	GridBlock3DByDevice* deviceNext;		// указатель на блок расчетной сетки, обрабатываемый следующим вычислительным устройством

	size_t fragmentNx;			// количество узлов во фрагментах блока устройства вычислительного узла по оси Ox (кроме последнего)
	size_t fragmentNy;			// количество узлов во фрагментах блока устройства вычислительного узла по оси Oy (кроме последнего)
	size_t fragmentNz;			// количество узлов во фрагментах блока устройства вычислительного узла по оси Oz (кроме последнего)
	size_t fragmentNxl;			// количество узлов во фрагментах блока устройства вычислительного узла по оси Ox (последний)
	size_t fragmentNyl;			// количество узлов во фрагментах блока устройства вычислительного узла по оси Oy (последний)
	size_t fragmentNzl;			// количество узлов во фрагментах блока устройства вычислительного узла по оси Oz (последний)

	std::vector< std::vector <std::vector<Grid3DFragment> > > fragments;	// трехмерный массив фрагментов блока расчетной сетки вычислительного устройства вычислительного узла

	std::vector<Grid2DTransferPlaneXZ> grid2DTransferPlanesXZ; // Вектор двумерных плоскостей XZ для передачи данных между вычислителями

	GridBlock3DByDevice(size_t BlockNx, size_t BlockNy, size_t BlockNz, size_t DeviceIndex, std::string DeviceName, 
		size_t FragmentsNumX, size_t FragmentsNumY, size_t FragmentsNumZ,
		size_t DeviceOffsetYByNode, size_t DeviceOffsetY, GridBlock3DByDevice* DevicePrev = NULL, GridBlock3DByDevice* DeviceNext = NULL) :
		blockNx(BlockNx), blockNy(BlockNy), blockNz(BlockNz), deviceIndex(DeviceIndex), deviceName(DeviceName),
		fragmentsNumX(FragmentsNumX), fragmentsNumY(FragmentsNumY), fragmentsNumZ(FragmentsNumZ),
		deviceOffsetYByNode(DeviceOffsetYByNode), deviceOffsetY(DeviceOffsetY), devicePrev(DevicePrev), deviceNext(DeviceNext)
	{
		size_t _nx;
		size_t _nxl;
		size_t _ny;
		size_t _nyl;
		size_t _nz;
		size_t _nzl;

		if (blockNx % fragmentsNumX == 0)
		{
			_nx = blockNx / fragmentsNumX;
			_nxl = _nx;
		}
		else
		{
			double _nxd = (double)blockNx / fragmentsNumX;
			if ((_nxd - blockNx / fragmentsNumX) < 0.5)
			{
				_nx = blockNx / fragmentsNumX;
				_nxl = blockNx - _nx * (fragmentsNumX - 1);
			}
			else
			{
				_nx = blockNx / fragmentsNumX + 1;
				_nxl = blockNx - _nx * (fragmentsNumX - 1);
			}
		}

		if (blockNy % fragmentsNumY == 0)
		{
			_ny = blockNy / fragmentsNumY;
			_nyl = _ny;
		}
		else
		{
			double _nyd = (double)blockNy / fragmentsNumY;
			if ((_nyd - blockNy / fragmentsNumY) < 0.5)
			{
				_ny = blockNy / fragmentsNumY;
				_nyl = blockNy - _ny * (fragmentsNumY - 1);
			}
			else
			{
				_ny = blockNy / fragmentsNumY + 1;
				_nyl = blockNy - _ny * (fragmentsNumY - 1);
			}
		}

		if (blockNz % fragmentsNumZ == 0)
		{
			_nz = blockNz / fragmentsNumZ;
			_nzl = _nz;
		}
		else
		{
			double _nzd = (double)blockNz / fragmentsNumZ;
			if ((_nzd - blockNz / fragmentsNumZ) < 0.5)
			{
				_nz = blockNz / fragmentsNumZ;
				_nzl = blockNz - _nz * (fragmentsNumZ - 1);
			}
			else
			{
				_nz = blockNz / fragmentsNumZ + 1;
				_nzl = blockNz - _nz * (fragmentsNumZ - 1);
			}
		}

		fragmentNx = _nx;
		fragmentNy = _ny;
		fragmentNz = _nz;
		fragmentNxl = _nxl;
		fragmentNyl = _nyl;
		fragmentNzl = _nzl;


		size_t fragmentOffsetXByDevice = 0;
		size_t fragmentOffsetYByDevice = 0;
		size_t fragmentOffsetZByDevice = 0;

		for (size_t k = 0; k < fragmentsNumZ; k++)
		{
			std::vector <std::vector <Grid3DFragment>> fragmentsByXY;
			for (size_t j = 0; j < fragmentsNumY; j++)
			{
				std::vector <Grid3DFragment> fragmentsByX;
				for (size_t i = 0; i < fragmentsNumX; i++)
				{
					size_t __nx = _nx;
					size_t __ny = _ny;
					size_t __nz = _nz;

					if (k == fragmentsNumZ - 1)
					{
						__nz = _nzl;
					}

					if (j == fragmentsNumY - 1)
					{
						__ny = _nyl;
					}

					if (i == fragmentsNumX - 1)
					{
						__nx = _nxl;
					}
					
					Grid3DFragment newFragment(__nx, __ny, __nz, i, j, k,
						fragmentOffsetXByDevice, fragmentOffsetYByDevice, fragmentOffsetZByDevice,
						fragmentOffsetXByDevice, fragmentOffsetYByDevice + deviceOffsetYByNode, fragmentOffsetZByDevice,
						fragmentOffsetXByDevice, fragmentOffsetYByDevice + deviceOffsetY, fragmentOffsetZByDevice);
					fragmentsByX.push_back(newFragment);
					fragmentOffsetXByDevice += fragmentNx;
				}
				fragmentsByXY.push_back(fragmentsByX);
				fragmentOffsetXByDevice = 0;
				fragmentOffsetYByDevice += fragmentNy;
			}
			fragments.push_back(fragmentsByXY);
			fragmentOffsetYByDevice = 0;
			fragmentOffsetZByDevice += fragmentNz;
		}

		for (size_t k = 0; k < fragmentsNumZ; k++)
		{
			for (size_t j = 0; j < fragmentsNumY; j++)
			{
				for (size_t i = 0; i < fragmentsNumX; i++)
				{
					Grid3DFragment* fragmentXNext = NULL;
					Grid3DFragment* fragmentXPrev = NULL;
					Grid3DFragment* fragmentYNext = NULL;
					Grid3DFragment* fragmentYPrev = NULL;
					Grid3DFragment* fragmentZNext = NULL;
					Grid3DFragment* fragmentZPrev = NULL;

					if (i != 0)
					{
						fragmentXPrev = &fragments[k][j][i - 1];
					}

					if (i < fragmentsNumX - 1)
					{
						fragmentXNext = &fragments[k][j][i + 1];
					}

					if (j != 0)
					{
						fragmentYPrev = &fragments[k][j - 1][i];
					}

					if (j < fragmentsNumY - 1)
					{
						fragmentYNext = &fragments[k][j + 1][i];
					}

					if (k != 0)
					{
						fragmentZPrev = &fragments[k - 1][j][i];
					}

					if (k < fragmentsNumZ - 1)
					{
						fragmentZNext = &fragments[k + 1][j][i];
					}

					Grid3DFragment* curFragment = &fragments[k][j][i];
					curFragment->fragmentXNext = fragmentXNext;
					curFragment->fragmentXPrev = fragmentXPrev;
					curFragment->fragmentYNext = fragmentYNext;
					curFragment->fragmentYPrev = fragmentYPrev;
					curFragment->fragmentZNext = fragmentZNext;
					curFragment->fragmentZPrev = fragmentZPrev;
				}
			}
		}
	}

	// Методы

	/// <summary>
	/// Создаёт двумерные плоскости XZ для передачи данных между вычислителями
	/// </summary>
	void CreateGrid2DTransferPlanesXZ()
	{
		for (size_t j = 0; j < fragmentsNumY-1; j++)
		{
			Grid2DTransferPlaneXZ newPlaneXZ(fragmentsNumX, fragmentsNumZ,
				fragmentNx, fragmentNz,
				fragmentNxl, fragmentNzl);
			grid2DTransferPlanesXZ.push_back(newPlaneXZ);
		}
	}

	/// <summary>
	/// Добавляет в объект двумерной плоскости XZ для передачи данных массив требуемого типа
	/// </summary>
	/// <param name="modelDataName"></param>
	void Grid2DTransferPlanesXZLinearArrayCreate(ModelDataName modelDataName)
	{
		for (size_t j = 0; j < fragmentsNumY - 1; j++)
		{			
			auto& curPlaneXZ = grid2DTransferPlanesXZ.at(j);
			curPlaneXZ.LinearArrayCreate(modelDataName);
		}
				
	}

	Grid3DFragment* GetGrid3DFragment(size_t indX, size_t indY, size_t indZ)
	{
		Grid3DFragment* fragment = &fragments[indZ][indY][indX];
		return fragment;
	}

	/// <summary>
	/// --------------
	/// </summary>
	/// <param name="modelDataName"></param>
	void LinearArrayCreate(ModelDataName modelDataName)
	{
		for (size_t k = 0; k < fragmentsNumZ; k++)
		{
			for (size_t j = 0; j < fragmentsNumY; j++)
			{
				for (size_t i = 0; i < fragmentsNumX; i++)
				{					
					Grid3DFragment* fragment = GetGrid3DFragment(i,j,k);
					fragment->LinearArrayCreate(modelDataName);
				}
			}
		}
	}

	/// <summary>
	/// Выводит в консоль объект двумерной плоскости XZ для передачи данных
	/// </summary>	
	/// <param name="transferPlaneXZIndex">Индекс переходного слоя</param>
	/// <param name="fragmentIndX">Индекс фрагмента переходного слоя по OX</param>
	/// <param name="fragmentIndZ">Индекс фрагмента переходного слоя по OZ</param>
	/// <param name="modelDataName">Тип массива данных</param>
	void Grid2DTransferPlanesXZLinearArrayPrint(size_t transferPlaneXZIndex,
		size_t fragmentIndX, size_t fragmentIndZ, ModelDataName modelDataName)
	{
		Grid2DTransferPlaneXZ& grid2DTransferPlaneXZ = grid2DTransferPlanesXZ.at(transferPlaneXZIndex);
		grid2DTransferPlaneXZ.PrintGrid2DTransferPlaneXZ(fragmentIndX, fragmentIndZ, modelDataName);
	}

	/// <summary>
	/// -----------
	/// </summary>
	/// <param name="data"></param>
	/// <param name="nx"></param>
	/// <param name="ny"></param>
	/// <param name="nz"></param>
	/// <param name="modelDataName"></param>
	void AssignData(double* data, size_t nx, size_t ny, size_t nz, ModelDataName modelDataName)
	{
		for (size_t k = 0; k < fragmentsNumZ; k++)
		{
			for (size_t j = 0; j < fragmentsNumY; j++)
			{
				for (size_t i = 0; i < fragmentsNumX; i++)
				{
					Grid3DFragment* fragment = GetGrid3DFragment(i, j, k);
					fragment->AssignData(data, nx, ny, nz, modelDataName);
				}
			}
		}
	}
		
	void Compose(double* data, size_t nx, size_t ny, size_t nz, ModelDataName modelDataName)
	{
		for (size_t k = 0; k < fragmentsNumZ; k++)
		{
			for (size_t j = 0; j < fragmentsNumY; j++)
			{
				for (size_t i = 0; i < fragmentsNumX; i++)
				{
					Grid3DFragment* fragment = GetGrid3DFragment(i, j, k);
					fragment->Compose(data, nx, ny, nz, modelDataName);
				}
			}
		}
	}

	/// <summary>
	/// Возвращает объём оперативной памяти, занимаемый объектом GridBlock3DByDevice
	/// </summary>
	/// <returns></returns>
	double GetDataSizeInMb()
	{
		double result = 0;
		for (size_t k = 0; k < fragmentsNumZ; k++)
		{
			for (size_t j = 0; j < fragmentsNumY; j++)
			{
				for (size_t i = 0; i < fragmentsNumX; i++)
				{
					Grid3DFragment* fragment = GetGrid3DFragment(i, j, k);
					result += fragment->GetDataSizeInMb();
				}
			}
		}
		return result;
	}

	// Выводит в консоль УИД и наименование вычислительного узла
	void PrintDeviceName(GridBlock3DByDevice* gridBlock3DByDevice)
	{
		if (gridBlock3DByDevice == NULL)
		{
			std::cout << "(NULL)";
		}
		else
		{
			std::cout << "(" << gridBlock3DByDevice->deviceIndex;
			std::cout << ": " << gridBlock3DByDevice->deviceName;
			std::cout << ")";
		}
	}

	void PrintDevice(Grid3DPrintDetalisation grid3DPrintDetalisation)
	{
		std::cout << " \t\t-----------------DEVICE " << deviceIndex << ": " << deviceName << "-------------------" << std::endl;
		std::cout << " \t\tblockNx = " << blockNx << "; blockNy = " << blockNy << "; blockNz = " << blockNz << std::endl;
		std::cout << " \t\tdeviceOffsetYByNode = " << deviceOffsetYByNode << std::endl;
		std::cout << " \t\tdeviceOffsetY = " << deviceOffsetY << std::endl;

		if (devicePrev == NULL)
		{
			std::cout << " \t\tdevicePrev = NULL" << std::endl;
		}
		else
		{
			std::cout << " \t\tdevicePrev = " << devicePrev->deviceIndex << " (" << devicePrev->deviceName << ")" << std::endl;
		}
		
		if (deviceNext == NULL)
		{
			std::cout << " \t\tdeviceNext = NULL" << std::endl;
		}
		else
		{
			std::cout << " \t\tdeviceNext = " << deviceNext->deviceIndex << " (" << deviceNext->deviceName << ")" << std::endl;
		}

		double dataSizeInMb = GetDataSizeInMb();
		std::cout << "\t\tdataSizeInMb = " << dataSizeInMb << " Mb" << std::endl;

		if (grid3DPrintDetalisation > DEVICES)
		{
			for (auto& fragmentsXY : fragments)
			{
				for (auto& fragmentsX : fragmentsXY)
				{
					for (auto& fragment : fragmentsX)
					{
						fragment.PrintFragment(grid3DPrintDetalisation);
					}
				}
			}
		}
	}

};


/// <summary>
/// Трехмерный блок расчетной сетки вычислительного узла
/// </summary>
struct GridBlock3DByNode
{
	size_t nodeNx;			// количество узлов в блоке вычислительного узла по оси Ox
	size_t nodeNy;			// количество узлов в блоке вычислительного узла по оси Oy
	size_t nodeNz;			// количество узлов в блоке вычислительного узла по оси Oz
	size_t nodeIndex;		// номер блока вычислительного узла
	std::string nodeName;	// наименование вычислительного узла, обрабатывающего блок
	size_t nodeOffsetY;			// смещение блока вычислительного узла по оси Oy (относительно узла с координатами (0, 0, 0) глобальной расчетной сетки)
	GridBlock3DByNode* nodePrev;		// указатель на блок расчетной сетки, обрабатываемый предыдущим вычислительным узлом
	GridBlock3DByNode* nodeNext;		// указатель на блок расчетной сетки, обрабатываемый следующим вычислительным узлом
	std::vector<std::string> deviceNames;		// вектор идентификаторов вычислительных устройств
	std::map<std::string, GridBlock3DByDevice> gridBlock3DByDevices;		// хранилище типа ключ-значение объектов GridBlock3DByDevice

	GridBlock3DByNode(size_t NodeNx, size_t NodeNy, size_t NodeNz, size_t NodeIndex, std::string NodeName, size_t NodeOffsetY,
		GridBlock3DByNode* NodePrev = NULL, GridBlock3DByNode* NodeNext = NULL) :
		nodeNx(NodeNx), nodeNy(NodeNy), nodeNz(NodeNz), nodeIndex(NodeIndex), nodeName(NodeName), nodeOffsetY(NodeOffsetY), nodePrev(NodePrev), nodeNext(NodeNext)
	{

	}

	// Методы

	/// <summary>
	/// Создаёт двумерные плоскости XZ для передачи данных между вычислительными устройствами
	/// и вызывает функцию создания двумерных плоскостей XZ для передачи данных между вычислителями
	/// </summary>
	void CreateGrid2DTransferPlanesXZ()
	{
		// 1. Создаём двумерные плоскости XZ для передачи данных между вычислительными устройствами


		// 2. Вызываем методы создания двумерных плоскостей XZ для передачи данных между вычислителями
		for (auto itByDevices = gridBlock3DByDevices.begin(); itByDevices != gridBlock3DByDevices.end(); itByDevices++)
		{
			auto deviceKey = itByDevices->first;
			GridBlock3DByDevice* deviceObj = &(itByDevices->second);
			deviceObj->CreateGrid2DTransferPlanesXZ();
		}
	}

	/// <summary>
	/// Добавляет в объект двумерной плоскости XZ для передачи данных массив требуемого типа
	/// </summary>
	/// <param name="modelDataName"></param>
	void Grid2DTransferPlanesXZLinearArrayCreate(ModelDataName modelDataName)
	{
		// 1. Добавляем массивы для передачи данных между вычислительными устройствами

		// 2. Вызываем методы добавления массивов для передачи данных между вычислителями				
		for (auto itByDevices = gridBlock3DByDevices.begin(); itByDevices != gridBlock3DByDevices.end(); itByDevices++)
		{
			auto deviceKey = itByDevices->first;
			GridBlock3DByDevice* deviceObj = &(itByDevices->second);
			deviceObj->Grid2DTransferPlanesXZLinearArrayCreate(modelDataName);
		}
	}

	/// <summary>
	/// Выводит в консоль объект двумерной плоскости XZ для передачи данных
	/// </summary>	
	/// <param name="deviceName">УИД вычислительного устройства</param>
	/// <param name="transferPlaneXZIndex">Индекс переходного слоя</param>
	/// <param name="fragmentIndX">Индекс фрагмента переходного слоя по OX</param>
	/// <param name="fragmentIndZ">Индекс фрагмента переходного слоя по OZ</param>
	/// <param name="modelDataName">Тип массива данных</param>
	void Grid2DTransferPlanesXZLinearArrayPrint(std::string deviceName,
		size_t transferPlaneXZIndex, size_t fragmentIndX, size_t fragmentIndZ, ModelDataName modelDataName)
	{
		auto* device = GetGridBlock3DByDevice(deviceName);
		device->Grid2DTransferPlanesXZLinearArrayPrint(transferPlaneXZIndex, fragmentIndX, fragmentIndZ, modelDataName);
	}

	/// <summary>
	/// ---------------
	/// </summary>
	/// <param name="computingNode"></param>
	/// <param name="fragmentsNumX"></param>
	/// <param name="fragmentsNumZ"></param>
	void Decompose(ComputingNode computingNode, size_t fragmentsNumX, size_t fragmentsNumZ)	//	Выполняет декомпозицию блока расчетной сетки вычислительного узла
	{		
		size_t indexStart = 0;
		double sumDevicePerfomance = computingNode.GetSumDevicePerformance();
		size_t curDeviceOffsetY = 0;

		int i = 0;
		for (auto itByDevices = computingNode.computingDevices.begin(); itByDevices != computingNode.computingDevices.end(); itByDevices++)
		{
			auto deviceKey = itByDevices->first;
			auto& deviceObj = itByDevices->second;
			if (i < computingNode.CountDevices() - 1)
			{
				size_t deviceNx = nodeNx;
				size_t deviceNy = nodeNy * deviceObj.devicePerfomance / sumDevicePerfomance;
				size_t deviceNz = nodeNz;				
				size_t deviceOffsetY = nodeOffsetY + curDeviceOffsetY;
				size_t fragmentsNumY = deviceObj.GetNumThreads();
				GridBlock3DByDevice* gridBlock3DByDevice = new GridBlock3DByDevice(deviceNx, deviceNy, deviceNz, i, deviceKey,
				fragmentsNumX, fragmentsNumY, fragmentsNumZ, curDeviceOffsetY, deviceOffsetY);
				deviceNames.emplace_back(deviceKey);
				gridBlock3DByDevices.emplace(deviceKey, *gridBlock3DByDevice);
				curDeviceOffsetY += deviceNy;
				i += 1;
			}
			else
			{
				size_t deviceNx = nodeNx;
				size_t deviceNy = nodeNy - curDeviceOffsetY;
				size_t deviceNz = nodeNz;
				size_t deviceOffsetY = nodeOffsetY + curDeviceOffsetY;
				size_t fragmentsNumY = deviceObj.GetNumThreads();
				GridBlock3DByDevice* gridBlock3DByDevice = new GridBlock3DByDevice(deviceNx, deviceNy, deviceNz, i, deviceKey,
				fragmentsNumX, fragmentsNumY, fragmentsNumZ, curDeviceOffsetY, deviceOffsetY);
				deviceNames.emplace_back(deviceKey);
				gridBlock3DByDevices.emplace(deviceKey, *gridBlock3DByDevice);
			}		
		}
	}

	/// <summary>
	/// --------------
	/// </summary>
	/// <param name="deviceIndex"></param>
	/// <returns></returns>
	GridBlock3DByDevice* GetGridBlock3DByDevice(size_t deviceIndex)
	{
		auto deviceName = deviceNames[deviceIndex];
		auto gridBlock3DByDevice = GetGridBlock3DByDevice(deviceName);
		return gridBlock3DByDevice;
	}

	/// <summary>
	/// --------------
	/// </summary>
	/// <param name="deviceName"></param>
	/// <returns></returns>
	GridBlock3DByDevice* GetGridBlock3DByDevice(std::string deviceName)
	{
		GridBlock3DByDevice* gridBlock3DByDevice = &(gridBlock3DByDevices.at(deviceName));
		return gridBlock3DByDevice;
	}

	// Устанавливает указатели на соседние блоки
	void SetPrevNextPointers()
	{
		GridBlock3DByDevice* prevGridBlock3DByDevice = NULL;
		GridBlock3DByDevice* curGridBlock3DByDevice = NULL;
		for (auto deviceName : deviceNames)
		{
			curGridBlock3DByDevice = GetGridBlock3DByDevice(deviceName);
			curGridBlock3DByDevice->devicePrev = prevGridBlock3DByDevice;
			prevGridBlock3DByDevice = curGridBlock3DByDevice;
		}

		GridBlock3DByDevice* nextGridBlock3DByDevice = NULL;
		for (int nodeIndex = deviceNames.size() - 1; nodeIndex >= 0; nodeIndex--)
		{
			curGridBlock3DByDevice = GetGridBlock3DByDevice(nodeIndex);
			curGridBlock3DByDevice->deviceNext = nextGridBlock3DByDevice;
			nextGridBlock3DByDevice = curGridBlock3DByDevice;
		}

		// Устанавливаем указатели на соседние блоки, рассчитываемые различными устройствами
		//SetPrevNextPointersForDeviceBlocks();
	}

	// Выводит в консоль УИД и наименование вычислительного узла
	void PrintNodeName(GridBlock3DByNode* gridBlock3DByNode)
	{
		if (gridBlock3DByNode == NULL)
		{
			std::cout << "(NULL)";
		}
		else
		{
			std::cout << "(" << gridBlock3DByNode->nodeIndex;
			std::cout << ": " << gridBlock3DByNode->nodeName;
			std::cout << ")";
		}		
	}

	/// <summary>
	/// -------------------
	/// </summary>
	/// <param name="modelDataName"></param>
	void LinearArrayCreate(ModelDataName modelDataName)
	{
		for (auto itByDevices = gridBlock3DByDevices.begin(); itByDevices != gridBlock3DByDevices.end(); itByDevices++)
		{
			auto deviceKey = itByDevices->first;
			GridBlock3DByDevice* deviceObj = &(itByDevices->second);
			deviceObj->LinearArrayCreate(modelDataName);
		}
	}

	/// <summary>
	/// -----------------
	/// </summary>
	/// <param name="data"></param>
	/// <param name="nx"></param>
	/// <param name="ny"></param>
	/// <param name="nz"></param>
	/// <param name="modelDataName"></param>
	void AssignData(double* data, size_t nx, size_t ny, size_t nz, ModelDataName modelDataName)
	{
		for (auto itByDevices = gridBlock3DByDevices.begin(); itByDevices != gridBlock3DByDevices.end(); itByDevices++)
		{
			auto deviceKey = itByDevices->first;
			GridBlock3DByDevice* deviceObj = &(itByDevices->second);
			deviceObj->AssignData(data, nx, ny, nz, modelDataName);
		}
	}

	
	void Compose(double* data, size_t nx, size_t ny, size_t nz, ModelDataName modelDataName)
	{
		for (auto itByDevices = gridBlock3DByDevices.begin(); itByDevices != gridBlock3DByDevices.end(); itByDevices++)
		{
			auto deviceKey = itByDevices->first;
			GridBlock3DByDevice* deviceObj = &(itByDevices->second);
			deviceObj->Compose(data, nx, ny, nz, modelDataName);
		}
	}

	/// <summary>
	/// Возвращает объём оперативной памяти, занимаемый объектом GridBlock3DByNode
	/// </summary>
	/// <returns></returns>
	double GetDataSizeInMb()
	{
		double result = 0;

		for (auto itByDevices = gridBlock3DByDevices.begin(); itByDevices != gridBlock3DByDevices.end(); itByDevices++)
		{
			auto deviceKey = itByDevices->first;
			GridBlock3DByDevice* deviceObj = &(itByDevices->second);
			result += deviceObj->GetDataSizeInMb();
		}

		return result;
	}

	void PrintNode(Grid3DPrintDetalisation grid3DPrintDetalisation)
	{
		std::cout << " \t-----------------NODE " << nodeIndex << ": " << nodeName << "-------------------" << std::endl;
		std::cout << " \tnodeNx = " << nodeNx << "; nodeNy = " << nodeNy << "; nodeNz = " << nodeNz << std::endl;
		std::cout << " \tnodeOffsetY = " << nodeOffsetY << std::endl;
		
		std::cout << "\tnodePrev = ";
		PrintNodeName(nodePrev);
		std::cout << "; nodeNext = ";
		PrintNodeName(nodeNext);
		std::cout << std::endl;

		std::cout << "\tdeviceNames contains " << deviceNames.capacity() << " elements" << std::endl;
		
		double dataSizeInMb = GetDataSizeInMb();
		std::cout << "\tdataSizeInMb = " << dataSizeInMb << " Mb" << std::endl;

		if (grid3DPrintDetalisation > NODES)
		{
			for (auto deviceName : deviceNames)
			{
				GridBlock3DByDevice* gridBlock3DByDevice = GetGridBlock3DByDevice(deviceName);
				gridBlock3DByDevice->PrintDevice(grid3DPrintDetalisation);
			}
		}
	}
};

/// <summary>
/// Трехмерная расчетная сетка
/// </summary>
struct Grid3D
{
	size_t gridNx;			// количество узлов расчетной сетки по оси Ox
	size_t gridNy;			// количество узлов расчетной сетки по оси Oy
	size_t gridNz;			// количество узлов расчетной сетки по оси Oz
	double hx;			// шаг расчетной сетки по оси Ox
	double hy;			// шаг расчетной сетки по оси Oy
	double hz;			// шаг расчетной сетки по оси Oz
	std::vector<std::string> nodeNames;		// вектор идентификаторов вычислительных узлов
	std::map<std::string, GridBlock3DByNode> gridBlock3DByNodes;		// хранилище типа ключ-значение объектов GridBlock3DByNode

	Grid3D(size_t GridNx, size_t GridNy, size_t GridNz, double Hx, double Hy, double Hz) :
		gridNx(GridNx), gridNy(GridNy), gridNz(GridNz), hx(Hx), hy(Hy), hz(Hz)
	{

	}

	// Методы

	/// <summary>
	/// Создаёт двумерные плоскости XZ для передачи данных между вычислителями
	/// </summary>
	void CreateGrid2DTransferPlanesXZ()
	{
		// 1. Создаём двумерные плоскости XZ для передачи данных между вычислительными узлами

		// 2. Вызываем методы создания двумерных плоскостей XZ для передачи данных
		//    между вычислительными устройствами и между вычислителями				
		for (auto itByNodes = gridBlock3DByNodes.begin(); itByNodes != gridBlock3DByNodes.end(); itByNodes++)
		{
			auto nodeKey = itByNodes->first;
			GridBlock3DByNode* nodeObj = &(itByNodes->second);
			nodeObj->CreateGrid2DTransferPlanesXZ();
		}
	}

	/// <summary>
	/// Добавляет в объект двумерной плоскости XZ для передачи данных массив требуемого типа
	/// </summary>
	/// <param name="modelDataName"></param>
	void Grid2DTransferPlanesXZLinearArrayCreate(ModelDataName modelDataName)
	{
		// 1. Добавляем массивы для передачи данных между вычислительными узлами

		// 2. Вызываем методы добавления массивов для передачи данных
		//    между вычислительными устройствами и между вычислителями				
		for (auto itByNodes = gridBlock3DByNodes.begin(); itByNodes != gridBlock3DByNodes.end(); itByNodes++)
		{
			auto nodeKey = itByNodes->first;
			GridBlock3DByNode* nodeObj = &(itByNodes->second);
			nodeObj->Grid2DTransferPlanesXZLinearArrayCreate( modelDataName);
		}
	}

	/// <summary>
	/// Выводит в консоль объект двумерной плоскости XZ для передачи данных
	/// </summary>
	/// <param name="nodeName">УИД узла кластера</param>
	/// <param name="deviceName">УИД вычислительного устройства</param>
	/// <param name="transferPlaneXZIndex">Индекс переходного слоя</param>
	/// <param name="fragmentIndX">Индекс фрагмента переходного слоя по OX</param>
	/// <param name="fragmentIndZ">Индекс фрагмента переходного слоя по OZ</param>
	/// <param name="modelDataName">Тип массива данных</param>
	void Grid2DTransferPlanesXZLinearArrayPrint(std::string nodeName, std::string deviceName,
		size_t transferPlaneXZIndex, size_t fragmentIndX, size_t fragmentIndZ,  ModelDataName modelDataName)
	{
		auto* node = GetGridBlock3DByNode(nodeName);
		node->Grid2DTransferPlanesXZLinearArrayPrint(deviceName,
			transferPlaneXZIndex, fragmentIndX, fragmentIndZ, modelDataName);
	}

	/// <summary>
	/// -----------
	/// </summary>
	/// <param name="nodeIndex"></param>
	/// <returns></returns>
	GridBlock3DByNode* GetGridBlock3DByNode(size_t nodeIndex)
	{
		auto nodeName = nodeNames[nodeIndex];
		GridBlock3DByNode* gridBlock3DByNode = GetGridBlock3DByNode(nodeName);
		return gridBlock3DByNode;
	}

	/// <summary>
	/// ----------------
	/// </summary>
	/// <param name="nodeName"></param>
	/// <returns></returns>
	GridBlock3DByNode* GetGridBlock3DByNode(std::string nodeName)
	{
		GridBlock3DByNode* gridBlock3DByNode = &(gridBlock3DByNodes.at(nodeName));
		return gridBlock3DByNode;
	}

	/// <summary>
	/// -------------
	/// </summary>
	/// <param name="cluster"></param>
	/// <param name="fragmentsNumX"></param>
	/// <param name="fragmentsNumZ"></param>
	void Decompose(ComputingCluster cluster, size_t fragmentsNumX, size_t fragmentsNumZ)	//	Выполняет декомпозицию расчетной сетки: делит сетку Grid на блоки для выч.устройств кластера (Nodes)
	{
		double sumNyNode = 0;
		double sumPerfomance = cluster.GetClusterPerfomance();
		size_t nodeOffsetY = 0;	

		// Распределение по узлам кластера
		auto itByNodes = cluster.computingNodes.begin();
		int i = 0;
		for (auto itByNodes = cluster.computingNodes.begin(); itByNodes != cluster.computingNodes.end(); itByNodes++)
		{
			auto nodeKey = itByNodes->first;
			auto nodeObj = itByNodes->second;
			if (i < cluster.CountNodes() - 1)
			{
				size_t nodeNx = gridNx;
				size_t nodeNy = gridNy * nodeObj.nodePerfomance / sumPerfomance;
				size_t nodeNz = gridNz;
				sumNyNode += nodeNy;
				GridBlock3DByNode gridBlock3DByNode(nodeNx, nodeNy, nodeNz, i, nodeKey, nodeOffsetY);
				gridBlock3DByNode.Decompose(nodeObj, fragmentsNumX, fragmentsNumZ);
				nodeNames.emplace_back(nodeKey);
				gridBlock3DByNodes.emplace(nodeKey, std::ref(gridBlock3DByNode));
				nodeOffsetY = nodeOffsetY + nodeNy;
				i += 1;
			}
			else
			{
				size_t nodeNx = gridNx;
				size_t nodeNy = gridNy - sumNyNode;
				size_t nodeNz = gridNz;
				GridBlock3DByNode gridBlock3DByNode(nodeNx, nodeNy, nodeNz, i, nodeKey, nodeOffsetY);
				gridBlock3DByNode.Decompose(nodeObj, fragmentsNumX, fragmentsNumZ);
				nodeNames.emplace_back(nodeKey);
				gridBlock3DByNodes.emplace(nodeKey, std::ref(gridBlock3DByNode));

			}

		}

		// Устанавливаем указатели на соседние блоки
		SetPrevNextPointers();
	}
	
	// Устанавливает указатели на соседние блоки
	void SetPrevNextPointers()
	{
		GridBlock3DByNode* prevGridBlock3DByNode = NULL;
		GridBlock3DByNode* curGridBlock3DByNode = NULL;
		for (auto nodeName : nodeNames)
		{			
			curGridBlock3DByNode = GetGridBlock3DByNode(nodeName);
			curGridBlock3DByNode->nodePrev = prevGridBlock3DByNode;
			prevGridBlock3DByNode = curGridBlock3DByNode;
		}

		GridBlock3DByNode* nextGridBlock3DByNode = NULL;
		for (int nodeIndex = nodeNames.size() - 1; nodeIndex >= 0 ; nodeIndex--)
		{			
			curGridBlock3DByNode = GetGridBlock3DByNode(nodeIndex);
			curGridBlock3DByNode->nodeNext = nextGridBlock3DByNode;
			nextGridBlock3DByNode = curGridBlock3DByNode;
		}

		// Устанавливаем указатели на соседние блоки, рассчитываемые различными устройствами
		SetPrevNextPointersForDeviceBlocks();
	}
	
	/// <summary>
	/// ------------------
	/// </summary>
	void SetPrevNextPointersForDeviceBlocks()
	{
		GridBlock3DByNode* curGridBlock3DByNode = NULL;
		for (auto nodeName : nodeNames)
		{
			curGridBlock3DByNode = GetGridBlock3DByNode(nodeName);
			curGridBlock3DByNode->SetPrevNextPointers();
		}
	}

	/// <summary>
	/// -----------------
	/// </summary>
	/// <param name="modelDataName"></param>
	void LinearArrayCreate(ModelDataName modelDataName)
	{
		for (auto itByNodes = gridBlock3DByNodes.begin(); itByNodes != gridBlock3DByNodes.end(); itByNodes++)
		{
			auto nodeKey = itByNodes->first;
			GridBlock3DByNode* nodeObj = &(itByNodes->second);
			nodeObj->LinearArrayCreate(modelDataName);
		}
	}

	/// <summary>
	/// --------------
	/// </summary>
	/// <param name="data"></param>
	/// <param name="nx"></param>
	/// <param name="ny"></param>
	/// <param name="nz"></param>
	/// <param name="modelDataName"></param>
	void AssignData(double* data, size_t nx, size_t ny, size_t nz, ModelDataName modelDataName)
	{
		for (auto itByNodes = gridBlock3DByNodes.begin(); itByNodes != gridBlock3DByNodes.end(); itByNodes++)
		{
			auto nodeKey = itByNodes->first;
			GridBlock3DByNode* nodeObj = &(itByNodes->second);
			nodeObj->AssignData(data, nx, ny, nz, modelDataName);
		}
	}

	/// <summary>
	/// ---------------------
	/// </summary>
	/// <param name="modelDataName"></param>
	/// <returns></returns>
	double* Compose(ModelDataName modelDataName)	//	Выполняет сборку элементов массива, разбросанных по узлам расчетной сетки
	{
		long ram = sizeof(double) * gridNx * gridNy * gridNz;
		double* data = (double*)malloc(ram);
		
		for (auto itByNodes = gridBlock3DByNodes.begin(); itByNodes != gridBlock3DByNodes.end(); itByNodes++)
		{
			auto nodeKey = itByNodes->first;
			GridBlock3DByNode* nodeObj = &(itByNodes->second);
			nodeObj->Compose(data, gridNx, gridNy, gridNz, modelDataName);
		}

		return data;
	}

	/// <summary>
	/// Возвращает объём оперативной памяти, занимаемый объектом Grid3D
	/// </summary>
	/// <returns></returns>
	double GetDataSizeInMb()
	{
		double result = 0;

		for (auto itByNodes = gridBlock3DByNodes.begin(); itByNodes != gridBlock3DByNodes.end(); itByNodes++)
		{
			auto nodeKey = itByNodes->first;
			GridBlock3DByNode* nodeObj = &(itByNodes->second);
			result += nodeObj->GetDataSizeInMb();
		}

		return result;
	}

	void Print(Grid3DPrintDetalisation grid3DPrintDetalisation)
	{
		std::cout << "-----------------GRID-------------------" << std::endl;
		std::cout << "gridNx = " << gridNx << "; gridNy = " << gridNy << "; gridNz = " << gridNz << std::endl;
		std::cout << "hx = " << hx << "; hy = " << hy << "; hz = " << hz << std::endl;		

		double dataSizeInMb = GetDataSizeInMb();
		std::cout << "dataSizeInMb = " << dataSizeInMb << " Mb" << std::endl;

		if (grid3DPrintDetalisation > GRID)
		{
			for (auto nodeName : nodeNames)
			{
				GridBlock3DByNode* gridBlock3DByNode = GetGridBlock3DByNode(nodeName);
				gridBlock3DByNode->PrintNode(grid3DPrintDetalisation);
			}
		}

	}
};

#pragma endregion Grid
