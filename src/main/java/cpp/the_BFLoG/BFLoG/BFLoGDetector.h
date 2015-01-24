/*

 */

#pragma once
#include "DescriptorBase.h"
#include "sift_bflog.h"
using namespace std;
/**
 * @class BFLoGDetector
 * Implementation of the Block based frequency domain LoG keypoint detector and descriptor.
 * @author  Jie Chen
 * @date 2013.12.23
 */
class BFLoGDetector: public DescriptorBase
{
public:
	BFLoGDetector();

	/**
	 * Detect all DoG keypoints from this image.
	 */
	void detect();

	/**
	 * Extract the SIFT descriptor of each keypoint and store it back.
	 * @param num the absolute maximum number of features to be extracted from this image
	 */
	void extract(int num);

	/**
	 * Sort the given features in descending order of importance.
	 */
	void sortKeypoints();

private:
	// constants used for feature selection
	static const float DistC[];
	static const float DistP[];
	static const float ScaleC[];
	static const float ScaleP[];
	static const float OrientC[];
	static const float OrientP[];
	static const float PeakC[];
	static const float PeakP[];
	static const float DetHessianC[];
	static const float DetHessianP[];
	static const float CurvRatioC[];
	static const float CurvRatioP[];
};

