
import numpy as np
import copy


def iteration(a, f, tau, maxit=1000, eps=1e-6):
    x = []

    for i in range(0, a.shape[1]):
        x.append(0)

    it = 0
    res = []
    while it < maxit:
        r = a.dot(x) - f
        x = x - tau * r
        res.append(np.linalg.norm(r))
        if np.linalg.norm(r) < eps:
            return x, res
        it += 1
    print('Method has not converged in %d iterations' % maxit)
    return x, res