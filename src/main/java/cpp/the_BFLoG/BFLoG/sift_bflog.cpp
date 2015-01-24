/** @file sift.cpp
 ** @brief SIFT - Definition
 ** @author Jie Chen @ Peking University
 **/

#include "sift_bflog.h"
#include "time.h"
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>

#define EXPN_SZ  256          /**< ::fast_expn table size @internal */
#define EXPN_MAX 25.0         /**< ::fast_expn table max  @internal */

static const double expn_tab [EXPN_SZ+1]  /**< ::fast_expn table      @internal */
= {
		1, 0.906961, 0.822578, 0.746045, 0.676634, 0.61368, 0.556584, 0.5048, 0.457833, 0.415237,  // 10
		0.376603, 0.341564, 0.309786, 0.280963, 0.254823, 0.231114, 0.209611, 0.190109, 0.172422, 0.15638,  // 20
		0.14183, 0.128634, 0.116666, 0.105812, 0.0959671, 0.0870384, 0.0789404, 0.0715958, 0.0649346, 0.0588931,  // 30
		0.0534137, 0.0484441, 0.0439369, 0.0398491, 0.0361415, 0.0327789, 0.0297292, 0.0269632, 0.0244546, 0.0221793,  // 40
		0.0201158, 0.0182442, 0.0165468, 0.0150073, 0.013611, 0.0123447, 0.0111961, 0.0101544, 0.00920968, 0.00835282,  // 50
		0.00757568, 0.00687084, 0.00623158, 0.0056518, 0.00512596, 0.00464904, 0.0042165, 0.0038242, 0.0034684, 0.0031457,  // 60
		0.00285303, 0.00258758, 0.00234684, 0.00212849, 0.00193045, 0.00175085, 0.00158795, 0.00144021, 0.00130621, 0.00118468,  // 70
		0.00107446, 0.000974492, 0.000883826, 0.000801596, 0.000727016, 0.000659375, 0.000598027, 0.000542387, 0.000491923, 0.000446155,  // 80
		0.000404645, 0.000366997, 0.000332852, 0.000301884, 0.000273797, 0.000248323, 0.000225219, 0.000204265, 0.00018526, 0.000168024,  // 90
		0.000152391, 0.000138212, 0.000125353, 0.00011369, 0.000103113, 9.35192e-05, 8.48182e-05, 7.69268e-05, 6.97696e-05, 6.32783e-05,  // 100
		5.73909e-05, 5.20513e-05, 4.72085e-05, 4.28162e-05, 3.88326e-05, 3.52197e-05, 3.19428e-05, 2.89709e-05, 2.62755e-05, 2.38308e-05,  // 110
		2.16136e-05, 1.96027e-05, 1.77789e-05, 1.61247e-05, 1.46245e-05, 1.32638e-05, 1.20298e-05, 1.09105e-05, 9.89543e-06, 8.97477e-06,  // 120
		8.13976e-06, 7.38244e-06, 6.69558e-06, 6.07263e-06, 5.50764e-06, 4.99521e-06, 4.53046e-06, 4.10895e-06, 3.72665e-06, 3.37993e-06,  // 130
		3.06546e-06, 2.78025e-06, 2.52158e-06, 2.28697e-06, 2.07419e-06, 1.88121e-06, 1.70619e-06, 1.54744e-06, 1.40347e-06, 1.27289e-06,  // 140
		1.15446e-06, 1.04705e-06, 9.49636e-07, 8.61282e-07, 7.81149e-07, 7.08471e-07, 6.42556e-07, 5.82773e-07, 5.28552e-07, 4.79376e-07,  // 150
		4.34775e-07, 3.94324e-07, 3.57636e-07, 3.24362e-07, 2.94183e-07, 2.66813e-07, 2.41989e-07, 2.19474e-07, 1.99054e-07, 1.80535e-07,  // 160
		1.63738e-07, 1.48504e-07, 1.34687e-07, 1.22156e-07, 1.1079e-07, 1.00483e-07, 9.11338e-08, 8.26547e-08, 7.49646e-08, 6.79899e-08,  // 170
		6.16642e-08, 5.5927e-08, 5.07236e-08, 4.60043e-08, 4.17241e-08, 3.78421e-08, 3.43213e-08, 3.11281e-08, 2.82319e-08, 2.56052e-08,  // 180
		2.32229e-08, 2.10623e-08, 1.91027e-08, 1.73254e-08, 1.57134e-08, 1.42515e-08, 1.29255e-08, 1.17229e-08, 1.06322e-08, 9.64302e-09,  // 190
		8.74584e-09, 7.93213e-09, 7.19413e-09, 6.5248e-09, 5.91773e-09, 5.36715e-09, 4.86779e-09, 4.4149e-09, 4.00414e-09, 3.6316e-09,  // 200
		3.29371e-09, 2.98727e-09, 2.70934e-09, 2.45726e-09, 2.22864e-09, 2.02129e-09, 1.83323e-09, 1.66267e-09, 1.50797e-09, 1.36767e-09,  // 210
		1.24042e-09, 1.12502e-09, 1.02035e-09, 9.25413e-10, 8.39313e-10, 7.61224e-10, 6.904e-10, 6.26166e-10, 5.67908e-10, 5.1507e-10,  // 220
		4.67148e-10, 4.23685e-10, 3.84265e-10, 3.48514e-10, 3.16088e-10, 2.86679e-10, 2.60007e-10, 2.35816e-10, 2.13876e-10, 1.93977e-10,  // 230
		1.7593e-10, 1.59561e-10, 1.44716e-10, 1.31251e-10, 1.1904e-10, 1.07964e-10, 9.79195e-11, 8.88092e-11, 8.05464e-11, 7.30524e-11,  // 240
		6.62557e-11, 6.00913e-11, 5.45004e-11, 4.94297e-11, 4.48308e-11, 4.06598e-11, 3.68768e-11, 3.34458e-11, 3.03341e-11, 2.75118e-11,  // 250
		2.49521e-11, 2.26306e-11, 2.05251e-11, 1.86154e-11, 1.68834e-11, 1.53126e-11, 1.38879e-11
};

#define NBO 8
#define NBP 4

#define log2(x) (log(x)/VL_LOG_OF_2)

