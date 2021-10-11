// DecomposeGrid3D.cpp - декомпозиция расчетной области

#include <iostream>
#include "Grid3D.cpp"

double* CreateArrayDouble(size_t numElements)
{
    size_t dataSize = numElements * sizeof(double);
    double* data = (double*)malloc(dataSize);
    return data;
}

void InitArrayByIndexes(double* data, size_t numElements)
{
    for (int i = 0; i < numElements; i++)
    {
        data[i] = i;
    }
}

void PrintArrayElement(double* data, size_t numX, size_t numY, size_t numZ, size_t indX, size_t indY, size_t indZ)
{
    size_t ind = indX + indY * numX + indZ * numX * numY;
    std::cout << "(" << indX << ", " << indY << ", " << indZ << "): " << data[ind] << std::endl;
}

void PrintArrayFragment(double* data, size_t startIndex, size_t endIndex)
{
    std::cout << "Printing data from " << startIndex << " to " << endIndex << ":" << std::endl;
    for (int i = startIndex; i <= endIndex; i++)
    {
        std::cout << data[i] << " ";
    }
    std::cout << std::endl;
}

bool ComparingArrays(double* data1, double* data2, size_t numElements)
{
    double eps = 0.0000001;
    bool isEquals = true;
    for (int i = 0; i < numElements; i++)
    {
        if (abs(data1[i] - data2[i]) > eps)
        {
            std::cout << "ERROR!!! ";
            std::cout << "data1[" << i << "] = " << data1[i] << ", "
                << "data2[" << i << "] = " << data2[i] << std::endl;
            isEquals = false;
            break;
        }        
    }
    return isEquals;
}


