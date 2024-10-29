#/**
# * @file      Makefile
# *
# * @author    David Sladký \n
# *            xsladk07@stud.fit.vutbr.cz
# *
# * @brief     PCG Assignment 2
# *            N-Body simulation in ACC - Makefile
# *
# * @version   2022
# *
# * @date      11 November  2020, 11:22 (created) \n
# * @date      11 November  2022, 11:37 (revised) \n
# *
# */

N=4096
DT=0.01
STEPS=500
WRITE_INTESITY=20

INPUT=../sampledata/sampleInput.h5
SAMPLE_OUTPUT=../sampledata/sampleOutput.h5
OUTPUT=step3Output.h5

INCLUDE=../commons
LIBS=-lhdf5 -lm

FLAGS=  

.PHONY: all clean run check_output data


CC=nvc++
CFLAGS=-march=native --c++14

# Acceleration for Barbora GPUs
ACC=-acc=gpu -gpu=cc80,fastmath,pinned

# Acceleration for Barbora GPUs
#ACC=-acc=multicore

OPT=-O3
LOG=-Minfo=accel

# OpenACC acceleration types
#[no]acc[=gpu|host|multicore|[no]autopar|[no]routineseq|legacy|strict|verystrict|sync|[no]wait]
#                    Enable OpenACC directives
#    gpu             OpenACC directives are compiled for GPU execution only; please refer to -gpu for target specific options
#    host            Compile for serial execution on the host CPU
#    multicore       Compile for parallel execution on the host CPU
#    [no]autopar     Enable (default) or disable loop autoparallelization within acc parallel
#    [no]routineseq  Compile every routine for the device
#    legacy          Suppress warnings about deprecated PGI accelerator directives
#    strict          Issue warnings for non-OpenACC accelerator directives
#    verystrict      Fail with an error for any non-OpenACC accelerator directive
#    sync            Ignore async clauses
#    [no]wait        Wait for each device kernel to finish
#-cuda[=charstring|madconst]
#                    Enable CUDA; please refer to -gpu for target specific options
#    charstring      Enable limited support for character strings in GPU kernels
#    madconst        Put Module Array Descriptors in CUDA Constant Memory

#GPU architecture
#-gpu=cc35|cc50|cc60|cc62|cc70|cc72|cc75|cc80|ccall|cudaX.Y|fastmath|[no]flushz|[no]fma|keep|[no]lineinfo|llc|zeroinit|[no]autocollapse|deepcopy|loadcache:{L1|L2}|maxregcount:<n>|pinned|[no]rdc|safecache|[no]unroll|[no]managed|beta|autocompare|redundant
#                    Select specific options for GPU code generation
#    cc35            Compile for compute capability 3.5
#    cc50            Compile for compute capability 5.0
#    cc60            Compile for compute capability 6.0
#    cc62            Compile for compute capability 6.2
#    cc70            Compile for compute capability 7.0
#    cc72            Compile for compute capability 7.2
#    cc75            Compile for compute capability 7.5
#    cc80            Compile for compute capability 8.0
#    ccall           Compile for all supported compute capabilities
#    cudaX.Y         Use CUDA X.Y Toolkit compatibility, where installed
#    fastmath        Use fast math library
#    [no]flushz      Enable flush-to-zero mode on the GPU
#    [no]fma         Generate fused mul-add instructions (default at -O3)
#    keep            Keep kernel files
#    [no]lineinfo    Generate GPU line information
#    zeroinit        Initialize allocated device memory with zero
#    [no]autocollapse
#                    Automatically collapse tightly nested parallel loops
#    deepcopy        Enable Full Deepcopy support in OpenACC Fortran
#    loadcache       Choose what hardware level cache to use for global memory loads
#     L1             Use L1 cache
#     L2             Use L2 cache
#    maxregcount:<n> Set maximum number of registers to use on the GPU
#    pinned          Use CUDA Pinned Memory
#    [no]rdc         Generate relocatable device code
#    safecache       Allows variable-sized array sections in cache directives and assumes they fit into CUDA shared memory
#    [no]unroll      Enable automatic inner loop unrolling (default at -O3)
#    [no]managed     Use CUDA Managed Memory
#    beta            Enable beta code generation features
#    autocompare     Automatically compare CPU/GPU results: implies redundant
#    redundant       Redundant CPU/GPU execution

all: nbody

nbody: nbody.cpp main.cpp nbody.h
	${CC} ${CFLAGS} ${OPT} ${ACC} ${LOG} -I${INCLUDE} nbody.cpp main.cpp ../commons/h5Helper.cpp ${LIBS} -o nbody

clean:
	rm -f *.o nbody

run:
	./nbody ${N} ${DT} ${STEPS} ${WRITE_INTESITY} $(INPUT) $(OUTPUT)