float vl_mod_2pi_f (float x)
{
	while (x > (float)(VL_PI2)) x -= (float) (VL_PI2) ;
	while (x < 0.0F) x += (float) (VL_PI2);
	return x ;
}
float vl_fast_atan2_f (float y, float x)
{
	float angle, r ;
	float abs_y    = fabsf (y) + VL_EPSILON_F ;

	if (x >= 0) {
		r = (x - abs_y) / (x + abs_y) ;
		angle = VL_QUTER_PI;
	} else {
		r = (x + abs_y) / (abs_y - x) ;
		angle = VL_THR_QUTER_PI;
	}
	angle += (0.1821F*r*r - 0.9675F) * r ;
	return (y < 0) ? VL_PI2 - angle : angle ;
}
long int vl_floor_f (float x)
{
	long int xi = (long int) x ;
	if (x >= 0 || (float) xi == x) return xi ;
	else return xi - 1 ;
}
float vl_fast_resqrt_f (float x)
{
	float xhalf = 0.5f*x;
	int i = *(int*)&x; // get bits for floating VALUE 
	i = 0x5f375a86- (i>>1); // gives initial guess y0
	x = *(float*)&i; // convert bits BACK to float
	x = x*(1.5f-xhalf*x*x); // Newton step, repeating increases accuracy
	return x;
}
float vl_fast_sqrt_f (float x)
{
	return (x < 1e-8) ? 0 : x * vl_fast_resqrt_f (x) ;
}
long int vl_floor_d (double x)
{
	long int xi = (long int) x ;
	if (x >= 0 || (double) xi == x) return xi ;
	else return xi - 1 ;
}
double fast_expn (double x)
{
  //double a,b,r ;
  int i ;
  /*assert(0 <= x && x <= EXPN_MAX) ;*/

  if (x > EXPN_MAX) return 0.0 ;

  x *= 10.24;//EXPN_SZ / EXPN_MAX ;
  i = (int)x;
  return expn_tab [i    ] + (x - i) * (expn_tab [i + 1] - expn_tab [i    ]) ;
}
/*
Initialize the block-wise filtering
*/
FilterUnit * init_block()
{
	FilterUnit * p;
	int i;
	int total_memory = 0;
	int imageSize;
	int fblock_size;
	int block_width = BLOCK_WIDTH;
	int maxfilter_width = MAX_FILTER_WIDTH;
	p = new FilterUnit;
	int fblock_width = block_width + maxfilter_width - 1; //the block filtering size
	imageSize = fblock_width * fblock_width;				//size of spacial domain
	fblock_size = fblock_width * (fblock_width / 2 + 1);	//size of frequency domain

	p->inmat = (float*) fftwf_malloc (sizeof(float) * imageSize) ;total_memory += sizeof(float) * imageSize;

	p->fblock     = (fftwf_complex*) fftwf_malloc (sizeof(fftwf_complex) * fblock_size) ; total_memory += sizeof(fftwf_complex) * fblock_size;
	p->fblock_flog = (fftwf_complex*) fftwf_malloc (sizeof(fftwf_complex) * fblock_size) ; total_memory += sizeof(fftwf_complex) * fblock_size;

	for(i = 0; i < 2; i++)
	{
		fblock_width = block_width + maxfilter_width - 1;

		p->fftplan[i] = fftwf_plan_dft_r2c_2d(fblock_width, fblock_width, p->inmat, p->fblock, FFTW_MEASURE);
		p->ifftplan[i] = fftwf_plan_dft_c2r_2d(fblock_width, fblock_width, p->fblock_flog, p->inmat, FFTW_MEASURE);

	}	
	return p;
}
/*
Release the resource of the block-wise filtering
*/
void release_block(FilterUnit * p)
{
	if(!p) return;
	int i;
	if (p->inmat) fftwf_free (p->inmat);
	if (p->fblock) fftwf_free(p->fblock);
	if (p->fblock_flog) fftwf_free(p->fblock_flog);

	for (i = 0; i < 2; ++i)
	{
		if (p->fftplan[i]) fftwf_destroy_plan(p->fftplan[i]);
		if (p->ifftplan[i]) fftwf_destroy_plan(p->ifftplan[i]);
	}
	delete p;
}
/*
Assign memory for each octave processing
*/
void assign_memory (SiftFilt * f, int width, int height)
{
	int nlevels, ex, s;
	double sigma;

	nlevels = f->S;

	//////////////////////////////////////////////////////////////////////////
	f-> S       = nlevels ;
	f-> s_max   = nlevels+1;

	f-> sigman  = 0.5 ;
	f-> sigmak  = pow (2.0, 1.0 / nlevels) ;
	f-> sigma0  = 1.6 * f->sigmak ;
	f-> dsigma0 = f->sigma0 * sqrt (1.0 - 1.0 / (f->sigmak*f->sigmak)) ;
	///
	for (s = 0; s < nlevels+2; ++s)
	{
		sigma = 1.6 * pow (2.0, (s) * 1.0 / nlevels); 
		normFactors[s] = 1.0 * sigma * sigma / 160000.0;		// adjust for scale
	}

	//allocate memory for Gaussian block buffer
	f -> block_row_n = (width / BLOCK_WIDTH + (width % BLOCK_WIDTH == 0 ? 0 : 1));
	f -> block_col_n = (height / BLOCK_WIDTH + (height % BLOCK_WIDTH == 0 ? 0 : 1));
	f -> block_row_remain_width = width + BLOCK_WIDTH - f->block_row_n * BLOCK_WIDTH;
	f -> block_col_remain_height = height + BLOCK_WIDTH - f->block_col_n * BLOCK_WIDTH;
	ex = f->block_row_n;
	if(f->block_col_n > 1)
	{
		ex++;
		if(f->block_row_n > 1)
		{
			ex++;
		}
	}
	f->block_buffer_len = ex;
	if(f->block_buffer) free(f->block_buffer);
	//re-allocate the memory for save the block buffer
	f -> block_buffer	= (gs_type *) malloc (sizeof(gs_type) * nlevels * BLOCK_SIZE * (f->block_buffer_len));
	for(s = 0; s < f->block_row_n * f->block_col_n; s++) f->block_map[s] = (s % ex) * BLOCK_SIZE;
}

SiftFilt * sift_new (int width, int height, int noctaves, int nlevels, int o_min)
{
	SiftFilt *f = (SiftFilt*)malloc (sizeof(SiftFilt)) ;

	int fblock_width, fblock_size, data_size, imageSize, LoG_size, Gradient_Size, Input_Size;
	int s, ex;
	double sigma;

	/* negative value O => calculate max. value */
	if (noctaves < 0) {
		noctaves = (int)VL_MAX (floor (log2 ((float)VL_MIN(width, height))) - o_min - 3, 1);
	}
	f-> filter	= init_block();
	f-> width   = width ;
	f-> height  = height ;
	f-> O       = noctaves ;
	f-> S       = nlevels ;
	f-> o_min   = o_min ;
	f-> s_min   = -1 ;
	f-> s_max   = nlevels+1;
	f-> o_cur   = o_min ;

	f-> sigman  = 0.0 ;
	f-> sigmak  = pow (2.0, 1.0 / nlevels) ;
	f-> sigma0  = 1.6 * f->sigmak ;
	f-> dsigma0 = f->sigma0 * sqrt (1.0 - 1.0 / (f->sigmak*f->sigmak)) ;

	f -> block_width = BLOCK_WIDTH;

	f-> octave_width  = 0 ;
	f-> octave_height = 0 ;

	f-> keys     = NULL ;
	f-> nkeys    = 0 ;
	f-> keys_res = 0 ;
	f-> peak_thresh = 0.5;
	f-> edge_thresh = 10.0;
	f-> norm_thresh = 0.0 ;
	f-> magnif      = 3.0 ;
	f-> windowSize  = NBP / 2 ;
	f-> maxfilter_width = MAX_FILTER_WIDTH;

	f-> fgaussian_filter.data = NULL;
	f-> fgaussian_filter.mask = NULL;
	f-> fgaussian_filter.mask_pos = NULL;
	f-> fgaussian_filter.pos = NULL;
	f-> flog_filter.data = NULL;
	f-> flog_filter.mask = NULL;
	f-> flog_filter.mask_pos = NULL;
	f-> flog_filter.pos = NULL;

	//allocate memory for LoG buffer and Gradient
	LoG_size = (BLOCK_WIDTH + 4) * (BLOCK_WIDTH + 4) * (nlevels + 2) * sizeof(element_type);
	Gradient_Size = 120 * 120 * 2 * sizeof(sift_pix);
	f -> buffer			= (char *) malloc (VL_MAX(LoG_size, Gradient_Size));
	f -> log_response	= (element_type *)f->buffer;
	f -> grad			= (sift_pix *)f->buffer;


	for (s = 0; s < nlevels+2; ++s)
	{
		sigma = 1.6 * pow (2.0, (s) * 1.0 / nlevels); 
		normFactors[s] = 1.0 * sigma * sigma / 160000.0;		// adjust for scale
	}

	Input_Size = width * height * sizeof(input_type) / 4;
	f -> data  = (input_type *) malloc (Input_Size);
	f -> block_buffer = NULL;

	return f ;
}