#pragma region Grid3DTest01()
void Grid3DTest01()
{
    std::cout << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << "-------------------------Grid3DTest01()---------------------------" << std::endl;

    size_t gridNx = 300;
    size_t gridNy = 2000;
    size_t gridNz = 100;

    ////////////// Подготовка исходных данных //////////////
    size_t gridN = gridNx * gridNy * gridNz;
    double* data = CreateArrayDouble(gridN);
    InitArrayByIndexes(data, gridN);
    PrintArrayFragment(data, 0, 10);

    ////////////////// Настройка кластера //////////////////
    // Node_0
    ComputingDeviceCPU cpu0_0("cpu0.0", 2345, 4);
    ComputingDeviceGPU gpu0_0("gpu0.0", 600, 8, 64, 32);
    ComputingDeviceGPU gpu0_1("gpu0.1", 600, 8, 64, 32);
    ComputingDeviceGPU gpu0_2("gpu0.2", 600, 8, 64, 32);
    ComputingDeviceGPU gpu0_3("gpu0.3", 600, 8, 64, 32);

    ComputingNode node0("node_0", 768, 500);
    node0.computingDevices.emplace(cpu0_0.deviceId, cpu0_0);
    node0.computingDevices.emplace(gpu0_0.deviceId, gpu0_0);
    node0.computingDevices.emplace(gpu0_1.deviceId, gpu0_1);
    node0.computingDevices.emplace(gpu0_2.deviceId, gpu0_2);
    node0.computingDevices.emplace(gpu0_3.deviceId, gpu0_3);

    //Node_1
    ComputingDeviceCPU cpu1_0("cpu1.0", 2345, 4);
    ComputingDeviceGPU gpu1_0("gpu1.0", 600, 8, 64, 32);
    ComputingDeviceGPU gpu1_1("gpu1.1", 600, 8, 64, 32);
    ComputingDeviceGPU gpu1_2("gpu1.2", 600, 8, 64, 32);
    ComputingDeviceGPU gpu1_3("gpu1.3", 600, 8, 64, 32);

    ComputingNode node1("node_1", 768, 700);
    node1.computingDevices.emplace(cpu1_0.deviceId, cpu1_0);
    node1.computingDevices.emplace(gpu1_0.deviceId, gpu1_0);
    node1.computingDevices.emplace(gpu1_1.deviceId, gpu1_1);
    node1.computingDevices.emplace(gpu1_2.deviceId, gpu1_2);
    node1.computingDevices.emplace(gpu1_3.deviceId, gpu1_3);

    ComputingCluster cluster;
    cluster.computingNodes.emplace(node0.nodeId, node0);
    cluster.computingNodes.emplace(node1.nodeId, node1);
    std::cout << std::endl;
    std::cout << "------------device characteristics-------------" << std::endl;
    cluster.Print();

    std::cout << "--- Creating Grid3D object ---" << std::endl;
    Grid3D grid3D(gridNx, gridNy, gridNz, 1, 2, 3);
    grid3D.Print(GRID);
    std::cout << "--------------END Creating Grid3D----------------" << std::endl << std::endl;

    std::cout << "--- Grid3D object decompose by nodes ---" << std::endl;
    size_t fragmentsNumX = 15;
    size_t fragmentsNumZ = 14;
    grid3D.Decompose(cluster, fragmentsNumX, fragmentsNumZ);
    grid3D.LinearArrayCreate(U);
    grid3D.LinearArrayCreate(V);
    grid3D.LinearArrayCreate(W);
    grid3D.AssignData(data, gridNx, gridNy, gridNz, U);
    grid3D.Print(NODES);
    std::cout << "---------------END Grid3D decompose by nodes----------------" << std::endl << std::endl;

    std::cout << "--- Grid3D object print by devices ---" << std::endl;
    grid3D.Print(DEVICES);
    std::cout << "--------------END Grid3D decompose by devices----------------" << std::endl << std::endl;

    std::cout << "--- Grid3D object print by fragments ---" << std::endl;
    //grid3D.Print(FRAGMENTS);
    std::cout << "-------------END Grid3D decompose by fragments----------------" << std::endl << std::endl;

    std::cout << "------------Comparing arrays-------------" << std::endl;
    double* data_out = grid3D.Сompose(U);
    bool isEquals = ComparingArrays(data, data_out, gridNx * gridNy * gridNz);
    std::cout << "isEquals = " << isEquals;

    GridBlock3DByNode* gridBlock3DByNode_0 = grid3D.GetGridBlock3DByNode(0);
    GridBlock3DByDevice* gridBlock3DByDevice0 = gridBlock3DByNode_0->GetGridBlock3DByDevice(0);
    Grid3DFragment* fragment = gridBlock3DByDevice0->GetGrid3DFragment(0, 0, 0);
    size_t fragmentN = fragment->GetNodesNumber();
    size_t fragmentNxy = fragment->GetNodesNumberXY();
    size_t fragmentNxz = fragment->GetNodesNumberXZ();
    size_t fragmentNyz = fragment->GetNodesNumberYZ();
    LinearArray3D* linArrayU = fragment->GetLinearArray3D(U);
    //linArrayU->Print();

    PrintArrayElement(data, gridNx, gridNy, gridNz, 0, 0, 16);
    PrintArrayElement(data, gridNx, gridNy, gridNz, 0, 0, 17);

    Grid2DFragmentXY* fragmentXYFirst = fragment->GetGrid2DFragmentXYFirst(U);
    //fragmentXYFirst->PrintFragment();

    Grid2DFragmentXY* fragmentXYLast = fragment->GetGrid2DFragmentXYLast(U);
    //fragmentXYLast->PrintFragment();

    Grid2DFragmentYZ* fragmentYZFirst = fragment->GetGrid2DFragmentYZFirst(U);
    //fragmentYZFirst->PrintFragment();

    Grid2DFragmentYZ* fragmentYZLast = fragment->GetGrid2DFragmentYZLast(U);
    //fragmentYZLast->PrintFragment();

    Grid2DFragmentXZ* fragmentXZFirst = fragment->GetGrid2DFragmentXZFirst(U);
    fragmentXZFirst->PrintFragment();

    Grid2DFragmentXZ* fragmentXZLast = fragment->GetGrid2DFragmentXZLast(U);
    fragmentXZLast->PrintFragment();

    std::cout << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << std::endl;
}

#pragma endregion

