#include <iostream>
#include <chrono>

namespace ch = std::chrono;

template <typename duration = ch::seconds, typename clock = ch::high_resolution_clock>
class timer
{
	typename clock::time_point m_start, m_stop;

	typename clock::rep get_time() const
	{
		return ch::duration_cast<duration>(m_stop - m_start).count();
	}

public:
	void         start() { m_start = clock::now(); }
	const timer& stop() { m_stop = clock::now(); return *this; }

	std::ostream& print() const
	{
		return std::cout << "Time running: [" << get_time() << "]";
	}
};


struct LinearArray3D
{
	size_t nx;             // число узлов в фрагменте по оси Ox
	size_t ny;             // число узлов в фрагменте по оси Oy
	size_t nz;             // число узлов в фрагменте по оси Oz
	double* data;          // указатель на массив данных

	// Конструктор
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

	// Деструктор
	~LinearArray3D()
	{
		free(data);
	}

	// Методы

	/// <summary>
	/// Возвращает индекс элемента массива данных по индексам элемента в фрагменте
	/// </summary>
	/// <param name="IndX"></param>
	/// <param name="IndY"></param>
	/// <param name="IndZ"></param>
	/// <returns></returns>
	size_t GetIndex(size_t IndX, size_t IndY, size_t IndZ)
	{
		return IndX + IndY * nx + IndZ * nx * ny;
	}

	/// <summary>
	/// Возвращает значение элемента массива с индексом, вычисленным по индексам элемента в фрагменте
	/// </summary>
	/// <param name="IndX"></param>
	/// <param name="IndY"></param>
	/// <param name="IndZ"></param>
	/// <returns></returns>
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
	/// <param name="IndX"></param>
	/// <param name="IndY"></param>
	/// <param name="IndZ"></param>
	/// <param name="Value"></param>
	/// <returns></returns>
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
	/// Возвращает объём оперативной памяти, занимаемый объектом LinearArray3D
	/// </summary>
	/// <returns></returns>
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

};

void alg1(LinearArray3D* linAr)
{
	int nx = linAr->nx;
	int ny = linAr->ny;
	int nz = linAr->nz;

	for (int k = 1; k < nz-1; k++)
	{
		for (int j = 1; j < ny-1; j++)
		{
			for (int i = 1; i < nx-1; i++)
			{
				double val0 = linAr->GetElement(i, j, k);
				double val2 = linAr->GetElement(i-1, j, k);
				double val4 = linAr->GetElement(i, j-1, k);
				double val6 = linAr->GetElement(i, j, k-1);
				double newVal = val0 + val2 + val4 + val6;
				linAr->SetElement(i, j, k, newVal);
			}
		}
	}
}

void alg1Start(LinearArray3D* linAr)
{
	timer<ch::milliseconds> aTimer;

	aTimer.start();
	alg1(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " milliseconds" << std::endl;

	aTimer.start();
	alg1(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " milliseconds" << std::endl;

	aTimer.start();
	alg1(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " milliseconds" << std::endl;

	aTimer.start();
	alg1(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " milliseconds" << std::endl;

	aTimer.start();
	alg1(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " milliseconds" << std::endl;
}


// alg2
void alg2(LinearArray3D* linAr)
{
	int nx = linAr->nx;
	int ny = linAr->ny;
	int nz = linAr->nz;

	for (int k = 1; k < nz - 1; k++)
	{
		for (int j = 1; j < ny - 1; j++)
		{
			for (int i = 1; i < nx - 1; i++)
			{
				double val2 = linAr->GetElement(i - 1, j, k);
				double val0 = linAr->GetElement(i, j, k);
				
				double val4 = linAr->GetElement(i, j - 1, k);
				double val6 = linAr->GetElement(i, j, k - 1);
				double newVal = val0 + val2 + val4 + val6;
				linAr->SetElement(i, j, k, newVal);
			}
		}
	}
}


void alg2Start(LinearArray3D* linAr)
{
	timer<ch::milliseconds> aTimer;

	aTimer.start();
	alg2(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " milliseconds" << std::endl;

	aTimer.start();
	alg2(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " milliseconds" << std::endl;

	aTimer.start();
	alg2(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " milliseconds" << std::endl;

	aTimer.start();
	alg2(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " milliseconds" << std::endl;

	aTimer.start();
	alg2(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " milliseconds" << std::endl;
}


// alg3
void alg3(LinearArray3D* linAr)
{
	int nx = linAr->nx;
	int ny = linAr->ny;
	int nz = linAr->nz;

	for (int k = 1; k < nz - 1; k++)
	{
		for (int j = 1; j < ny - 1; j++)
		{
			for (int i = 1; i < nx - 1; i++)
			{
				int m0 = i + j * nx + k * nx * ny;
				int m2 = m0 - 1;
				int m4 = m0 - nx;
				int m6 = m0 - nx * ny;

				double val2 = linAr->GetElement(i - 1, j, k);
				double val0 = linAr->GetElement(i, j, k);

				double val4 = linAr->GetElement(i, j - 1, k);
				double val6 = linAr->GetElement(i, j, k - 1);
				double newVal = val0 + val2 + val4 + val6;
				linAr->SetElement(i, j, k, newVal);
			}
		}
	}
}

void alg3Start(LinearArray3D* linAr)
{
	timer<ch::milliseconds> aTimer;

	aTimer.start();
	alg3(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " milliseconds" << std::endl;

	aTimer.start();
	alg3(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " milliseconds" << std::endl;

	aTimer.start();
	alg3(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " milliseconds" << std::endl;

	aTimer.start();
	alg3(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " milliseconds" << std::endl;

	aTimer.start();
	alg3(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " milliseconds" << std::endl;
}

// alg4
void alg4(LinearArray3D* linAr)
{
	int nx = linAr->nx;
	int ny = linAr->ny;
	int nz = linAr->nz;

	//1. Выделяем буферы для m2+m0, m4, m6
	double* b20 = (double*)malloc(nx * sizeof(double));
	double* b4 = (double*)malloc(nx * sizeof(double));
	double* b6 = (double*)malloc(nx * sizeof(double));

	//2. Загрузка данных для буферов (start: b20, b4, b6; next: b20, b6) 

	//3. Расчет для тек. буферов

	//4. Перестановка указателя с b20 на b4

	//5. Сохранение b20

	//6. Возврат к шагу 2

	
}

void alg4Start(LinearArray3D* linAr)
{
	timer<ch::milliseconds> aTimer;

	aTimer.start();
	alg4(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " milliseconds" << std::endl;

	aTimer.start();
	alg4(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " milliseconds" << std::endl;

	aTimer.start();
	alg4(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " milliseconds" << std::endl;

	aTimer.start();
	alg4(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " milliseconds" << std::endl;

	aTimer.start();
	alg4(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " milliseconds" << std::endl;
}

int main()
{
	int nx = 2000;
	int ny = 40;
	int nz = 3;
	auto linAr1 = new LinearArray3D(nx, ny, nz);
	for (int gi = 0; gi < nx * ny * nz; gi++)
		linAr1->data[gi] = gi;

	std::cout << "---alg1---\n";
	alg1Start(linAr1);
	std::cout << "---alg2---\n";
	alg2Start(linAr1);
	std::cout << "---alg3---\n";
	alg3Start(linAr1);
	std::cout << "---alg4---\n";
	alg4Start(linAr1);

	//linAr1->Print();

    //std::cout << "Hello World!\n";

	
}

