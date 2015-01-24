/*
 * This software module was originally developed by:

 *
 */

#include "BFLoGDetector.h"
#include <vector>
#include <iostream>
#include <algorithm>
// exceptions
#define VL_PI 3.141592653589793

using namespace std;

BFLoGDetector::BFLoGDetector()
{

}
/*
 * Detect keypoints using the DoG algorithm.
 *
 * DoG: Different of Gaussian
 *
 * keypoint information includes:
 * - Horizontal coordinate
 * - Vertical coordinate
 * - Scale
 * - Orientation
 * - Peak
 */
void BFLoGDetector::extract(int num)
{
	if (num > features.size())
		num = features.size();	// cannot extract more points than those available!

	if (numExtracted >= num)
		return;			// nothing to do: the required number of features have already been extracted
	
	numExtracted = num;		// next time start extracting from num
}


void BFLoGDetector::detect()
{
	/* algorithm default parameters */
	int O		= - 1 ;
	int S		=   3 ;
	int o_min	=   0 ;
	SiftFilt *filt = 0 ;

	if (!data) return;
	filt = sift_new (width, height, O, S, o_min) ;
	if (!filt) return;
	// Get image data
	filt->data0 = data;
	for(int filterIterator = 0; filterIterator < filt->O; ++filterIterator)
	{
		filt->nkeys = 0 ;
		keypoint_detect(filt, filterIterator, features);
	}
	/* release filter */
	if (filt)
	{
		sift_delete(filt) ;
		filt = NULL ;
	}
	sortKeypoints();
}

const float BFLoGDetector::DistC[] = {
	236.6142425537f, 228.2534637451f, 211.4427642822f, 219.8725585938f, 202.9744262695f, 245.5096893311f, 194.4166564941f, 185.7111358643f, 254.7625732422f, 159.0561828613f,
	177.0009307861f, 264.3377075195f, 168.1789245605f, 274.1414184570f, 284.2149963379f, 149.8331451416f, 316.0087585449f, 294.4043579102f, 304.9752197266f, 140.3899230957f,
	130.6065673828f, 120.4344406128f, 109.8616027832f,  98.7058868408f,  86.8511657715f, 329.2155761719f,  74.0838470459f,  60.0149383545f, 345.9376220703f,  43.5756835938f,
	22.4543666840f, 369.2438964844f
};

const float BFLoGDetector::DistP[] = {
	0.0220398754f,  0.0238624662f,  0.0276106168f,  0.0261496734f,  0.0288298205f,  0.0219966639f,  0.0306943282f,  0.0331666134f,  0.0213320162f,  0.0372393429f,  // 10
	0.0342464559f,  0.0202504732f,  0.0362297818f,  0.0194976330f,  0.0186509714f,  0.0393599831f,  0.0127053130f,  0.0170345809f,  0.0152353840f,  0.0412544832f,  // 20
	0.0429147556f,  0.0442708395f,  0.0463888831f,  0.0474954657f,  0.0487268977f,  0.0119834812f,  0.0506855324f,  0.0524494164f,  0.0103358822f,  0.0545994230f,  // 30
	0.0559572875f,  0.0068056579f
};

const float BFLoGDetector::ScaleC[] = {
	2.0037319660f,   2.5626039505f,   3.3531446457f,   4.6013789177f,   6.6070671082f,  10.1078004837f,  16.8908882141f,  33.2625312805f
};

const float BFLoGDetector::ScaleP[] = {
	0.1809079796f,  0.2700507343f,  0.3348271549f,  0.4203188419f,  0.4220668674f,  0.4392355382f,  0.3710923493f,  0.3111107647f
};

const float BFLoGDetector::OrientC[] = {
	-1.5368843079f, 1.4983994961f, -4.6412000656f, -0.0169844721f, -1.6649979353f, -3.1754138470f,  0.1449248195f, -3.0298063755f, -1.3944710493f,  1.3232766390f,
	-3.3388459682f,-4.4705252647f, -1.8167932034f, -0.1744092405f, -1.2183139324f,  0.3518850803f, -2.8577301502f,  1.1060111523f, -4.2607388496f, -3.5427241325f,
	-1.9989310503f,-0.3603723645f, -1.0142878294f, -4.0234694481f,  0.8552971482f,  0.5963328481f, -2.6501891613f, -3.7760767937f, -2.2037825584f, -0.5725694895f,
	-2.4249286652f,-0.7950484753f
};