#pragma region Тест создания смежной двумерной плоскости XZ для передачи данных между устройствами внутри узла
void Grid2DTransferPlaneXZTest01()
{
    std::cout << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << "-------------------Grid2DTransferPlaneXZTest01()------------------" << std::endl;
    // 1. Создаём тестовый кластер
    size_t gridNx = 30;
    size_t gridNy = 20;
    size_t gridNz = 10;

    ////////////// Подготовка исходных данных //////////////
    size_t gridN = gridNx * gridNy * gridNz;
    double* data = CreateArrayDouble(gridN);
    InitArrayByIndexes(data, gridN);
    PrintArrayFragment(data, 0, 10);

    ////////////////// Настройка кластера //////////////////
    // Node_0
    ComputingDeviceCPU cpu0_0("cpu0.0", 100, 2); // Задействуем 2 ядра

    ComputingNode node0("node_0", 8, 100);
    node0.computingDevices.emplace(cpu0_0.deviceId, cpu0_0);

    ComputingCluster cluster;
    cluster.computingNodes.emplace(node0.nodeId, node0);

    std::cout << std::endl;
    std::cout << "------------device characteristics-------------" << std::endl;
    cluster.Print();

    std::cout << "--- Creating Grid3D object ---" << std::endl;
    Grid3D grid3D(gridNx, gridNy, gridNz, 1, 2, 3);
    grid3D.Print(GRID);
    std::cout << "--------------END Creating Grid3D----------------" << std::endl << std::endl;

    std::cout << "--- Grid3D object decompose by nodes ---" << std::endl;
    size_t fragmentsNumX = 1;
    size_t fragmentsNumZ = 1;
    grid3D.Decompose(cluster, fragmentsNumX, fragmentsNumZ);
    grid3D.CreateGrid2DTransferPlanesXZ();
    grid3D.LinearArrayCreate(U);
    grid3D.Grid2DTransferPlanesXZLinearArrayCreate(U);
    grid3D.Grid2DTransferPlanesXZLinearArrayPrint("node_0", "cpu0.0", 0, 0, 0, U);
    grid3D.AssignData(data, gridNx, gridNy, gridNz, U);
    grid3D.Print(NODES);
    std::cout << "---------------END Grid3D decompose by nodes----------------" << std::endl << std::endl;

    std::cout << "--- Grid3D object print by devices ---" << std::endl;
    grid3D.Print(DEVICES);
    std::cout << "--------------END Grid3D decompose by devices----------------" << std::endl << std::endl;

    std::cout << "--- Grid3D object print by fragments ---" << std::endl;
    grid3D.Print(FRAGMENTS);
    std::cout << "-------------END Grid3D decompose by fragments----------------" << std::endl << std::endl;

    std::cout << "------------Comparing arrays-------------" << std::endl;
    double* data_out = grid3D.Сompose(U);
    bool isEquals = ComparingArrays(data, data_out, gridNx * gridNy * gridNz);
    std::cout << "isEquals = " << isEquals;

    std::cout << std::endl;
    std::cout << "------------------------------------------------------------------" << std::endl;
    std::cout << std::endl;
}
#pragma endregion

