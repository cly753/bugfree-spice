#include "Wrap.h"

#include <iostream>
#include "jni/BFLoG/BFLoGDetector.h"


int Extract(unsigned char*, int, int);

JNIEXPORT void JNICALL Java_Wrap_sayHi(JNIEnv *, jobject) {
	std::cout << "hi" << std::endl;
}

JNIEXPORT jint JNICALL Java_Wrap_process(JNIEnv *env, jobject o_j, jbyteArray data_jbyteArray, jint width_jint, jint height_jint) {
	jbyte * data_jbyte = (jbyte*)env->GetByteArrayElements(data_jbyteArray, 0);

	char* result;
	int size = Extract((unsigned char*)data_jbyte, (int)width_jint, (int)height_jint);
	env->ReleaseByteArrayElements(data_jbyteArray, data_jbyte, 0);
	return size;
}

int Extract(unsigned char * data, int width, int height) {
	// BFLoGDetector detector;

	// detector.Init(data, width, height);
	// detector.detect();

	// int numOfPoints = detector.features.size();

	return 1;
}