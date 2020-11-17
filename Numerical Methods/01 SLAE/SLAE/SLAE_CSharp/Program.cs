using System;

namespace SLAE_CSharp
{
    class Program
    {
        public static void Main(string[] args)
        {
            //var a = new double[,]
            //    {
            //        { 1, 2, 1 },
            //        { 1, 3, 1 },
            //        { 0, 1, 1 }
            //    };

            //var b = new double[] { 1, 1, 2 };

            var a = new double[,]
                {
                    { 1, 0.6, -0.4 },
                    { 0, 1, -1.5 },
                    { 0, 0, 1 }
                };

            var b = new double[] { 0.4, 1.5, 1 };


            //var a = new double[,]
            //    {
            //        { 2, 10, -3 },
            //        { -3, -24, 5 },
            //        { 1, 3, -5 }
            //    };

            //var b = new double[] { 38, -86, 27 };

            //var x = Gauss(a, b);
            var x = SimpleIteration(a, b, 0.25);

        }

        /// <summary>
        /// Метод Гаусса для решения СЛАУ
        /// </summary>
        /// <param name="a">Коэффициенты левой части СЛАУ</param>
        /// <param name="b">Коэффициенты правой части СЛАУ</param>
        /// <returns>Вектор решений</returns>
        private static double[] Gauss(double[,] a, double[] b)
        {
            var razm = a.GetLength(0);

            for (int row = 0; row < razm; row++)
            {
                var max = a[row, row];
                var r = row;

                for (int i = r + 1; i < razm; i++)
                {
                   if (a[i, r] > max)
                    {
                        max = a[i, r];
                        r = i;
                    }
                }

                if (row != r)
                {
                    for (int col = 0; col < razm; col++)
                    {
                        var c = a[row, col];
                        a[row, col] = a[r, col];
                        a[r, col] = c;

                        var d = b[row];
                        b[row] = b[r];
                        b[r] = d;
                    }
                }
                
                for (int i = row + 1; i < razm; i++)
                {
                    var M = a[i, row] / a[row, row];

                    for (int j = row; j < razm; j++)
                    {
                        a[i, j] = a[i, j] - M * a[row, j];                        
                    }

                    b[i] = b[i] - M * b[row];
                }

                if (Math.Abs(a[razm - 1, razm - 1] ) < double.Epsilon)
                {
                    if (Math.Abs(b[razm-1]) < double.Epsilon)
                    {
                        throw new Exception("Бесконечное множество решений");
                    }
                    else
                    {
                        throw new Exception("Решений нет");
                    }
                }               

            }

            var x = new double[razm];

            for (int row = razm - 1; row >= 0; row--)
            {
                double s = 0;
                for (int col = row + 1; col < razm; col++)
                {
                    s += a[row, col] * x[col];                    
                }
                x[row] = (b[row] - s) / a[row, row];
            }

            return x;
        }

        private static double[] SimpleIteration(double[,] a, double[] b, double tau, int maxit= 1000, double eps= 1e-6)
        {
            var razm = a.GetLength(0);
            var x = new double[razm];

            int it = 0;            
            while (it < maxit)
            {
                var r = ProductMatrixVector(a, x);
                r = DifferenceVector(r, b);

                for (int row = 0; row < razm; row++)
                {
                    x[row] -= tau * r[row];
                }

                var norm = NormVector(r);

                if (norm < eps)
                {
                    return x;
                }
                it += 1;
            }

            throw new Exception($"Метод не сходится за {maxit} итераций");              
                
        }

        /// <summary>
        /// Произведение матрицы на вектор
        /// </summary>
        /// <param name="a">Матрица размерности n на n</param>
        /// <param name="x">Вектор размерности n</param>
        /// <returns>Вектор размерности n</returns>
        private static double[] ProductMatrixVector(double[,] a, double[] x)
        {
            var razm = a.GetLength(0);
            var r = new double[razm];

            for (int row = 0; row < razm; row++)
            {
                for (int col = 0; col < razm; col++)
                {
                    r[row] += a[row, col] * x[col];
                }
            }

            return r;
        }

        /// <summary>
        /// Вычисление нормы вектора (корень квадратный из суммы квадратов элементов вектора)
        /// </summary>
        /// <param name="x">Вектор</param>
        /// <returns></returns>
        private static double NormVector(double[] x)
        {
            var razm = x.GetLength(0);
            double norma = 0;

            for (int row = 0; row < razm; row++)
            {
                norma += Math.Pow(x[row], 2);
            }

            norma = Math.Sqrt(norma);
            return norma;
        }

        /// <summary>
        /// Вычисление разности элементов двух векторов
        /// </summary>
        /// <param name="a">Вектор-уменьшаемое</param>
        /// <param name="b">Вектор-вычитаемое</param>
        /// <returns>Разность векторов</returns>
        private static double[] DifferenceVector(double[] a, double[] b)
        {
            var razm = a.GetLength(0);
            var r = new double[razm];

            for (int row = 0; row < razm; row++)
            {
                r[row] = a[row] - b[row];
            }

            return r;
        }
    }
}