void sift_delete(SiftFilt* f)
{
	if (f) {
		if (f->keys) free (f->keys) ;
		//if (f->grad) free (f->grad) ;

		if (f->data) free(f->data);
		if (f->buffer) free(f->buffer);

		if (f->block_buffer) free(f->block_buffer);

		if(f->filter) release_block(f->filter);

		free (f) ;
	}
}

/*
Gaussian smooth in space domain
*/
void GaussianSmooth(float * src, float * dst, float sigma, int n)  
{  
	sigma = sigma > 0 ? sigma : -sigma;  
	int size = n * n;
	int ksize = ceil(sigma * 4) * 2 + 1;
	if(ksize == 1)  
	{  
		memcpy(dst, src, sizeof(float) * size);    
		return;  
	}  

	float *kernel = new float[ksize];
	float *temp = new float[size];
	float scale = -0.5/(sigma*sigma);
	float sum = 0;
	int kcenter = ksize * 0.5;
	int i = 0;
	float *p, *q;
	float mul;
	int x, y, t;
	for(i = 0; i < ksize; i++)
	{  
		int x = i - kcenter;  
		*(kernel+i) = exp(x * x * scale);
		sum += *(kernel+i); 
	}  
	for(i = 0; i < ksize; i++)  
		*(kernel+i) /= sum;

	//convolution for x direction
	p = src;q = temp;
	for(y = 0; y < n; y++)
	{
		for(x = 0; x < n; x++)
		{
			mul = 0;
			sum = 0;
			for(i = -kcenter; i <= kcenter; i++)
			{
				scale = *(kernel+kcenter+i);
				t = x+i;
				if(t >= 0 && t < n)  
				{
					mul += (*(p+t)) * scale;
					sum += scale;
				}
			}
			*(q+x) = mul / sum;
		}
		p += n;q += n;
	}
	//convolution for y direction
	p = temp; q = dst;
	for(x = 0; x < n; x++)
	{
		p = temp + x;
		q = dst + x;
		for(y = 0; y < n; y++)
		{
			mul = 0;
			sum = 0;
			for(i = -kcenter; i <= kcenter; i++)
			{
				scale = *(kernel+kcenter+i);
				t = y+i;
				if(t >= 0 && t < n)
				{
					mul += (*(p+i*n)) * scale;
					sum += scale;
				}
			}
			*q = mul / sum;
			p += n;q += n;
		}
	}//*/
	delete[] temp;
	delete[] kernel;
}  
node get_filter(SiftFilt const *f, node filter, int s)
{
	node res;
	res.pos = filter.pos + s-f->s_min;
	res.mask_pos = filter.mask_pos + s-f->s_min;
	res.data = filter.data + res.pos[0];
	res.mask = filter.mask + res.mask_pos[0];
	return res;
}

static node get_filters(int o_cur, int type)
{
	node res;
	switch (o_cur)
	{
	case 0:
		res.data = type ? g_fgaussian_data0:g_flog_data0;
		res.mask = type ? g_fgaussian_mask0:g_flog_mask0;
		res.mask_pos = type ? g_fgaussian_maskpos0:g_flog_maskpos0;
		res.pos = type ? g_fgaussian_pos0:g_flog_pos0;
		break;
	case 1:
		res.data = type ? g_fgaussian_data1:g_flog_data1;
		res.mask = type ? g_fgaussian_mask1:g_flog_mask1;
		res.mask_pos = type ? g_fgaussian_maskpos1:g_flog_maskpos1;
		res.pos = type ? g_fgaussian_pos1:g_flog_pos1;
		break;
	default:
		break;
	}
	return res;
}
static void complex_mutil(fftwf_complex *srcA,const node srcB,fftwf_complex *dest,size_t size, int fwidth, float factor)
{
	int j, k, h;
	int ii, jj;
	float v;
	j = 0;
	k = 0;
	h = 0;
	int locate_1;
	int locate_2;
	int locate_3;
	int locate_4;
	int pos1, pos2;
	pos1 = 0;
	pos2 = (fwidth + fwidth - 2) * fwidth;
	memset(dest, 0, sizeof(fftwf_complex)*size);
	for(ii = 0; ii < fwidth; ii++)
	{
		locate_1 = locate_2 = pos1 + ii;
		locate_3 = locate_4 = pos2 + ii;
		pos1 += fwidth;
		pos2 -= fwidth;
		for(jj = ii; jj < fwidth; jj++)
		{
			if(srcB.mask[k] & bits[h])
			{
				v = (float)srcB.data[j] * factor;
				dest[locate_1][0] = srcA[locate_1][0] * v;
				dest[locate_1][1] = srcA[locate_1][1] * v;

				dest[locate_2][0] = srcA[locate_2][0] * v;
				dest[locate_2][1] = srcA[locate_2][1] * v;

				if(ii > 0 && ii < fwidth - 1)
				{
					dest[locate_3][0] = srcA[locate_3][0] * v;
					dest[locate_3][1] = srcA[locate_3][1] * v;
				}
				if(jj > 0 && jj < fwidth - 1)
				{
					dest[locate_4][0] = srcA[locate_4][0] * v;
					dest[locate_4][1] = srcA[locate_4][1] * v;
				}
				
				j++;
			}
			h++;
			if(h == 8)
			{
				h = 0;
				k++;
			}
			locate_1++;
			locate_2 += fwidth;
			locate_3++;
			locate_4 -= fwidth;
		}
	}
}
float normalize_histogram(float * p)
{
	sift_pix  norm = 0.0 ;
	int i;
	for (i = 0; i < 128; i++)
		norm += p[i] * p[i] ;

	norm = sqrt (norm) + VL_EPSILON_F ;

	if(norm > 0)
		for (i = 0; i < 128; i++)
			p[i] /= norm;

	return norm;
}

