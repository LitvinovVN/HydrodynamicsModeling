#pragma once

#include <iostream>
#include <map>
#include <vector>

static struct GridHelper		// ��������������� �������������� ������ ��� Grid
{
	static size_t CountN(size_t nx, size_t ny, size_t nz)
	{
		size_t N = nx * ny * nz;
		return N;
	}
};

enum ModelDataName     // ������������ ������������ �������� ������ ������
{
	U,
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

enum Grid3DPrintDetalisation		// ������������ ������� ����������� ������ ������ Print ������ Grid3D
{
	GRID,
	NODES,
	DEVICES,
	FRAGMENTS,
	ARRAYS
};

enum Fragment3DPlane		// ������������ ���������� ��� �������� ����� �����������
{
	XOZ_Prev,
	XOZ_Next,
	XOY_Prev,
	XOY_Next,
	YOZ_Prev,
	YOZ_Next	
};




#pragma region LinearArrays

/// <summary>
/// ����� ��� ������ � ��������� ��������, ����������� ��� ����������
/// </summary>
struct LinearArray2D
{
	size_t n1;             // ����� ����� � ��������� �� ������ ���
	size_t n2;             // ����� ����� � ��������� �� ������ ���	
	double* data;          // ��������� �� ������ ������

	// �����������
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

	// ����������
	~LinearArray2D()
	{
		free(data);
	}

	// ������

	/// <summary>
	/// ���������� ������ �������� ������� ������ �� �������� �������� � ���������
	/// </summary>
	/// <param name="Ind1"></param>
	/// <param name="Ind2"></param>
	/// <returns></returns>
	size_t GetIndex(size_t Ind1, size_t Ind2)
	{
		return Ind1 + Ind2 * n1;
	}

	/// <summary>
	/// ���������� �������� �������� ������� � ��������, ����������� �� �������� �������� � ���������
	/// </summary>
	/// <param name="Ind1"></param>
	/// <param name="Ind2"></param>
	/// <returns></returns>
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
	/// ��������� �������� �������� ������� � ��������, ����������� �� �������� �������� � ���������
	/// </summary>
	/// <param name="Ind1"></param>
	/// <param name="Ind2"></param>
	/// <param name="Value"></param>
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
	/// ���������� ����� ����������� ������, ���������� �������� LinearArray2D
	/// </summary>
	/// <returns></returns>
	double GetDataSizeInMb()
	{
		double result = (double)n1 * n2 * sizeof(double) / 1024 / 1024;
		return result;
	}

	/// <summary>
	/// ������� �������� ��������� �� ����� XY � �������
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
/// &&&&------------
/// </summary>
struct LinearArray3D
{
	size_t nx;             // ����� ����� � ��������� �� ��� Ox
	size_t ny;             // ����� ����� � ��������� �� ��� Oy
	size_t nz;             // ����� ����� � ��������� �� ��� Oz
	double* data;          // ��������� �� ������ ������

	// �����������
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

	// ����������
	~LinearArray3D()
	{
		free(data);
	}

	// ������

	/// <summary>
	/// ���������� ������ �������� ������� ������ �� �������� �������� � ���������
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
	/// ���������� �������� �������� ������� � ��������, ����������� �� �������� �������� � ���������
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
	/// ��������� �������� �������� ������� � ��������, ����������� �� �������� �������� � ���������
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
	/// �������� ������ ������ (��������� XOY) ����� ����������� ������
	/// </summary>
	/// <param name="linearArray3DNext"></param>
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
	/// �������� ������ ������ (��������� XOY) ����� ����������� �����
	/// </summary>
	/// <param name="linearArray3DPrev"></param>
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
	/// �������� ������ ������ (��������� XOZ) ����� ����������� ������
	/// </summary>
	/// <param name="linearArray3DNext"></param>
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
	/// �������� ������ ������ (��������� XOZ) ����� ����������� �����
	/// </summary>
	/// <param name="linearArray3DPrev"></param>
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
	/// �������� ������ ������ (��������� YOZ) ����� ����������� ������
	/// </summary>
	/// <param name="linearArray3DNext"></param>
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
	/// �������� ������ ������ (��������� YOZ) ����� ����������� �����
	/// </summary>
	/// <param name="linearArray3DPrev"></param>
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
	/// �������� ������ ������ ��������� (��������� �� ��� OZ ��������� XOY) ��������� planeXOY
	/// </summary>
	/// <param name="planeXOY"></param>
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
	/// �������� ������ ������ ��������� (������ �� ��� OZ ��������� XOY) ��������� planeXOY
	/// </summary>
	/// <param name="planeXOY"></param>
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
	/// �������� ������ ������ ��������� (��������� �� ��� OY ��������� XOZ) ��������� planeXOZ
	/// </summary>
	/// <param name="planeXOZ"></param>
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
	/// �������� ������ ������ ��������� (������ �� ��� OY ��������� XOZ) ��������� planeXOZ
	/// </summary>
	/// <param name="planeXOZ"></param>
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
	/// �������� ������ ������ ��������� (��������� �� ��� OX ��������� YOZ) ��������� planeYOZ
	/// </summary>
	/// <param name="planeYOZ"></param>
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
	/// �������� ������ ������ ��������� (������ �� ��� OX ��������� YOZ) ��������� planeYOZ
	/// </summary>
	/// <param name="planeYOZ"></param>
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
	/// �������� ������ ������ �� ��������� planeXOY ��������� �� ��� OZ ��������� ���������
	/// </summary>
	/// <param name="planeXOY"></param>
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
	/// �������� ������ ������ �� ��������� planeXOY ������ �� ��� OZ ��������� ���������
	/// </summary>
	/// <param name="planeXOY"></param>
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
	/// �������� ������ ������ �� ��������� ��������� planeXOZ ��������� �� ��� OY ��������� ���������
	/// </summary>
	/// <param name="planeXOZ"></param>
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
	/// �������� ������ ������ �� ��������� planeXOZ ������ �� ��� OY ��������� ���������
	/// </summary>
	/// <param name="planeXOZ"></param>
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
	/// �������� ������ ������ �� ��������� planeYOZ ��������� �� ��� OX ��������� ���������
	/// </summary>
	/// <param name="planeYOZ"></param>
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
	/// �������� ������ ������ �� ��������� planeYOZ ������ �� ��� OX ��������� ���������
	/// </summary>
	/// <param name="planeYOZ"></param>
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
	/// ���������� ����� ����������� ������, ���������� �������� LinearArray3D
	/// </summary>
	/// <returns></returns>
	double GetDataSizeInMb()
	{
		double result = (double)nx * ny * nz * sizeof(double) / 1024 / 1024;
		return result;
	}

