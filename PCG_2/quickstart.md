* qsub -A OPEN-24-47 -q qgpu -l select=1:ngpus=1,walltime=4:00:00 -I
* ml NVHPC/22.2 HDF5/1.12.1-NVHPC-22.2

```c++
#pragma acc data copyin(x,y) copyout(z)
{
...
    #pragma acc parallel
    {

        #pragma acc loop gang vector
            for (i = 0; i < n; ++i) {
                z[i] = x[i] + y[i];
            }
    }
...
}
```

```c++
#pragma acc parallel loop`
#pragma acc kernels
```

```c++
#pragma acc kernels
{
    #pragma acc loop independent
    for(int i = 0; i < N; i++)
    {
        a[i] = 0.0;
        b[i] = 1.0;
        c[i] = 2.0;
    }

    #pragma acc loop independent
    for(int i = 0; i < N; i++)
    {
        a[i] = b[i] + c[i]
    }
}
```

```c++
#pragma acc parallel loop reductin(max:err)
```

* private
* firstprivate

```c++
#pragma acc routine seq
#pragma acc atomic [read | write | update | capture]

#pragma acc atomic
x++; | ++x; | x--; | --x;
x binop = expr; | x = x binop expr;
+,*, -, /, bitwise AND, XOR, OR (&, ^, |), <<, >>
```

```c++
#pragma acc data
{
#pragma acc kernels
...
#pragma acc kernels
...
}
```

```c++
#pragma acc parallel \
loop copy(a)
{
...
}
// data copied back and
forth between CPU and ACC
#pragma acc parallel \
loop copy(a)
{
...
}
```

* copy(list) - Allocates memory on GPU and copies data from host to GPU when
entering region and copies data to the host when exiting region.
* copyin(list) - Allocates memory on GPU and copies data from host to GPU when
entering region.
* copyout(list) - Allocates memory on GPU and copies data to the host when exiting
region.
* create(list) - Allocates memory on GPU but does not copy.
* present(list) - Data is already present on GPU from another containing data region.
* present_or_*(list) - Checks whether is already present and if not do *
* deviceptr(list) - The variable is a device pointer (e.g. CUDA) and can be used directly
on the device.

copy(array[starting_index:length])