#pragma region Тест создания плоскости YOZ элементов для передачи данных между соседними фрагментами
void Grid2DTransferPlaneYZFragmentTest02()
{
    // параметры fragment_prev
    size_t fragmentNx_prev = 3;
    size_t fragmentNy_prev = 4;
    size_t fragmentNz_prev = 4;
    size_t fragmentIndX_prev = 1;
    size_t fragmentIndY_prev = 2;
    size_t fragmentIndZ_prev = 2;
    size_t fragmentOffsetXByDevice_prev = 0;
    size_t fragmentOffsetYByDevice_prev = 0;
    size_t fragmentOffsetZByDevice_prev = 0;
    size_t fragmentOffsetXByNode_prev = 0;
    size_t fragmentOffsetYByNode_prev = 0;
    size_t fragmentOffsetZByNode_prev = 0;
    size_t fragmentOffsetX_prev = 0;
    size_t fragmentOffsetY_prev = 0;
    size_t fragmentOffsetZ_prev = 0;

    // параметры fragment_next
    size_t fragmentNx_next = 3;
    size_t fragmentNy_next = 4;
    size_t fragmentNz_next = 4;
    size_t fragmentIndX_next = 2;
    size_t fragmentIndY_next = 2;
    size_t fragmentIndZ_next = 2;
    size_t fragmentOffsetXByDevice_next = 48;
    size_t fragmentOffsetYByDevice_next = 0;
    size_t fragmentOffsetZByDevice_next = 0;
    size_t fragmentOffsetXByNode_next = 48;
    size_t fragmentOffsetYByNode_next = 0;
    size_t fragmentOffsetZByNode_next = 0;
    size_t fragmentOffsetX_next = 48;
    size_t fragmentOffsetY_next = 0;
    size_t fragmentOffsetZ_next = 0;
    

    Grid3DFragment fragment_prev(fragmentNx_prev, fragmentNy_prev, fragmentNz_prev, fragmentIndX_prev, fragmentIndY_prev, fragmentIndZ_prev, fragmentOffsetXByDevice_prev,
        fragmentOffsetYByDevice_prev, fragmentOffsetZByDevice_prev, fragmentOffsetXByNode_prev, fragmentOffsetYByNode_prev, fragmentOffsetZByNode_prev, fragmentOffsetX_prev,
        fragmentOffsetY_prev, fragmentOffsetZ_prev);
    Grid3DFragment fragment_next(fragmentNx_next, fragmentNy_next, fragmentNz_next, fragmentIndX_next, fragmentIndY_next, fragmentIndZ_next, fragmentOffsetXByDevice_next,
        fragmentOffsetYByDevice_next, fragmentOffsetZByDevice_next, fragmentOffsetXByNode_next, fragmentOffsetYByNode_next, fragmentOffsetZByNode_next, fragmentOffsetX_next,
        fragmentOffsetY_next, fragmentOffsetZ_next);

    fragment_prev.linearArrays3D.LinearArrayCreate(fragmentNx_prev, fragmentNy_prev, fragmentNz_prev, U);
    fragment_next.linearArrays3D.LinearArrayCreate(fragmentNx_next, fragmentNy_next, fragmentNz_next, U);

    size_t globalIndx = 0;
    LinearArray3D* linearArray_prev = fragment_prev.GetLinearArray3D(U);
    //double arr = 0;

    for (int k = 0; k < fragmentNz_prev; k++)
    {
        for (int j = 0; j < fragmentNy_prev; j++)
        {
            for (int i = 0; i < fragmentNx_prev; i++)
            {                              
                linearArray_prev->SetElement(i, j, k, globalIndx);
                //arr = linearArray_prev->GetElement(i, j, k);
                //std::cout << arr << " ";
                globalIndx += 1;
            }
            std::cout << std::endl;
        }
    }

    LinearArray3D* linearArray_next = fragment_next.GetLinearArray3D(U); 

    for (int k = 0; k < fragmentNz_next; k++)
    {
        for (int j = 0; j < fragmentNy_next; j++)
        {
            for (int i = 0; i < fragmentNx_next; i++)
            {                               
                linearArray_next->SetElement(i, j, k, 0);                
            }
        }
    }

    

    //linearArray_prev->PrintData();
    fragment_prev.PrintFragment(Grid3DPrintDetalisation::ARRAYS);
    fragment_prev.PrintFragmentPlane(U, XOY_Prev);
    fragment_next.PrintFragment(Grid3DPrintDetalisation::ARRAYS);
    fragment_next.PrintFragmentPlane(U, XOY_Next);
}
#pragma endregion

#pragma region Тест скорости обработки одного фрагмента и передачи данных о значениях в смежных плоскостях
void FragmentCalculationAndDataTransferTests()
{
    
}

#pragma endregion 




int main()
{
#pragma region Grid3DTest01()
    //Grid3DTest01();
#pragma endregion

    // Тест создания смежной двумерной плоскости XZ для передачи данных между устройствами внутри узла
    //Grid2DTransferPlaneXZTest01();

    // Тест создания плоскости YOZ элементов для передачи данных между соседними фрагментами
    Grid2DTransferPlaneYZFragmentTest02();

//// ----- Не готово
#pragma region Тест скорости обработки одного фрагмента и передачи данных о значениях в смежных плоскостях
    //FragmentCalculationAndDataTransferTests();
#pragma endregion 
}

