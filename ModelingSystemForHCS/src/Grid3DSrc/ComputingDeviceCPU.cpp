#ifndef COMPUTING_DEVICE_CPU
#define COMPUTING_DEVICE_CPU

/// <summary>
/// Наследник абстрактного класса ComputingDevice, описывает характеристики CPU
/// </summary>
struct ComputingDeviceCPU : ComputingDevice
{
	int coreNumber;			// число ядер CPU на вычислительном узле

	ComputingDeviceCPU(std::string DeviceId, double DevicePerfomance, int CoreNumber) :
		ComputingDevice(DeviceId, DevicePerfomance), coreNumber(CoreNumber)
	{

	}

	// Методы

	/// <summary>
	/// Возвращает количество потоков, равное количеству ядер CPU
	/// </summary>
	/// <returns> Количество потоков, равное количеству ядер CPU</returns>
	long GetNumThreads() override
	{
		return coreNumber;
	}

	/// <summary>
	/// Выводит в консоль характеристики устройства (CPU)
	/// </summary>
	void PrintDevice() override
	{
		ComputingDevice::PrintDevice();
		std::cout << "\tcoreNumber = " << coreNumber << std::endl;
	}
};

#endif