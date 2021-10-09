#include <iostream>
#include <chrono>

template <typename duration = std::chrono::seconds, typename clock = std::chrono::high_resolution_clock>
class timer
{
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
};

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
	/// Возвращает значение элемента массива с индексом, вычисленным по индексам элемента в фрагменте
	/// </summary>
	/// <param name="IndX"></param>
	/// <param name="IndY"></param>
	/// <param name="IndZ"></param>
	/// <returns></returns>
	double GetElement(size_t IndX)
	{
		if (IndX > nx)
		{
			throw - 1;
		}		

		return data[IndX];
	}
		
	/// <summary>
	/// Сохраняет значение элемента массива с индексом, вычисленным по индексам элемента в фрагменте
	/// </summary>
	/// <param name="IndX"></param>	
	/// <param name="Value"></param>
	/// <returns></returns>
	double SetElement(size_t IndX, double Value)
	{
		if (IndX > nx)
		{
			throw - 1;
		}		

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

struct LinearArray2D
{
	size_t nx;             // число узлов в фрагменте по оси Ox
	size_t ny;             // число узлов в фрагменте по оси Oy	
	double* data;          // указатель на массив данных

	// Конструктор
	LinearArray2D(size_t Nx, size_t Ny) :
		nx(Nx), ny(Ny)
	{
		size_t n = nx * ny;
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
	/// <param name="IndX"></param>
	/// <param name="IndY"></param>
	/// <param name="IndZ"></param>
	/// <returns></returns>
	size_t GetIndex(size_t IndX, size_t IndY)
	{
		return IndX + IndY * nx;
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
		if ((IndX > nx) || (IndY > ny))
		{
			throw - 1;
		}
		size_t indx = GetIndex(IndX, IndY);

		return data[indx];
	}

	/// <summary>
	/// Заполняет объект LinearArray1D данными, расположенными вдоль оси Ox по указанной координате Oy
	/// </summary>
	/// <param name="IndY"></param>	
	/// <param name="linAr1D"></param>
	void GetLineX(size_t IndY, LinearArray1D* linAr1D)
	{
		size_t indStart = GetIndex(0, IndY);

		size_t cnt = 0;
		for (size_t i = indStart; i < indStart + nx; i++)
		{
			linAr1D->data[cnt++] = data[i];
		}
	}

	/// <summary>
	/// Заполняет массив данными, расположенными вдоль оси Ox по указанной координате Oy
	/// </summary>
	/// <param name="IndY"></param>
	/// <param name="linArray"></param>
	void GetLineX(size_t IndY, double* linArray)
	{
		size_t indStart = GetIndex(0, IndY);

		size_t cnt = 0;
		for (size_t i = indStart; i < indStart + nx; i++)
		{
			linArray[cnt++] = data[i];
		}
	}

	/// <summary>
	/// Заполняет элементы объекта, расположенные вдоль оси Ox по указанной координате Oy, данными из LinearArray1D
	/// </summary>
	/// <param name="IndY"></param>
	/// <param name="linAr1D"></param>
	void SetLineX(size_t IndY, LinearArray1D* linAr1D)
	{
		size_t indStart = GetIndex(0, IndY);

		size_t cnt = 0;
		for (size_t i = indStart; i < indStart + nx; i++)
		{
			data[i] = linAr1D->data[cnt++];
		}
	}

	/// <summary>
	/// Заполняет элементы объекта, расположенные вдоль оси Ox по указанной координате Oy, данными из LinearArray1D
	/// </summary>
	/// <param name="IndY"></param>
	/// <param name="linAr1D"></param>
	void SetLineX(size_t IndY, double* linArray)
	{
		size_t indStart = GetIndex(0, IndY);

		size_t cnt = 0;
		for (size_t i = indStart; i < indStart + nx; i++)
		{
			data[i] = linArray[cnt++];
		}
	}
		
	/// <summary>
	/// Сохраняет значение элемента массива с индексом, вычисленным по индексам элемента в фрагменте
	/// </summary>
	/// <param name="IndX"></param>
	/// <param name="IndY"></param>
	/// <param name="Value"></param>
	/// <returns></returns>
	double SetElement(size_t IndX, size_t IndY, double Value)
	{
		if ((IndX > nx) || (IndY > ny))
		{
			throw - 1;
		}
		size_t indx = GetIndex(IndX, IndY);

		data[indx] = Value;
	}

	/// <summary>
	/// Возвращает объём оперативной памяти, занимаемый объектом LinearArray2D
	/// </summary>
	/// <returns></returns>
	double GetDataSizeInMb()
	{
		double result = (double)nx * ny * sizeof(double) / 1024 / 1024;
		return result;
	}

	/// <summary>
	/// Вывод элементов фрагмента в консоль
	/// </summary>
	void Print()
	{
		std::cout << "=================PrintArray=================" << std::endl;		
		for (int j = 0; j < ny; j++)
		{
			std::cout << "y = " << j << ": ";
			for (int i = 0; i < nx; i++)
			{
				std::cout << data[GetIndex(i, j)] << " ";
			}
			std::cout << std::endl;
		}
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
	/// Заполняет объект LinearArray1D данными, расположенными вдоль оси Ox по указанным координатам Oy, Oz
	/// </summary>
	/// <param name="IndY"></param>
	/// <param name="IndZ"></param>
	/// <param name="linAr1D"></param>
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
	/// <param name="IndY"></param>
	/// <param name="IndZ"></param>
	/// <param name="linArray"></param>
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
	/// <param name="IndY"></param>
	/// <param name="IndZ"></param>
	/// <param name="linAr1D"></param>
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
	/// <param name="IndY"></param>
	/// <param name="IndZ"></param>
	/// <param name="linAr1D"></param>
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
	/// <param name="IndZ"></param>
	/// <param name="layerZ"></param>
	void GetLayerZ(size_t IndZ, double* layerZ)
	{
		size_t indStart = GetIndex(0, 0, IndZ);			

		size_t cnt = 0;
		for (size_t i = indStart; i < indStart + nx*ny; i++)
		{
			layerZ[cnt++] = data[i];
		}
			
	}

	/// <summary>
	/// Заполняет двумерный массив layerZ значениями указанного слоя z
	/// </summary>
	/// <param name="IndZ"></param>
	/// <param name="layerZ"></param>
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
	/// <param name="IndZ"></param>
	/// <param name="layerZ"></param>
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
	/// <param name="IndZ"></param>
	/// <param name="layerZ"></param>
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
	timer<std::chrono::microseconds> aTimer;
	//timer<ch::milliseconds> aTimer;

	aTimer.start();
	alg1(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg1(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg1(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg1(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg1(linAr);
	std::cout << "alg1: "; aTimer.stop().print() << " microseconds" << std::endl;
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
	timer<std::chrono::microseconds> aTimer;
	//timer<ch::milliseconds> aTimer;

	aTimer.start();
	alg2(linAr);
	std::cout << "alg2: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg2(linAr);
	std::cout << "alg2: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg2(linAr);
	std::cout << "alg2: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg2(linAr);
	std::cout << "alg2: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg2(linAr);
	std::cout << "alg2: "; aTimer.stop().print() << " microseconds" << std::endl;
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

				double val2 = linAr->data[m2];
				double val0 = linAr->data[m0];

				double val4 = linAr->data[m4];
				double val6 = linAr->data[m6];
				double newVal = val0 + val2 + val4 + val6;
				linAr->SetElement(i, j, k, newVal);
			}
		}
	}
}

void alg3Start(LinearArray3D* linAr)
{
	timer<std::chrono::microseconds> aTimer;
	//timer<ch::milliseconds> aTimer;

	aTimer.start();
	alg3(linAr);
	std::cout << "alg3: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg3(linAr);
	std::cout << "alg3: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg3(linAr);
	std::cout << "alg3: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg3(linAr);
	std::cout << "alg3: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg3(linAr);
	std::cout << "alg3: "; aTimer.stop().print() << " microseconds" << std::endl;
}

// alg4
void alg4(LinearArray3D* linAr)
{
	int nx = linAr->nx;
	int ny = linAr->ny;
	int nz = linAr->nz;

	//1. Выделяем буферы для m2+m0, m4, m6
	//double* b20 = (double*)malloc(nx * sizeof(double));
	//double* b4 = (double*)malloc(nx * sizeof(double));
	//double* b6 = (double*)malloc(nx * sizeof(double));
	LinearArray1D* b20 = new LinearArray1D(nx);
	LinearArray1D* b4 = new LinearArray1D(nx);
	LinearArray1D* b6 = new LinearArray1D(nx);	

	for (size_t k = 1; k < nz-1; k++)
	{
		for (size_t j = 1; j < ny-1; j++)
		{
			//2. Загрузка данных для буферов (start: b20, b4, b6; next: b20, b6) 
			linAr->GetLineX(j, k, b20);
			if (j == 1)
			{
				linAr->GetLineX(j - 1, k, b4);
			}			
			linAr->GetLineX(j, k-1, b6);
			/*b20->Print();
			b4->Print();
			b6->Print();*/

			//3. Расчет для тек. буферов
			for (size_t i = 1; i < nx - 1; i++)
			{
				double _b2 = b20->data[i - 1];
				double _b0 = b20->data[i];
				double _b4 = b4->data[i];
				double _b6 = b6->data[i];
				double _res = _b0 + _b2 + _b4 + _b6;
				//b20->data[i] = b20->data[i] + b20->data[i - 1] + b4->data[i] + b6->data[i];
				b20->data[i] = _res;
			}
			/*b20->Print();
			b4->Print();
			b6->Print();*/

			//5. Сохранение b20
			linAr->SetLineX(j, k, b20);
			//linAr->Print();

			//4. Перестановка указателя с b20 на b4
			LinearArray1D* tmp = b4;
			b4 = b20;
			b20 = tmp;

			/*b20->Print();
			b4->Print();
			b6->Print();*/			
		}
	}	
}

void alg4Start(LinearArray3D* linAr)
{
	timer<std::chrono::microseconds> aTimer;
	//timer<ch::milliseconds> aTimer;

	aTimer.start();
	alg4(linAr);
	std::cout << "alg4: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg4(linAr);
	std::cout << "alg4: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg4(linAr);
	std::cout << "alg4: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg4(linAr);
	std::cout << "alg4: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg4(linAr);
	std::cout << "alg4: "; aTimer.stop().print() << " microseconds" << std::endl;
}

// alg5
void alg5(LinearArray3D* linAr)
{
	size_t nx = linAr->nx;
	size_t ny = linAr->ny;
	size_t nz = linAr->nz;

	auto layerZprev = (double*)malloc(nx * ny * sizeof(double));
	auto layerZcurr = (double*)malloc(nx * ny * sizeof(double));
	linAr->GetLayerZ(0, layerZprev);
		
	/*for (size_t j = 0; j < ny; j++)
	{
		for (size_t i = 0; i < nx; i++)
		{
			std::cout << layerZprev[i + j*nx] << " ";
		}
		std::cout << std::endl;
	}		*/

	for (int k = 1; k < nz - 1; k++)
	{
		linAr->GetLayerZ(k, layerZcurr);
		for (int j = 1; j < ny - 1; j++)
		{
			for (int i = 1; i < nx - 1; i++)
			{
				int m0 = i + j * nx;
				int m2 = m0 - 1;
				int m4 = m0 - nx;
				int m6 = m0;

				double val2 = layerZcurr[m2];
				double val0 = layerZcurr[m0];

				double val4 = layerZcurr[m4];
				double val6 = layerZprev[m6];
				double newVal = val0 + val2 + val4 + val6;
				layerZcurr[m0] = newVal;
			}
		}

		linAr->SetLayerZ(k, layerZcurr);

		double* tmp = layerZprev;
		layerZprev = layerZcurr;
		layerZcurr = tmp;
	}
}

void alg5Start(LinearArray3D* linAr)
{
	timer<std::chrono::microseconds> aTimer;
	//timer<ch::milliseconds> aTimer;

	aTimer.start();
	alg5(linAr);
	std::cout << "alg5: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg5(linAr);
	std::cout << "alg5: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg5(linAr);
	std::cout << "alg5: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg5(linAr);
	std::cout << "alg5: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg5(linAr);
	std::cout << "alg5: "; aTimer.stop().print() << " microseconds" << std::endl;
}

// alg6
void alg6(LinearArray3D* linAr)
{
	size_t nx = linAr->nx;
	size_t ny = linAr->ny;
	size_t nz = linAr->nz;
		
	auto layerZprev = new LinearArray2D(nx, ny);
	auto layerZcurr = new LinearArray2D(nx, ny);
	linAr->GetLayerZ(0, layerZprev);	

	for (int k = 1; k < nz - 1; k++)
	{
		linAr->GetLayerZ(k, layerZcurr);
		for (int j = 1; j < ny - 1; j++)
		{
			for (int i = 1; i < nx - 1; i++)
			{
				int m0 = i + j * nx;
				int m2 = m0 - 1;
				int m4 = m0 - nx;
				int m6 = m0;

				double val2 = layerZcurr->data[m2];
				double val0 = layerZcurr->data[m0];

				double val4 = layerZcurr->data[m4];
				double val6 = layerZprev->data[m6];
				double newVal = val0 + val2 + val4 + val6;
				layerZcurr->data[m0] = newVal;
			}
		}

		linAr->SetLayerZ(k, layerZcurr);

		auto tmp = layerZprev;
		layerZprev = layerZcurr;
		layerZcurr = tmp;
	}
}

void alg6Start(LinearArray3D* linAr)
{
	timer<std::chrono::microseconds> aTimer;
	//timer<ch::milliseconds> aTimer;

	aTimer.start();
	alg6(linAr);
	std::cout << "alg6: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg6(linAr);
	std::cout << "alg6: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg6(linAr);
	std::cout << "alg6: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg6(linAr);
	std::cout << "alg6: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg6(linAr);
	std::cout << "alg6: "; aTimer.stop().print() << " microseconds" << std::endl;
}

// alg7
void alg7(LinearArray3D* linAr)
{
	size_t nx = linAr->nx;
	size_t ny = linAr->ny;
	size_t nz = linAr->nz;

	auto layerZprev = new LinearArray2D(nx, ny);
	auto layerZcurr = new LinearArray2D(nx, ny);
	linAr->GetLayerZ(0, layerZprev);

	LinearArray1D* b20 = new LinearArray1D(nx);
	LinearArray1D* b4  = new LinearArray1D(nx);
	//LinearArray1D* b6  = new LinearArray1D(nx);

	for (int k = 1; k < nz - 1; k++)
	{
		linAr->GetLayerZ(k, layerZcurr);
		layerZcurr->GetLineX(0, b4);
		for (int j = 1; j < ny - 1; j++)
		{
			layerZcurr->GetLineX(j, b20);			
			//layerZprev->GetLineX(j, b6);
			for (int i = 1; i < nx - 1; i++)
			{
				double val2 = b20->data[i-1];
				double val0 = b20->data[i];

				double val4 = b4->data[i];
				double val6 = layerZprev->data[i+j*nx];
				double newVal = val0 + val2 + val4 + val6;
				b20->data[i] = newVal;
			}

			//Сохранение b20
			layerZcurr->SetLineX(j, b20);			

			//Перестановка указателя с b20 на b4
			auto* tmp = b4;
			b4 = b20;
			b20 = tmp;
		}

		linAr->SetLayerZ(k, layerZcurr);

		auto tmp = layerZprev;
		layerZprev = layerZcurr;
		layerZcurr = tmp;
	}
}

void alg7Start(LinearArray3D* linAr)
{
	timer<std::chrono::microseconds> aTimer;
	//timer<ch::milliseconds> aTimer;

	aTimer.start();
	alg7(linAr);
	std::cout << "alg7: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg7(linAr);
	std::cout << "alg7: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg7(linAr);
	std::cout << "alg7: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg7(linAr);
	std::cout << "alg7: "; aTimer.stop().print() << " microseconds" << std::endl;

	aTimer.start();
	alg7(linAr);
	std::cout << "alg7: "; aTimer.stop().print() << " microseconds" << std::endl;
}

void initLinearArray3DByGlobalIndexes(LinearArray3D* linAr)
{
	size_t nx = linAr->nx;
	size_t ny = linAr->ny;
	size_t nz = linAr->nz;

	for (int gi = 0; gi < nx * ny * nz; gi++)
		linAr->data[gi] = gi;
}

int main()
{
	int nx = 100;
	int ny = 50;
	int nz = 40;
	auto linAr1 = new LinearArray3D(nx, ny, nz);
	
	std::cout << "---alg1---\n";
	initLinearArray3DByGlobalIndexes(linAr1);
	alg1Start(linAr1);
		
	std::cout << "---alg2---\n";
	initLinearArray3DByGlobalIndexes(linAr1);
	alg2Start(linAr1);

	std::cout << "---alg3---\n";
	initLinearArray3DByGlobalIndexes(linAr1);
	alg3Start(linAr1);

	std::cout << "---alg4---\n";
	initLinearArray3DByGlobalIndexes(linAr1);
	alg4Start(linAr1);

	std::cout << "---alg5---\n";
	initLinearArray3DByGlobalIndexes(linAr1);
	alg5Start(linAr1);

	std::cout << "---alg6---\n";
	initLinearArray3DByGlobalIndexes(linAr1);
	alg6Start(linAr1);

	std::cout << "---alg7---\n";
	initLinearArray3DByGlobalIndexes(linAr1);
	alg7Start(linAr1);

	//linAr1->Print();

    //std::cout << "Hello World!\n";

	
}

