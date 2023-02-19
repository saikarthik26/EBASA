#include <iostream>
#include <stdio.h>
#include <ap_int.h>

#define nrow_A 262
#define ncol_A 262
#define sizeof_sa 7
#define kernel_size 7

using namespace std;

// EXACT COMPRESSORS -----------------------------------

ap_uint<2> FA(ap_uint<1>a, ap_uint<1>b, ap_uint<1>c)
{
	ap_uint<1> sum;
	ap_uint<1> carry;
	sum = a^b^c;
	carry = (a&b) | (b&c) | (a&c);
	return sum,carry;
}

ap_uint<2> HA(ap_uint<1>a, ap_uint<1>b)
{
	ap_uint<1> sum;
	ap_uint<1> carry;
	sum = a^b;
	carry = (a&b);
	return sum,carry;
}

ap_uint<3> Exact_Compressor_4_2_NEW(ap_uint<1>a,
		ap_uint<1>b,
		ap_uint<1>c,
		ap_uint<1>d,
		ap_uint<1>cin)
{
	ap_uint<1> sum, carry, cout, w1;
	(w1, cout) = FA(a,b,c);
	(sum, carry) = FA(d, w1, cin);
	return sum, carry, cout;
}

// APPROXIMATE COMPRESSORS -----------------------------------

ap_uint<2> Positive_Compressor_3_2(ap_uint<1> a,
		ap_uint<1> b,
		ap_uint<1> c)
{
	ap_uint<1> sum, carry, w1;
	sum = a|b|c;
	w1 = a|b;
	carry = w1&c;
	return sum, carry;
}

ap_uint<2> Positive_Compressor_4_2(ap_uint<1> a,
		ap_uint<1> b,
		ap_uint<1> c,
		ap_uint<1> d)
{
	ap_uint<1> sum, carry, w1, w2, t1, t2;
	sum = a|b|c|d;
	w1 = a|c;
	w2 = b|c;
	t1 = w1&b;
	t2 = w2&d;
	carry = t1|t2;
	return sum, carry;
}

ap_uint<2> Positive_Compressor_5_2new(ap_uint<1> a,
		ap_uint<1> b,
		ap_uint<1> c,
		ap_uint<1> d,
		ap_uint<1> e)
{
	ap_uint<1> sum, carry, w1, w2, w3, t1, t2, t3;
	sum = a|b|c|d|e;
	w1 = b|c|d|e;
	t1 = a&w1;
	w2 = d|e;
	w3 = b|c;
	t2 = w2&w3;
	t3 = d&e;
	carry = t1|t2|t3;
	return sum, carry;
}

ap_uint<2> Negative_Compressor_3_2(ap_uint<1> a,
		ap_uint<1> b,
		ap_uint<1> c)
{
	ap_uint<1> sum, carry;
	sum = a|b|c;
	carry = b&c;
	return sum, carry;
}

ap_uint<2> Negative_Compressor_4_2(ap_uint<1> a,
		ap_uint<1> b,
		ap_uint<1> c,
		ap_uint<1> d)
{
	ap_uint<1> sum, carry, t1, t2;
	sum = a|b|c|d;
	t1 = a&b;
	t2 = c&d;
	carry = t1|t2;
	return sum, carry;
}

ap_uint<2> Negative_Compressor_5_2new(ap_uint<1> a,
		ap_uint<1> b,
		ap_uint<1> c,
		ap_uint<1> d,
		ap_uint<1> e)
{
	ap_uint<1> sum, carry, w1, w2, w3, t1, t2, t3;
	sum = a|b|c|d|e;
	w1 = b|c|d|e;
	t1 = a&w1;
	w2 = d|e;
	t2 = b&c&w2;
	w3 = b|c;
	t3 = d&e&w3;
	carry = t1|t2|t3;
	return sum, carry;
}

// FAST ADDERS -----------------------------------

ap_uint<17> CLA16(ap_uint<16>a, ap_uint<16>b )
{
	ap_uint<17> sum_out ;
	ap_uint<17> w_c;
	ap_uint<16> w_g, w_p, w_sum;
	int i;

	w_c[0] = 0;

	for(i=0; i<16; i++)
	{
		w_g[i] = a[i] & b[i];
		w_p[i] = a[i] | b[i];
		w_c[i+1] = w_g[i] | (w_p[i] & w_c[i]);
	}

	for(i=0; i<16; i++)
	{
		sum_out[i] = a[i] ^ b[i] ^ w_c[i];
	}

	sum_out[16] = w_c[16];

	return sum_out;

}

