#include <yaml-cpp/yaml.h> //yaml-cpp ͷ�ļ�
#include <iostream>

int main(int argc, char** argv) {
    try {
        YAML::Node node = YAML::LoadFile("file.yml");
        std::cout << node << std::endl; //���yaml����
    } catch (...) { //�ļ�Ϊ��yaml��ʽ�׳��쳣
        std::cout << "error" << std::endl;
    }
    return 0;
}

