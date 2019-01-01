//
// Created by zeval on 31-12-2018.
//

#ifndef THEGAMEOFLIFE_GAMEOFLIFE_H
#define THEGAMEOFLIFE_GAMEOFLIFE_H

#include <CL/cl.h>
#include <vector>
#include <string>

#define LIFE_PROB 0.3

class GameOfLife {

public:

    static constexpr const char * kernelSourcePath = "../kernels/gameOfLife.cl";
    static constexpr const char * kernelFunction = "gameOfLife";
    size_t globalSize;  // Total work items
    size_t localSize;  // Work items per group
    std::vector<cl_short> hostBuffer;
    cl_mem currentBuffer;  // RW buffer
    cl_mem nextBuffer;  // RW buffer

    GameOfLife(size_t cols, size_t rows, size_t globalSize, size_t localSize);

    void initBuffers(cl_context context);

    void setKernelArgs(cl_kernel kernel);

    const std::vector<cl_short> & readKernelOutput(cl_command_queue queue);

    void releaseResources();

    /**
     *
     * @param iterations
     * @param queue
     * @param kernel
     * @param printPolicy 0: No prints, 1: prints every generation after the first, 2: prints at the end
     */
    void setIterations(int iterations, cl_command_queue queue, cl_kernel kernel, int printPolicy);

private:
    size_t cols, rows;
    cl_int numGroups;  // numgroups = globalSize/localSize
};


#endif //THEGAMEOFLIFE_GAMEOFLIFE_H