// GENERATE PARTIAL PRODUCTS -----------------------------------

void generate_partial_products_8(ap_uint<8> x,ap_uint<8> y, ap_uint<1> p[8][8])
{
	int i,j;
	for (i=0;i<8;i++)
	{
		for(j=0;j<8;j++)
		{
			p[i][j] = x[j]&y[i];
		}
	}
}

// PROCESSING BLOCKS -----------------------------------

ap_uint<30> EXACTCOMP_processing_blk(	ap_uint<1> pp[8][8])
{
	ap_uint<15> out1;
	ap_uint<15> out2;
	ap_uint<1> r1[15][4], r2[15][2];
	ap_uint<1> c0,c1,c2,c3,c4,c5,c6,c7_1,c7_2,c8_1,c8_2,c9,c10,c11;
	ap_uint<1> w0,w1,w2,w3,w4,w5,w6,w7,w8,w9,w10,w11,w12,w13;

	for(int i=0;i<15;i++)
	{
		for(int j=0;j<4; j++)
		{
			r1[i][j] = 0;
		}
		out1[i] = 0;
		out2[i] = 0;
	}

	// PP REDUCTION STEP ONE

	// column-0
	r1[0][0] = pp[0][0];

	// column-1
	r1[1][0] = pp[1][0];
	r1[1][1] = pp[0][1];

	// column-2
	r1[2][0] = pp[2][0];
	r1[2][1] = pp[1][1];
	r1[2][2] = pp[0][2];

	// column-3
	r1[3][0] = pp[3][0];
	r1[3][1] = pp[2][1];
	r1[3][2] = pp[1][2];
	r1[3][3] = pp[0][3];

	// column-4
	(r1[4][0], r1[5][0]) = HA(pp[4][0], pp[3][1]);
	r1[4][1] = pp[2][2];
	r1[4][2] = pp[1][3];
	r1[4][3] = pp[0][4];

	// column-5
	(r1[5][1], r1[6][0], c5) = Exact_Compressor_4_2_NEW(pp[5][0], pp[4][1], pp[3][2], pp[2][3], 0);
	r1[5][2] = pp[1][4];
	r1[5][3] = pp[0][5];

	// column-6
	(r1[6][1], r1[7][0], c6) = Exact_Compressor_4_2_NEW(pp[6][0], pp[5][1], pp[4][2], pp[3][3], c5);
	(r1[6][2], r1[7][1]) = HA(pp[2][4], pp[1][5]);
	r1[6][3] = pp[0][6];

	// column-7
	(r1[7][2], r1[8][0], c7_1) = Exact_Compressor_4_2_NEW(pp[7][0], pp[6][1], pp[5][2], pp[4][3], c6);
	(r1[7][3], r1[8][1], c7_2) = Exact_Compressor_4_2_NEW(pp[3][4], pp[2][5], pp[1][6], pp[0][7], 0);

	// column-8
	(r1[8][2], r1[9][0], c8_1) = Exact_Compressor_4_2_NEW(pp[7][1], pp[6][2], pp[5][3], pp[4][4], c7_1);
	(r1[8][3], r1[9][1], c8_2) = Exact_Compressor_4_2_NEW(pp[3][5], pp[2][6], pp[1][7], 0, c7_2);

	// column-9
	(r1[9][2], r1[10][0], c9) = Exact_Compressor_4_2_NEW(pp[7][2], pp[6][3], pp[5][4], pp[4][5], c8_1);
	(r1[9][3], r1[10][1]) = FA(pp[3][6], pp[2][7], c8_2);

	// column-10
	(r1[10][2], r1[11][0], c10) = Exact_Compressor_4_2_NEW(pp[7][3], pp[6][4], pp[5][5], pp[4][6], c9);
	r1[10][3] = pp[3][7];

	// column-11
	(r1[11][1], r1[12][0]) = FA(pp[7][4], pp[6][5], c10);
	r1[11][2] = pp[5][6];
	r1[11][3] = pp[4][7];

	// column-12
	r1[12][1] = pp[7][5];
	r1[12][2] = pp[6][6];
	r1[12][3] = pp[5][7];

	// column-13
	r1[13][0] = pp[7][6];
	r1[13][1] = pp[6][7];

	//column-14
	r1[14][0] = pp[7][7];

	// PP REDUCTION STEP TWO

	// column-0
	r2[0][0] = r1[0][0];

	// column-1
	r2[1][0] = r1[1][0];
	r2[1][1] = r1[1][1];

	// column-2
	(r2[2][0], r2[3][0]) = HA(r1[2][0], r1[2][1]);
	r2[2][1] = r1[2][2];

	// column-3
	(r2[3][1], r2[4][0], w3) = Exact_Compressor_4_2_NEW(r1[3][0], r1[3][1], r1[3][2], r1[3][3], 0);

	// column-4
	(r2[4][1], r2[5][0], w4) = Exact_Compressor_4_2_NEW(r1[4][0], r1[4][1], r1[4][2], r1[4][3], w3);

	// column-5
	(r2[5][1], r2[6][0], w5) = Exact_Compressor_4_2_NEW(r1[5][0], r1[5][1], r1[5][2], r1[5][3], w4);

	// column-6
	(r2[6][1], r2[7][0], w6) = Exact_Compressor_4_2_NEW(r1[6][0], r1[6][1], r1[6][2], r1[6][3], w5);

	// column-7
	(r2[7][1], r2[8][0], w7) = Exact_Compressor_4_2_NEW(r1[7][0], r1[7][1], r1[7][2], r1[7][3], w6);

	// column-8
	w0 = 0;
	(r2[8][1], r2[9][0], w8) = Exact_Compressor_4_2_NEW(r1[8][0], r1[8][1], r1[8][2], r1[8][3], w7);

	// column-9
	(r2[9][1], r2[10][0], w9) = Exact_Compressor_4_2_NEW(r1[9][0], r1[9][1], r1[9][2], r1[9][3], w8);

	// column-10
	(r2[10][1], r2[11][0], w10) = Exact_Compressor_4_2_NEW(r1[10][0], r1[10][1], r1[10][2], r1[10][3], w9);

	// column-11
	(r2[11][1], r2[12][0], w11) = Exact_Compressor_4_2_NEW(r1[11][0], r1[11][1], r1[11][2], r1[11][3], w10);

	// column-12
	(r2[12][1], r2[13][0], w12) = Exact_Compressor_4_2_NEW(r1[12][0], r1[12][1], r1[12][2], r1[12][3], w11);

	// column-13
	(r2[13][1], r2[14][0]) = FA(r1[13][0], r1[13][1], w12);

	// column-14
	r2[14][1] = r1[14][0];

	// OUTPUTS OF PROCESSING BLOCKS
	out1[0] = r2[0][0];
	out1[1] = r2[1][0];
	out1[2] = r2[2][0];
	out1[3] = r2[3][0];
	out1[4] = r2[4][0];
	out1[5] = r2[5][0];
	out1[6] = r2[6][0];
	out1[7] = r2[7][0];
	out1[8] = r2[8][0];
	out1[9] = r2[9][0];
	out1[10] = r2[10][0];
	out1[11] = r2[11][0];
	out1[12] = r2[12][0];
	out1[13] = r2[13][0];
	out1[14] = r2[14][0];

	out2[0] = 0;  //r2[0][1];
	out2[1] = r2[1][1];
	out2[2] = r2[2][1];
	out2[3] = r2[3][1];
	out2[4] = r2[4][1];
	out2[5] = r2[5][1];
	out2[6] = r2[6][1];
	out2[7] = r2[7][1];
	out2[8] = r2[8][1];
	out2[9] = r2[9][1];
	out2[10] = r2[10][1];
	out2[11] = r2[11][1];
	out2[12] = r2[12][1];
	out2[13] = r2[13][1];
	out2[14] = r2[14][1];

	return out1, out2;
}

