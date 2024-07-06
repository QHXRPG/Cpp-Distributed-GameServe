#include <yaml-cpp/yaml.h> //yaml-cpp 头文件
#include <iostream>

int main(int argc, char** argv) {
    try {
        YAML::Node node = YAML::LoadFile("file.yml");
        std::cout << node << std::endl; //输出yaml数据
    } catch (...) { //文件为非yaml格式抛出异常
        std::cout << "error" << std::endl;
    }
    return 0;
}