const float BFLoGDetector::OrientP[] = {
	0.2052661479f,  0.2003512084f,  0.2017586380f,  0.1751748621f,  0.1990270615f,  0.1747285426f,  0.1745616049f,  0.1758822352f,  0.1916013956f,  0.1793494225f,  // 10
	0.1792997271f,  0.1788771898f,  0.1834550649f,  0.1745595038f,  0.1807001084f,  0.1745832562f,  0.1684573591f,  0.1673879176f,  0.1615773141f,  0.1694945097f,  // 20
	0.1749896258f,  0.1648799777f,  0.1693636328f,  0.1672000736f,  0.1666287184f,  0.1736336648f,  0.1676661670f,  0.1665415317f,  0.1717979163f,  0.1691450924f,  // 30
	0.1697088629f,  0.1677394360f
};

const float BFLoGDetector::PeakC[] = {
	0.2547715306f, 0.7862316966f,  1.4677816629f,  2.2217512131f,  3.0260436535f,  3.8845551014f,  4.7957448959f,  5.7748422623f,  6.8275876045f,  7.9892172813f,
	9.2980575562f,10.8110599518f, 12.6346817017f, 14.9174613953f, 18.1477069855f, 23.8209209442f

};

const float BFLoGDetector::PeakP[] = {
	0.0372311398f,  0.0892269239f,  0.1461775154f,  0.1787834764f,  0.1981789768f,  0.2148804218f,  0.2268953770f,  0.2374276817f,  0.2490262240f,  0.2592820823f,  // 10
	0.2710254490f,  0.2913467288f,  0.3080614507f,  0.3262100518f,  0.3563414216f,  0.3579748571f
};//*/
const float BFLoGDetector::CurvRatioC[] = {
	4.096389f, 4.318419f, 4.572773f, 4.860124f, 5.181817f, 5.540463f, 5.937543f, 6.374206f, 6.853724f, 7.379163f, 7.952244f, 8.574980f, 9.252090f, 9.986070f, 10.780889f, 11.641243f 
};

const float BFLoGDetector::CurvRatioP[] = {
	0.292347f, 0.291691f, 0.290343f, 0.288487f, 0.287424f, 0.283554f, 0.280145f, 0.275622f, 0.269978f, 0.262611f, 0.254009f, 0.242384f, 0.227699f, 0.209303f, 0.188510f, 0.165667f 
};


void BFLoGDetector::sortKeypoints()
{
	/*if(features.size()>0)
	{
		float distFromCenter;
		float halfImageWidth = (float)width/2;
		float halfImageHeight = (float)height/2;

		for(std::vector<Feature>::iterator p=features.begin(); p<features.end(); ++p)
		{
			distFromCenter = sqrt(pow(p->x-halfImageWidth, 2)+pow(p->y-halfImageHeight, 2));
			p->pdf = DistP[scalarQuantize(distFromCenter, DistC, sizeof(DistC)/sizeof(float))];
			p->pdf = p->pdf * ScaleP[scalarQuantize(p->scale, ScaleC, sizeof(ScaleC)/sizeof(float))];
			p->pdf = p->pdf * OrientP[scalarQuantize(p->orientation, OrientC, sizeof(OrientC)/sizeof(float))];
			p->pdf = p->pdf * PeakP[scalarQuantize(p->peak, PeakC, sizeof(PeakC)/sizeof(float))];
			p->pdf = p->pdf * CurvRatioP[scalarQuantize(p->curvRatio, CurvRatioC, sizeof(CurvRatioC)/sizeof(float))];
		}

		std::stable_sort(features.begin(), features.end(), sortPdfPredicate);
	}//*/

	if(features.size()>0)
	{
		float distFromCenter;
		float diffx, diffy;
		float halfImageWidth = (float) width/2;
		float halfImageHeight = (float) height/2;

		for(std::vector<Feature>::iterator p=features.begin(); p<features.end(); ++p)
		{
			diffx = p->x - halfImageWidth;
			diffy = p->y - halfImageHeight;
			distFromCenter = sqrt(diffx*diffx + diffy*diffy);

			p->pdf = fastScalarQuantize(distFromCenter, DistC, DistP, sizeof(DistC)/sizeof(float))
				   * fastScalarQuantize(p->scale, ScaleC, ScaleP, sizeof(ScaleC)/sizeof(float))
				   * fastScalarQuantize(p->orientation, OrientC, OrientP, sizeof(OrientC)/sizeof(float))
				   * fastScalarQuantize(fabs(p->peak), PeakC, PeakP, sizeof(PeakC)/sizeof(float))
				   * fastScalarQuantize(fabs(p->curvRatio), CurvRatioC, CurvRatioP, sizeof(CurvRatioC)/sizeof(float));
		}

		std::stable_sort(features.begin(), features.end(), sortPdfPredicate);
	}//*/
}
