#ifndef TIMER
#define TIMER

/// <summary>
/// Таймер
/// </summary>
/// <typeparam name="duration">Единица измерения</typeparam>
/// <typeparam name="clock">Настройки часов</typeparam>
template <typename duration = std::chrono::seconds, typename clock = std::chrono::high_resolution_clock>
class timer
{
	/// Старт, стоп
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

	double get_time_as_double()
	{
		double time = get_time();
		return time;
	}
};

#endif