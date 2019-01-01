//
// Created by zeval on 31-12-2018.
//

#include "GameOfLife.h"
#include <random>
#include "utils.h"

GameOfLife::GameOfLife(size_t cols, size_t rows, size_t globalSize, size_t localSize) : cols(cols), rows(rows),
                                                                                        globalSize(globalSize),
                                                                                        localSize(localSize) {
    hostBuffer.reserve(cols * rows);

    std::default_random_engine generator;
    std::uniform_real_distribution<double> distribution(0.0, 1.0);

    cl_short val;
    for (int i = 0; i < cols * rows; i++) {
        if (distribution(generator) <= LIFE_PROB) {
            val = 1;
        } else {
            val = 0;
        }
        hostBuffer.push_back(val);
    }

    numGroups = globalSize / localSize;
}

void GameOfLife::initBuffers(cl_context context) {
    cl_int errCode;

    currentBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, cols * rows * sizeof(cl_short),
                                   hostBuffer.data(), &errCode);
    checkError(errCode, "No se pudo crear buffer para currentBuffer");

    nextBuffer = clCreateBuffer(context, CL_MEM_READ_WRITE, cols * rows * sizeof(cl_short), nullptr, &errCode);
    checkError(errCode, "No se pudo crear buffer para nextBuffer");

}

void GameOfLife::setKernelArgs(cl_kernel kernel) {
    cl_int errCode;

    errCode = clSetKernelArg(kernel, 0, sizeof(cl_mem), &currentBuffer);
    checkError(errCode, "No se pudo setear el primer argumento del kernel");

    errCode = clSetKernelArg(kernel, 1, sizeof(cl_mem), &nextBuffer);
    checkError(errCode, "No se pudo setear el segundo argumento del kernel");

    errCode = clSetKernelArg(kernel, 2, sizeof(cl_int), &cols);
    checkError(errCode, "No se pudo setear el tercer argumento del kernel");

    errCode = clSetKernelArg(kernel, 3, sizeof(cl_int), &rows);
    checkError(errCode, "No se pudo setear el cuarto argumento del kernel");

}

const std::vector<cl_short> & GameOfLife::readKernelOutput(cl_command_queue queue) {
    cl_int errCode;

    errCode = clEnqueueReadBuffer(queue, nextBuffer, CL_TRUE, 0, sizeof(cl_short) * cols * rows, hostBuffer.data(),
                                  0,
                                  nullptr,
                                  nullptr);

    checkError(errCode, "No se pudo leer la salida del kernel");
    return hostBuffer;
}

void GameOfLife::releaseResources() {
    clReleaseMemObject(currentBuffer);
    clReleaseMemObject(nextBuffer);
}

void GameOfLife::setIterations(int iterations, cl_command_queue queue, cl_kernel kernel, int printPolicy) {
    int errCode;
    cl_mem aux;
    for (int i = 0; i < iterations; i++) {
        // setear argumentos
        setKernelArgs(kernel);
        // encolar
        errCode = clEnqueueNDRangeKernel(queue, kernel, 1, nullptr, &globalSize, &localSize, 0, nullptr, nullptr);
        checkError(errCode, "No se pudo encolar en iteración " + std::to_string(i));
        clFinish(queue);
        if (printPolicy == 1){
            // leer salida
            readKernelOutput(queue);
            // imprimir salida
            printMatrix(hostBuffer, cols, rows);
        }
        // swappear buffers
        if (i != iterations - 1) {
            aux = currentBuffer;
            currentBuffer = nextBuffer;
            nextBuffer = aux;
        }
    }
    if (printPolicy == 2) {
        readKernelOutput(queue);
        // imprimir salida
        printMatrix(hostBuffer, cols, rows);
    }

}
