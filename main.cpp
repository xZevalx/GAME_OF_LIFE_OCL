#include <iostream>
#include <cmath>
#include <chrono>
#include "utils.h"
#include "GameOfLife.h"

void
runExperiment(int iterations, size_t cols, size_t rows, cl_context context, cl_command_queue queue, cl_kernel kernel) {
    GameOfLife gameOfLife(cols, rows, cols * rows, 1);

    /* Imprimir matriz de entrada */
    // printMatrix(gameOfLife.hostBuffer, cols, rows);

    gameOfLife.initBuffers(context);

    /* Configurar y correr iteraciones */
    gameOfLife.setIterations(iterations, queue, kernel, 0);

    gameOfLife.releaseResources();
}


int main() {
    cl_int errCode;

    cl_platform_id platform_id = nullptr;
    cl_device_id device_id = nullptr;
    cl_context context;
    cl_program program;
    cl_uint num_platforms;
    cl_uint num_devices;

    configPlatformAndDevice(platform_id, device_id, &num_platforms, &num_devices);
    context = clCreateContext(nullptr, 1, &device_id, nullptr, nullptr, &errCode);

    checkError(errCode, "Problema al crear el contexto");

    /* Crear programa */
    program = createProgram(context, device_id, GameOfLife::kernelSourcePath);

    /* Crear cola de comandos */
    cl_command_queue queue = clCreateCommandQueueWithProperties(context, device_id, nullptr, &errCode);

    /* Crear kernel */
    cl_kernel kernel = clCreateKernel(program, GameOfLife::kernelFunction, &errCode);

    /* Crear buffers de datos */
    int iterations = 1000;
    for (int i = 5; i<=9; i++){
        size_t cols, rows;
        cols = rows = (size_t) pow(2, i);
        std::cout << "Ancho/alto de matriz " << cols << "x" << rows << std::endl;
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
        runExperiment(iterations, cols, rows, context, queue, kernel);
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>( t2 - t1 ).count();
        std::cout << "Tiempo de ejecucion " << duration << " us" << std::endl;
    }

    /* Liberar recursos */
    clReleaseKernel(kernel);
    clReleaseCommandQueue(queue);
    clReleaseProgram(program);
    clReleaseContext(context);

    return 0;
}