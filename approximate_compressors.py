from array import *

def Positive_Compressor_3_2(a,b,c):
    s = a | b | c
    carry = c & (a | b)
    return s,carry

def Negative_Compressor_3_2(a,b,c):
    s = a | b | c
    carry = c & b
    return s,carry

def Positive_Compressor_4_2(a,b,c,d):
    s = a | b | c | d
    carry = ( (a | c) & b ) | ((b | c) & d)
    return s,carry

def Negative_Compressor_4_2(a,b,c,d):
    s = a | b | c | d
    carry = (a&b) | (c&d)
    return s,carry

def Positive_Compressor_5_2new(a,b,c,d,e):
    s = a | b | c | d | e
    carry = (a&(b|c|d|e)) | ((b|c)&(d|e)) | (d&e)
    return s,carry

def Negative_Compressor_5_2new(a,b,c,d,e):
    s = a | b | c | d | e
    carry = (a&(b|c|d|e)) | ((b&c)&(d|e)) | ((d&e)&(b|c))
    return s,carry
