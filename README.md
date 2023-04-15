### 完整作业文档：C-Python编译器

潘乐怡 2020012374

孙骜     2020012367

徐霈然 2020012355

---

#### 1. 文件结构

├── **src 源代码文件夹**

​		    ├──  bubbleSort.c 排序测例

​		    ├──  palindrome_string.c 检测回文串测例

​		    ├──  lex.l 词法分析文件

​		    ├──  yacc.ypp 语法分析文件

​		    ├──  AST.h 语法树节点定义文件

​		    ├──  AST.cpp 语法树节点类方法实现文件

​		    ├──  codegen.h 代码生成相关头文件

​		    ├──  codegen.cpp 代码生成相关函数实现文件

​		    ├──  cjson文件夹，工具文件（不是我们自己写的），提供了一些生成json格式文件的接口

​		    ├──  parser_main.cpp 语法分析main文件

​		    ├──  main.cpp 整体main文件

├── **exe 包含两个测例 & 三个错误测例 & parser可执行文件 & main可执行文件**

​		    ├──  bubbleSort.c 排序测例

​		    ├──  palindrome_string.c 检测回文串测例

​		    ├──  error1.c  error2.c  error3.c  三个错误测例

​		    ├── parser 语法分析可执行文件

​		    ├── main  C to Python编译器可执行文件

​		    ├── readme执行说明

├── **result 执行结果**

​		    ├──  bubbleSort.json 排序语法分析产生的.json文件

​		    ├──  bubbleSort.png  排序语法分析可视化图像

​		    ├──  bubbleSort.py     排序.c文件翻译出的python文件

​	        ├──  palindrome_string.json 回文串检测语法分析产生的.json文件

​		    ├──  palindrome_string.png  回文串检测语法分析可视化图像

​		    ├──  palindrome_string.py     回文串检测.c文件翻译出的python文件

├── **README**

#### 2. 编译运行方法

- 只进行语法分析

    ```c
    $ cd src
    $ make clean
    $ make parser
    $ ./parser bubbleSort.c > bubbleSort.json
    $ ./parser palindrome_string.c > palindrome_string.json
    ```
    
    即可生成bubbleSort.json与palindrome_string.json文件展示两个测例的语法分析结果。

- 进行C转python

  ```c
  $ cd src
  $ make clean
  $ make code
  $ ./main bubbleSort.c // 即可看到test.py，为bubbleSort.c翻译结果
  $ ./main palindrome_string.c // 即可看到test.py，palindrome_string.c翻译结果
  ```

#### 3. 语法支持

我们的编译器支持**[常量、变量、全局变量、一维数组、算术表达式、一般逻辑表达式、if语句、For语句、函数声明与调用、printf和scanf库函数]**等，基本覆盖C语言的常用语法体系。此外，我们还对C语言文件做了<font color = red>**三类常见错误的检测**</font>：**变量是否声明过再使用，变量是否被重定义了，函数是否被重定义了**。



#### 4. 技术方案和实现方法简顾

##### 4.1 技术方案

我们使**用Lex做词法分析**，使用**Yacc做语法分析**自底向上生成了**抽象语法树**，再为抽象语法树的**每个节点类定义codegen()函数**，自顶向下递归调用codegen()生成Python代码。

##### 4.2 抽象语法树节点类继承关系

根据测例中涉及的语法，我们设计了如下节点类。各个节点类的继承关系如下：

<img src="C:\Users\ply\AppData\Roaming\Typora\typora-user-images\image-20221222132009576.png" alt="image-20221222132009576" style="zoom:20%;" />

##### 4.3 常见错误检测

详见5.1



#### 5. 创新点与难点

##### 5.1 三类常见错误检测 & 提示报错信息

我们的C-Python编译器可以支持**变量是否声明过再使用，变量是否被重定义了，函数是否被重定义了**这三类常见错误的检测。

实现方法为：**制作两张符号表 **

- 变量符号表：二维结构，一维是作用域，另一维度是该作用域中已经定义过的所有变量

  这样的二维结构可以支持在检测不同作用域内变量的重定义与未定义。

  - 每次在定义一个新变量时，即VariableDeclarationNode的codegen()函数中，会检查变量在该作用域内是否之前已经定义过。如果已经定义过，则报错；如果在之前更大的作用域内定义过，则自动为新变量重命名。
  - 每次在进行变量的Python代码生成时，即IdentifierNode的codegen()函数中，会检查变量在之前是否被定义过。如果没有定义过，则报错。

- 函数符号表：一维结构，因为函数不涉及作用域问题；存放已经定义过的函数名。

  

**实现结果展示：**

- 检测变量未定义

  测试代码`error1.c`

  ```c
  int a[100];
  
  int main()
  {
      a[1] = 1;
      num = 2;
  }
  ```

  测试结果：

  <img src="C:\Users\ply\AppData\Roaming\Typora\typora-user-images\image-20221222133615101.png" alt="image-20221222133615101" style="zoom:40%;" />

- 检测变量重定义

  测试代码`error2.c`

  ```c
  int a[100];
  
  int main()
  {
      int num = 1;
      a[1] = 1;
      int num = 2;
  }
  ```

  <img src="C:\Users\ply\AppData\Roaming\Typora\typora-user-images\image-20221222134531344.png" alt="image-20221222134531344" style="zoom:40%;" />

- 检测函数重定义

  测试代码`error3.c`

  ```c
  int func()
  {
      return 0;
  }
  int main()
  {
      return 0;
  }
  int func()
  {
      return 0;
  }
  ```

  <img src="C:\Users\ply\AppData\Roaming\Typora\typora-user-images\image-20221222134807432.png" alt="image-20221222134807432" style="zoom:40%;" />



##### 5.2 printf，scanf库函数转python

我们知道，C的printf和scanf库函数是非常常用的，但如果对他们做直接转换生成Python代码，它们的格式是不被Python认可的。因此，我们在函数定义节点类的codegen()中，对这两类函数做了特判：

- printf

  - 对于只输出一个常量字符串的printf，我们只需要将printf改为print即可：

    C代码：

    ```c
    printf("hello world");
    ```

    python代码：

    ```python
    print("hello world")
    ```

  - 对于格式化输出，我们需要将printf改为print，且还需要将，改为%，并添加括号：

    C代码：

    ```c
    printf("%d %d", a, b);
    ```

    python代码：

    ```python
    print("%d %d" % (a, b,))
    ```

- scanf

  scanf的实现更为复杂一些，我们需要将scanf转化为python中的input函数；

  - 接收字符串

    C代码：

    ```c
    scanf("%s", a);
    ```

    python代码：

    ```python
    a = input()
    ```

  - 接收数

    C代码：

    ```c
    scanf("%d", a);
    ```

    python代码：

    ```python
    a = eval(input())
    ```

    

#### 6. 实现结果

两个测例的转换结果可以直接在result文件夹中查看，也可以根据exe中的readme执行说明用可执行文件对两个测例进行C-python转换。

#### 7. 小组分工

| 组员   | 分工                                                         |
| ------ | ------------------------------------------------------------ |
| 潘乐怡 | 调研实现思路，设计节点类，由语法分析树生成json并可视化，编写codegen()函数，撰写文档 |
| 孙骜   | 调研实现思路，词法分析，编写测例，设计产生式，撰写文档       |
| 徐霈然 | 调研实现思路，编写测例，构建语法分析树，撰写文档             |





