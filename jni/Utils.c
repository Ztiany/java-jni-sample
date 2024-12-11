#include "Utils.h"
#include <malloc.h>
#include <string.h>

/**
 * Java String 转换为 C 字符串，转换后的字符串是可以修改的。
 */
char* jstring2Cstring(JNIEnv* env, const jstring jstr) {
    char* result = NULL;
    const jclass clazz = (*env)->FindClass(env, "java/lang/String");
    const jstring encoding = (*env)->NewStringUTF(env, "UTF-8");
    jmethodID getBytes = (*env)->GetMethodID(env, clazz, "getBytes", "(Ljava/lang/String;)[B");
    const jbyteArray barr = (*env)->CallObjectMethod(env, jstr, getBytes, encoding);
    const jsize length = (*env)->GetArrayLength(env, barr);
    // 动态的获取内在堆内存中，需要被释放
    jbyte* byteArray = (*env)->GetByteArrayElements(env, barr, NULL);

    if (length > 0) {
        result = (char*)malloc(length + 1); // C 语言字符串以 \0 结尾。
        memcpy(result, byteArray, length);
        result[length] = 0; //让最后一个字符为 '\0',=，表示是字符串的结尾。
    }

    (*env)->ReleaseByteArrayElements(env, barr, byteArray, 0); //释放内存
    return result;
}

/**
 * 解决某些情况下可能的中文乱码的问题，调用 Java 中 String 的构造函数来创建字符串。
 */
jstring cstring2Jstring(JNIEnv* env, const char* cstr) {
    // 获取 String 的构造函数
    const jclass clazz = (*env)->FindClass(env, "java/lang/String");
    jmethodID constructor = (*env)->GetMethodID(
        env,
        clazz,
        "<init>",
        "([BLjava/lang/String;)V"
    );

    // 创建字节数组
    const jbyteArray bytes = (*env)->NewByteArray(env, strlen(cstr));

    // byte 数组赋值：从 cstr 这个字符数组，复制到 bytes 这个字符数组
    (*env)->SetByteArrayRegion(env, bytes, 0, strlen(cstr), cstr);

    // 字符编码 jstring
    const jstring encoding = (*env)->NewStringUTF(env, "UTF-8");

    // 调用构造函数，返回编码之后的 jstring
    return (*env)->NewObject(env, clazz, constructor, bytes, encoding);
}

void throwByName(JNIEnv* env, const char* name, const char* msg) {
    const jclass clazz = (*env)->FindClass(env, name);
    // if cls is NULL, an exception has already been thrown
    if (clazz != NULL) {
        (*env)->ThrowNew(env, clazz, msg);
    }
    // free the local ref
    (*env)->DeleteLocalRef(env, clazz);
}

jvalue callMethodByName(
    JNIEnv* env,
    jboolean* hasException,
    jobject obj,
    const char* name,
    const char* descriptor,
    ...
) {
    va_list args;
    jclass clazz;
    jmethodID mid;
    jvalue result;

    if ((*env)->EnsureLocalCapacity(env, 2) == JNI_OK) {
        clazz = (*env)->GetObjectClass(env, obj);
        mid = (*env)->GetMethodID(env, clazz, name, descriptor);

        if (mid) {
            const char* p = descriptor;
            /* skip over argument types to find out the  return type */
            while (*p != ')') {
                p++; /* skip ')' */
            }
            p++;
            va_start(args, descriptor); //可变参数的处理逻辑
            switch (*p) {
            case 'V':
                (*env)->CallVoidMethodV(env, obj, mid, args);
                break;
            case '[':
            case 'L':
                result.l = (*env)->CallObjectMethodV(env, obj, mid, args);
                break;
            case 'Z':
                result.z = (*env)->CallBooleanMethodV(env, obj, mid, args);
                break;
            case 'B':
                result.b = (*env)->CallByteMethodV(env, obj, mid, args);
                break;
            case 'C':
                result.c = (*env)->CallCharMethodV(env, obj, mid, args);
                break;
            case 'S':
                result.s = (*env)->CallShortMethodV(env, obj, mid, args);
                break;
            case 'I':
                result.i = (*env)->CallIntMethodV(env, obj, mid, args);
                break;
            case 'J':
                result.j = (*env)->CallLongMethodV(env, obj, mid, args);
                break;
            case 'F':
                result.f = (*env)->CallFloatMethodV(env, obj, mid, args);
                break;
            case 'D':
                result.d = (*env)->CallDoubleMethodV(env, obj, mid, args);
                break;
            default:
                (*env)->FatalError(env, "illegal descriptor");
            }
            va_end(args);
        }
        (*env)->DeleteLocalRef(env, clazz);
    }
    if (hasException) {
        *hasException = (*env)->ExceptionCheck(env);
    }
    return result;
}
