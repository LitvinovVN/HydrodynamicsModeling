#ifndef ENUMS
#define ENUMS


/// <summary>
/// Перечисление наименований массивов данных модели
/// </summary>
enum ModelDataName
{
	U,///< Проекция вектора скорости на ось Ox
	V,///< Проекция вектора скорости на ось Oy
	W,///< Проекция вектора скорости на ось Oz
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

/// <summary>
/// Перечисление уровней детализации вывода метода Print класса Grid3D
/// </summary>
enum Grid3DPrintDetalisation
{
	GRID,
	NODES,
	DEVICES,
	FRAGMENTS,
	ARRAYS
};

/// <summary>
/// Перечисление плоскостей для передачи между фрагментами
/// </summary>
enum Fragment3DPlane
{
	XOZ_Prev,
	XOZ_Next,
	XOY_Prev,
	XOY_Next,
	YOZ_Prev,
	YOZ_Next
};

#endif