ap_uint<30> PM_PC_PC_processing_blk_8x8(	ap_uint<1> p[8][8])
{
	ap_uint<15> out1;
	ap_uint<15> out2;
	ap_uint<1> r1[15][5];
	ap_uint<1> r2[15][2];
	ap_uint<1> cin0, cin1, cin2, cin3;
	ap_uint<1> w0,w1,w2,w3,w4,w5;

	for(int i=0;i<15;i++)
	{
		for(int j=0;j<5; j++)
		{
			if (j<2)
			{
				r2[i][j]=0;
			}
			r1[i][j] = 0;
		}
		out1[i] = 0;
		out2[i] = 0;
	}

	// PR REDUCTION STEP ONE

	// column 0
    r1[0][0] = p[0][0];

    // column 1
    (r1[1][0], r1[2][0]) = HA(p[1][0], p[0][1]);

    // column 2
    (r1[2][1], r1[3][0]) = Positive_Compressor_3_2(p[2][0], p[1][1], p[0][2]);

    // column 3
    (r1[3][1], r1[4][0]) = Positive_Compressor_4_2(p[3][0], p[2][1], p[1][2], p[0][3]);

    // column 4
    (r1[4][1], r1[5][0]) = Positive_Compressor_5_2new(p[4][0], p[3][1], p[2][2], p[1][3], p[0][4]);

    // column 5
    (r1[5][1], r1[6][0]) = Positive_Compressor_5_2new(p[5][0], p[4][1], p[3][2], p[2][3], p[1][4]);
    r1[5][2] = p[0][5];

    // column 6
    (r1[6][1], r1[7][0]) = Positive_Compressor_5_2new(p[6][0], p[5][1], p[4][2], p[3][3], p[2][4]);
    r1[6][2] = p[1][5];
    r1[6][3] = p[0][6];

    // column 7
    (r1[7][1], r1[8][0]) = Positive_Compressor_5_2new(p[7][0], p[6][1], p[5][2], p[4][3], p[3][4]);
    (r1[7][2], r1[8][1]) = HA(p[2][5], p[1][6]);
    r1[7][3] = p[0][7];

    // column 8
    cin0=0;

    (r1[8][2], r1[9][0], cin1) = Exact_Compressor_4_2_NEW(p[7][1], p[6][2], p[5][3], p[4][4], cin0);
    (r1[8][3], r1[9][1]) = FA(p[3][5], p[2][6], p[1][7]);

    // column 9
    (r1[9][2], r1[10][0], cin2) = Exact_Compressor_4_2_NEW(p[7][2], p[6][3], p[5][4], p[4][5], cin1);
    (r1[9][3], r1[10][1]) = HA(p[3][6], p[2][7]);

    // column 10
    (r1[10][2], r1[11][0], cin3) = Exact_Compressor_4_2_NEW(p[7][3], p[6][4], p[5][5], p[4][6], cin2);
    r1[10][3] = p[3][7];

	// column 11
    (r1[11][1], r1[12][0]) = FA(p[7][4], p[6][5], cin3);
    r1[11][2] = p[5][6];
    r1[11][3] = p[4][7];

    // column 12
    r1[12][1] = p[7][5];
    r1[12][2] = p[6][6];
    r1[12][3] = p[5][7];

    // column 13
    r1[13][0] = p[7][6];
    r1[13][1] = p[6][7];

    // column 14
    r1[14][0] = p[7][7];

    // PR REDUCTION STEP TWO

	// column 0
    r2[0][0] = r1[0][0];

    // column 1
    r2[1][0] = r1[1][0];

    // column 2
    r2[2][0] = r1[2][0];
    r2[2][1] = r1[2][1];

    // column 3
    r2[3][0] = r1[3][0];
    r2[3][1] = r1[3][1];

    // column 4
    r2[4][0] = r1[4][0];
    r2[4][1] = r1[4][1];

    // column 5
    (r2[5][0], r2[6][0]) = HA(r1[5][0], r1[5][1]);
    r2[5][1] = r1[5][2];

    // column 6
    (r2[6][1], r2[7][0]) = Positive_Compressor_4_2(r1[6][0], r1[6][1], r1[6][2], r1[6][3]);

    // column 7
    (r2[7][1], r2[8][0]) = Positive_Compressor_4_2(r1[7][0], r1[7][1], r1[7][2], r1[7][3]);

    // column 8
    w0 = 0;
    (r2[8][1], r2[9][0], w1) = Exact_Compressor_4_2_NEW(r1[8][0], r1[8][1], r1[8][2], r1[8][3], w0);

	// column 9
    (r2[9][1], r2[10][0], w2) = Exact_Compressor_4_2_NEW(r1[9][0], r1[9][1], r1[9][2], r1[9][3], w1);

	// column 10
    (r2[10][1], r2[11][0], w3) = Exact_Compressor_4_2_NEW(r1[10][0], r1[10][1], r1[10][2], r1[10][3], w2);

	// column 11
    (r2[11][1], r2[12][0], w4) = Exact_Compressor_4_2_NEW(r1[11][0], r1[11][1], r1[11][2], r1[11][3], w3);

	// column 12
    (r2[12][1], r2[13][0], w5) = Exact_Compressor_4_2_NEW(r1[12][0], r1[12][1], r1[12][2], r1[12][3], w4);

	// column 13
    (r2[13][1], r2[14][0]) = FA(r1[13][0], r1[13][1], w5);

	//column 14
    r2[14][1] = r1[14][0];

   // Outputs of processing block
   out1[0] = r2[0][0];
   out1[1] = r2[1][0];
   out1[2] = r2[2][0];
   out1[3] = r2[3][0];
   out1[4] = r2[4][0];
   out1[5] = r2[5][0];
   out1[6] = r2[6][0];
   out1[7] = r2[7][0];
   out1[8] = r2[8][0];
   out1[9] = r2[9][0];
   out1[10] = r2[10][0];
   out1[11] = r2[11][0];
   out1[12] = r2[12][0];
   out1[13] = r2[13][0];
   out1[14] = r2[14][0];

   out2[0] = 0; // r2[0][1]
   out2[1] = 0; // r2[1][1]
   out2[2] = r2[2][1];
   out2[3] = r2[3][1];
   out2[4] = r2[4][1];
   out2[5] = r2[5][1];
   out2[6] = r2[6][1];
   out2[7] = r2[7][1];
   out2[8] = r2[8][1];
   out2[9] = r2[9][1];
   out2[10] = r2[10][1];
   out2[11] = r2[11][1];
   out2[12] = r2[12][1];
   out2[13] = r2[13][1];
   out2[14] = r2[14][1];

   return out1, out2;
}

