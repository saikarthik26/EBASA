from array import *
import numpy as np
from fast_adders import *
from gen_partial_prod import *
from PM_NM_Processing_blocks import *

def PM8_top_NoIntrlv(x,y):
    prod = np.array([0 for i in range(17)])
    
    p = gen_partial_prod_8x8(x,y)
    
    ppr_out1 = np.array([0 for i in range(16)])
    ppr_out2 = np.array([0 for i in range(16)])
    ppr_out1, ppr_out2 = PM_PC_PC_processing_blk_8x8(p)
    
    #prod[0:15], prod[16] = CLA16(ppr_out1, ppr_out2)
    ppr_out1_flip = np.flip(ppr_out1)
    ppr_out2_flip = np.flip(ppr_out2)
    #print(ppr_out1_dec, ppr_out2_dec)
    ppr_out1_dec = int(''.join(map(lambda ppr_out1_flip: str(int(ppr_out1_flip)), ppr_out1_flip)), 2)
    ppr_out2_dec = int(''.join(map(lambda ppr_out2_flip: str(int(ppr_out2_flip)), ppr_out2_flip)), 2)
    #print(ppr_out1_dec, ppr_out2_dec)
    prod = ppr_out1_dec+ppr_out2_dec
    return prod

def NM8_top_NoIntrlv(x,y):
    prod = np.array([0 for i in range(17)])
    
    p = gen_partial_prod_8x8(x,y)
    
    ppr_out1 = np.array([0 for i in range(16)])
    ppr_out2 = np.array([0 for i in range(16)])
    ppr_out1, ppr_out2 = NM_NC_NC_processing_blk_8x8(p)
    ppr_out1_flip = np.flip(ppr_out1)
    ppr_out2_flip = np.flip(ppr_out2)
    ppr_out1_dec = int(''.join(map(lambda ppr_out1_flip: str(int(ppr_out1_flip)), ppr_out1_flip)), 2)
    ppr_out2_dec = int(''.join(map(lambda ppr_out2_flip: str(int(ppr_out2_flip)), ppr_out2_flip)), 2)
    
    #prod[0:15], prod[16] = CLA16(ppr_out1, ppr_out2)
    prod = ppr_out1_dec+ppr_out2_dec
    
    return prod