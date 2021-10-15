#include "testFunctions.h"



void testFunction1(int a, int b)
{
	std::cout << "testFunction1(int a, int b): " << (a + b) << std::endl;
}


void testFunction4()
{
	ModelDataName mdn = C6;
	std::cout << "testFunction4(): " << mdn << std::endl;
	timer<std::chrono::milliseconds, std::chrono::high_resolution_clock> t{};
	t.start();
	t.stop().print();
	LinearArray1D* la1 = new LinearArray1D(10);
	la1->Print();
	LinearArray2D* la2 = new LinearArray2D(10,2);
	la2->Print();
	LinearArray3D* la3 = new LinearArray3D(10, 2, 2);
	la3->Print();

	LinearArrays2D las2{};
	las2.Print();
	LinearArrays3D las3{};
	las3.Print();

}

