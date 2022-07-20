# My_Cube(简体中文)
这是一个解魔方的程序, 是用C++写的，但是并没有实现图形化界面, 并且借鉴了一下一个非常好的项目:https://github.com/miskcoo/rubik-cube
教程请看:
#### https://blog.csdn.net/weixin_46645613/article/details/117479516
#### https://blog.csdn.net/weixin_46645613/article/details/119207969
#### https://blog.csdn.net/weixin_46645613/article/details/119208081
## **程序需要**

- C++

## **Makefile文件用法**
-Linux

```
$ cd path/cube
$ make
```
-Windows(需要事先安装MinGW)
```
cd path/cube
mingw32-make
```
## **程序使用方法**
- Linux下你可以直接make之后运行example
```
$ ./example
```
- Windows下同理.
```
example.exe
```
- example程序默认生成随机打乱, 然后还原, 如果你要指定打乱方式, 请在文件"***example_program/example.cpp***"中修改程序, 或者自己写程序实现. 同时也得修改输入文件, 你的输入文件格式为:
```
"一次操作"+"空格"+"一次操作"+......
```
- 这些操作表示的是魔方打乱步骤
- 如果你还是无法理解,请看示例文件: "***example_disarrange_steps.txt***".
- 你也可以输入整个魔方的状态, 但是并不是很建议, 因为魔方的状态必须符合魔方自身的规律, 而不是传入一个无法还原的魔方状态. 示例文件: "***example_cube.txt***".

---
# My_Cube(English)
#### Sorry, I'm not good at English, so there may be some grammatical errors.
This is a program which is used for solving rubik's cube,  and writen by C++. But here doesn't have a GUI. However, I borrowed from a nice project:https://github.com/miskcoo/rubik-cube.
You can get a course from there: 
#### https://blog.csdn.net/weixin_46645613/article/details/117479516
#### https://blog.csdn.net/weixin_46645613/article/details/119207969
#### https://blog.csdn.net/weixin_46645613/article/details/119208081

## **Requirement**
- C++
## **Makefile-Usage**
```
$ cd path/cube
$ make
```
## **Program-Usage**
- In Linux, you can run command make, the run example.
```
$ ./example
```
- The same as Windows(be sure your compurter has installed the MinGW)
```
example.exe
```
- The example program will default disarrange the rubick's cube by random operations , so if you want to specify a series of operations, plese modify the source code in "***example_program/example.cpp***", or write a new program to realize it by yourself. Meanwhile, you should modify the input file, and the style of the file is like this:
```
"operation"+"space"+"operation"+......
```
- the operation means how to rotate the cube.
- If you can't understand, there is an example file:"***example_disarrange_steps.txt***".
- Also, you can input the whole state of the cube, but this is not recommended, because of the state must conform to the rule of rubik's cube, instead of input a state that can't be restored. Example file: "***example_cube.txt***".