	/// <summary>
	/// ����� ��������� ��������� �� ����� XY � �������
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
	/// ������� � ������� �������� ������� ������ data
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

	/// <summary>
	/// ������� � ������� �������� ������� data ��� ��������� fragment3DPlane ���������
	/// </summary>
	/// <param name="fragment3DPlane"></param>
	/// <param name="Ind1_1"></param>
	/// <param name="Ind1_2"></param>
	/// <param name="Ind2_1"></param>
	/// <param name="Ind2_2"></param>
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

	// ������

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
	/// ���������� ����� ����������� ������, ���������� �������� LinearArrays
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

	// ������

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
	/// ���������� ����� ����������� ������, ���������� �������� LinearArrays3D
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
	std::string deviceId;			// ��� ���������� (CPU ��� GPU)
	double devicePerfomance;		// ������������������ ����������

	ComputingDevice(std::string DeviceId, double DevicePerfomance) :
		deviceId(DeviceId), devicePerfomance(DevicePerfomance)
	{

	}

	// ������

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
	long spNumber;				// ���������� ����������������� SP
	long sharedMemory;			// ����� �������������� ������
	long globalMemory;			// ����� ����������� GPU

	ComputingDeviceGPU(std::string DeviceId, double DevicePerfomance, long SpNumber, long SharedMemory, long GlobalMemory) :
		ComputingDevice(DeviceId, DevicePerfomance), spNumber(SpNumber), sharedMemory(SharedMemory), globalMemory(GlobalMemory)
	{

	}

	// ������

	long GetNumThreads() override	// ���������� ���������� �������
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
	int coreNumber;			// ����� ���� CPU �� �������������� ����

	ComputingDeviceCPU(std::string DeviceId, double DevicePerfomance, int CoreNumber) :
		ComputingDevice(DeviceId, DevicePerfomance), coreNumber(CoreNumber)
	{

	}

	// ������

	long GetNumThreads() override	// ���������� ���������� �������
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
	std::string nodeId;			// ��� ��������������� ����
	double ramMemorySize;		// ������ ����������� ������ ��������������� ����
	double nodePerfomance;		// ������������������ ��������������� ����
	std::map<std::string, ComputingDevice&> computingDevices;		// ������ �������� ComputingDevice

	ComputingNode(std::string NodeId, double RamMemorySize, double NodePerfomance) :
		nodeId(NodeId), ramMemorySize(RamMemorySize), nodePerfomance(NodePerfomance)
	{

	}

	// ������

	int CountDevices()	// ��������� ���������� devices
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

	double GetSumDevicePerformance()	// ��������� ��������� ������������������ ���� ��������� �� �������������� ����
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

struct ComputingCluster
{
	std::map<std::string, ComputingNode> computingNodes;

	// ������

	int CountNodes()	// ��������� ���������� nodes �� ��������
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