ap_uint<30> NM_NC_NC_processing_blk_8x8(	ap_uint<1> p[8][8])
{
	ap_uint<15> out1;
	ap_uint<15> out2;
	ap_uint<1> r1[15][5];
	ap_uint<1> r2[15][2];
	ap_uint<1> cin0, cin1, cin2, cin3;
	ap_uint<1> w0,w1,w2,w3,w4,w5;

	for(int i=0;i<15;i++)
	{
		for(int j=0;j<5; j++)
		{
			if (j<2)
			{
				r2[i][j]=0;
			}
			r1[i][j] = 0;
		}
		out1[i] = 0;
		out2[i] = 0;
	}

	// PR REDUCTION STEP ONE

	// column 0
    r1[0][0] = p[0][0];

    // column 1
    (r1[1][0], r1[2][0]) = HA(p[1][0], p[0][1]);

    // column 2
    (r1[2][1], r1[3][0]) = Negative_Compressor_3_2(p[2][0], p[1][1], p[0][2]);

    // column 3
    (r1[3][1], r1[4][0]) = Negative_Compressor_4_2(p[3][0], p[2][1], p[1][2], p[0][3]);

    // column 4
    (r1[4][1], r1[5][0]) = Negative_Compressor_5_2new(p[4][0], p[3][1], p[2][2], p[1][3], p[0][4]);

    // column 5
    (r1[5][1], r1[6][0]) = Negative_Compressor_5_2new(p[5][0], p[4][1], p[3][2], p[2][3], p[1][4]);
    r1[5][2] = p[0][5];

    // column 6
    (r1[6][1], r1[7][0]) = Negative_Compressor_5_2new(p[6][0], p[5][1], p[4][2], p[3][3], p[2][4]);
    r1[6][2] = p[1][5];
    r1[6][3] = p[0][6];

    // column 7
    (r1[7][1], r1[8][0]) = Negative_Compressor_5_2new(p[7][0], p[6][1], p[5][2], p[4][3], p[3][4]);
    (r1[7][2], r1[8][1]) = HA(p[2][5], p[1][6]);
    r1[7][3] = p[0][7];

    // column 8
    cin0=0;

    (r1[8][2], r1[9][0], cin1) = Exact_Compressor_4_2_NEW(p[7][1], p[6][2], p[5][3], p[4][4], cin0);
    (r1[8][3], r1[9][1]) = FA(p[3][5], p[2][6], p[1][7]);

    // column 9
    (r1[9][2], r1[10][0], cin2) = Exact_Compressor_4_2_NEW(p[7][2], p[6][3], p[5][4], p[4][5], cin1);
    (r1[9][3], r1[10][1]) = HA(p[3][6], p[2][7]);

    // column 10
    (r1[10][2], r1[11][0], cin3) = Exact_Compressor_4_2_NEW(p[7][3], p[6][4], p[5][5], p[4][6], cin2);
    r1[10][3] = p[3][7];

	// column 11
    (r1[11][1], r1[12][0]) = FA(p[7][4], p[6][5], cin3);
    r1[11][2] = p[5][6];
    r1[11][3] = p[4][7];

    // column 12
    r1[12][1] = p[7][5];
    r1[12][2] = p[6][6];
    r1[12][3] = p[5][7];

    // column 13
    r1[13][0] = p[7][6];
    r1[13][1] = p[6][7];

    // column 14
    r1[14][0] = p[7][7];

    // PR REDUCTION STEP TWO

	// column 0
    r2[0][0] = r1[0][0];

    // column 1
    r2[1][0] = r1[1][0];

    // column 2
    r2[2][0] = r1[2][0];
    r2[2][1] = r1[2][1];

    // column 3
    r2[3][0] = r1[3][0];
    r2[3][1] = r1[3][1];

    // column 4
    r2[4][0] = r1[4][0];
    r2[4][1] = r1[4][1];

    // column 5
    (r2[5][0], r2[6][0]) = HA(r1[5][0], r1[5][1]);
    r2[5][1] = r1[5][2];

    // column 6
    (r2[6][1], r2[7][0]) = Negative_Compressor_4_2(r1[6][0], r1[6][1], r1[6][2], r1[6][3]);

    // column 7
    (r2[7][1], r2[8][0]) = Negative_Compressor_4_2(r1[7][0], r1[7][1], r1[7][2], r1[7][3]);

    // column 8
    w0 = 0;
    (r2[8][1], r2[9][0], w1) = Exact_Compressor_4_2_NEW(r1[8][0], r1[8][1], r1[8][2], r1[8][3], w0);

	// column 9
    (r2[9][1], r2[10][0], w2) = Exact_Compressor_4_2_NEW(r1[9][0], r1[9][1], r1[9][2], r1[9][3], w1);

	// column 10
    (r2[10][1], r2[11][0], w3) = Exact_Compressor_4_2_NEW(r1[10][0], r1[10][1], r1[10][2], r1[10][3], w2);

	// column 11
    (r2[11][1], r2[12][0], w4) = Exact_Compressor_4_2_NEW(r1[11][0], r1[11][1], r1[11][2], r1[11][3], w3);

	// column 12
    (r2[12][1], r2[13][0], w5) = Exact_Compressor_4_2_NEW(r1[12][0], r1[12][1], r1[12][2], r1[12][3], w4);

	// column 13
    (r2[13][1], r2[14][0]) = FA(r1[13][0], r1[13][1], w5);

	//column 14
    r2[14][1] = r1[14][0];

   // Outputs of processing block
   out1[0] = r2[0][0];
   out1[1] = r2[1][0];
   out1[2] = r2[2][0];
   out1[3] = r2[3][0];
   out1[4] = r2[4][0];
   out1[5] = r2[5][0];
   out1[6] = r2[6][0];
   out1[7] = r2[7][0];
   out1[8] = r2[8][0];
   out1[9] = r2[9][0];
   out1[10] = r2[10][0];
   out1[11] = r2[11][0];
   out1[12] = r2[12][0];
   out1[13] = r2[13][0];
   out1[14] = r2[14][0];

   out2[0] = 0; // r2[0][1]
   out2[1] = 0; // r2[1][1]
   out2[2] = r2[2][1];
   out2[3] = r2[3][1];
   out2[4] = r2[4][1];
   out2[5] = r2[5][1];
   out2[6] = r2[6][1];
   out2[7] = r2[7][1];
   out2[8] = r2[8][1];
   out2[9] = r2[9][1];
   out2[10] = r2[10][1];
   out2[11] = r2[11][1];
   out2[12] = r2[12][1];
   out2[13] = r2[13][1];
   out2[14] = r2[14][1];

   return out1, out2;
}

