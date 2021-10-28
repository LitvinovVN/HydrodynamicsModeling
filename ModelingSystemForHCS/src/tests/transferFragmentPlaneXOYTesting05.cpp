#include "testFunctions.h"

void algStartTransferFragmentTesting_2(void (*algFunPntr)(LinearArray3D*, LinearArray3D*, LinearArray2D*), LinearArray3D* linearArray_next,
    LinearArray3D* linearArray_prev, LinearArray2D* planeXOYForward, int numberOfLaunches)
{
	timer<std::chrono::microseconds> aTimer;

	SimpleStatistics statistics{};

	for (size_t i = 0; i < numberOfLaunches; i++)
	{
		aTimer.start();
		algFunPntr(linearArray_next, linearArray_prev, planeXOYForward);
		auto elapsed = aTimer.stop();		
		statistics.add(elapsed.get_time_as_double());
	}

	statistics.print();
}

#pragma region Тест создания плоскости элементов для передачи данных между соседними фрагментами
void Grid2DTransferPlaneXOYFragmentTest05(LinearArray3D* linearArray_next, LinearArray3D* linearArray_prev, LinearArray2D* planeXOYForward)
{      
    linearArray_prev->SetPlaneXOYLast(planeXOYForward);
    linearArray_next->GetPlaneXOYFirst(planeXOYForward);
}
#pragma endregion


void transferFragmentPlaneXOYTesting05()
{    
    //int numberOfLaunches = 10;

    std::cout << "--------------------Test05--------------------" << std::endl;
    for (int fragmentNx_prev = 10; fragmentNx_prev <= 100; fragmentNx_prev = fragmentNx_prev + 10)
    {
        for (int fragmentNy_prev = 10; fragmentNy_prev <= 100; fragmentNy_prev = fragmentNy_prev + 10)
        {
            std::cout << "-------------------------fragmentNx_prev = " << fragmentNx_prev << "    fragmentNy_prev = " << fragmentNy_prev << std::endl;
            // параметры fragment_prev
            //size_t fragmentNx_prev = 50;
            //size_t fragmentNy_prev = 50;
            size_t fragmentNz_prev = 50;
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
            size_t fragmentNx_next = fragmentNx_prev;
            size_t fragmentNy_next = fragmentNy_prev;
            size_t fragmentNz_next = fragmentNz_prev;
            size_t fragmentIndX_next = 2;
            size_t fragmentIndY_next = 2;
            size_t fragmentIndZ_next = 2;
            size_t fragmentOffsetXByDevice_next = 125000;
            size_t fragmentOffsetYByDevice_next = 0;
            size_t fragmentOffsetZByDevice_next = 0;
            size_t fragmentOffsetXByNode_next = 125000;
            size_t fragmentOffsetYByNode_next = 0;
            size_t fragmentOffsetZByNode_next = 0;
            size_t fragmentOffsetX_next = 125000;
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

            for (int k = 0; k < fragmentNz_prev; k++)
            {
                for (int j = 0; j < fragmentNy_prev; j++)
                {
                    for (int i = 0; i < fragmentNx_prev; i++)
                    {
                        linearArray_prev->SetElement(i, j, k, globalIndx);
                        globalIndx += 1;
                    }
                }
            }

            int globalIndx1 = 0;
            LinearArray3D* linearArray_next = fragment_next.GetLinearArray3D(U);

            for (int k = 0; k < fragmentNz_next; k++)
            {
                for (int j = 0; j < fragmentNy_next; j++)
                {
                    for (int i = 0; i < fragmentNx_next; i++)
                    {
                        linearArray_next->SetElement(i, j, k, globalIndx1);
                        globalIndx1 -= 1;
                    }
                }
            }

            LinearArray2D* planeXOYForward = new LinearArray2D(fragmentNx_prev, fragmentNy_prev);
            for (int j = 0; j < fragmentNy_prev; j++)
            {
                for (int i = 0; i < fragmentNx_prev; i++)
                {
                    planeXOYForward->SetElement(i, j, 0);
                }
            }

            LinearArray2D* planeXOZForward = new LinearArray2D(fragmentNx_prev, fragmentNz_prev);
            for (int k = 0; k < fragmentNz_prev; k++)
            {
                for (int i = 0; i < fragmentNx_prev; i++)
                {
                    planeXOZForward->SetElement(i, k, 0);
                }
            }

            LinearArray2D* planeYOZForward = new LinearArray2D(fragmentNy_prev, fragmentNz_prev);
            for (int k = 0; k < fragmentNz_prev; k++)
            {
                for (int j = 0; j < fragmentNy_prev; j++)
                {
                    planeYOZForward->SetElement(j, k, 0);
                }
            }

            


            algStartTransferFragmentTesting_2(Grid2DTransferPlaneXOYFragmentTest05, linearArray_next, linearArray_prev, planeXOYForward, 20);
            
        }
    }
    
    

}
