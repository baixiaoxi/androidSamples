package com.example.jni;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

public class MainActivity extends AppCompatActivity {

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("hello");
    }

    private static double staticNumber = 55.66;
    private static String tag = "BAI";

    private int number = 88;
    private String message = "Hello from java";

    private native void modifyStaticVariable();
    private native void modifyInstanceVariable();
    private native double[] sumAndAverage(int[] numbers);
    private native String sayHello(String input);
    private native void nativeMethod();
    private native Double[] sumAndAverage2(Integer[] numbers);
    private native Integer getIntegerObject(int number);
    private native Integer anotherGetIntegerObject(int number);

    private void callback() {
        Log.e(tag, "In Java");
    }

    private void callback(String message) {
        Log.e(tag, "In Java with " + message);
    }

    private double callbackAverage(int n1, int n2) {
        return ((double)n1 + n2) / 2.0;
    }

    private static String callbackStatic() {
        return "From static Java method";
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        String hello = sayHello("xi");
        Log.e(tag, "sayHello()=>" + hello);

        Log.e(tag, "**********");

        int[] numbers={22, 33, 33};
        double[] results = sumAndAverage(numbers);
        String resultMsg = "";
        for(double result:results) {
            resultMsg += result + " ";
        }
        Log.e(tag, resultMsg);

        Log.e(tag, "**********");

        modifyInstanceVariable();
        Log.e(tag, "In Java, int is " + number);
        Log.e(tag, "In Java, String is " + message);

        Log.e(tag, "**********");

        modifyStaticVariable();
        Log.e(tag, "In Java, the double is " + staticNumber);

        Log.e(tag, "**********");

        nativeMethod();

        Log.e(tag, "**********");

        Integer[] numbers2 = {11, 22, 32};
        Double[] results2 = sumAndAverage2(numbers2);
        Log.e(tag, "In Java, the sum is " + results2[0]);
        Log.e(tag, "In Java, the average is " + results2[1]);

        Log.e(tag, "**********");

        Log.e(tag, "" + getIntegerObject(1));
        Log.e(tag, "" + getIntegerObject(2));
        Log.e(tag, "" + anotherGetIntegerObject(1));
        Log.e(tag, "" + anotherGetIntegerObject(2));
        Log.e(tag, "" + getIntegerObject(3));
        Log.e(tag, "" + anotherGetIntegerObject(13));
    }
}