	double GetClusterPerfomance()	// ��������� ������������������ ��������
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
		for (auto itByNodes = computingNodes.begin(); itByNodes != computingNodes.end(); itByNodes++) {	// ������� nodes
			auto nodeKey = itByNodes->first;
			ComputingNode* nodeObj = &(itByNodes->second);
			std::cout << nodeKey << ": " << "\tramMemorySize=" << nodeObj->ramMemorySize
				<< "\tnodePerfomance=" << nodeObj->nodePerfomance << std::endl;

			for (auto itByDevices = nodeObj->computingDevices.begin(); itByDevices != nodeObj->computingDevices.end(); itByDevices++) {	// ������� devices
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
/// ��������� �������� ��������� ����� - ��������� XY(����� ����������� ���������)
/// </summary>
struct Grid2DFragmentXY
{
	size_t fragmentNx;			// ���������� ����� � ��������� ����� ���������� ��������������� ���� �� ��� Ox
	size_t fragmentNy;			// ���������� ����� � ��������� ����� ���������� ��������������� ���� �� ��� Oy
	size_t fragmentIndX;			// ������ 3D-��������� � ����� ���������� ��������������� ���� �� ��� Ox, �������� ����������� ������ 2D-��������
	size_t fragmentIndY;			// ������ 3D-��������� � ����� ���������� ��������������� ���� �� ��� Oy, �������� ����������� ������ 2D-��������
	size_t fragmentIndZ;			// ������ 3D-��������� � ����� ���������� ��������������� ���� �� ��� Oz, �������� ����������� ������ 2D-��������
	size_t fragmentOffsetXByDevice;			// �������� ��������� ������������ ����� ��������������� ���������� �� ��� Ox (������������ ���� � ������������ (0, 0, 0) ����� ��������������� ����������)
	size_t fragmentOffsetYByDevice;			// �������� ��������� ������������ ����� ��������������� ���������� �� ��� Oy (������������ ���� � ������������ (0, 0, 0) ����� ��������������� ����������)
	size_t fragmentOffsetZByDevice;			// �������� ��������� ������������ ����� ��������������� ���������� �� ��� Oz (������������ ���� � ������������ (0, 0, 0) ����� ��������������� ����������)
	size_t fragmentOffsetXByNode;			// �������� ��������� ������������ ����� ��������������� ���� �� ��� Ox (������������ ���� � ������������ (0, 0, 0) ����� ��������������� ����)
	size_t fragmentOffsetYByNode;			// �������� ��������� ������������ ����� ��������������� ���� �� ��� Oy (������������ ���� � ������������ (0, 0, 0) ����� ��������������� ����)
	size_t fragmentOffsetZByNode;			// �������� ��������� ������������ ����� ��������������� ���� �� ��� Oz (������������ ���� � ������������ (0, 0, 0) ����� ��������������� ����)
	size_t fragmentOffsetX;			// �������� ��������� ������������ ���������� ��������� ����� �� ��� Ox (������������ ���� � ������������ (0, 0, 0) ���������� ��������� �����)
	size_t fragmentOffsetY;			// �������� ��������� ������������ ���������� ��������� ����� �� ��� Oy (������������ ���� � ������������ (0, 0, 0) ���������� ��������� �����)
	size_t fragmentOffsetZ;			// �������� ��������� ������������ ���������� ��������� ����� �� ��� Oz (������������ ���� � ������������ (0, 0, 0) ���������� ��������� �����)
	LinearArrays2D linearArrays2D;		// ��������� ������ ��� ���������

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
	/// ���������� ����� ����� � ��������� ��������� �����
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

	/// <summary>
	/// &&&&----------
	/// </summary>
	/// <param name="data"></param>
	/// <param name="nx"></param>
	/// <param name="ny"></param>
	/// <param name="nz"></param>
	/// <param name="modelDataName"></param>
	void �ompose(double* data, size_t nx, size_t ny, size_t nz, ModelDataName modelDataName)
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
	/// ���������� ����� ����������� ������, ���������� ����������
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
/// ��������� �������� ��������� ����� - ��������� YZ(����� ����������� ���������)
/// </summary>
struct Grid2DFragmentYZ
{
	size_t fragmentNy;			// ���������� ����� � ��������� ����� ���������� ��������������� ���� �� ��� Oy
	size_t fragmentNz;			// ���������� ����� � ��������� ����� ���������� ��������������� ���� �� ��� Oz
	size_t fragmentIndX;			// ������ 3D-��������� � ����� ���������� ��������������� ���� �� ��� Ox, �������� ����������� ������ 2D-��������
	size_t fragmentIndY;			// ������ 3D-��������� � ����� ���������� ��������������� ���� �� ��� Oy, �������� ����������� ������ 2D-��������
	size_t fragmentIndZ;			// ������ 3D-��������� � ����� ���������� ��������������� ���� �� ��� Oz, �������� ����������� ������ 2D-��������
	size_t fragmentOffsetXByDevice;			// �������� ��������� ������������ ����� ��������������� ���������� �� ��� Ox (������������ ���� � ������������ (0, 0, 0) ����� ��������������� ����������)
	size_t fragmentOffsetYByDevice;			// �������� ��������� ������������ ����� ��������������� ���������� �� ��� Oy (������������ ���� � ������������ (0, 0, 0) ����� ��������������� ����������)
	size_t fragmentOffsetZByDevice;			// �������� ��������� ������������ ����� ��������������� ���������� �� ��� Oz (������������ ���� � ������������ (0, 0, 0) ����� ��������������� ����������)
	size_t fragmentOffsetXByNode;			// �������� ��������� ������������ ����� ��������������� ���������� �� ��� Ox (������������ ���� � ������������ (0, 0, 0) ����� ��������������� ����)
	size_t fragmentOffsetYByNode;			// �������� ��������� ������������ ����� ��������������� ���������� �� ��� Oy (������������ ���� � ������������ (0, 0, 0) ����� ��������������� ����)
	size_t fragmentOffsetZByNode;			// �������� ��������� ������������ ����� ��������������� ���������� �� ��� Oz (������������ ���� � ������������ (0, 0, 0) ����� ��������������� ����)
	size_t fragmentOffsetX;			// �������� ��������� ������������ ����� ��������������� ���������� �� ��� Ox (������������ ���� � ������������ (0, 0, 0) ���������� ��������� �����)
	size_t fragmentOffsetY;			// �������� ��������� ������������ ����� ��������������� ���������� �� ��� Oy (������������ ���� � ������������ (0, 0, 0) ���������� ��������� �����)
	size_t fragmentOffsetZ;			// �������� ��������� ������������ ����� ��������������� ���������� �� ��� Oz (������������ ���� � ������������ (0, 0, 0) ���������� ��������� �����)
	LinearArrays2D linearArrays2D;		// ��������� ������ ��� ���������

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
	/// ���������� ����� ����� � ��������� ��������� �����
	/// </summary>
	/// <returns></returns>
	size_t GetNodesNumber()
	{
		size_t nodesNumber = fragmentNy * fragmentNz;
		return nodesNumber;
	}

	/// <summary>
	/// �������� ������ � ������� ��������� ������ ������ ���� ModelDataName ��������� fragmentNy �� fragmentNz
	/// </summary>
	/// <param name="modelDataName"></param>
	void LinearArrayCreate(ModelDataName modelDataName)
	{
		linearArrays2D.LinearArrayCreate(fragmentNy, fragmentNz, modelDataName);
	}

	/// <summary>
	/// ���������� ��������� �� ������ linearArray2D ���� ModelDataName
	/// </summary>
	/// <param name="modelDataName"></param>
	/// <returns></returns>
	LinearArray2D* GetLinearArray2D(ModelDataName modelDataName)
	{
		LinearArray2D* linearArray2D = &(linearArrays2D.linearArrays.at(modelDataName));
		return linearArray2D;
	}
	
	/// <summary>
	/// ���������� ������� ������� ������ ���� ModelDataName �� ��������� ����������� IndX � IndZ
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
	/// ���������� ����� ����������� ������, ���������� ����������
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
/// ��������� �������� ��������� ����� - ��������� XZ(����� ����������� ���������)
/// </summary>
struct Grid2DFragmentXZ
{
	size_t fragmentNx;			// ���������� ����� � ��������� ����� ���������� ��������������� ���� �� ��� Ox
	size_t fragmentNz;			// ���������� ����� � ��������� ����� ���������� ��������������� ���� �� ��� Oz
	size_t fragmentIndX;			// ������ 3D-��������� � ����� ���������� ��������������� ���� �� ��� Ox, �������� ����������� ������ 2D-��������
	size_t fragmentIndY;			// ������ 3D-��������� � ����� ���������� ��������������� ���� �� ��� Oy, �������� ����������� ������ 2D-��������
	size_t fragmentIndZ;			// ������ 3D-��������� � ����� ���������� ��������������� ���� �� ��� Oz, �������� ����������� ������ 2D-��������
	size_t fragmentOffsetXByDevice;			// �������� ��������� ������������ ����� ��������������� ���������� �� ��� Ox (������������ ���� � ������������ (0, 0, 0) ����� ��������������� ����������)
	size_t fragmentOffsetYByDevice;			// �������� ��������� ������������ ����� ��������������� ���������� �� ��� Oy (������������ ���� � ������������ (0, 0, 0) ����� ��������������� ����������)
	size_t fragmentOffsetZByDevice;			// �������� ��������� ������������ ����� ��������������� ���������� �� ��� Oz (������������ ���� � ������������ (0, 0, 0) ����� ��������������� ����������)
	size_t fragmentOffsetXByNode;			// �������� ��������� ������������ ����� ��������������� ���������� �� ��� Ox (������������ ���� � ������������ (0, 0, 0) ����� ��������������� ����)
	size_t fragmentOffsetYByNode;			// �������� ��������� ������������ ����� ��������������� ���������� �� ��� Oy (������������ ���� � ������������ (0, 0, 0) ����� ��������������� ����)
	size_t fragmentOffsetZByNode;			// �������� ��������� ������������ ����� ��������������� ���������� �� ��� Oz (������������ ���� � ������������ (0, 0, 0) ����� ��������������� ����)
	size_t fragmentOffsetX;			// �������� ��������� ������������ ����� ��������������� ���������� �� ��� Ox (������������ ���� � ������������ (0, 0, 0) ���������� ��������� �����)
	size_t fragmentOffsetY;			// �������� ��������� ������������ ����� ��������������� ���������� �� ��� Oy (������������ ���� � ������������ (0, 0, 0) ���������� ��������� �����)
	size_t fragmentOffsetZ;			// �������� ��������� ������������ ����� ��������������� ���������� �� ��� Oz (������������ ���� � ������������ (0, 0, 0) ���������� ��������� �����)
	LinearArrays2D linearArrays2D;		// ��������� ������ ��� ���������

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
	/// ���������� ����� ����� � ��������� ��������� �����
	/// </summary>
	/// <returns></returns>
	size_t GetNodesNumber()
	{
		size_t nodesNumber = fragmentNx * fragmentNz;
		return nodesNumber;
	}

	/// <summary>
	/// �������� ������� ���������� ����
	/// </summary>
	/// <param name="modelDataName"></param>
	void LinearArrayCreate(ModelDataName modelDataName)
	{
		linearArrays2D.LinearArrayCreate(fragmentNx, fragmentNz, modelDataName);
	}

	/// <summary>
	/// ���������� ��������� �� ��������� ������ ���������� ���� ModelDataName
	/// </summary>
	/// <param name="modelDataName"></param>
	/// <returns></returns>
	LinearArray2D* GetLinearArray2D(ModelDataName modelDataName)
	{
		LinearArray2D* linearArray2D = &(linearArrays2D.linearArrays.at(modelDataName));
		return linearArray2D;
	}
		
	/// <summary>
	/// ���������� ����� ����������� ������, ���������� ����������
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
/// ������� ��������� ��������� XZ ��� �������� ������ ����� ������������
/// </summary>
struct Grid2DTransferPlaneXZ
{
	size_t fragmentsNumX;		// ���������� ���������� � ��������� �� ��� Ox 
	size_t fragmentsNumZ;		// ���������� ���������� � ��������� �� ��� Oz

	size_t fragmentNx;			// ���������� ����� �� ���������� ����� ���������� ��������������� ���� �� ��� Ox (����� ����������)
	size_t fragmentNz;			// ���������� ����� �� ���������� ����� ���������� ��������������� ���� �� ��� Oz (����� ����������)
	size_t fragmentNxl;			// ���������� ����� �� ���������� ����� ���������� ��������������� ���� �� ��� Ox (���������)
	size_t fragmentNzl;			// ���������� ����� �� ���������� ����� ���������� ��������������� ���� �� ��� Oz (���������)
		
	std::vector<std::vector<LinearArrays2D>> linearArrays2D;//���������

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

	// ������

	/// <summary>
	/// �������� ������� ������ ���������� ���� ModelDataName
	/// </summary>
	/// <param name="modelDataName">��� ������������ �������</param>
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
	/// ����� � ������� ��������� ����������� ���� ��������� ����
	/// </summary>
	/// <param name="fragmentIndX">������ ��������� ����������� ���� �� OX</param>
	/// <param name="fragmentIndZ">������ ��������� ����������� ���� �� OZ</param>
	/// <param name="modelDataName">��� ������� ������</param>
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
/// ���������� �������� ����� ��������� ����� ��������������� ����������
/// </summary>
struct Grid3DFragment
{
	size_t fragmentNx;			// ���������� ����� � ��������� ����� ���������� ��������������� ���� �� ��� Ox
	size_t fragmentNy;			// ���������� ����� � ��������� ����� ���������� ��������������� ���� �� ��� Oy
	size_t fragmentNz;			// ���������� ����� � ��������� ����� ���������� ��������������� ���� �� ��� Oz
	size_t fragmentIndX;			// ������ ��������� � ����� ���������� ��������������� ���� �� ��� Ox
	size_t fragmentIndY;			// ������ ��������� � ����� ���������� ��������������� ���� �� ��� Oy
	size_t fragmentIndZ;			// ������ ��������� � ����� ���������� ��������������� ���� �� ��� Oz
	size_t fragmentOffsetXByDevice;			// �������� ��������� ������������ ����� ��������������� ���������� �� ��� Ox (������������ ���� � ������������ (0, 0, 0) ����� ��������������� ����������)
	size_t fragmentOffsetYByDevice;			// �������� ��������� ������������ ����� ��������������� ���������� �� ��� Oy (������������ ���� � ������������ (0, 0, 0) ����� ��������������� ����������)
	size_t fragmentOffsetZByDevice;			// �������� ��������� ������������ ����� ��������������� ���������� �� ��� Oz (������������ ���� � ������������ (0, 0, 0) ����� ��������������� ����������)
	size_t fragmentOffsetXByNode;			// �������� ��������� ������������ ����� ��������������� ���� �� ��� Ox (������������ ���� � ������������ (0, 0, 0) ����� ��������������� ����)
	size_t fragmentOffsetYByNode;			// �������� ��������� ������������ ����� ��������������� ���� �� ��� Oy (������������ ���� � ������������ (0, 0, 0) ����� ��������������� ����)
	size_t fragmentOffsetZByNode;			// �������� ��������� ������������ ����� ��������������� ���� �� ��� Oz (������������ ���� � ������������ (0, 0, 0) ����� ��������������� ����)
	size_t fragmentOffsetX;			// �������� ��������� ������������ ���������� ��������� ����� �� ��� Ox (������������ ���� � ������������ (0, 0, 0) ���������� ��������� �����)
	size_t fragmentOffsetY;			// �������� ��������� ������������ ���������� ��������� ����� �� ��� Oy (������������ ���� � ������������ (0, 0, 0) ���������� ��������� �����)
	size_t fragmentOffsetZ;			// �������� ��������� ������������ ���������� ��������� ����� �� ��� Oz (������������ ���� � ������������ (0, 0, 0) ���������� ��������� �����)
	Grid3DFragment* fragmentXPrev;		// ��������� �� �������� �������� �� ��� Ox (������������� �����������)
	Grid3DFragment* fragmentXNext;		// ��������� �� �������� �������� �� ��� Ox (������������� �����������)	
	Grid3DFragment* fragmentYPrev;		// ��������� �� �������� �������� �� ��� Oy (������������� �����������)	
	Grid3DFragment* fragmentYNext;		// ��������� �� �������� �������� �� ��� Oy (������������� �����������)	
	Grid3DFragment* fragmentZPrev;		// ��������� �� �������� �������� �� ��� Oz (������������� �����������)
	Grid3DFragment* fragmentZNext;		// ��������� �� �������� �������� �� ��� Oz (������������� �����������)	
	LinearArrays3D linearArrays3D;		// ��������� ������ ��� ���������

	
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

	// ������
	
	// ��������� �� ������� ��������� ��������� ��� �������� ������
	/// <summary>
	/// ���������� ��������� �� ������ ��������� XY ��������� �� ��� Oz (������ ��������� 3D-���������)
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
	/// ���������� ��������� �� ��������� ��������� XY ��������� �� ��� Oz (������� ��������� 3D-���������)
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
	/// ���������� ��������� �� ������ ��������� YZ ��������� �� ��� Ox (����� ��������� 3D-���������----)
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
	/// ���������� ��������� �� ��������� ��������� YZ ��������� �� ��� Ox (������ ��������� 3D-���������-----)
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
	/// ���������� ��������� �� ������ ��������� XZ ��������� �� ��� Oy �� ������ ���������
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
	/// ���������� ��������� �� ��������� ��������� XZ ��������� �� ��� Oy �� ������ ���������
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
	/// ���������� ����� ����� � ��������� ��������� �����
	/// </summary>
	/// <returns></returns>
	size_t GetNodesNumber()
	{
		size_t nodesNumber = fragmentNx * fragmentNy * fragmentNz;
		return nodesNumber;
	}

	/// <summary>
	/// ���������� ����� ����� � ��������� XY ��������� ��������� �����
	/// </summary>
	/// <returns></returns>
	size_t GetNodesNumberXY()
	{
		size_t nodesNumber = fragmentNx * fragmentNy;
		return nodesNumber;
	}

	/// <summary>
	/// ���������� ����� ����� � ��������� XZ ��������� ��������� �����
	/// </summary>
	/// <returns></returns>
	size_t GetNodesNumberXZ()
	{
		size_t nodesNumber = fragmentNx * fragmentNz;
		return nodesNumber;
	}

	/// <summary>
	/// ���������� ����� ����� � ��������� YZ ��������� ��������� �����
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

	/// <summary>
	/// ----------------------
	/// </summary>
	/// <param name="data"></param>
	/// <param name="nx"></param>
	/// <param name="ny"></param>
	/// <param name="nz"></param>
	/// <param name="modelDataName"></param>
	void �ompose(double* data, size_t nx, size_t ny, size_t nz, ModelDataName modelDataName)
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
	/// ���������� ����� ����������� ������, ���������� ����������
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
	/// ����� �� ������ ���� ��������� ��������� Fragment3DPlane ��������� ��� ���������� ���� ModelDataName
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
	/// ����� �� ������ ����� ��������� ��������� Fragment3DPlane ��������� ��� ���������� ���� ModelDataName, ����� �������� (Ind1_1, Ind1_2) �� (Ind2_1, Ind2_2)
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
/// ���������� ���� ��������� ����� ��������������� ����������
/// </summary>
struct GridBlock3DByDevice
{
	size_t blockNx;			// ���������� ����� � ����� ���������� ��������������� ���� �� ��� Ox
	size_t blockNy;			// ���������� ����� � ����� ���������� ��������������� ���� �� ��� Oy
	size_t blockNz;			// ���������� ����� � ����� ���������� ��������������� ���� �� ��� Oz
	size_t deviceIndex;		// ����� ��������������� ���������� � �������������� ����
	std::string deviceName;	// ������������ ��������������� ����������, ��������������� ����
	size_t fragmentsNumX;			// ���������� ���������� � ����� ���������� �� ��� Ox 
	size_t fragmentsNumY;			// ���������� ���������� � ����� ���������� �� ��� Oy
	size_t fragmentsNumZ;			// ���������� ���������� � ����� ���������� �� ��� Oz
	size_t deviceOffsetYByNode;		// �������� ����� ��������������� ���������� �� ��� Oy (������������ ���� � ������������ (0, 0, 0) ��������������� ����)
	size_t deviceOffsetY;			// �������� ����� ��������������� ���������� �� ��� Oy (������������ ���� � ������������ (0, 0, 0) ���������� ��������� �����)
	GridBlock3DByDevice* devicePrev;		// ��������� �� ���� ��������� �����, �������������� ���������� �������������� �����������
	GridBlock3DByDevice* deviceNext;		// ��������� �� ���� ��������� �����, �������������� ��������� �������������� �����������

	size_t fragmentNx;			// ���������� ����� �� ���������� ����� ���������� ��������������� ���� �� ��� Ox (����� ����������)
	size_t fragmentNy;			// ���������� ����� �� ���������� ����� ���������� ��������������� ���� �� ��� Oy (����� ����������)
	size_t fragmentNz;			// ���������� ����� �� ���������� ����� ���������� ��������������� ���� �� ��� Oz (����� ����������)
	size_t fragmentNxl;			// ���������� ����� �� ���������� ����� ���������� ��������������� ���� �� ��� Ox (���������)
	size_t fragmentNyl;			// ���������� ����� �� ���������� ����� ���������� ��������������� ���� �� ��� Oy (���������)
	size_t fragmentNzl;			// ���������� ����� �� ���������� ����� ���������� ��������������� ���� �� ��� Oz (���������)

	std::vector< std::vector <std::vector<Grid3DFragment> > > fragments;	// ���������� ������ ���������� ����� ��������� ����� ��������������� ���������� ��������������� ����

	std::vector<Grid2DTransferPlaneXZ> grid2DTransferPlanesXZ; // ������ ��������� ���������� XZ ��� �������� ������ ����� �������������

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

	// ������

	/// <summary>
	/// ������ ��������� ��������� XZ ��� �������� ������ ����� �������������
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
	/// ��������� � ������ ��������� ��������� XZ ��� �������� ������ ������ ���������� ����
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
	/// ������� � ������� ������ ��������� ��������� XZ ��� �������� ������
	/// </summary>	
	/// <param name="transferPlaneXZIndex">������ ����������� ����</param>
	/// <param name="fragmentIndX">������ ��������� ����������� ���� �� OX</param>
	/// <param name="fragmentIndZ">������ ��������� ����������� ���� �� OZ</param>
	/// <param name="modelDataName">��� ������� ������</param>
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

	/// <summary>
	/// -----------------
	/// </summary>
	/// <param name="data"></param>
	/// <param name="nx"></param>
	/// <param name="ny"></param>
	/// <param name="nz"></param>
	/// <param name="modelDataName"></param>
	void �ompose(double* data, size_t nx, size_t ny, size_t nz, ModelDataName modelDataName)
	{
		for (size_t k = 0; k < fragmentsNumZ; k++)
		{
			for (size_t j = 0; j < fragmentsNumY; j++)
			{
				for (size_t i = 0; i < fragmentsNumX; i++)
				{
					Grid3DFragment* fragment = GetGrid3DFragment(i, j, k);
					fragment->�ompose(data, nx, ny, nz, modelDataName);
				}
			}
		}
	}

	/// <summary>
	/// ���������� ����� ����������� ������, ���������� �������� GridBlock3DByDevice
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

	// ������� � ������� ��� � ������������ ��������������� ����
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
/// ���������� ���� ��������� ����� ��������������� ����
/// </summary>
struct GridBlock3DByNode
{
	size_t nodeNx;			// ���������� ����� � ����� ��������������� ���� �� ��� Ox
	size_t nodeNy;			// ���������� ����� � ����� ��������������� ���� �� ��� Oy
	size_t nodeNz;			// ���������� ����� � ����� ��������������� ���� �� ��� Oz
	size_t nodeIndex;		// ����� ����� ��������������� ����
	std::string nodeName;	// ������������ ��������������� ����, ��������������� ����
	size_t nodeOffsetY;			// �������� ����� ��������������� ���� �� ��� Oy (������������ ���� � ������������ (0, 0, 0) ���������� ��������� �����)
	GridBlock3DByNode* nodePrev;		// ��������� �� ���� ��������� �����, �������������� ���������� �������������� �����
	GridBlock3DByNode* nodeNext;		// ��������� �� ���� ��������� �����, �������������� ��������� �������������� �����
	std::vector<std::string> deviceNames;		// ������ ��������������� �������������� ���������
	std::map<std::string, GridBlock3DByDevice> gridBlock3DByDevices;		// ��������� ���� ����-�������� �������� GridBlock3DByDevice

	GridBlock3DByNode(size_t NodeNx, size_t NodeNy, size_t NodeNz, size_t NodeIndex, std::string NodeName, size_t NodeOffsetY,
		GridBlock3DByNode* NodePrev = NULL, GridBlock3DByNode* NodeNext = NULL) :
		nodeNx(NodeNx), nodeNy(NodeNy), nodeNz(NodeNz), nodeIndex(NodeIndex), nodeName(NodeName), nodeOffsetY(NodeOffsetY), nodePrev(NodePrev), nodeNext(NodeNext)
	{

	}

	// ������

	/// <summary>
	/// ������ ��������� ��������� XZ ��� �������� ������ ����� ��������������� ������������
	/// � �������� ������� �������� ��������� ���������� XZ ��� �������� ������ ����� �������������
	/// </summary>
	void CreateGrid2DTransferPlanesXZ()
	{
		// 1. ������ ��������� ��������� XZ ��� �������� ������ ����� ��������������� ������������


		// 2. �������� ������ �������� ��������� ���������� XZ ��� �������� ������ ����� �������������
		for (auto itByDevices = gridBlock3DByDevices.begin(); itByDevices != gridBlock3DByDevices.end(); itByDevices++)
		{
			auto deviceKey = itByDevices->first;
			GridBlock3DByDevice* deviceObj = &(itByDevices->second);
			deviceObj->CreateGrid2DTransferPlanesXZ();
		}
	}

	/// <summary>
	/// ��������� � ������ ��������� ��������� XZ ��� �������� ������ ������ ���������� ����
	/// </summary>
	/// <param name="modelDataName"></param>
	void Grid2DTransferPlanesXZLinearArrayCreate(ModelDataName modelDataName)
	{
		// 1. ��������� ������� ��� �������� ������ ����� ��������������� ������������

		// 2. �������� ������ ���������� �������� ��� �������� ������ ����� �������������				
		for (auto itByDevices = gridBlock3DByDevices.begin(); itByDevices != gridBlock3DByDevices.end(); itByDevices++)
		{
			auto deviceKey = itByDevices->first;
			GridBlock3DByDevice* deviceObj = &(itByDevices->second);
			deviceObj->Grid2DTransferPlanesXZLinearArrayCreate(modelDataName);
		}
	}

	/// <summary>
	/// ������� � ������� ������ ��������� ��������� XZ ��� �������� ������
	/// </summary>	
	/// <param name="deviceName">��� ��������������� ����������</param>
	/// <param name="transferPlaneXZIndex">������ ����������� ����</param>
	/// <param name="fragmentIndX">������ ��������� ����������� ���� �� OX</param>
	/// <param name="fragmentIndZ">������ ��������� ����������� ���� �� OZ</param>
	/// <param name="modelDataName">��� ������� ������</param>
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
	void Decompose(ComputingNode computingNode, size_t fragmentsNumX, size_t fragmentsNumZ)	//	��������� ������������ ����� ��������� ����� ��������������� ����
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

	// ������������� ��������� �� �������� �����
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

		// ������������� ��������� �� �������� �����, �������������� ���������� ������������
		//SetPrevNextPointersForDeviceBlocks();
	}

	// ������� � ������� ��� � ������������ ��������������� ����
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

	/// <summary>
	/// ------------------
	/// </summary>
	/// <param name="data"></param>
	/// <param name="nx"></param>
	/// <param name="ny"></param>
	/// <param name="nz"></param>
	/// <param name="modelDataName"></param>
	void �ompose(double* data, size_t nx, size_t ny, size_t nz, ModelDataName modelDataName)
	{
		for (auto itByDevices = gridBlock3DByDevices.begin(); itByDevices != gridBlock3DByDevices.end(); itByDevices++)
		{
			auto deviceKey = itByDevices->first;
			GridBlock3DByDevice* deviceObj = &(itByDevices->second);
			deviceObj->�ompose(data, nx, ny, nz, modelDataName);
		}
	}

	/// <summary>
	/// ���������� ����� ����������� ������, ���������� �������� GridBlock3DByNode
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
/// ���������� ��������� �����
/// </summary>
struct Grid3D
{
	size_t gridNx;			// ���������� ����� ��������� ����� �� ��� Ox
	size_t gridNy;			// ���������� ����� ��������� ����� �� ��� Oy
	size_t gridNz;			// ���������� ����� ��������� ����� �� ��� Oz
	double hx;			// ��� ��������� ����� �� ��� Ox
	double hy;			// ��� ��������� ����� �� ��� Oy
	double hz;			// ��� ��������� ����� �� ��� Oz
	std::vector<std::string> nodeNames;		// ������ ��������������� �������������� �����
	std::map<std::string, GridBlock3DByNode> gridBlock3DByNodes;		// ��������� ���� ����-�������� �������� GridBlock3DByNode

	Grid3D(size_t GridNx, size_t GridNy, size_t GridNz, double Hx, double Hy, double Hz) :
		gridNx(GridNx), gridNy(GridNy), gridNz(GridNz), hx(Hx), hy(Hy), hz(Hz)
	{

	}

	// ������

	/// <summary>
	/// ������ ��������� ��������� XZ ��� �������� ������ ����� �������������
	/// </summary>
	void CreateGrid2DTransferPlanesXZ()
	{
		// 1. ������ ��������� ��������� XZ ��� �������� ������ ����� ��������������� ������

		// 2. �������� ������ �������� ��������� ���������� XZ ��� �������� ������
		//    ����� ��������������� ������������ � ����� �������������				
		for (auto itByNodes = gridBlock3DByNodes.begin(); itByNodes != gridBlock3DByNodes.end(); itByNodes++)
		{
			auto nodeKey = itByNodes->first;
			GridBlock3DByNode* nodeObj = &(itByNodes->second);
			nodeObj->CreateGrid2DTransferPlanesXZ();
		}
	}

	/// <summary>
	/// ��������� � ������ ��������� ��������� XZ ��� �������� ������ ������ ���������� ����
	/// </summary>
	/// <param name="modelDataName"></param>
	void Grid2DTransferPlanesXZLinearArrayCreate(ModelDataName modelDataName)
	{
		// 1. ��������� ������� ��� �������� ������ ����� ��������������� ������

		// 2. �������� ������ ���������� �������� ��� �������� ������
		//    ����� ��������������� ������������ � ����� �������������				
		for (auto itByNodes = gridBlock3DByNodes.begin(); itByNodes != gridBlock3DByNodes.end(); itByNodes++)
		{
			auto nodeKey = itByNodes->first;
			GridBlock3DByNode* nodeObj = &(itByNodes->second);
			nodeObj->Grid2DTransferPlanesXZLinearArrayCreate( modelDataName);
		}
	}

	/// <summary>
	/// ������� � ������� ������ ��������� ��������� XZ ��� �������� ������
	/// </summary>
	/// <param name="nodeName">��� ���� ��������</param>
	/// <param name="deviceName">��� ��������������� ����������</param>
	/// <param name="transferPlaneXZIndex">������ ����������� ����</param>
	/// <param name="fragmentIndX">������ ��������� ����������� ���� �� OX</param>
	/// <param name="fragmentIndZ">������ ��������� ����������� ���� �� OZ</param>
	/// <param name="modelDataName">��� ������� ������</param>
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
	void Decompose(ComputingCluster cluster, size_t fragmentsNumX, size_t fragmentsNumZ)	//	��������� ������������ ��������� �����: ����� ����� Grid �� ����� ��� ���.��������� �������� (Nodes)
	{
		double sumNyNode = 0;
		double sumPerfomance = cluster.GetClusterPerfomance();
		size_t nodeOffsetY = 0;	

		// ������������� �� ����� ��������
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

		// ������������� ��������� �� �������� �����
		SetPrevNextPointers();
	}
	
	// ������������� ��������� �� �������� �����
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

		// ������������� ��������� �� �������� �����, �������������� ���������� ������������
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
	double* �ompose(ModelDataName modelDataName)	//	��������� ������ ��������� �������, ������������ �� ����� ��������� �����
	{
		long ram = sizeof(double) * gridNx * gridNy * gridNz;
		double* data = (double*)malloc(ram);
		
		for (auto itByNodes = gridBlock3DByNodes.begin(); itByNodes != gridBlock3DByNodes.end(); itByNodes++)
		{
			auto nodeKey = itByNodes->first;
			GridBlock3DByNode* nodeObj = &(itByNodes->second);
			nodeObj->�ompose(data, gridNx, gridNy, gridNz, modelDataName);
		}

		return data;
	}

	/// <summary>
	/// ���������� ����� ����������� ������, ���������� �������� Grid3D
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
