#ifndef COMPUTING_DEVICE
#define COMPUTING_DEVICE

/// <summary>
/// Абстрактный класс, описывает общие характеристики устройств
/// </summary>
struct ComputingDevice
{
	std::string deviceId;			// Вид устройства (CPU или GPU)
	double devicePerfomance;		// Производительность устройства

	ComputingDevice(std::string DeviceId, double DevicePerfomance) :
		deviceId(DeviceId), devicePerfomance(DevicePerfomance)
	{

	}

	// Методы

	/// <summary>
	/// Возвращает количество потоков
	/// </summary>
	/// <returns>Количество потоков</returns>
	virtual long GetNumThreads()
	{
		return 0;
	}

	/// <summary>
	/// Выводит в консоль характеристики устройства
	/// </summary>
	virtual void PrintDevice()
	{
		std::cout << "\t" << deviceId << ": " << "\tdevicePerfomance = " << devicePerfomance;
	}
};

#endif