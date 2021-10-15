#ifndef GRID_HELPER
#define GRID_HELPER

/// <summary>
/// Вспомогательные вычислительные методы для Grid
/// </summary>
static struct GridHelper
{
	static size_t CountN(size_t nx, size_t ny, size_t nz)
	{
		size_t N = nx * ny * nz;
		return N;
	}
} GridHelper;

#endif