#  A JNI Sample on Windows

环境准备：

1. 安装 MinGW，推荐使用 msys；
2. 拷贝 JDK 目录下的 include 文件夹到 jni 目录下。

编码开发：

1. 使用 CLion 打开 jni 目录编写 Native 代码；
2. 打开 CMD 窗口（不要使用 PowerShell），进入项目 jni 目录，执行 `gcc -Wl,--add-stdcall-alias -I "include" -I "include\win32"  -shared -o native.dll native-lib.c Utils.c FileUtils.c` 即可生成 `native.dll` 动态库；
3. 使用 IDEA 打开 JNI-Sample 项目，在 Java 层使用 `System.loadLibrary("jni/native");` 加载  `native.dll` 动态库，之后就可以调用 Native 方法了。

注意配置项目的 `Working directory` 为项目根目录。

相关命令：

- `javah` 命令用于根据 class 文件生成 C 头文件。
- `javap` 可以查看 Java 方法的签名。
