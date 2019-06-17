#include <jni.h>
#include <string>
#include <iostream>

static jclass classInteger;
static jmethodID midIntegerInit;

jobject getInteger(JNIEnv *env, jobject thisObj, jint number) {
    if (NULL == classInteger) {
        printf("Find java.lang.Integer\n");
        jclass classIntegerLocal = env->FindClass("java/lang/Integer");
        classInteger = static_cast<jclass>(env->NewGlobalRef(classIntegerLocal));
        env->DeleteLocalRef(classIntegerLocal);
    }

    if (NULL == classInteger) return NULL;

    if (NULL == midIntegerInit) {
        printf("Get Method ID for java.lang.Integer's constructor\n");
        midIntegerInit= env->GetMethodID(classInteger, "<init>", "(I)V");
    }

    if (NULL == midIntegerInit) return NULL;

    jobject newObj = env->NewObject(classInteger, midIntegerInit, number);
    printf("In C, constructed java.lang.Integer with number %d\n", number);
    return newObj;
}

extern "C" JNIEXPORT jobject JNICALL Java_com_example_jni_MainActivity_getIntegerObject
        (JNIEnv *env, jobject thisObj, jint number) {
    return getInteger(env, thisObj, number);
}

extern "C" JNIEXPORT jobject JNICALL Java_com_example_jni_MainActivity_anotherGetIntegerObject
        (JNIEnv *env, jobject thisObj, jint number) {
    return getInteger(env, thisObj, number);
}

