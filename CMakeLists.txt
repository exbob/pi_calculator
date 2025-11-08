cmake_minimum_required(VERSION 3.10)

# 项目名称
project(pi_calculator)

# 设置C标准
set(CMAKE_C_STANDARD 99)

# 添加可执行文件
add_executable(pi_calculator pi_calculator.c)

# 链接数学库
target_link_libraries(pi_calculator m)