// EXACT PM NM TOP -----------------------------------

ap_uint<17> Exact8_Top(ap_uint<8> x, ap_uint<8> y)
{
	ap_uint<17> prod;
	ap_uint<1> pp[8][8];
	ap_uint<16> ppr_out1, ppr_out2;
	ap_uint<30> ppr_out3;
	int a=0,i;

	ppr_out1[15] = 0;
	ppr_out2[15] = 0;

	generate_partial_products_8(x, y, pp);
	ppr_out3 = EXACTCOMP_processing_blk(pp);

	for(i=0; i<15; i++)
	{
		ppr_out1[i] = ppr_out3[i];
	}

	for(i=15; i<30; i++)
	{
		ppr_out2[a] = ppr_out3[i];
		a = a+1;
	}

	prod = CLA16(ppr_out1, ppr_out2);

	return prod;
}


ap_uint<17> PM8_top_NoIntrlv(ap_uint<8> x, ap_uint<8> y)
{
	ap_uint<1> p[8][8];
	ap_uint<16> ppr_out1, ppr_out2;

	ppr_out1[15] = 0;
	ppr_out2[15] = 0;

	ap_uint<30> ppr_out3;
	ap_uint<17> prod;
	int a=0,i;

	generate_partial_products_8(x, y, p);
	ppr_out3 = PM_PC_PC_processing_blk_8x8(p);

	for(i=0; i<15; i++)
	{
		ppr_out1[i] = ppr_out3[i];
	}

	for(i=15; i<30; i++)
	{
		ppr_out2[a] = ppr_out3[i];
		a = a+1;
	}

	prod = CLA16(ppr_out1, ppr_out2);

	return prod;

}