data:
	./nbody 25600 ${DT} ${STEPS} ${WRITE_INTESITY} data/25600.h5 data/25600_out.h5
	./nbody 28160 ${DT} ${STEPS} ${WRITE_INTESITY} data/28160.h5 data/28160_out.h5
	./nbody 30720 ${DT} ${STEPS} ${WRITE_INTESITY} data/30720.h5 data/30720_out.h5
	./nbody 33280 ${DT} ${STEPS} ${WRITE_INTESITY} data/33280.h5 data/33280_out.h5
	./nbody 35840 ${DT} ${STEPS} ${WRITE_INTESITY} data/35840.h5 data/35840_out.h5
	./nbody 38400 ${DT} ${STEPS} ${WRITE_INTESITY} data/38400.h5 data/38400_out.h5
	./nbody 40960 ${DT} ${STEPS} ${WRITE_INTESITY} data/40960.h5 data/40960_out.h5
	./nbody 43520 ${DT} ${STEPS} ${WRITE_INTESITY} data/43520.h5 data/43520_out.h5
	./nbody 46080 ${DT} ${STEPS} ${WRITE_INTESITY} data/46080.h5 data/46080_out.h5
	./nbody 48640 ${DT} ${STEPS} ${WRITE_INTESITY} data/48640.h5 data/48640_out.h5
	./nbody 51200 ${DT} ${STEPS} ${WRITE_INTESITY} data/51200.h5 data/51200_out.h5
	./nbody 53760 ${DT} ${STEPS} ${WRITE_INTESITY} data/53760.h5 data/53760_out.h5
	./nbody 56320 ${DT} ${STEPS} ${WRITE_INTESITY} data/56320.h5 data/56320_out.h5
	./nbody 58880 ${DT} ${STEPS} ${WRITE_INTESITY} data/58880.h5 data/58880_out.h5
	./nbody 61440 ${DT} ${STEPS} ${WRITE_INTESITY} data/61440.h5 data/61440_out.h5
	./nbody 64000 ${DT} ${STEPS} ${WRITE_INTESITY} data/64000.h5 data/64000_out.h5

check_output:
	./nbody 4096 0.01f 2500 ${THREADS_PER_BLOCK} 50 ${RED_THREADS} ${RED_THREADS_PER_BLOCK} $(INPUT) $(OUTPUT)
	-h5diff -v2 -p 0.00001 $(SAMPLE_OUTPUT) $(OUTPUT) /pos_x_final
	-h5diff -v2 -p 0.00001 $(SAMPLE_OUTPUT) $(OUTPUT) /pos_y_final
	-h5diff -v2 -p 0.00001 $(SAMPLE_OUTPUT) $(OUTPUT) /pos_z_final
	-h5diff -v2 -p 0.00001 $(SAMPLE_OUTPUT) $(OUTPUT) /vel_x_final
	-h5diff -v2 -p 0.00001 $(SAMPLE_OUTPUT) $(OUTPUT) /vel_y_final
	-h5diff -v2 -p 0.00001 $(SAMPLE_OUTPUT) $(OUTPUT) /vel_z_final
	-h5diff -v2 -p 0.00001 $(SAMPLE_OUTPUT) $(OUTPUT) /weight_final
	-h5diff -v2 -p 0.00001 $(SAMPLE_OUTPUT) $(OUTPUT) /com_x_final
	-h5diff -v2 -p 0.00001 $(SAMPLE_OUTPUT) $(OUTPUT) /com_y_final
	-h5diff -v2 -p 0.00001 $(SAMPLE_OUTPUT) $(OUTPUT) /com_z_final
	-h5diff -v2 -p 0.00001 $(SAMPLE_OUTPUT) $(OUTPUT) /com_w_final
	-h5diff -v2 -p 0.00001 $(SAMPLE_OUTPUT) $(OUTPUT) /pos_x
	-h5diff -v2 -p 0.00001 $(SAMPLE_OUTPUT) $(OUTPUT) /pos_y
	-h5diff -v2 -p 0.00001 $(SAMPLE_OUTPUT) $(OUTPUT) /pos_z
	-h5diff -v2 -p 0.00001 $(SAMPLE_OUTPUT) $(OUTPUT) /vel_x
	-h5diff -v2 -p 0.00001 $(SAMPLE_OUTPUT) $(OUTPUT) /vel_y
	-h5diff -v2 -p 0.00001 $(SAMPLE_OUTPUT) $(OUTPUT) /vel_z
	-h5diff -v2 -p 0.00001 $(SAMPLE_OUTPUT) $(OUTPUT) /weight
	-h5diff -v2 -p 0.00001 $(SAMPLE_OUTPUT) $(OUTPUT) /com_x
	-h5diff -v2 -p 0.00001 $(SAMPLE_OUTPUT) $(OUTPUT) /com_y
	-h5diff -v2 -p 0.00001 $(SAMPLE_OUTPUT) $(OUTPUT) /com_z
	-h5diff -v2 -p 0.00001 $(SAMPLE_OUTPUT) $(OUTPUT) /com_w
