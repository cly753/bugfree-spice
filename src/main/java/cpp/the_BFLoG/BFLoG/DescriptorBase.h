/*
 *
 */

#pragma once
#include <iostream>
#include <fstream>
#include <vector>
#include <memory.h>
#include <math.h>
using namespace std;

#define DES_LENGTH 128
/**
 * @class Feature
 * Container class for the SIFT features of a single point (storing coordinates, scale, orientation, peak, curvRtio and descriptor of a point). 
 * @author Jie Chen
 * @date 2013.12.23
 */
class Feature
{
public:
	float x;				  ///< the X coordinate of the SIFT point
	float y;				  ///< the Y coordinate of the SIFT point
	float scale;			  ///< the scale of the SIFT point
	float orientation;		  ///< the orientation of the SIFT point
	float peak;				  ///< the peak of the SIFT point
	float curvRatio;		  ///< the ratio of the curvatures
	float descr[DES_LENGTH]; ///< the descriptor of the SIFT point
	float pdf;				  ///< probability of this point to be matched
	int ioctave;

	Feature(void)
	{
		x = 0;	
		y = 0;			
		scale = 0;    
		orientation = 0; 
		peak = 0;
		curvRatio = 0;
		pdf = 0;
	}
	int getSize()
	{
		int size = 0;
		size += sizeof(x);
		size += sizeof(y);
		size += sizeof(scale);
		size += sizeof(orientation);
		size += sizeof(peak);
		size += sizeof(curvRatio);
		size += sizeof(descr);
		size += sizeof(pdf);
		return size;
	}
	// default copy constructor, assignment op, and destructor;
	int toStream(char * stream)
	{
		int size = 0;
		memcpy(stream, (char *)&x, sizeof(x)); stream += sizeof(x); size += sizeof(x);
		memcpy(stream, (char *)&y, sizeof(y)); stream += sizeof(y); size += sizeof(y);
		memcpy(stream, (char *)&scale, sizeof(scale)); stream += sizeof(scale); size += sizeof(scale);
		memcpy(stream, (char *)&orientation, sizeof(orientation)); stream += sizeof(orientation); size += sizeof(orientation);
		memcpy(stream, (char *)&peak, sizeof(peak)); stream += sizeof(peak); size += sizeof(peak);
		memcpy(stream, (char *)&curvRatio, sizeof(curvRatio)); stream += sizeof(curvRatio); size += sizeof(curvRatio);
		memcpy(stream, (char *)&descr, sizeof(descr)); stream += sizeof(descr); size += sizeof(descr);
		memcpy(stream, (char *)&pdf, sizeof(pdf)); stream += sizeof(pdf); size += sizeof(pdf);
		return size;
	}
	/**
	 * Write the feature into a file.
	 * @param file the output file.
	 */
	void toFile(FILE * file) const
	{
		size_t fout;
		fout = fwrite(&x, sizeof(x), 1, file);
		fout = fwrite(&y, sizeof(y), 1, file);
		fout = fwrite(&scale, sizeof(scale), 1, file);
		fout = fwrite(&orientation, sizeof(orientation), 1, file);
		fout = fwrite(&peak, sizeof(peak), 1, file);
		fout = fwrite(&curvRatio, sizeof(curvRatio), 1, file);
		fout = fwrite(&descr, sizeof(descr), 1, file);
		fout = fwrite(&pdf, sizeof(pdf), 1, file);
	}

	/**
	 * Write the feature into a file.
	 * @param file the output file.
	 */
	void fromFile(FILE * file)
	{
		size_t fout;
		fout = fread(&x, sizeof(x), 1, file);
		fout = fread(&y, sizeof(y), 1, file);
		fout = fread(&scale, sizeof(scale), 1, file);
		fout = fread(&orientation, sizeof(orientation), 1, file);
		fout = fread(&peak, sizeof(peak), 1, file);
		fout = fread(&curvRatio, sizeof(curvRatio), 1, file);
		fout = fread(&descr, sizeof(descr), 1, file);
		fout = fread(&pdf, sizeof(pdf), 1, file);
	}
};


/**
 * @class DescriptorBase
 * A container class for a gray image; it's the base class of all keypoint detector classes.
 * @author  Jie Chen
 * @date 2013.12.23
 */
class DescriptorBase
{
public:
	int height;						///< height of the image
	int width;						///< width of the image
	unsigned char * data;			///< image data
	vector<Feature> features;		///< the vector of SIFT features that describe the image.
	int numExtracted;				///< number of features already extracted from the source image

	DescriptorBase()
	{
		height = 0;
		width = 0;
		data = NULL;
		numExtracted = 0;
		features.clear();
	}

	virtual ~DescriptorBase()
	{

	}

	void Init(unsigned char * d, int w, int h)
	{
		height = h;
		width = w;
		data = d;
		numExtracted = 0;
		features.clear();
	}

	/**
	 * Detect all keypoints from this image.
	 * @param featurelist the ouput list of keypoints with their associated features, in descending order of importance.
	 * @param params the running parameters.
	 * @throws CdvsException in case of error
	 */
	virtual void detect()
	{

	}

	/**
	 * Extract the SIFT descriptor of each keypoint and store it back in featurelist.
	 * @param featurelist the detected keypoints
	 * @param num the absolute maximum number of features to be extracted from this image
	 */
	virtual void extract(int num)
	{

	}

