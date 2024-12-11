package com.ztiany.jni.sample;

import java.util.Arrays;

/**
 * @author Ztiany
 */
public class JNIMain {

    private final JniBridge jniBridge = new JniBridge();

    static {
        System.loadLibrary("jni/native");
    }

    public static void main(String... args) {
        JNIMain jniMain = new JNIMain();
        jniMain.bubbleSort();
    }

    // 返回字符串
    private void stringFromC() {
        String stringFromC = JniBridge.stringFromC();
        System.out.println(stringFromC);
    }

    // 传递参数
    private void intFromC() {
        int a = 10;
        int b = 20;
        System.out.println("intFromC: " + (jniBridge.intFromC(a, b)));
    }

    // 修改每个元素后返回
    private void addArray() {
        int[] arr = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
        jniBridge.addArray(arr, 100);
        System.out.println("addArray: " + Arrays.toString(arr));
    }

    // 调用 C 实现的冒泡排序
    private void bubbleSort() {
        int[] originArr = initIntArr();
        System.out.println("originArr: " + Arrays.toString(originArr));
        long start = System.currentTimeMillis();
        jniBridge.bubbleSort(originArr);
        System.out.println("originArr: " + Arrays.toString(originArr));
        System.out.println("bubbleSort use time: " + (System.currentTimeMillis() - start));
    }

    //加密
    private void encryption() {
        String password = "Java 哈哈->";
        String encryption = jniBridge.encryption(password);
        System.out.println("加密 " + encryption);
    }

    // 让 C 调用 Java
    private void callJava() {
        jniBridge.callJava("Java message");
    }

    // 让 C 抛出异常
    private void throwError() {
        try {
            jniBridge.throwError("抛出一个异常吧");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    // 调用  C 动态注册的方法
    private void dynamicRegisterFromJni() {
        String registerFromJni = jniBridge.dynamicRegisterFromJni();
        System.out.println(registerFromJni);
    }

    private int[] initIntArr() {
        int[] arr = new int[10000];
        for (int x = 0; x < arr.length; x++) {
            arr[x] = (int) (Math.random() * 10000 + 1);
        }
        return arr;
    }

    // 分割文件
    private void splitFile() {
        final String path = "E:/code/studio/my_github/Repository/Java/JNI/file/size.exe";
        final String pathPattern = "E:/code/studio/my_github/Repository/Java/JNI/file/size_%d.exe";
        jniBridge.splitFile(path, pathPattern, 3);
    }

    // 合并文件
    private void mergeFile() {
        final String fileNewPath = "E:/code/studio/my_github/Repository/Java/JNI/file/size_new.exe";
        final String pathPattern = "E:/code/studio/my_github/Repository/Java/JNI/file/size_%d.exe";
        jniBridge.mergeFile(fileNewPath, pathPattern, 3);
    }

}