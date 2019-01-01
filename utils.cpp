//
// Created by zeval on 30-12-2018.
//

#include "utils.h"

void checkError(cl_int errCode, const std::string &msgFailure) {
    if (errCode < 0) {
        std::cerr << msgFailure << ": " << errCode << std::endl;
        throw errCode;
    }
}

void configPlatformAndDevice(cl_platform_id &platform_id,
                             cl_device_id &device_id,
                             cl_uint *num_platforms,
                             cl_uint *num_devices) {
    cl_int ret;
    ret = clGetPlatformIDs(1, &platform_id, num_platforms);

    checkError(ret, "No se pudo obtener la ID de plataforma");

    ret = clGetDeviceIDs(platform_id, CL_DEVICE_TYPE_GPU, 1, &device_id, num_devices);

    checkError(ret, "Error al obtener el dispositivo");
}

cl_program createProgram(cl_context context, cl_device_id device, const std::string &filename) {
    cl_program program;
    cl_int err;

    /* Leer y crear el programa */
    std::string sourceCode = readSourceFile(filename).c_str();
    const char *code = sourceCode.c_str();
    program = clCreateProgramWithSource(context, (cl_uint) 1, &code, nullptr, &err);

    checkError(err, "No se pudo crear el programa");

    /* Construir el programa */
    err = clBuildProgram(program, 1, &device, nullptr, nullptr, nullptr);
    if (err < 0) {
        size_t log_size;
        /* Find size of log and print to std output */
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, nullptr, &log_size);
        char program_log[log_size + 1];
        program_log[log_size] = '\0';
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size + 1, program_log, nullptr);
        std::cerr << program_log << std::endl;
        throw err;
    }

    return program;


}

std::string readSourceFile(const std::string &filename) {
    std::ifstream in;
    in.open(filename);

    std::string line;
    std::string sourceCode;

    while (std::getline(in, line)) {
        sourceCode.append(line + "\n");  // A new newline never hurted anyone
    }

    in.close();

    return sourceCode;
}