	virtual void sortKeypoints()
	{

	}
	int getSize()
	{
		int total_size = 0;
		total_size += sizeof(width);
		total_size += sizeof(height);
		unsigned int size = features.size();
		total_size += sizeof(size);
		for(unsigned int i = 0; i < size; i++)
		{
			total_size += features[i].getSize();
		}
		return total_size;
	}
	int toStream(char * stream)
	{
		int total_size = 0;
		memcpy(stream, (char *)&width, sizeof(width)); stream += sizeof(width); total_size += sizeof(width);
		memcpy(stream, (char *)&height, sizeof(height)); stream += sizeof(height); total_size += sizeof(height);
		unsigned int size = features.size();
		memcpy(stream, (char *)&size, sizeof(size)); stream += sizeof(size); total_size += sizeof(size);
		for(unsigned int i = 0; i < size; i++)
		{
			int t = features[i].toStream(stream);
			stream += t;
			total_size += t;
		}
		return total_size;
	}
	/**
	 * Write the entire Features into a file.
	 * @param filename the output filename.
	 */
	void toFile(const char * filename) const
	{
		FILE * file;
		fwrite(&width, sizeof(width), 1, file);
		fwrite(&height, sizeof(height), 1, file);
		// now save the vector of features
		unsigned int size = features.size();
		fwrite(&size, sizeof(size), 1, file);
		for(unsigned int i=0; i<size; i++)
		{
			features[i].toFile(file);
		}
		fclose(file);
	}

	/**
	 * Read the entire Features from a file.
	 * @param filename the input filename.
	 */
	void fromFile(const char * filename)
	{
		FILE * file;
		fread(&width, sizeof(width), 1, file);
		fread(&height, sizeof(height), 1, file);
		// now read the vector of features
		features.clear();
		unsigned int size;
		fread(&size, sizeof(size), 1, file);
		for(unsigned int i=0; i<size; i++)
		{
			Feature feature;
			feature.fromFile(file);
			features.push_back(feature);
		}
		numExtracted = features.size();		// all key points are extracted before storing the SIFT file
		fclose(file);
	}

	static bool sortPdfPredicate(const Feature &f1, const Feature &f2)
	{
		return f1.pdf > f2.pdf;
	}

	/**
	 * Perform scalar quantization on given data.
	 * @param value the value to quantize
	 * @param data the quantization centroids
	 * @param size the size of the quantization centroid array
	 * @return the index such that the distance between value and data[index] is minimum
	 */
	static unsigned int scalarQuantize(float value, const float *data, size_t size)
	{
		unsigned int bt;
		if(size>0)
		{
			float minDist = 1e10f;

			for(int i=0; i<size; ++i)
			{
				float dist = fabs(value - data[i]);
				if(dist<minDist)
				{
					bt = i;
					minDist = dist;
				}
			}
		}
		return bt;
	}

	/**
	 * Perform scalar quantization on given data and return the corresponding output.
	 * @param value the value to quantize
	 * @param data the quantization centroids
	 * @param output the corresponding output values (probabilities)
	 * @param size the size of the quantization centroid array
	 * @return the probability corresponding to the given value
	 */
	static float fastScalarQuantize(float value, const float *data, const float *output, size_t size)
	{
		int first = 0;
		int count = (int) size;
		while (count > 0)
		{
			int step = count / 2;
			int k = first + step;
			if (value < data[k])
				count = step;
			else
			{
				first = k + 1;
				count -= step + 1;
			}
		}

		// first and last are special cases

		if (first == 0)
			return output[0];

		if (first == (int) size)
			return output[first - 1];

		// all other cases are decided here

		if ((data[first] - value) < (value - data[first - 1]))
			return output[first];
		else
			return output[first - 1];
	}

	/**
	 * Perform scalar quantization then interpolate the output.
	 * @param value the value to quantize
	 * @param data the quantization centroids
	 * @param output the corresponding output values (probabilities)
	 * @param size the size of the quantization centroid array
	 * @return the interpolated probability corresponding to the given value
	 */
	static float fastInterpolate(float value, const float *data, const float *output, size_t size)
	{
		int first = 0;
		int count = (int) size;
		while (count > 0)
		{
			int step = count / 2;
			int k = first + step;
			if (value < data[k])
				count = step;
			else
			{
				first = k + 1;
				count -= step + 1;
			}
		}

		// first and last are special cases

		if (first == 0)
			return output[0];

		if (first == (int) size)
			return output[first - 1];

		// all other cases are interpolated here

		float norm = data[first] - data[first - 1];
		float weight1 = data[first] - value;			// distance from higher value ==> weight for lower value
		float weight2 = value - data[first - 1];		// distance from lower value ==> weight for higher value

		return (weight2*output[first] + weight1*output[first - 1]) / norm;
	}
	void select(const std::vector<int> &indices)
	{
		if(indices.size() > 0)
		{
			std::vector<Feature> ltemp;

			for(std::vector<int>::const_iterator p=indices.begin(); p<indices.end(); ++p)
			{
				ltemp.push_back(features[*p]);
			}

			features = ltemp;
			ltemp.clear();
		}
		else
		{
			features.clear();
		}
	}


	void selectFromTo(int startInd, int endInd)
	{
		if(startInd < 0 || startInd > endInd || startInd >= features.size())
		{
			return;
		}
		std::vector<int> v(endInd-startInd+1);
		for(std::vector<int>::iterator p=v.begin(); p<v.end(); ++p)
		{
			*p = startInd++;
		}

		select(v);

		v.clear();
	}

	void selectFirst(int n)
	{
		if (n < features.size())
			features.erase(features.begin() + n, features.end());
	}


private:


};

