##### 2024/1/21

进度：OLED显示

问题：

CMake Error: Could not find cmake module file: CMakeDeterminehomeCompiler.cmake
CMake Error: Error required internal CMake variable not set, cmake may not be built correctly.

解决：删去工程文件名中的空格



##### 2024/1/24

进度：添加DHT11相关文件

问题：

arm-none-eabi-gcc.exe: error: Core/Inc/dht11.h: No such file or directory
arm-none-eabi-gcc.exe: error: Core/Src/dht11.c: No such file or directory

解决：通过参考：Clion 2020.2.4 CMake报错，发现新加入的文件后CmakeList.txt会自动添加LINKER_SCRIPT。将其后面的头文件和源文件删除，既恢复到set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32F103RFTx_FLASH.ld)



##### 2024/1/25

进度：完成了温湿度数值的正确显示

问题：

温湿度数值显示只有一位

解决：sizeof(Data[0])函数获取的是元素的字节数，故都为1，只显示一位



##### 2024/1/29

进度：增加呼吸灯

问题：

cubemx生成新代码后，原main.c中的中文注释会变为乱码

解决：Windows默认使用UTF-8编码，而cubemx则使用GBK编码
