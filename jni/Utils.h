#ifndef JNISAMPLE_UTILS_H
#define JNISAMPLE_UTILS_H

#include <jni.h>

char* jstring2Cstring(JNIEnv* env, jstring jstr);
jstring cstring2Jstring(JNIEnv* env, const char* cstr);

void throwByName(JNIEnv* env, const char* name, const char* msg);

jvalue callMethodByName(
    JNIEnv* env,
    jboolean* hasException,
    jobject obj,
    const char* name,
    const char* descriptor,
    ...
);

#endif
