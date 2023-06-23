from array import *
import numpy as np

def gen_partial_prod_8x8(x,y):
    p = np.array([[0 for i in range(8)] for j in range(8)]);
    for i in range(0,8):
        for j in range(0,8):
            p[i][j] = x[j]&y[i]
    return p