int keypoint_extrema_block (SiftFilt * f, int w, int h, int v_mode, int h_mode, int pad)
{
	element_type* log   = f-> log_response ;
	element_type * pt = NULL;
  int          s_min = f-> s_min ;
  int          s_max = f-> s_max ;
  double       te    = f-> edge_thresh ;
  double       tp    = f-> peak_thresh / FACTOR_LOG;
  double       the   = tp * 0.8;
  double       xper  = pow (2.0, f->o_cur) ;
  
  int offset;

  int const    xo    = 1 ;      /* x-stride */
  int const    yo    = w ;      /* y-stride */
  int const    so    = w * h ;  /* s-stride */

  int x, y, s, i, ii, jj ;
  int l, r, u, d;
  sift_pix v ;
  SiftKeypoint *k ;
  int st_key;

  /* clear current list */
  //f-> nkeys = 0 ;
  st_key = f->nkeys;

  if(f->o_cur == 0) offset = 6;
  else if(f->o_cur == 1) offset = 3;
  else offset = 2;

  if(h_mode & 1) l = offset; else l = pad;
  if(h_mode & 2) r = offset; else r = pad;
  if(v_mode & 1) u = offset; else u = pad;
  if(v_mode & 2) d = offset; else d = pad;

  /* -----------------------------------------------------------------
   *                                          Find local maxima of DoG
   * -------------------------------------------------------------- */

  /* start from dog [1,1,s_min+1] */

  pt = log + l * xo + u * yo + so ;

  for(s = s_min + 1 ; s <= s_max - 2 ; ++s) 
  {
    for(y = u ; y < h - d; ++y) {
      for(x = l ; x < w - r ; ++x) {
        v = *pt ;

#define CHECK_NEIGHBORS(CMP,SGN)                    \
        ( v CMP ## = SGN 0.8 * tp &&                \
          v CMP *(pt + xo) &&                       \
          v CMP *(pt - xo) &&                       \
          v CMP *(pt + so) &&                       \
          v CMP *(pt - so) &&                       \
          v CMP *(pt + yo) &&                       \
          v CMP *(pt - yo) &&                       \
                                                    \
          v CMP *(pt + yo + xo) &&                  \
          v CMP *(pt + yo - xo) &&                  \
          v CMP *(pt - yo + xo) &&                  \
          v CMP *(pt - yo - xo) &&                  \
                                                    \
          v CMP *(pt + xo      + so) &&             \
          v CMP *(pt - xo      + so) &&             \
          v CMP *(pt + yo      + so) &&             \
          v CMP *(pt - yo      + so) &&             \
          v CMP *(pt + xo      - so) &&             \
          v CMP *(pt - xo      - so) &&             \
          v CMP *(pt + yo      - so) &&             \
          v CMP *(pt - yo      - so) &&             \
          v CMP *(pt + yo + xo + so) &&             \
          v CMP *(pt + yo - xo + so) &&             \
          v CMP *(pt - yo + xo + so) &&             \
          v CMP *(pt - yo - xo + so) &&             \
		  v CMP *(pt + yo + xo - so) &&             \
          v CMP *(pt + yo - xo - so) &&             \
          v CMP *(pt - yo + xo - so) &&             \
          v CMP *(pt - yo - xo - so) )//*/

        if (CHECK_NEIGHBORS(>,+) ||
            CHECK_NEIGHBORS(<,-) ) {

          /* make room for more keypoints */
          if (f->nkeys >= f->keys_res) {
            f->keys_res += 200 ;
            if (f->keys) {
              f->keys = (SiftKeypoint *)realloc (f->keys,
                                    f->keys_res *
                                    sizeof(SiftKeypoint)) ;
            } else {
              f->keys = (SiftKeypoint *)malloc (f->keys_res *
                                   sizeof(SiftKeypoint)) ;
            }
          }
		  
          k = f->keys + (f->nkeys ++) ;

          k-> ix = x ;
          k-> iy = y ;
          k-> is = s ;

		  k-> peak = (float)fabsf(v);

        }
        pt += 1 ;
      }
      pt += l + r ;
    }
    pt += (u + d) * yo ;
  }

  /* -----------------------------------------------------------------
   *                                               Refine local maxima
   * -------------------------------------------------------------- */

  /* this pointer is used to write the keypoints back */
  k = f->keys + st_key;

  for (i = st_key ; i < f->nkeys ; ++i) {

    int x = f-> keys [i] .ix ;
    int y = f-> keys [i] .iy ;
    int s = f-> keys [i]. is ;
	float p = f-> keys [i]. peak ;

    double Dx=0,Dy=0,Ds=0,Dxx=0,Dyy=0,Dss=0,Dxy=0,Dxs=0,Dys=0 ;
    double A [3*3], b [3] ;

    int dx = 0 ;
    int dy = 0 ;

    int iter, i, j ;

    for (iter = 0 ; iter < 5 ; ++iter) {

      x += dx ;
      y += dy ;

	  pt = log
		  + xo * x
		  + yo * y
		  + so * (s - s_min) ;


      /** @brief Index GSS @internal */
#define at(dx,dy,ds) (*( pt + (dx)*xo + (dy)*yo + (ds)*so))

      /** @brief Index matrix A @internal */
#define Aat(i,j)     (A[(i)+(j)*3])

      /* compute the gradient */
      Dx = 0.5 * (at(+1,0,0) - at(-1,0,0)) ;
      Dy = 0.5 * (at(0,+1,0) - at(0,-1,0));
      Ds = 0.5 * (at(0,0,+1) - at(0,0,-1)) ;

      /* compute the Hessian */
      Dxx = (at(+1,0,0) + at(-1,0,0) - 2.0 * at(0,0,0)) ;
      Dyy = (at(0,+1,0) + at(0,-1,0) - 2.0 * at(0,0,0)) ;
      Dss = (at(0,0,+1) + at(0,0,-1) - 2.0 * at(0,0,0)) ;

      Dxy = 0.25 * ( at(+1,+1,0) + at(-1,-1,0) - at(-1,+1,0) - at(+1,-1,0) ) ;
      Dxs = 0.25 * ( at(+1,0,+1) + at(-1,0,-1) - at(-1,0,+1) - at(+1,0,-1) ) ;
      Dys = 0.25 * ( at(0,+1,+1) + at(0,-1,-1) - at(0,-1,+1) - at(0,+1,-1) ) ;

      /* solve linear system ....................................... */
      Aat(0,0) = Dxx ;
      Aat(1,1) = Dyy ;
      Aat(2,2) = Dss ;
      Aat(0,1) = Aat(1,0) = Dxy ;
      Aat(0,2) = Aat(2,0) = Dxs ;
      Aat(1,2) = Aat(2,1) = Dys ;

      b[0] = - Dx ;
      b[1] = - Dy ;
      b[2] = - Ds ;

      /* Gauss elimination */
      for(j = 0 ; j < 3 ; ++j) {
        double maxa    = 0 ;
        double maxabsa = 0 ;
        int    maxi    = -1 ;
        double tmp ;

        /* look for the maximally stable pivot */
        for (i = j ; i < 3 ; ++i) {
          double a    = Aat (i,j) ;
          double absa = fabs (a) ;
          if (absa > maxabsa) {
            maxa    = a ;
            maxabsa = absa ;
            maxi    = i ;
          }
        }

        /* if singular give up */
        if (maxabsa < 1e-10f) {
          b[0] = 0 ;
          b[1] = 0 ;
          b[2] = 0 ;
          break ;
        }

        i = maxi ;

        /* swap j-th row with i-th row and normalize j-th row */
        for(jj = j ; jj < 3 ; ++jj) {
          tmp = Aat(i,jj) ; Aat(i,jj) = Aat(j,jj) ; Aat(j,jj) = tmp ;
          Aat(j,jj) /= maxa ;
        }
        tmp = b[j] ; b[j] = b[i] ; b[i] = tmp ;
        b[j] /= maxa ;

        /* elimination */
        for (ii = j+1 ; ii < 3 ; ++ii) {
          double x = Aat(ii,j) ;
          for (jj = j ; jj < 3 ; ++jj) {
            Aat(ii,jj) -= x * Aat(j,jj) ;
          }
          b[ii] -= x * b[j] ;
        }
      }

      /* backward substitution */
      for (i = 2 ; i > 0 ; --i) {
        double x = b[i] ;
        for (ii = i-1 ; ii >= 0 ; --ii) {
          b[ii] -= x * Aat(ii,i) ;
        }
      }

      /* .......................................................... */
      /* If the translation of the keypoint is big, move the keypoint
       * and re-iterate the computation. Otherwise we are all set.
       */

      dx= ((b[0] >  0.6 && x < w - 2 - pad) ?  1 : 0)
        + ((b[0] < -0.6 && x > 1 + pad    ) ? -1 : 0) ;

      dy= ((b[1] >  0.6 && y < h - 2 - pad) ?  1 : 0)
        + ((b[1] < -0.6 && y > 1 + pad   ) ? -1 : 0) ;

      if (dx == 0 && dy == 0) break ;
    }

    /* check threshold and other conditions */
    {
      double val   = at(0,0,0)
        + 0.5 * (Dx * b[0] + Dy * b[1] + Ds * b[2]) ;
      double score = (Dxx+Dyy)*(Dxx+Dyy) / (Dxx*Dyy - Dxy*Dxy) ;
      double xn = x + b[0] ;
      double yn = y + b[1] ;
      double sn = s + b[2] ;

      int good =
        fabs (val)  > tp                  &&
        score           < (te+1)*(te+1)/te    &&
        score           >= 0                  &&
        fabs (b[0]) <  1.5                &&
        fabs (b[1]) <  1.5                &&
        fabs (b[2]) <  1.5                &&
        xn              >= 0                  &&
        xn              <= w - 1              &&
        yn              >= 0                  &&
        yn              <= h - 1              &&
        sn              >= s_min              &&
        sn              <= s_max ;

	  
	  /*float  lowest_nabor = 1;
	  if(good){
		  for(SiftKeypoint * ii = f->keys; ii != k; ii++)
		  {
			  if(fabs(sn - ii->s) <= lowest_nabor && fabs(xn - ii->x) <= lowest_nabor && fabs(yn - ii->y) <= lowest_nabor)
			  {
				  //fprintf(stderr, "%d: (%.1f, %.1f, %.1f), (%.1f, %.1f, %.1f), %f, %f, %f\n", f->o_cur, ii->s, ii->x, ii->y, (float)sn, (float)xn, (float)yn, ii->peak, fabs(val) * 0.01, p*0.01);
				  if(fabs(val) * FACTOR_LOG > ii->peak)
				  {
					  ii->peak = fabs(val) * FACTOR_LOG;
					  ii-> ix = x;
					  ii-> iy = y;
					  ii-> s  = (float)sn;
					  ii-> x  = (float)xn;
					  ii-> y  = (float)yn;
					  ii-> ix = (int)(ii->x + 0.5);
					  ii-> iy = (int)(ii->y + 0.5);
					  ii-> is = (int)(ii->s + 0.5); if(ii->is < 0) ii->is = 0; if(ii->is > 2) ii->is = 2;
					  ii-> sigma = (float)(f->sigma0 * pow (2.0, sn/f->S));
				  }
				  good = 0;
				  break;
			  }
		  }
	  }//*/
      if (good) {
        k-> o     = f->o_cur ;
        k-> ix    = x;
        k-> iy    = y;
        k-> is    = s;
        k-> s     = (float)sn ;
        k-> x     = (float)xn;
        k-> y     = (float)yn;
		k-> ix	  = (int)(k->x + 0.5);
		k-> iy	  = (int)(k->y + 0.5);

		k-> is    = (int)(k->s + 0.5); if(k->is < 0) k->is = 0; if(k->is > 2) k->is = 2;
        k-> sigma = (float)(f->sigma0 * pow (2.0, sn/f->S));
		k-> peak = fabs(val) * FACTOR_LOG;
		k-> curvRatio = score;
		//fprintf(stderr, "%f, %f\n", val, p);
        ++ k ;
      }

    } /* done checking */
  } /* next keypoint to refine */

  /* update keypoint count */
  f-> nkeys = (int)(k - f->keys) ;
  return f->nkeys - st_key;
}

