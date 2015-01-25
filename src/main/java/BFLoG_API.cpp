#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include "cly753_process_BFLoG_API.h"
#include "BFLoGDetector.h"
using namespace std;

JNIEXPORT jint JNICALL Java_cly753_process_BFLoG_1API_Extract
  (JNIEnv * env, jobject obj, jbyteArray jdata, jint jwidth, jint jheight)
{
	jbyte * olddata = (jbyte*)env->GetByteArrayElements(jdata, 0);
	unsigned char* bytearr = (unsigned char*)olddata;

	BFLoGDetector detector;
	detector.Init(bytearr, (int)jwidth, (int)jheight);
	detector.detect();
	int numOfPoints = detector.features.size();
	env->ReleaseByteArrayElements(jdata, olddata, 0);
	return numOfPoints;
}
