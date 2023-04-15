### readme执行说明

#### 1. 单独执行语法分析

**用parser可执行文件进行语法分析**

在Linux系统内输入：

```
cd exe
./parser ./bubbleSort.c > bubbleSort.json
./parser ./palindrome_string.c > palindrome_string.json
```

即可生成语法分析树的json格式；

如果想直接查看这两个测试程序的语法分析结果，可以直接在result文件夹中查看对应的json文件和可视化后的图片。

#### 2. 生成python代码

**用main可执行文件进行代码翻译**

在Linux系统内输入：

```
cd exe
./main ./bubbleSort.c 
```

即可生成test.py文件，文件中为翻译出的冒泡排序python代码，直接运行该文件进行冒泡排序；运行test.py截图如下：

<img src="C:\Users\ply\AppData\Roaming\Typora\typora-user-images\image-20221222122407982.png" alt="image-20221222122407982" style="zoom:50%;" />



在Linux系统内输入：

```
cd
./main ./palindrome_string.c 
```

此时test.py文件中为翻译出的回文串检测python代码，直接运行该文件进行回文串检测；运行test.py截图如下：

<img src="C:\Users\ply\AppData\Roaming\Typora\typora-user-images\image-20221222122602191.png" alt="image-20221222122602191" style="zoom:50%;" />

如果想直接查看这两个测试程序转换生成的python代码，可以直接在result文件夹中查看bubbleSort.py和palindrome_string.py。



**注：错误测例的翻译**

在Linux系统内输入

```
cd exe
./main ./error1.c 
```

即可在控制台看到输出信息，检测出变量num未定义错误：

<img src="C:\Users\ply\AppData\Roaming\Typora\typora-user-images\image-20221222133615101.png" alt="image-20221222133615101" style="zoom:40%;" />



在Linux系统内输入

```
cd exe
./main ./error2.c 
```

即可在控制台看到输出信息，检测出变量num重定义错误：

<img src="C:\Users\ply\AppData\Roaming\Typora\typora-user-images\image-20221222134531344.png" alt="image-20221222134531344" style="zoom:50%;" />



在Linux系统内输入

```
cd exe
./main ./error2.c 
```

即可在控制台看到输出信息，检测出函数func重定义错误：

<img src="C:\Users\ply\AppData\Roaming\Typora\typora-user-images\image-20221222134807432.png" alt="image-20221222134807432" style="zoom:50%;" />