void keypoint_block_desc(SiftFilt *f, int blockid, int block_width, int block_height, vector<Feature> & featurelist, double xper)
{
	int i, iter, iori;
	SiftKeypoint * k;
	int block_row, block_col, cur_block_row, cur_block_col;
	int s_cur;
	gs_type * blockPtr = NULL;
	gs_type * p, *pt;
	float * src;
	int xi, yi, ys, xs, x1, x2, y1, y2, x0, y0;
	int left, right, up, bottom;
	float x, y, r2;
	int sig_x1, sig_y1, sig_x2, sig_y2, sig_x0, sig_y0;

	enum {nbins = 36} ;
	double hist [nbins], maxh ;
	float bin2Pi = (float)(nbins * 1.0 / VL_PI2);
	double const magnif = f-> magnif ;
	int nangles;
	float orientation[4];
	float rbuf [128] ;
	float hist_desc[NBP+2][NBP+2][NBO+2];

	double st0, ct0, SBP, SBP_1, sigma2;
	int W_Des, W_Ori;
	float wsigma = (float)(1.0 / (f->windowSize * f->windowSize * 2));

	int const binto = 1 ;          /* bin theta-stride */
	int const binyo = NBO * NBP ;  /* bin y-stride */
	int const binxo = NBO ;        /* bin x-stride */

	block_row = blockid / f->block_row_n; //current block row
	block_col = blockid % f->block_row_n; //current block col
	//grad = (float *)vl_malloc(sizeof(float) * size * 2);

	Feature d;
	for(i = 0; i < f->nkeys; i++)
	{
		k = f->keys + i;
		if(k->mode == MODE_FINISH)
			continue;
		//the current block
		if(k->block_id == blockid){
			if((k->mode & MODE_RIGHT) || (k->mode & MODE_BOTTOM))
				continue;
		}
		//the left block
		else if(block_col > 0 && k->block_id + 1 == blockid ){
			if(k->mode & MODE_BOTTOM)
				continue;
		}
		//the upper block
		else if(block_row > 0 && k->block_id + f->block_row_n == blockid){
			if((k->mode & MODE_BOTTOM) && (k->mode & MODE_RIGHT))
				continue;
		}
		//the upper-left block
		else if(block_row > 0 && block_col > 0 && k->block_id + 1 + f->block_row_n == blockid){

		}
		else continue;

		cur_block_row = k->block_id / f->block_row_n; //current block row
		cur_block_col = k->block_id % f->block_row_n; //current block col
		s_cur = k->is;
		blockPtr = f->block_buffer + s_cur * f->block_buffer_len * BLOCK_SIZE;			
		//////////////////////////////////////////////////////////////////////////
		SBP      = magnif * k->sigma + VL_EPSILON_D ;
		SBP_1	 = 1.0 / SBP;
		sigma2	 = 1.0 / (4.5 * k->sigma * k->sigma);
		W_Des    = (int)(floor(sqrt(2.0) * SBP * (NBP + 1) / 2.0 + 0.5));
		W_Ori	 = (int)(VL_MAX(floor(4.5 * k->sigma), 1));
		float W_Ori2 = W_Ori * W_Ori + 0.6f;

		k->mode = MODE_FINISH;
		memset(hist, 0, sizeof(hist));
		xi = (int) (k->x + 0.5);
		yi = (int) (k->y + 0.5);
		if(xi == BLOCK_WIDTH) xi = BLOCK_WIDTH - 1;	//risk
		if(yi == BLOCK_WIDTH) yi = BLOCK_WIDTH - 1;
		up = yi - W_Des;
		if(up <= 0 && cur_block_row == 0)
		{
			up = 1;
		}
		bottom = yi + W_Des;
		if(cur_block_row == f->block_col_n - 1 && bottom >= f->block_col_remain_height - 1)
		{
			bottom = f->block_col_remain_height - 2;
		}
		else if(cur_block_row == f->block_col_n - 2 && bottom >= f->block_col_remain_height - 1 + BLOCK_WIDTH)
		{
			bottom = f->block_col_remain_height - 2 + BLOCK_WIDTH;
		}
		left = xi - W_Des;
		if(left <= 0 && cur_block_col == 0)
		{
			left = 1;
		}
		right = xi + W_Des;
		if(cur_block_col == f->block_row_n - 1 && right >= f->block_row_remain_width - 1)
		{
			right = f->block_row_remain_width - 2;
		}
		else if(cur_block_col == f->block_row_n - 2 && right >= f->block_row_remain_width - 1 + BLOCK_WIDTH)
		{
			right = f->block_row_remain_width - 2 + BLOCK_WIDTH;
		}
		src = f->grad;

		//////////////////////////////////////////////////////////////////////////
		for(ys = up; ys <= bottom; ys++)
		{
			float ddy, ddx;
			y0 = ys;
			y1 = y0 - 1;
			y2 = y0 + 1;

			y = ys - k->y;
			y = y * y;

			if(y0 < 0){ sig_y0 = k->block_id-f->block_row_n; y0 += BLOCK_WIDTH;}
			else if(y0 < BLOCK_WIDTH){ sig_y0 = k->block_id;}
			else{ sig_y0 = k->block_id+f->block_row_n; y0 -= BLOCK_WIDTH;}

			if(y1 < 0){ sig_y1 = k->block_id-f->block_row_n; y1 += BLOCK_WIDTH;}
			else if(y1 < BLOCK_WIDTH){ sig_y1 = k->block_id;}
			else{ sig_y1 = k->block_id+f->block_row_n; y1 -= BLOCK_WIDTH;}

			if(y2 < 0){ sig_y2 = k->block_id-f->block_row_n; y2 += BLOCK_WIDTH;}
			else if(y2 < BLOCK_WIDTH){ sig_y2 = k->block_id;}
			else{ sig_y2 = k->block_id+f->block_row_n; y2 -= BLOCK_WIDTH;}

			y1 *= BLOCK_WIDTH;
			y2 *= BLOCK_WIDTH;
			y0 *= BLOCK_WIDTH;//*/


			for(xs = left; xs <= right; xs++)
			{
				float wgt, mod, ang, fbin, gx, gy, rbin;
				int bin, bin1, bin2;
				x0 = xs;
				x1 = x0 - 1;
				x2 = x0 + 1;

				x = xs - k->x;
				r2 = x * x + y;
			
				if(x0 < 0){ sig_x0 = -1; x0 += BLOCK_WIDTH;}
				else if(x0 < BLOCK_WIDTH){ sig_x0 = 0;}
				else{ sig_x0 = 1; x0 -= BLOCK_WIDTH;}

				if(x1 < 0){ sig_x1 = -1; x1 += BLOCK_WIDTH;}
				else if(x1 < BLOCK_WIDTH){ sig_x1 = 0;}
				else{ sig_x1 = 1; x1 -= BLOCK_WIDTH;}
		
				if(x2 < 0){ sig_x2 = -1; x2 += BLOCK_WIDTH;}
				else if(x2 < BLOCK_WIDTH){ sig_x2 = 0;}
				else{ sig_x2 = 1; x2 -= BLOCK_WIDTH;}//*/


				p = blockPtr + f->block_map[sig_y1 + sig_x0] + y1 + x0;
				pt = blockPtr + f->block_map[sig_y2 + sig_x0] + y2 + x0;
				gy = ((int)(*pt) - (int)(*p));


				p = blockPtr + f->block_map[sig_y0 + sig_x1] + y0 + x1;
				pt = blockPtr + f->block_map[sig_y0 + sig_x2] + y0 + x2;
				gx = ((int)(*pt) - (int)(*p));


				mod  = sqrt (gx*gx + gy*gy);
				ang  = vl_fast_atan2_f (gy, gx);

				*src++ = (float)mod;
				*src++ = (float)ang;

				if (r2 < W_Ori2)
				{
					wgt  = fast_expn (r2 * sigma2) * mod;
					fbin = bin2Pi * ang - 0.5;
					if(fbin < 0)
					{
						bin = -1;
						bin1 = nbins - 1;
						bin2 = 0;
					}
					else
					{
						bin = (int)fbin;
						bin1 = bin;
						bin2 = bin + 1;
						if(bin2 >= nbins)
						{
							bin2 = 0;
						}
					}
					rbin = (fbin - bin) * wgt ;
					hist [bin1] += wgt - rbin ;
					hist [bin2] += rbin ;
				}
			}//end x
		}//end y

		for (iter = 0; iter < 6; iter ++) {
			double prev  = hist [nbins - 1] ;
			double first = hist [0] ;
			int i ;
			for (i = 0; i < nbins - 1; i++) {
				double newh = (prev + hist[i] + hist[i+1]) / 3.0;
				prev = hist[i] ;
				hist[i] = newh ;
			}
			hist[i] = (prev + hist[i] + first) / 3.0 ;
		}

		maxh = 0 ;
		for (iori = 0 ; iori < nbins ; ++iori)
			maxh = VL_MAX (maxh, hist [iori]) ;

		/* find peaks within 80% from max */
		nangles = 0 ;
		for(iori = 0 ; iori < nbins ; ++iori) {
			double h0 = hist [iori] ;
			double hm = hist [(iori - 1 + nbins) % nbins] ;
			double hp = hist [(iori + 1 + nbins) % nbins] ;

			if (h0 > 0.8*maxh && h0 > hm && h0 > hp) {
				double di = - 0.5 * (hp - hm) / (hp + hm - 2 * h0) ;
				double th = VL_PI2 * (iori + di + 0.5) / nbins ;
				orientation [ nangles++ ] = th ;
				if( nangles == 4 )
					break;
			}
		}//*/
		//////////////////////////////////////////////////////////////////////////
		//output the detector's character
		d.x = (k->x + BLOCK_WIDTH * cur_block_col) * xper;
		d.y = (k->y + BLOCK_WIDTH * cur_block_row) * xper;
		d.scale = k->sigma * xper;
		d.peak = k->peak;

		d.curvRatio = k->curvRatio;

		//////////////////////////////////////////////////////////////////////////
		int binx, biny, bint, binx1, biny1, bint1;
		for(iori = 0; iori < nangles; iori++)
		{
			int bin;
			float *dpt ;
			float norm;
			float angle0 = orientation[iori];
			st0 = sin (angle0) * SBP_1;
			ct0 = cos (angle0) * SBP_1;
			d.orientation = angle0;
			/* clear descriptor */
			memset(hist_desc, 0, sizeof(hist_desc));

			src = f->grad;
			float dx, dy, nx, ny, nx1, ny1, nx2, ny2;
			float rbinx, rbiny, rbint;
			float v_r1, v_r0, v_rc11, v_rc10, v_rc01, v_rc00;  
			float v_rco111, v_rco110, v_rco101, v_rco100;  
			float v_rco011, v_rco010, v_rco001, v_rco000;  
			dy = up - k->y - 1;
			dx = left - k->x - 1;
			nx2 = ct0 * dx + st0 * dy;
			ny2 = -st0 * dx + ct0 * dy;
			for(y0 = up; y0 <= bottom; y0++)
			{
				//float dy = y0 - k->y;
				nx1 = (nx2 += st0);
				ny1 = (ny2 += ct0);
				for(x0 = left; x0 <= right; x0++)
				{	
					nx1 += ct0;
					ny1 -= st0;
					if(nx1 >= 2.5 || nx1 < -2.5 || ny1 >= 2.5 || ny1 < -2.5) { src += 2; continue;}
					//get the gradient value
					float mod = *src++;
					float angle = *src++;

					float theta = angle - angle0;
					if(theta >= VL_PI2) theta -= VL_PI2;
					if(theta < 0) theta += VL_PI2;

					float nt = BPO_P_PI2 * theta;
					
					//get the weight
					float win = fast_expn((nx1*nx1 + ny1*ny1) * wsigma) * mod;

					nx = nx1 + 2.5;
					ny = ny1 + 2.5;

					binx = (int)nx; biny = (int)ny; bint = (int)nt;
					binx1 = binx + 1;
					biny1 = biny + 1;
					bint1 = bint + 1;
					rbinx = nx - binx ;
					rbiny = ny - biny ;
					rbint = nt - bint ;

					v_r1 = win * rbinx, v_r0 = win - v_r1;
					v_rc11 = v_r1 * rbiny, v_rc10 = v_r1 - v_rc11;  
					v_rc01 = v_r0 * rbiny, v_rc00 = v_r0 - v_rc01;  
					v_rco111 = v_rc11 * rbint, v_rco110 = v_rc11 - v_rco111;  
					v_rco101 = v_rc10 * rbint, v_rco100 = v_rc10 - v_rco101;  
					v_rco011 = v_rc01 * rbint, v_rco010 = v_rc01 - v_rco011;  
					v_rco001 = v_rc00 * rbint, v_rco000 = v_rc00 - v_rco001;  

					hist_desc[binx][biny][bint] += v_rco000;  
					hist_desc[binx][biny][bint1] += v_rco001;  
					hist_desc[binx][biny1][bint] += v_rco010;  
					hist_desc[binx][biny1][bint1] += v_rco011;  
					hist_desc[binx1][biny][bint] += v_rco100;  
					hist_desc[binx1][biny][bint1] += v_rco101;  
					hist_desc[binx1][biny1][bint] += v_rco110;  
					hist_desc[binx1][biny1][bint1] += v_rco111;  //*/
				}//end x
			}//end y

			//output the descriptor
			dpt = rbuf;
			for(biny = 1; biny <= NBP; biny++)
			{
				for(binx = 1; binx <= NBP; binx++)
				{
					hist_desc[binx][biny][0] += hist_desc[binx][biny][NBO];
					hist_desc[binx][biny][1] += hist_desc[binx][biny][NBO+1];
					for(bint = 0; bint < NBO; bint++)
					{
						*dpt++ = hist_desc[binx][biny][bint];
					}
				}
			}//*/
			/* Normalize the histogram to L2 unit length. */
			norm = normalize_histogram (rbuf) ;

			/* Set the descriptor to zero if it is lower than our norm_threshold */
			if(f-> norm_thresh && norm < f-> norm_thresh) {
				for(bin = 0; bin < 128; ++ bin)
					rbuf[bin] = 0;
			}
			else {

				/* Truncate at 0.2. */
				for(bin = 0; bin < 128; ++ bin) {
					if (rbuf[bin] > 0.2) rbuf[bin] = 0.2;
				}

				/* Normalize again. */
				normalize_histogram (rbuf) ;
			}
			
			for (int j = 0 ; j < 128 ; ++j) {
				float x = 512.0F * rbuf [j] ;
				x = (x < 255.0F) ? x : 255.0F ;
				d.descr[j] = (unsigned char) x ;		// return uint8
			}

			featurelist.push_back(d);
		}//end orient
	}//end key
}

