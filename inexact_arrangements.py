# INEXACT ARRANGEMENTS
from array import *
import numpy as np
from exact_compressors import *
from approximate_compressors import *
from fast_adders import *
from gen_partial_prod import *
from PM_NM_Processing_blocks import *
from PM_NM_Top import *

n_kernel = 7

# READING IMAGE AND CONVERTING TO 8 BIT BINARY
image_file = open('D:/IIIT-B/Semester3/pm_nm_sources/data/image_7x7.txt', 'r')
image_final = []
image_lines = image_file.readlines()
for image_line in image_lines:
    a_dec = int(image_line.strip())
    a_bin = [int(i) for i in list('{0:08b}'.format(a_dec))]
    a_bin.reverse()
    image_final.append(a_bin)

# READING KERNEL AND CONVERTING TO 8 BIT BINARY
kernel_file = open('D:/IIIT-B/Semester3/pm_nm_sources/data/kernel_smoothning_7x7.txt', 'r')
kernel_final = []
kernel_lines = kernel_file.readlines()
for kernel_line in kernel_lines:
    b_dec = int(kernel_line.strip())
    b_bin = [int(i) for i in list('{0:08b}'.format(b_dec))]
    b_bin.reverse()
    kernel_final.append(b_bin)

# DIFFERENT ARRANGEMENTS, UNCOMMENT THE REQUIRED ARRANGEMENT
for n in range(0, len(image_final), (n_kernel*n_kernel)):
    for m in range(n_kernel*n_kernel):
        
        # ALL APEM
        # prod = PM8_top_NoIntrlv(image_final[n+m],kernel_final[n+m])
        
        # ALL ANEM
        # prod = NM8_top_NoIntrlv(image_final[n+m],kernel_final[n+m])
        
        # ALTERNATE APEM ANEM
        # if m%2 == 0:
        #     prod = PM8_top_NoIntrlv(image_final[n+m],kernel_final[n+m])
        # else
        #     prod = NM8_top_NoIntrlv(image_final[n+m],kernel_final[n+m])
        
        # ALTERNATE ANEM APEM
        # if m%2 == 0:
        #     prod = NM8_top_NoIntrlv(image_final[n+m],kernel_final[n+m])
        # else
        #     prod = PM8_top_NoIntrlv(image_final[n+m],kernel_final[n+m])
        
        # ALTERNATE APEM ANEM RINGS
        # if m in [0,1,2,3,4,5,6, 7,13, 14,20, 21,27, 28,34, 35,41, 42,43,44,45,46,47,48, 16,17,18, 23,25, 30,31,32]:
        #     prod = PM8_top_NoIntrlv(image_final[n+m],kernel_final[n+m])
        # else
        #     prod = NM8_top_NoIntrlv(image_final[n+m],kernel_final[n+m])
        
        # ALTERNATE ANEM APEM RINGS
        # if m in [0,1,2,3,4,5,6, 7,13, 14,20, 21,27, 28,34, 35,41, 42,43,44,45,46,47,48, 16,17,18, 23,25, 30,31,32]:
        #     prod = NM8_top_NoIntrlv(image_final[n+m],kernel_final[n+m])
        # else
        #     prod = PM8_top_NoIntrlv(image_final[n+m],kernel_final[n+m])
        
        # ALTERNATE ROWS APEM ANEM
        # if m in [7,8,9,10,11,12,13, 21,22,23,24,25,26,27, 35,36,37,38,39,40,41]:
        #     prod = NM8_top_NoIntrlv(image_final[n+m],kernel_final[n+m])
        # else
        #     prod = PM8_top_NoIntrlv(image_final[n+m],kernel_final[n+m])
        
        # ALTERNATE ROWS ANEM APEM
        # if m in [7,8,9,10,11,12,13, 21,22,23,24,25,26,27, 35,36,37,38,39,40,41]:
        #     prod = PM8_top_NoIntrlv(image_final[n+m],kernel_final[n+m])
        # else
        #     prod = NM8_top_NoIntrlv(image_final[n+m],kernel_final[n+m])
        
        # ALTERNATE COLS APEM ANEM
        # if m in [1,8,15,22,29,36,43, 3,10,17,24,31,38,45, 5,12,19,26,33,40,47]:
        #     prod = NM8_top_NoIntrlv(image_final[n+m],kernel_final[n+m])
        # else:
        #     prod = PM8_top_NoIntrlv(image_final[n+m],kernel_final[n+m])
        
        # ALTERNATE COLS ANEM APEM
        # if m in [1,8,15,22,29,36,43, 3,10,17,24,31,38,45, 5,12,19,26,33,40,47]:
        #     prod = PM8_top_NoIntrlv(image_final[n+m],kernel_final[n+m])
        # else:
        #     prod = NM8_top_NoIntrlv(image_final[n+m],kernel_final[n+m])
        
        
        conv_temp.append(prod)


conv_final = []
for x in range(0, len(conv_temp), n_kernel*n_kernel):
    s = 0 
    for k in range(0, (n_kernel*n_kernel)):
        s = s + conv_temp[x+k]
    conv_final.append(s)

conv_final_matrix = np.array([[0 for i in range(256)] for j in range(256)])
k = 0
for i in range(256):
    for j in range(256):
        conv_final_matrix[i][j] = conv_final[k]
        k = k+1

#np.savetxt('D:/IIIT-B/Semester3/pm_nm_sources/data/7x7/outline/without_scaling/all_positive.csv', conv_final_matrix, delimiter=',')
#np.savetxt('D:/IIIT-B/Semester3/pm_nm_sources/data/7x7/outline/without_scaling/all_negative.csv', conv_final_matrix, delimiter=',')

