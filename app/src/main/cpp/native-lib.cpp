#include <jni.h>
#include <string>

std::string jstring_to_string(JNIEnv *pEnv, jstring jStr);

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_hellojni_MainActivity_stringFromJNI(
        JNIEnv* env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_hellojni_MainActivity_helloFromJNI(
        JNIEnv* env,
        jobject,
        jstring jStr) {
    std::string str = jstring_to_string(env, jStr);
    std::string ret = "Hello " + str;
    return env->NewStringUTF(ret.c_str());
}


std::string jstring_to_string(JNIEnv *env, jstring jStr) {
    if (!jStr) return "";
    const jclass stringClass = env->GetObjectClass(jStr);
    const jmethodID getBytes = env->GetMethodID(stringClass, "getBytes", "(Ljava/lang/String;)[B");
    const auto stringJbytes = (jbyteArray) env->CallObjectMethod(jStr, getBytes, env->NewStringUTF("UTF-8"));
    auto length = size_t(env->GetArrayLength(stringJbytes));
    jbyte* pBytes = env->GetByteArrayElements(stringJbytes, nullptr);

    std::string ret = std::string((char *)pBytes, length);
    env->ReleaseByteArrayElements(stringJbytes, pBytes, JNI_ABORT);

    env->DeleteLocalRef(stringJbytes);
    env->DeleteLocalRef(stringClass);
    return ret;
}