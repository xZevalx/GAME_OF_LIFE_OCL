//
// Created by zeval on 30-12-2018.
//

#ifndef THEGAMEOFLIFE_UTILS_H
#define THEGAMEOFLIFE_UTILS_H

#include <CL/cl.h>
#include <iostream>
#include <fstream>
#include <vector>

void checkError(cl_int errCode, const std::string &msgFailure);

void configPlatformAndDevice(cl_platform_id &platform_id,
                             cl_device_id &device_id,
                             cl_uint *num_platforms,
                             cl_uint *num_devices);

cl_program createProgram(cl_context context, cl_device_id device, const std::string &filename);

std::string readSourceFile(const std::string &filename);

template<typename T>
void printMatrix(const std::vector<T> &matrix, int cols, int rows) {
    for (int i=0; i<rows; i++){
        for (int j=0; j<cols; j++){
            std::cout << matrix.at(i*rows + j) << "  ";
        }
        std::cout << std::endl;
    }
    std::cout << std::endl;
}

#endif //THEGAMEOFLIFE_UTILS_H
