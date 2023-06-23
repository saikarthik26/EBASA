from array import *
import numpy as np
from exact_compressors import *
from approximate_compressors import *
from fast_adders import *
from gen_partial_prod import *

def PM_PC_PC_processing_blk_8x8(p):
    r1 = np.array([[0 for i in range(5)] for j in range(15)])
    out1 = np.array([0 for i in range(15)])
    out2 = np.array([0 for i in range(15)])
    
    # column 0
    r1[0][0] = p[0][0]
    # column 1
    r1[1][0], r1[2][0] = HA(p[1][0], p[0][1])
    # column 2
    r1[2][1], r1[3][0] = Positive_Compressor_3_2(p[2][0], p[1][1], p[0][2])
    # column 3
    r1[3][1], r1[4][0] = Positive_Compressor_4_2(p[3][0], p[2][1], p[1][2], p[0][3])
    # column 4
    r1[4][1], r1[5][0] = Positive_Compressor_5_2new(p[4][0], p[3][1], p[2][2], p[1][3], p[0][4])
    # column 5
    r1[5][1], r1[6][0] = Positive_Compressor_5_2new(p[5][0], p[4][1], p[3][2], p[2][3], p[1][4])
    r1[5][2] = p[0][5]
    
    # column 6
    r1[6][1], r1[7][0] = Positive_Compressor_5_2new(p[6][0], p[5][1], p[4][2], p[3][3], p[2][4])
    
    r1[6][2] = p[1][5]
    r1[6][3] = p[0][6]
    
    # column 7
    r1[7][1], r1[8][0] = Positive_Compressor_5_2new(p[7][0], p[6][1], p[5][2], p[4][3], p[3][4])
    r1[7][2], r1[8][1] = HA(p[2][5], p[1][6])
    r1[7][3] = p[0][7]
    # column 8
    cin0=0
    r1[8][2], r1[9][0], cin1 = Exact_Compressor_4_2_NEW(p[7][1], p[6][2], p[5][3], p[4][4], cin0)
    r1[8][3], r1[9][1] = FA(p[3][5], p[2][6], p[1][7])
    # column 9
    r1[9][2], r1[10][0], cin2 = Exact_Compressor_4_2_NEW(p[7][2], p[6][3], p[5][4], p[4][5], cin1)
    r1[9][3], r1[10][1] = HA(p[3][6], p[2][7])
    # column 10
    r1[10][2], r1[11][0], cin3 = Exact_Compressor_4_2_NEW(p[7][3], p[6][4], p[5][5], p[4][6], cin2)
    r1[10][3] = p[3][7]
    # column 11
    r1[11][1], r1[12][0] = FA(p[7][4], p[6][5], cin3)
    r1[11][2] = p[5][6]
    r1[11][3] = p[4][7]
    # column 12
    r1[12][1] = p[7][5]
    r1[12][2] = p[6][6]
    r1[12][3] = p[5][7]
    # column 13
    r1[13][0] = p[7][6]
    r1[13][1] = p[6][7]
    # column 14
    r1[14][0] = p[7][7]

    
    # PR REDUCTION STEP TWO
    r2 = np.array([[0 for i in range(2)] for j in range(15)])
    
    # column 0
    r2[0][0] = r1[0][0]
    # column 1
    r2[1][0] = r1[1][0]
    # column 2
    r2[2][0] = r1[2][0]
    r2[2][1] = r1[2][1]
    # column 3
    r2[3][0] = r1[3][0]
    r2[3][1] = r1[3][1]
    # column 4
    r2[4][0] = r1[4][0]
    r2[4][1] = r1[4][1]
    # column 5
    r2[5][0], r2[6][0] = HA(r1[5][0], r1[5][1])
    r2[5][1] = r1[5][2]
    # column 6
    r2[6][1], r2[7][0] = Positive_Compressor_4_2(r1[6][0], r1[6][1], r1[6][2], r1[6][3])
    # column 7
    r2[7][1], r2[8][0] = Positive_Compressor_4_2(r1[7][0], r1[7][1], r1[7][2], r1[7][3])
    # column 8
    w0 = 0
    r2[8][1], r2[9][0], w1 = Exact_Compressor_4_2_NEW(r1[8][0], r1[8][1], r1[8][2], r1[8][3], w0)
    # column 9
    r2[9][1], r2[10][0], w2 = Exact_Compressor_4_2_NEW(r1[9][0], r1[9][1], r1[9][2], r1[9][3], w1)
    # column 10
    r2[10][1], r2[11][0], w3 = Exact_Compressor_4_2_NEW(r1[10][0], r1[10][1], r1[10][2], r1[10][3], w2)
    # column 11
    r2[11][1], r2[12][0], w4 = Exact_Compressor_4_2_NEW(r1[11][0], r1[11][1], r1[11][2], r1[11][3], w3)
    # column 12
    r2[12][1], r2[13][0], w5 = Exact_Compressor_4_2_NEW(r1[12][0], r1[12][1], r1[12][2], r1[12][3], w4)
    # column 13
    r2[13][1], r2[14][0] = FA(r1[13][0], r1[13][1], w5)
    # column 14
    r2[14][1] = r1[14][0]
    
    # Outputs of processing block
    out1[0] = r2[0][0]
    out1[1] = r2[1][0]
    out1[2] = r2[2][0]
    out1[3] = r2[3][0]
    out1[4] = r2[4][0]
    out1[5] = r2[5][0]
    out1[6] = r2[6][0]
    out1[7] = r2[7][0]
    out1[8] = r2[8][0]
    out1[9] = r2[9][0]
    out1[10] = r2[10][0]
    out1[11] = r2[11][0]
    out1[12] = r2[12][0]
    out1[13] = r2[13][0]
    out1[14] = r2[14][0]
    
    out2[0] = 0 #r2[0][1]
    out2[1] = 0 #r2[1][1]
    out2[2] = r2[2][1]
    out2[3] = r2[3][1]
    out2[4] = r2[4][1]
    out2[5] = r2[5][1]
    out2[6] = r2[6][1]
    out2[7] = r2[7][1]
    out2[8] = r2[8][1]
    out2[9] = r2[9][1]
    out2[10] = r2[10][1]
    out2[11] = r2[11][1]
    out2[12] = r2[12][1]
    out2[13] = r2[13][1]
    out2[14] = r2[14][1]
    
    return out1, out2

