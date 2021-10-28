#include "testFunctions.h"

void algStartTransferFragmentTesting(void (*algFunPntr)(LinearArray3D*, LinearArray3D*), LinearArray3D* linearArray_next, LinearArray3D* linearArray_prev, int numberOfLaunches)
{
	timer<std::chrono::microseconds> aTimer;

	SimpleStatistics statistics{};

	for (size_t i = 0; i < numberOfLaunches; i++)
	{
		aTimer.start();
		algFunPntr(linearArray_next,linearArray_prev);
		auto elapsed = aTimer.stop();		
		statistics.add(elapsed.get_time_as_double());
	}

	statistics.print();
}

#pragma region ���� ������ �������� ������ ����� ��������� �����������
void Grid2DTransferPlaneFragmentTest02(LinearArray3D* linearArray_next, LinearArray3D* linearArray_prev)
{
    linearArray_prev->LinearArray3DTrasferXOZForward(linearArray_next);
}
#pragma endregion

void transferFragmentTesting02()
{
    //int numberOfLaunches = 10;

    std::cout << "--------------------Test02--------------------" << std::endl;

    for (int fragmentNx_prev = 10; fragmentNx_prev <= 100; fragmentNx_prev = fragmentNx_prev + 10)
    {
        for (int fragmentNz_prev = 10; fragmentNz_prev <= 100; fragmentNz_prev = fragmentNz_prev + 10)
        {
            std::cout <<"-------------------------fragmentNx_prev = " << fragmentNx_prev << "    fragmentNz_prev = " << fragmentNz_prev << std::endl;
            // ��������� fragment_prev
            //size_t fragmentNx_prev = 50;
            size_t fragmentNy_prev = 50;
            //size_t fragmentNz_prev = 50;
            size_t fragmentIndX_prev = 2;
            size_t fragmentIndY_prev = 1;
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

            // ��������� fragment_next
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

            algStartTransferFragmentTesting(Grid2DTransferPlaneFragmentTest02, linearArray_next, linearArray_prev, 20);
        }
    }
        
}