extern "C" JNIEXPORT jobjectArray JNICALL
Java_com_example_jni_MainActivity_sumAndAverage2(
        JNIEnv* env,
        jobject thisObj,
        jobjectArray inJNIArray
        ) {
    jclass classInteger = env->FindClass("java/lang/Integer");
    jmethodID midIntValue = env->GetMethodID(classInteger, "intValue", "()I");
    if (NULL == midIntValue) return NULL;

    jsize length = env->GetArrayLength(inJNIArray);
    jint sum = 0;
    int i;
    for (i = 0; i < length; i++) {
        jobject objInteger = env->GetObjectArrayElement(inJNIArray, i);
        if (NULL == objInteger) return NULL;
        jint value = env->CallIntMethod(objInteger, midIntValue);
        sum += value;
    }

    double average = (double)sum / length;
    printf("In C, the sum is %d\n", sum);
    printf("In C, the average is %f\n", average);

    // Get a class reference for java.lang.Double
    jclass classDouble = env->FindClass("java/lang/Double");

    // Allocate a jobjectArray of 2 java.lang.Double
    jobjectArray outJNIArray = env->NewObjectArray(2, classDouble, NULL);

    // Construct 2 Double objects by calling the constructor
    jmethodID midDoubleInit = env->GetMethodID(classDouble, "<init>", "(D)V");
    if (NULL == midDoubleInit) return NULL;
    jobject objSum = env->NewObject(classDouble, midDoubleInit, (double)sum);
    jobject objAve = env->NewObject(classDouble, midDoubleInit, average);

    // Set to the jobjectArray
    env->SetObjectArrayElement(outJNIArray, 0, objSum);
    env->SetObjectArrayElement(outJNIArray, 1, objAve);

    return outJNIArray;
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_jni_MainActivity_nativeMethod(
        JNIEnv *env,
        jobject thisObj
        ) {
    jclass thisClass = env->GetObjectClass(thisObj);

    jmethodID midCallback = env->GetMethodID(thisClass, "callback", "()V");
    if(NULL == midCallback) return;
    env->CallVoidMethod(thisObj, midCallback);

    jmethodID  midCallbackStr = env->GetMethodID(thisClass, "callback", "(Ljava/lang/String;)V");
    if(NULL == midCallbackStr) return;
    jstring message = env->NewStringUTF("Hello from C");
    env->CallVoidMethod(thisObj, midCallbackStr, message);

    jmethodID midCallbackAverage = env->GetMethodID(thisClass, "callbackAverage", "(II)D");
    if(NULL == midCallbackAverage) return;
    jdouble average = env->CallDoubleMethod(thisObj, midCallbackAverage, 2, 3);

    jmethodID midCallbackStatic = env->GetStaticMethodID(thisClass, "callbackStatic", "()Ljava/lang/String;");
    if(NULL == midCallbackStatic) return;
    jstring resultJNIStr = static_cast<jstring>(env->CallStaticObjectMethod(thisClass, midCallbackStatic));
    const char* resultCStr = env->GetStringUTFChars(resultJNIStr, NULL);
    if(NULL == resultCStr) return;
    env->ReleaseStringUTFChars(resultJNIStr, resultCStr);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_jni_MainActivity_modifyStaticVariable(
        JNIEnv *env,
        jobject  thisObj
        ) {
    jclass  cls = env->GetObjectClass(thisObj);
    jfieldID fidNumber = env->GetStaticFieldID(cls, "staticNumber", "D");
    if(NULL == fidNumber) return;

    jdouble number = env->GetStaticDoubleField(cls, fidNumber);
    number = 77.88;
    env->SetStaticDoubleField(cls, fidNumber, number);
}

extern "C" JNIEXPORT void JNICALL
Java_com_example_jni_MainActivity_modifyInstanceVariable(
        JNIEnv *env,
        jobject  thisObj
        ) {
    jclass thisClass = env->GetObjectClass(thisObj);
    jfieldID fidNumber = env->GetFieldID(thisClass, "number", "I");
    if(NULL == fidNumber) return;

    jint number = env->GetIntField(thisObj, fidNumber);
    number = 99;
    env->SetIntField(thisObj, fidNumber, number);

    jfieldID fidMessage = env->GetFieldID(thisClass, "message", "Ljava/lang/String;");
    if(NULL == fidMessage) return;

    jstring message = static_cast<jstring>(env->GetObjectField(thisObj, fidMessage));
    const char* cStr = env->GetStringUTFChars(message, NULL);
    if(NULL == cStr) return;

    env->ReleaseStringUTFChars(message, cStr);
    message = env->NewStringUTF("Hello from C");
    if(NULL == message) return;

    env->SetObjectField(thisObj, fidMessage, message);

}

extern "C" JNIEXPORT jdoubleArray JNICALL
Java_com_example_jni_MainActivity_sumAndAverage(
        JNIEnv *env,
        jobject jobj,
        jintArray intArr
        ) {
    jint *inCArry = env->GetIntArrayElements(intArr, NULL);
    if(NULL == inCArry) return NULL;
    jsize length = env->GetArrayLength(intArr);

    jdouble sum = 0;
    int i;
    for(i = 0; i < length; ++i)
    {
        sum += inCArry[i];
    }

    jdouble average = sum / length;
    env->ReleaseIntArrayElements(intArr, inCArry, 0);
    jdouble outCArray[] = {sum, average};
    jdoubleArray outJNIArray = env->NewDoubleArray(2);
    if(NULL == outJNIArray) return NULL;
    env->SetDoubleArrayRegion(outJNIArray, 0, 2, outCArray);

    return outJNIArray;
}

extern "C" JNIEXPORT jstring JNICALL
Java_com_example_jni_MainActivity_sayHello(
        JNIEnv *env,
        jobject jobj,
        jstring jstr
        ) {
    std::string hello = "Hello ";

    const char *str = env->GetStringUTFChars(jstr, NULL);
    if(str == NULL)
    {
        return NULL;
    }

    char outCStr[128];
    int index;
    for(index = 0; index < 6; ++index)
    {
        outCStr[index] = hello[index];
    }

    jsize length = env->GetStringUTFLength(jstr);
    for(; index - 6 < length; ++index)
    {
        outCStr[index] = str[index - 6];
    }

    outCStr[index] = '\0';
    env->ReleaseStringUTFChars(jstr, str);

    return env->NewStringUTF(outCStr);
}
