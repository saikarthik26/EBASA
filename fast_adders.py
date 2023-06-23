from array import *
import numpy as np

def CLA16(a, b):
    sum_out = np.array([0 for i in range(17)])
    w_c = np.array([0 for i in range(17)])
    w_g = np.array([0 for i in range(16)])
    w_p = np.array([0 for i in range(16)])
    #w_sum = np.array([0 for i in range(16)])
    w_c[0] = 0;

    for i in range(0,16):
        w_g[i] = a[i] & b[i];
        w_p[i] = a[i] | b[i];
        w_c[i+1] = w_g[i] | (w_p[i] & w_c[i]);

    for i in range(0,16):
        sum_out[i] = a[i] ^ b[i] ^ w_c[i];

    sum_out[16] = w_c[16];
    return sum_out[0:15], sum_out[16];
