
# 添加可执行文件
add_executable(type_state_pattern TypeStatePattern.cpp)

target_compile_features(type_state_pattern PRIVATE cxx_std_23)
# 添加编译选项
target_compile_options(type_state_pattern PRIVATE
    -Wall
    -Wextra
) 
