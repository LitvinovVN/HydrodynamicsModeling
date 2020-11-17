
import Iteration
import numpy

a = numpy.array([[1., 2, 1],
                [1, 3 , 1],
                [0, 1, 1]])

f = numpy.array([1, 1, 2])

#a= numpy.array([[ 1. ,  0.6, -0.4,  0.4],
               # [ 0.,   1.,  -1.5,  1.5],
               # [ 0.,   0.,   1.,   1. ]])

#a = numpy.array([[ 2. ,  10.0, -3.0],
#                [ -3.,   -24.,  5.0],
#                [ 1.,   3.,   -5.]])

#f = numpy.array([38.0, -86.0, 27.0 ])

print(a)
print("\n")

x, res = Iteration.iteration(a, f, 0.25, 1000, 1e-6)
print("Ответ:")
print(x)
print("Вектор r")
print(res)