int keypoint_detect (SiftFilt *f, int o_cur, vector<Feature> & featurelist)
{
  int blockw, fblockw, blocksize, fblocksize;
  int padWidth;
  int x, y, s, i, j, ii, jj, iii, jjj, w, h;
  int ocur;
  int out_cnt = 0;
  /* shortcuts */
  int width           = f-> width ;
  int height          = f-> height;
  int o_min           = f-> o_min ;
  int s_min           = f-> s_min ;
  int s_max           = f-> s_max ;
  int S				  = f-> S;
  int O				  = f-> O;
  
  //fprintf(stderr, "%f %f %f\n", __frsqrte(2.0), __frsqrte(3.0), __frsqrte(4.0));
  float * pdata			= NULL;
  float * pdata2		= NULL;
  gs_type * p		= NULL;
  gs_type * pt		= NULL;
  input_type * p2		= NULL;
  input_type * pt2		= NULL;
  int w2, h2;
  sift_pix * pgrad	= NULL;
  sift_pix * pgrad2	= NULL;
  element_type * pLoG	= NULL;
  element_type * p2LoG	= NULL;
  input_type *data		= f->data;
  unsigned char *data0	= f->data0;
  unsigned char * p0	= NULL;
  element_type *log		= f->log_response;

  int h_mode, v_mode;
  int gradient_w, gradient_h, gradient_sx, gradient_sy, gradient_size;
  int extrema_w, extrema_h, extrema_sx, extrema_sy, extrema_size;
  int R_Gradient = MAX_FILTER_WIDTH / 2;
  int R_Extrema = 2;
  int block_width, block_height;
  int st_key;
  int cnt;
  int out_total;
  int block_cnt;
  SiftKeypoint *k ;
  double xper;

  float *inmat = f->filter->inmat;
  fftwf_complex *fblock = f->filter->fblock;
  fftwf_complex *fblock_flog = f->filter->fblock_flog;
  fftwf_plan forward_plan;
  fftwf_plan backward_plan;

  float fft_scale_factor;
  float fft_scale_log_factor;
  
  f->o_cur = o_cur ;
  xper  = pow (2.0, f->o_cur) ;
  w = f-> octave_width  = VL_SHIFT_LEFT(f->width,  - f->o_cur) ;	//width of current octave
  h = f-> octave_height = VL_SHIFT_LEFT(f->height, - f->o_cur) ;	//height of current octave
  w2 = w / 2;
  h2 = h / 2;

  assign_memory(f, w, h);
  s_max           = f-> s_max ;
  S				  = f-> S;

  ocur = o_cur;				//use set 0 of filters for octave 0, and set 1 for other octaves
  if(ocur > 0) ocur = 1;	

  blockw = f->block_width;		//block width without padding
  blocksize = blockw * blockw;	

  padWidth = f->maxfilter_width / 2;		//padding width
  fblockw = blockw + padWidth + padWidth;

  fblocksize = fblockw * (fblockw / 2 + 1);

  fft_scale_log_factor = 1.0 / (fblockw * fblockw) / FACTOR_LOG;  
  fft_scale_factor = 1.0 / (fblockw * fblockw) / FACTOR_;

  /* -----------------------------------------------------------------
   *                                          Compute the octave
   * -------------------------------------------------------------- */
  forward_plan = f->filter->fftplan[ocur];
  backward_plan = f->filter->ifftplan[ocur];

  /* Load frequency filters */
  f->flog_filter =  get_filters(ocur, 0);
  f->fgaussian_filter = get_filters(ocur, 1);
 
  cnt = 0;
  out_total = 0;

  block_cnt = 0;


  /* Scan the block from left to right and from top to bottom*/
  for(y = 0; y < h; y += blockw)
  {
	  v_mode = 0;									//locate the boundary of the block
	  gradient_sy = padWidth - R_Gradient;			//the Y-offset of the gradient patch to the padded block
	  gradient_h = blockw + R_Gradient + R_Gradient;//the height of the gradient patch, with R padding, R <= padWidth
	  extrema_sy = padWidth - R_Extrema;
	  extrema_h = blockw + R_Extrema + R_Extrema;	//the height of the extrma patch
	  block_height = blockw;
	  if(y == 0)									//the condition of up bound
	  {
		  v_mode |= 1;
		  gradient_sy += R_Gradient;		//no padding
		  gradient_h -= R_Gradient;
		  extrema_sy += R_Extrema;
		  extrema_h -= R_Extrema;
	  }
	  if(y + blockw >= h)
	  {
		  v_mode |= 2;
		  gradient_h = gradient_h - blockw - R_Gradient + h - y;
		  extrema_h = extrema_h - blockw - R_Extrema + h - y;
		  block_height = h - y;
	  }
	  //extrema_sy = padWidth - R_Extrema - gradient_sy;// the Y-offset of the gradient patch to the extrema patch
	  for(x = 0; x < w; x += blockw)
	  {
		  h_mode = 0;
		  gradient_sx = padWidth - R_Gradient;
		  gradient_w = blockw + R_Gradient + R_Gradient;
		  extrema_sx = padWidth - R_Extrema;
		  extrema_w = blockw + R_Extrema + R_Extrema;
		  block_width = blockw;
		  if(x == 0)
		  {
			  h_mode |= 1;
			  gradient_sx += R_Gradient;
			  gradient_w -= R_Gradient;
			  extrema_sx += R_Extrema;
			  extrema_w -= R_Extrema;
		  }
		  if(x + blockw >= w)
		  {
			  h_mode |= 2;
			  gradient_w = gradient_w - blockw - R_Gradient + w - x;
			  extrema_w = extrema_w - blockw - R_Extrema + w - x;
			  block_width = w - x;
		  }
		  //extrema_sx = padWidth - R_Extrema - gradient_sx;
		  extrema_size = extrema_w * extrema_h;
		  gradient_size = gradient_w * gradient_h;

		  //get block with padding
		  memset(inmat, 0, sizeof(float) * fblockw * fblockw);
		  for(j = y - padWidth, jjj = 0; j < y + blockw + padWidth; j++, jjj++)
		  {
			  jj = j;
			  //mirror padding of the boundary
			  if(jj < 0) {jj = -jj-1;if(jj >= h) continue;}
			  if(jj >= h) {jj = h - jj + h - 1;if(jj < 0) continue;}

			  //the first octave
			  if(o_cur == 0)
			  {
				  p0 = data0 + jj * w;
				  pdata = inmat + jjj * fblockw;
				  for(i = x - padWidth, iii = 0; i < x + blockw + padWidth; i++, iii++)
				  {
					  ii = i;
					  //mirror padding of the boundary
					  if(ii < 0) {ii = -ii-1;if(ii >= w) continue;}
					  if(ii >= w){ii =w - ii + w - 1;if(ii < 0) continue;}

					  *(pdata+iii) = (float)*(p0 + ii);
				  }
			  }
			  else
			  {
				  p2 = data + jj * w;
				  pdata = inmat + jjj * fblockw;
				  for(i = x - padWidth, iii = 0; i < x + blockw + padWidth; i++, iii++)
				  {
					  ii = i;
					  //mirror padding of the boundary
					  if(ii < 0) {ii = -ii-1;if(ii >= w) continue;}
					  if(ii >= w){ii =w - ii + w - 1;if(ii < 0) continue;}

					  *(pdata+iii) = (float)*(p2 + ii) * FACTOR_;
				  }
			  }
			  
		  }
		  //timer.start();
		  fftwf_execute(forward_plan);
	  
		  /* Filtering the block using frequency log filter */
#ifdef MIXED
		  for(s = s_min; s < s_max ; ++s)
		  {
			  complex_mutil(fblock, get_filter(f, f->flog_filter, s), fblock_flog, fblocksize, fblockw / 2 + 1, normFactors[s-s_min]);			 

			  fftwf_execute(backward_plan);

			  //set block
			  pLoG = f->log_response + (s - s_min) * extrema_size;
			  pdata = inmat + extrema_sy * fblockw;
			  float t;
			  for(j = 0, jjj = extrema_sy; j < extrema_h; j++, jjj++)
			  {
				  for(i = 0, iii = extrema_sx; i < extrema_w; i++, iii++)
				  {
					  t = 0;
					  if(iii - 1 >= 0) t += *(pdata+iii) - *(pdata+iii-1);
					  if(iii + 1 < fblockw) t += *(pdata+iii) - *(pdata+iii+1);
					  if(jjj - 1 >= 0) t += *(pdata+iii) - *(pdata+iii-fblockw);
					  if(jjj + 1 < fblockw) t += *(pdata+iii) - *(pdata+iii+fblockw);
					  *(pLoG+i) = (element_type)(t * fft_scale_log_factor);
				  }
				  pLoG += extrema_w;
				  pdata += fblockw;
			  }
		  }
#else
		  for(s = s_min; s < s_max ; ++s)
		  {
			  complex_mutil(fblock, get_filter(f, f->flog_filter, s), fblock_flog, fblocksize, fblockw / 2 + 1, normFactors[s-s_min]);			 

			  fftwf_execute(backward_plan);

			  //set block
			  pLoG = f->log_response + (s - s_min) * extrema_size;
			  pdata = inmat + extrema_sy * fblockw;

			  for(j = 0, jjj = extrema_sy; j < extrema_h; j++, jjj++)
			  {
				  for(i = 0, iii = extrema_sx; i < extrema_w; i++, iii++)
					  *(pLoG+i) = (element_type)((*(pdata+iii)) * fft_scale_log_factor);
				  pLoG += extrema_w;
				  pdata += fblockw;
			  }
		  }
#endif

		  st_key = keypoint_extrema_block(f, extrema_w, extrema_h, v_mode, h_mode, R_Extrema);

		  cnt += st_key;
		  st_key = f->nkeys - st_key;     //the keypoint detected of this octave within this block

		  /* Filtering the block using gaussian filters */
		  for (s = s_min; s < s_min+S; ++s)
		  {
			  complex_mutil(fblock, get_filter(f, f->fgaussian_filter, s), fblock_flog, fblocksize, fblockw / 2 + 1, FACTOR);

			  fftwf_execute(backward_plan);

			  //set block

			  //////////////////////////////////////////////////////////////////////////
			  //save the data to the block_buffer;
			  p = f->block_buffer + (s - s_min) * f->block_buffer_len * BLOCK_SIZE + f->block_map[block_cnt];
			  pdata = inmat + padWidth * fblockw;

				for(j = 0, jjj = padWidth; j < block_height; j++, jjj++)
				{
					for(i = 0, iii = padWidth; i < block_width; i++, iii++)
						*(p+i) = ((gs_type)((int)((*(pdata+iii)) * fft_scale_factor + 0.5)));

					p += blockw;
					pdata += fblockw;
				}

			  //Copy the last scale to the data and downsampling
			  if(o_cur < f->O - 1 && s == s_min + S - 1)
			  {
				    pdata = inmat + padWidth * fblockw;
					int W = w;
					if(o_cur == 0) W = w2;
					pt2 = data + y / 2 * W;
					for(j = y / 2, jjj = padWidth; jjj < blockw + padWidth && j < h2; j++, jjj += 2)
					{
						for(i = x / 2, iii = padWidth; iii < blockw + padWidth && i < w2; i++, iii += 2)
							*(pt2+i) = ((input_type)((int)((*(pdata+iii)) * fft_scale_factor + 0.5)));

						pdata += fblockw + fblockw;
						pt2 += W;
					}
			  }//*/

			  for(i = st_key; i < f->nkeys; i++)
			  {
				  int w;
				  if(f->keys[i].is != s+1)
					  continue;
				  k = f->keys + i;
				  k->block_id = block_cnt;
				  k->ix += extrema_sx - padWidth;
				  k->iy += extrema_sy - padWidth;
				  k->x += extrema_sx - padWidth;
				  k->y += extrema_sy - padWidth;
				  k->mode = 0;

				  w = (int)(k->sigma * 10.6066 + 0.5);

				  if(!(h_mode & 1) && k->ix - w < 0)
					  k->mode |= MODE_LEFT;
				  if(!(h_mode & 2) && k->ix + w >= block_width)
					  k->mode |= MODE_RIGHT;
				  if(!(v_mode & 1) && k->iy - w < 0)
					  k->mode |= MODE_TOP;
				  if(!(v_mode & 2) && k->iy + w >= block_height)
					  k->mode |= MODE_BOTTOM;
			  }
		  }
		  keypoint_block_desc(f, block_cnt, block_width, block_height, featurelist, xper);//*/
		  block_cnt++;
	  }//end of block x
  }//end of block y
  if(o_cur > 0)
  {
	  p2 = data;
	  pt2 = data;
	  for(j = 0; j < h2; j++)
	  {
		  for(i = 0; i < w2; i++)
			  *(pt2+i) = *(p2+i);
		  p2 += w;
		  pt2 += w2;
	  }
  }
  return 0 ;
}