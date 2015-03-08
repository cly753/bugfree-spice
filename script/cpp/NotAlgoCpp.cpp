#include <stdio.h>
#include <stdlib.h>
#include <jni.h>
#include "cly753_process_NotAlgoCpp.h"
#include "NotAlgoCppInterface.hpp"

using namespace std;

JNIEXPORT jint JNICALL Java_cly753_process_NotAlgoCpp_Extract
  (JNIEnv * env, jobject obj, jbyteArray jdata, jint jwidth, jint jheight, jint jtype)
{
    jbyte * olddata = (jbyte*)env->GetByteArrayElements(jdata, 0);
    unsigned char* bytearr = (unsigned char*)olddata;

    //////
    NotAlgoCpp* algo = getAlgo();
    int result = algo->process(bytearr, (int)jwidth, (int)jheight, (int)jtype);
    delete algo;
    //////

    env->ReleaseByteArrayElements(jdata, olddata, 0);
    return result;
}
