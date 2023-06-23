from array import *

def FA(a,b,c):
    s = (a ^ b ^ c)
    carry = (a&b) | (b&c) | (a&c)
    return s,carry

def HA(a,b):
    s = (a ^ b) 
    carry = (a&b)
    return s,carry

def Exact_Compressor_4_2(a,b,c,d,cin):
    s = (a^b^c^d^cin)
    carry = ((a^b^c^d)&cin) | ((~(a^b^c^d))&d)
    cout = ((a^b)&c) | ((~(a^b))&a)
    return s,carry,cout
    
def Exact_Compressor_4_2_NEW(a,b,c,d,cin):
    w1, cout = FA(a,b,c)
    s, carry = FA(d,w1,cin)
    return s,carry,cout
