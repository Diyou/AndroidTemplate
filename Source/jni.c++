#include <jni.h>
#include <string>

extern "C"
JNIEXPORT jstring JNICALL
Java_com_diyou_dotcmake_MainActivity_stringFromJNI(JNIEnv *env, jobject thiz) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}