#ifndef COMPUTING_DEVICE_GPU
#define COMPUTING_DEVICE_GPU

/// <summary>
/// Наследник абстрактного класса ComputingDevice, описывает характеристики GPU
/// </summary>
struct ComputingDeviceGPU : ComputingDevice
{
	long spNumber;				///< Количество мультипроцессоров SP
	long sharedMemory;			///< Объем распределяемой памяти
	long globalMemory;			///< Объем видеопамяти GPU

	ComputingDeviceGPU(std::string DeviceId, double DevicePerfomance, long SpNumber, long SharedMemory, long GlobalMemory) :
		ComputingDevice(DeviceId, DevicePerfomance), spNumber(SpNumber), sharedMemory(SharedMemory), globalMemory(GlobalMemory)
	{

	}

	// Методы

	/// <summary>
	/// Возвращает количество потоков, равное количеству мультипроцессоров GPU
	/// </summary>
	/// <returns>Количество потоков, равное количеству мультипроцессоров GPU</returns>
	long GetNumThreads() override
	{
		return spNumber;
	}

	/// <summary>
	/// Выводит в консоль характеристики устройства (GPU)
	/// </summary>
	void PrintDevice() override
	{
		ComputingDevice::PrintDevice();
		std::cout << "\tspNumber = " << spNumber << "\tsharedMemory = " << sharedMemory << "\tglobalMemory = " << globalMemory << std::endl;
	}
};

#endif