ap_uint<17> NM8_top_NoIntrlv(ap_uint<8> x, ap_uint<8> y)
{
	ap_uint<1> p[8][8];
	ap_uint<16> ppr_out1, ppr_out2;

	ppr_out1[15] = 0;
	ppr_out2[15] = 0;

	ap_uint<30> ppr_out3;
	ap_uint<17> prod;
	int a=0,i;

	generate_partial_products_8(x, y, p);
	ppr_out3 = NM_NC_NC_processing_blk_8x8(p);

	for(i=0; i<15; i++)
	{
		ppr_out1[i] = ppr_out3[i];
	}

	for(i=15; i<30; i++)
	{
		ppr_out2[a] = ppr_out3[i];
		a = a+1;
	}

	prod = CLA16(ppr_out1, ppr_out2);

	return prod;

}

// SYSTOLIC ARRAY -----------------------------------


ap_uint<17> systolic_array(
		ap_uint<8> A[sizeof_sa][sizeof_sa],
		ap_uint<8> B[sizeof_sa][sizeof_sa])
{

	short x = 0, i, j, cycles, m, n;
	short a,b;

	ap_uint<17> C[sizeof_sa][sizeof_sa];
	ap_uint<17> conv_pixel=0;

	ap_uint<8> store_A[sizeof_sa][sizeof_sa+(sizeof_sa-1)];
	ap_uint<8> store_B[sizeof_sa+(sizeof_sa-1)][sizeof_sa];

	ap_uint<8> buf_A[sizeof_sa][sizeof_sa];
	ap_uint<8> buf_B[sizeof_sa][sizeof_sa];

	ap_uint<17> apprx_mult_out;

	for(i=0;i<sizeof_sa;i++)
	{
		for(j=0; j<(sizeof_sa+(sizeof_sa-1));j++)
			{
				store_A[i][j] = 0;
			}
	}

	for(i=0;i<(sizeof_sa+(sizeof_sa-1));i++)
	{
		for(j=0; j<sizeof_sa;j++)
		{
			store_B[i][j] = 0;
		}
	}

	for(i=0;i<sizeof_sa;i++)
	{
		// pragma HLS PIPELINE
		for(j=0; j<sizeof_sa;j++)
		{
			buf_A[i][j] = 0;
			buf_B[i][j]=0;
			C[i][j] = 0;
		}
	}

	// FOR STORING IN APPROPORIATE FORMAT
	for(i=0; i<sizeof_sa; i++)
	{
		for(j=0; j<sizeof_sa; j++)
		{
			store_A[i][j-i+sizeof_sa-1] = A[i][j];
		}
	}

	for(i=0; i<sizeof_sa; i++)
	{
		for(j=0; j<sizeof_sa; j++)
		{
			store_B[i-j+sizeof_sa-1][j] = B[i][j];
		}
	}

	for(cycles=0; cycles<((sizeof_sa+sizeof_sa-1)+(sizeof_sa-1)); cycles++)
	{
		// SHIFTING INPUTS
		for(i=sizeof_sa-2; i>=0; i--)
		{
			for(j=0; j<sizeof_sa; j++)
			{
				buf_A[j][i+1] = buf_A[j][i];
				buf_B[i+1][j] = buf_B[i][j];
			}
		}

		// STORING IN 1st ROW AND COLUMN
		if(x<(sizeof_sa+sizeof_sa-1))
		{
			for(i=0; i<sizeof_sa; i++)
			{
				buf_A[i][0] = store_A[i][(sizeof_sa+sizeof_sa-2)-x];
				buf_B[0][i] = store_B[(sizeof_sa+sizeof_sa-2)-x][i];
			}
			x = x+1;
		}


		// COMPUTING OUTPUT MATRIX

		
		for(i=0;i<sizeof_sa;i++)
		{
			for(j=0; j<sizeof_sa;j++)
			{

				// EXACT WITH COMPRESSORS
				apprx_mult_out = Exact8_Top(buf_A[i][j], buf_B[i][j]);

				/*
				// ALL APEM
				// apprx_mult_out = PM8_top_NoIntrlv(buf_A[i][j], buf_B[i][j]);

				// ALL ANEM
				// apprx_mult_out = NM8_top_NoIntrlv(buf_A[i][j], buf_B[i][j]);

				// Alternate APEM and ANEM
				if ((i%2==0 && j%2==0) || (i%2!=0 && j%2!=0))
				{
					apprx_mult_out = PM8_top_NoIntrlv(buf_A[i][j], buf_B[i][j]);
				}
				else
				{
					apprx_mult_out = NM8_top_NoIntrlv(buf_A[i][j], buf_B[i][j]);
				}

					
				// ALTERNATE PN RINGS
				if (i==1 || i==5)
				{
					if (j==1 || j==2 || j==3 || j==4 || j==5)
					{
						apprx_mult_out = PM8_top_NoIntrlv(buf_A[i][j], buf_B[i][j]);
					}
				}

				else if (j==1 || j==5)
				{
					if (i==2 || i==3 || i==4)
					{
						apprx_mult_out = PM8_top_NoIntrlv(buf_A[i][j], buf_B[i][j]);
					}
				}

				else if (i==3 && j==3)
				{
					apprx_mult_out = PM8_top_NoIntrlv(buf_A[i][j], buf_B[i][j]);
				}

				else
				{
					apprx_mult_out = NM8_top_NoIntrlv(buf_A[i][j], buf_B[i][j]);
				}


				// ALTERNATE ROWS APEM and ANEM
				if (i%2==0)
				{
					apprx_mult_out = PM8_top_NoIntrlv(buf_A[i][j], buf_B[i][j]);
				}
				else
				{
					apprx_mult_out = NM8_top_NoIntrlv(buf_A[i][j], buf_B[i][j]);
				}

				
				// ALTERNATE COLUMNS APEM and ANEM
				if (j%2==0)
				{
					apprx_mult_out = PM8_top_NoIntrlv(buf_A[i][j], buf_B[i][j]);
				}
				else
				{
					apprx_mult_out = NM8_top_NoIntrlv(buf_A[i][j], buf_B[i][j]);
				}
				*/

				C[i][j] = C[i][j] + apprx_mult_out;
			}
		}
		

		for(i=0;i<sizeof_sa;i++)
		{
			for(j=0; j<sizeof_sa;j++)
			{
				apprx_mult_out = NM8_top_NoIntrlv(buf_A[i][j], buf_B[i][j]);
				C[i][j] = C[i][j] + apprx_mult_out;
			}
		}

		
	} // END OF FOR CYCLES LOOP

	for(i=0;i<sizeof_sa;i++)
	{
		for(j=0; j<sizeof_sa;j++)
		{
			if (i==j)
			{
				conv_pixel = conv_pixel + C[i][j];
			}
		}
	}

	return conv_pixel;

}

// CONVOLUTION -----------------------------------

void convolution_using_sa (
			ap_uint<8> A[nrow_A][ncol_A],
			ap_uint<8> B[kernel_size][kernel_size],
			ap_uint<17> C[(nrow_A-kernel_size+1)][(ncol_A-kernel_size+1)])

{
	short i,j,m,n;

	ap_uint<8> temp_A[sizeof_sa][sizeof_sa];

	for(i=0; i<(nrow_A-kernel_size+1); i=i+1)
	{
		for(j=0; j<(ncol_A-kernel_size+1); j=j+1)
		{
			for(m=0; m<kernel_size; m=m+1)
			{
				for(n=0; n<kernel_size; n=n+1)
				{
					temp_A[m][n]=A[m+i][n+j];
				}
			}

			C[i][j] = systolic_array(temp_A, B);

		} // END OF j LOOP
	} // END OF i LOOP

}