def NM_NC_NC_processing_blk_8x8(p):
    r1 = np.array([[0 for i in range(5)] for j in range(15)])
    out1 = np.array([0 for i in range(15)])
    out2 = np.array([0 for i in range(15)])
    
    # column 0
    r1[0][0] = p[0][0]
    # column 1
    r1[1][0], r1[2][0] = HA(p[1][0], p[0][1])
    # column 2
    r1[2][1], r1[3][0] = Negative_Compressor_3_2(p[2][0], p[1][1], p[0][2])
    # column 3
    r1[3][1], r1[4][0] = Negative_Compressor_4_2(p[3][0], p[2][1], p[1][2], p[0][3])
    # column 4
    r1[4][1], r1[5][0] = Negative_Compressor_5_2new(p[4][0], p[3][1], p[2][2], p[1][3], p[0][4])
    # column 5
    r1[5][1], r1[6][0] = Negative_Compressor_5_2new(p[5][0], p[4][1], p[3][2], p[2][3], p[1][4])
    r1[5][2] = p[0][5]
    # column 6
    r1[6][1], r1[7][0] = Negative_Compressor_5_2new(p[6][0], p[5][1], p[4][2], p[3][3], p[2][4])
    
    r1[6][2] = p[1][5]
    r1[6][3] = p[0][6]
    
    # column 7
    r1[7][1], r1[8][0] = Negative_Compressor_5_2new(p[7][0], p[6][1], p[5][2], p[4][3], p[3][4])
    r1[7][2], r1[8][1] = HA(p[2][5], p[1][6])
    r1[7][3] = p[0][7]
    # column 8
    cin0=0
    r1[8][2], r1[9][0], cin1 = Exact_Compressor_4_2_NEW(p[7][1], p[6][2], p[5][3], p[4][4], cin0)
    r1[8][3], r1[9][1] = FA(p[3][5], p[2][6], p[1][7])
    # column 9
    r1[9][2], r1[10][0], cin2 = Exact_Compressor_4_2_NEW(p[7][2], p[6][3], p[5][4], p[4][5], cin1)
    r1[9][3], r1[10][1] = HA(p[3][6], p[2][7])
    # column 10
    r1[10][2], r1[11][0], cin3 = Exact_Compressor_4_2_NEW(p[7][3], p[6][4], p[5][5], p[4][6], cin2)
    r1[10][3] = p[3][7]
    # column 11
    r1[11][1], r1[12][0] = FA(p[7][4], p[6][5], cin3)
    r1[11][2] = p[5][6]
    r1[11][3] = p[4][7]
    # column 12
    r1[12][1] = p[7][5]
    r1[12][2] = p[6][6]
    r1[12][3] = p[5][7]
    # column 13
    r1[13][0] = p[7][6]
    r1[13][1] = p[6][7]
    # column 14
    r1[14][0] = p[7][7]
    
    #print(r1)
    
    # PR REDUCTION STEP TWO
    r2 = np.array([[0 for i in range(2)] for j in range(15)])
    
    # column 0
    r2[0][0] = r1[0][0]
    # column 1
    r2[1][0] = r1[1][0]
    # column 2
    r2[2][0] = r1[2][0]
    r2[2][1] = r1[2][1]
    # column 3
    r2[3][0] = r1[3][0]
    r2[3][1] = r1[3][1]
    # column 4
    r2[4][0] = r1[4][0]
    r2[4][1] = r1[4][1]
    # column 5
    r2[5][0], r2[6][0] = HA(r1[5][0], r1[5][1])
    r2[5][1] = r1[5][2]
    # column 6
    r2[6][1], r2[7][0] = Negative_Compressor_4_2(r1[6][0], r1[6][1], r1[6][2], r1[6][3])
    # column 7
    r2[7][1], r2[8][0] = Negative_Compressor_4_2(r1[7][0], r1[7][1], r1[7][2], r1[7][3])
    # column 8
    w0 = 0
    r2[8][1], r2[9][0], w1 = Exact_Compressor_4_2_NEW(r1[8][0], r1[8][1], r1[8][2], r1[8][3], w0)
    # column 9
    r2[9][1], r2[10][0], w2 = Exact_Compressor_4_2_NEW(r1[9][0], r1[9][1], r1[9][2], r1[9][3], w1)
    # column 10
    r2[10][1], r2[11][0], w3 = Exact_Compressor_4_2_NEW(r1[10][0], r1[10][1], r1[10][2], r1[10][3], w2)
    # column 11
    r2[11][1], r2[12][0], w4 = Exact_Compressor_4_2_NEW(r1[11][0], r1[11][1], r1[11][2], r1[11][3], w3)
    # column 12
    r2[12][1], r2[13][0], w5 = Exact_Compressor_4_2_NEW(r1[12][0], r1[12][1], r1[12][2], r1[12][3], w4)
    # column 13
    r2[13][1], r2[14][0] = FA(r1[13][0], r1[13][1], w5)
    # column 14
    r2[14][1] = r1[14][0]
    
    #print(r2)
    
    # Outputs of processing block
    out1[0] = r2[0][0]
    out1[1] = r2[1][0]
    out1[2] = r2[2][0]
    out1[3] = r2[3][0]
    out1[4] = r2[4][0]
    out1[5] = r2[5][0]
    out1[6] = r2[6][0]
    out1[7] = r2[7][0]
    out1[8] = r2[8][0]
    out1[9] = r2[9][0]
    out1[10] = r2[10][0]
    out1[11] = r2[11][0]
    out1[12] = r2[12][0]
    out1[13] = r2[13][0]
    out1[14] = r2[14][0]
    
    out2[0] = 0 #r2[0][1]
    out2[1] = 0 #r2[1][1]
    out2[2] = r2[2][1]
    out2[3] = r2[3][1]
    out2[4] = r2[4][1]
    out2[5] = r2[5][1]
    out2[6] = r2[6][1]
    out2[7] = r2[7][1]
    out2[8] = r2[8][1]
    out2[9] = r2[9][1]
    out2[10] = r2[10][1]
    out2[11] = r2[11][1]
    out2[12] = r2[12][1]
    out2[13] = r2[13][1]
    out2[14] = r2[14][1]
    
    return out1, out2