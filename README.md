# cabin
learnOpengl网站上学到的一些东西
[学习网站](https://learnopengl-cn.github.io/)

## 注意事项

1. 在VS2017中打开配置相应的库和lib文件
2. glm数学库已经更新，如果黑屏检查所有单位矩阵初始化问题，构造函数为 mat(1.0f)。
3. 库的使用主要是利用Cmake软件编译适合自己编译器的版本，在VS上生成解决方案，然后在VS上配置头文件、lib静态链接库以及dll动态链接库。