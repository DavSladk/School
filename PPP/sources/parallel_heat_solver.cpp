/**
 * @file    parallel_heat_solver.cpp
 * @author  xlogin00 <xlogin00@stud.fit.vutbr.cz>
 *
 * @brief   Course: PPP 2021/2022 - Project 1
 *
 * @date    2022-MM-DD
 */

#include "parallel_heat_solver.h"

using namespace std;

//============================================================================//
//                            *** BEGIN: NOTE ***
//
// Implement methods of your ParallelHeatSolver class here.
// Freely modify any existing code in ***THIS FILE*** as only stubs are provided 
// to allow code to compile.
//
//                             *** END: NOTE ***
//============================================================================//

ParallelHeatSolver::ParallelHeatSolver(SimulationProperties &simulationProps,
                                       MaterialProperties &materialProps)
    : BaseHeatSolver (simulationProps, materialProps),
    m_fileHandle(H5I_INVALID_HID, static_cast<void (*)(hid_t )>(nullptr))
{
    MPI_Comm_size(MPI_COMM_WORLD, &m_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &m_rank);
    
    // Creating EMPTY HDF5 handle using RAII "AutoHandle" type
    //
    // AutoHandle<hid_t> myHandle(H5I_INVALID_HID, static_cast<void (*)(hid_t )>(nullptr))
    //
    // This can be particularly useful when creating handle as class member!
    // Handle CANNOT be assigned using "=" or copy-constructor, yet it can be set
    // using ".Set(/* handle */, /* close/free function */)" as in:
    // myHandle.Set(H5Fopen(...), H5Fclose);
    if(!m_simulationProperties.GetOutputFileName().empty())
    {
        if(m_simulationProperties.IsUseParallelIO())
        {
            hid_t accesPList = H5Pcreate(H5P_FILE_ACCESS);
            H5Pset_fapl_mpio(accesPList, MPI_COMM_WORLD, MPI_INFO_NULL);
            m_fileHandle.Set(H5Fcreate(simulationProps.GetOutputFileName("par").c_str(),
                                    H5F_ACC_TRUNC, H5P_DEFAULT, accesPList), H5Fclose);
            H5Pclose(accesPList);
        }
        else
        {
            if(m_rank == ROOT_RANK)
            {
                m_fileHandle.Set(H5Fcreate(simulationProps.GetOutputFileName("par").c_str(),
                                    H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT), H5Fclose);
            }
        }
    }
    
    // Requested domain decomposition can be queried by
    // m_simulationProperties.GetDecompGrid(/* TILES IN X */, /* TILES IN Y */)
    m_simulationProperties.GetDecompGrid(nx, ny);
}

ParallelHeatSolver::~ParallelHeatSolver()
{

}

void ParallelHeatSolver::RunSolver(std::vector<float, AlignedAllocator<float> > &outResult)
{
    // UpdateTile(...) method can be used to evaluate heat equation over 2D tile
    //                 in parallel (using OpenMP).
    // NOTE: This method might be inefficient when used for small tiles such as 
    //       2xN or Nx2 (these might arise at edges of the tile)
    //       In this case ComputePoint may be called directly in loop.
    
    // ShouldPrintProgress(N) returns true if average temperature should be reported
    // by 0th process at Nth time step (using "PrintProgressReport(...)").
    
    // Finally "PrintFinalReport(...)" should be used to print final elapsed time and
    // average temperature in column.

    // fillDummyData();

    allocVectorsForLocalData();
    createMPIdataTypes();
    prepForScatterOrGather();
    scatterData();
    createMPIcomms();
    exchangeParamsAndMap();
    initHaloEx();
    sendHalo(localDataA.data(), localDataB.data());
    recvHalo(localDataA.data(), localDataB.data());
    finalizeHaloEx();
    copy(localDataB.begin(), localDataB.end(), localDataA.begin());
    setOffsets();

    double startTime = MPI_Wtime();
    calculateTile(outResult.data());
    double elapsedTime = MPI_Wtime() - startTime;

    if(MPI_MID_COMM != MPI_COMM_NULL)
    {
        float avg = calculateAvgTemp();
        if(colRank == ROOT_RANK)
        {
            PrintFinalReport(elapsedTime, avg, "par");
        }
    }


    gatherDataTo(outResult.data());
    freeMPIdataTypes();
    freeMPIcomms();
}

void ParallelHeatSolver::calculateTile(float* outResult)
{
    float *workTempArrays[] = { localDataA.data(), localDataB.data() };

    for(size_t iter = 0; iter < m_simulationProperties.GetNumIterations(); ++iter)
    {
        calculateHalos(workTempArrays[0], workTempArrays[1]);
        initHaloEx();
        sendHalo(workTempArrays[0], workTempArrays[1]);
        calculateCenter(workTempArrays[0], workTempArrays[1]);
        recvHalo(workTempArrays[0], workTempArrays[1]);
        finalizeHaloEx();
        swap(workTempArrays[0], workTempArrays[1]);

        storeData(iter, outResult, workTempArrays[1]);

        // print progress report
        if(MPI_MID_COMM != MPI_COMM_NULL && ShouldPrintProgress(iter))
        {
            float avg = calculateAvgTemp();
            if(colRank == ROOT_RANK)
            {
                PrintProgressReport(iter, avg);
            }
        }
    }

    if(m_simulationProperties.GetNumIterations() & 1)
        std::copy(localDataB.begin(), localDataB.end(), localDataA.begin());

}

void ParallelHeatSolver::storeData(size_t iteration, float *dataS, float *dataP)
{
    if(!m_simulationProperties.IsUseParallelIO() && !m_simulationProperties.GetOutputFileName().empty() && ((iteration % m_simulationProperties.GetDiskWriteIntensity()) == 0))
    {
        gatherDataTo(dataS);
        if(m_rank == ROOT_RANK)
        {
            StoreDataIntoFile(m_fileHandle, iteration, dataS);
        }
    }
    else if(m_simulationProperties.IsUseParallelIO() && !m_simulationProperties.GetOutputFileName().empty() && ((iteration % m_simulationProperties.GetDiskWriteIntensity()) == 0))
    {
        ParallelStoreDataInto(m_fileHandle, iteration, dataP);
    }
}

void ParallelHeatSolver::ParallelStoreDataInto(hid_t fileHandle, size_t iteration, const float *data)
{
    /*
     * HDF5 APIs definitions
     */ 	
    hid_t       dset_id;         /* file and dataset identifiers */
    hid_t       filespace, memspace;      /* file and memory dataspace identifiers */
    hsize_t     dimsf[2];                 /* dataset dimensions */
    hsize_t     chunk_dims[2];            /* chunk dimensions */
    hsize_t	count[2];	          /* hyperslab selection parameters */
    hsize_t	stride[2];
    hsize_t	block[2];
    hsize_t	offset[2];
    hid_t	plist_id;                 /* property list identifier */
    herr_t	status;
    // 1. Create new HDF5 file group named as "Timestep_N", where "N" is number
    //    of current snapshot. The group is placed into root of the file "/Timestep_N".
    std::string groupName = "Timestep_" + std::to_string(static_cast<unsigned long long>(iteration / m_simulationProperties.GetDiskWriteIntensity()));
    AutoHandle<hid_t> groupHandle(H5Gcreate(fileHandle, groupName.c_str(),
                                            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT), H5Gclose);

    /*
     * Create the dataspace for the dataset.
     */
    dimsf[0] = m_materialProperties.GetEdgeSize();
    dimsf[1] = m_materialProperties.GetEdgeSize();
    chunk_dims[0] = localYedgeSize;
    chunk_dims[1] = localXedgeSize;
    filespace = H5Screate_simple(2, dimsf, NULL);
    memspace = H5Screate_simple(2, chunk_dims, NULL);

    /*
     * Create chunked dataset.
     */
    plist_id = H5Pcreate(H5P_DATASET_CREATE);
    H5Pset_chunk(plist_id, 2, chunk_dims);
    dset_id = H5Dcreate(groupHandle, "Temperature", H5T_NATIVE_FLOAT, filespace, H5P_DEFAULT, plist_id, H5P_DEFAULT);
    H5Pclose(plist_id);
    H5Sclose(filespace);

    count[0] = 1;
    count[1] = 1;
    stride[0] = 1;
    stride[1] = 1;
    block[0] = chunk_dims[0];
    block[1] = chunk_dims[1];
    offset[0] = colRank * chunk_dims[0];
    offset[1] = rowRank * chunk_dims[1];

    /*
     * Select hyperslab in the file.
     */
    filespace = H5Dget_space(dset_id);
    status = H5Sselect_hyperslab(filespace, H5S_SELECT_SET, offset, stride, count, block);

    // fill store buffer
    for(int y = haloSize; y < YmemAllocSize - haloSize; y++)
    {
        for(int x = haloSize; x < XmemAllocSize - haloSize; x++)
        {
            dataToStore[(y-haloSize)*localXedgeSize + (x-haloSize)] = data[y*XmemAllocSize + x];
        }
    }

    /*
     * Create property list for collective dataset write.
     */
    plist_id = H5Pcreate(H5P_DATASET_XFER);
    H5Pset_dxpl_mpio(plist_id, H5FD_MPIO_COLLECTIVE);
    
    status = H5Dwrite(dset_id, H5T_NATIVE_FLOAT, memspace, filespace,
		      plist_id, dataToStore.data());

    /*
     * Close/release resources.
     */
    H5Dclose(dset_id);
    H5Sclose(filespace);
    H5Sclose(memspace);
    H5Pclose(plist_id);

    {
        // 3. Create Integer attribute in the same group "/Timestep_N/Time"
        //    in which we store number of current simulation iteration.
        std::string attributeName("Time");

        // 3.1 Dataspace is single value/scalar.
        AutoHandle<hid_t> dataSpaceHandle(H5Screate(H5S_SCALAR), H5Sclose);

        // 3.2 Create the attribute in the group as double.
        AutoHandle<hid_t> attributeHandle(H5Acreate2(groupHandle, attributeName.c_str(),
                                                     H5T_IEEE_F64LE, dataSpaceHandle,
                                                     H5P_DEFAULT, H5P_DEFAULT), H5Aclose);

        // 3.3 Write value into the attribute.
        double snapshotTime = double(iteration);
        H5Awrite(attributeHandle, H5T_IEEE_F64LE, &snapshotTime);

        // NOTE: Both dataspace and attribute handles will be released here.
    }
}

void ParallelHeatSolver::fillDummyData()
{
    if(m_rank == ROOT_RANK)
    {
        for(int i = 0; i < int(m_materialProperties.GetInitTemp().size()); i++)
        {
            m_materialProperties.GetInitTemp()[i] = i;
        }
    }
        
}

float ParallelHeatSolver::calculateAvgTemp()
{
    float localTotal = 0;
    float globalTotal = 0;
    int indexX = 0;

    // Get X index
    if(nx == 1)
    {
        indexX = XmemAllocSize/2+1;
    }
    else
    {
        indexX = haloSize;
    }

    // Calculate local total
    for(int i = haloSize; i<localYedgeSize+haloSize; i++)
    {
        localTotal += localDataA[i*XmemAllocSize+indexX];
    }

    // Reduce local total
    MPI_Reduce(&localTotal, &globalTotal, 1, MPI_FLOAT, MPI_SUM, ROOT_RANK, MPI_MID_COMM);

    // Calculate avg
    return globalTotal / m_materialProperties.GetEdgeSize();
}

void ParallelHeatSolver::calculateHalos(float* tmpA, float* tmpB)
{
    UpdateTile(tmpA, tmpB, localParams.data(), localMap.data(),
            offsetX[UP], offsetY[UP], sizeX[UP], sizeY[UP], XmemAllocSize,
            m_simulationProperties.GetAirFlowRate(), m_materialProperties.GetCoolerTemp());

    UpdateTile(tmpA, tmpB, localParams.data(), localMap.data(),
            offsetX[DOWN], offsetY[DOWN], sizeX[DOWN], sizeY[DOWN], XmemAllocSize,
            m_simulationProperties.GetAirFlowRate(), m_materialProperties.GetCoolerTemp());

    UpdateTile(tmpA, tmpB, localParams.data(), localMap.data(),
            offsetX[LEFT], offsetY[LEFT], sizeX[LEFT], sizeY[LEFT], XmemAllocSize,
            m_simulationProperties.GetAirFlowRate(), m_materialProperties.GetCoolerTemp());

    UpdateTile(tmpA, tmpB, localParams.data(), localMap.data(),
            offsetX[RIGHT], offsetY[RIGHT], sizeX[RIGHT], sizeY[RIGHT], XmemAllocSize,
                m_simulationProperties.GetAirFlowRate(), m_materialProperties.GetCoolerTemp());
}

void ParallelHeatSolver::calculateCenter(float* tmpA, float* tmpB)
{
    UpdateTile(tmpA, tmpB, localParams.data(), localMap.data(),
            offsetX[CENTER], offsetY[CENTER], sizeX[CENTER], sizeY[CENTER], XmemAllocSize,
            m_simulationProperties.GetAirFlowRate(), m_materialProperties.GetCoolerTemp());
}

void ParallelHeatSolver::setOffsets()
{
    offsetX[UP] = haloSize;
    offsetY[UP] = haloSize;
    sizeX[UP] = localXedgeSize;
    sizeY[UP] = haloSize;

    offsetX[DOWN] = haloSize;
    offsetY[DOWN] = localYedgeSize;
    sizeX[DOWN] = localXedgeSize;
    sizeY[DOWN] = haloSize;

    offsetX[LEFT] = haloSize;
    offsetY[LEFT] = 2*haloSize;
    sizeX[LEFT] = haloSize;
    sizeY[LEFT] = localYedgeSize-2*haloSize;

    offsetX[RIGHT] = localXedgeSize;
    offsetY[RIGHT] = 2*haloSize;
    sizeX[RIGHT] = haloSize;
    sizeY[RIGHT] = localYedgeSize-2*haloSize;
    
    offsetX[CENTER] = 2*haloSize;
    offsetY[CENTER] = 2*haloSize;
    sizeX[CENTER] = localXedgeSize-2*haloSize;
    sizeY[CENTER] = localYedgeSize-2*haloSize;

    // 1st rank in col
    if(colRank == 0)
    {
        offsetY[UP] += haloSize;

        offsetY[LEFT] += haloSize;
        sizeY[LEFT] -= haloSize;

        offsetY[RIGHT] += haloSize;
        sizeY[RIGHT] -= haloSize;
        
        offsetY[CENTER] += haloSize;
        sizeY[CENTER] -= haloSize;
    }
    // last rank in col
    if(colRank == ny - 1)
    {
        offsetY[DOWN] -= haloSize;

        sizeY[LEFT] -= haloSize;

        sizeY[RIGHT] -= haloSize;

        sizeY[CENTER] -= haloSize;
    }

    // 1st rank in row
    if(rowRank == 0)
    {
        offsetX[UP] += haloSize;
        sizeX[UP] -= haloSize;

        offsetX[DOWN] += haloSize;
        sizeX[DOWN] -= haloSize;

        offsetX[LEFT] += haloSize;

        offsetX[CENTER] += haloSize;
        sizeX[CENTER] -= haloSize;
    }
    // last rank in row
    if(rowRank == nx - 1)
    {
        sizeX[UP] -= haloSize;

        sizeX[DOWN] -= haloSize;

        offsetX[RIGHT] -= haloSize;
        
        sizeX[CENTER] -= haloSize;
    }
}

void ParallelHeatSolver::exchangeParamsAndMap()
{
    // PARAMS
    // Send up
    if(colRank != 0)
    {
        int offset = XmemAllocSize*haloSize+haloSize;
        MPI_Isend(localParams.data()+offset, 1, MPI_HALO_ROW_F, colRank-1, NO_TAG, MPI_COL_COMM, &req[UP]);
        MPI_Request_free(&req[UP]);
    }

    // Send down
    if(colRank != ny - 1)
    {
        int offset = (YmemAllocSize-2*haloSize)*XmemAllocSize+haloSize;
        MPI_Isend(localParams.data()+offset, 1, MPI_HALO_ROW_F, colRank+1, NO_TAG, MPI_COL_COMM, &req[DOWN]);
        MPI_Request_free(&req[DOWN]);
    }

    // Send left
    if(rowRank != 0)
    {
        int offset = XmemAllocSize*haloSize+haloSize;
        MPI_Isend(localParams.data()+offset, 1, MPI_HALO_COL_F, rowRank-1, NO_TAG, MPI_ROW_COMM, &req[LEFT]);
        MPI_Request_free(&req[LEFT]);
    }

    // Send right
    if(rowRank != nx - 1)
    {
        int offset = haloSize*XmemAllocSize+XmemAllocSize-2*haloSize;
        MPI_Isend(localParams.data()+offset, 1, MPI_HALO_COL_F, rowRank+1, NO_TAG, MPI_ROW_COMM, &req[RIGHT]);
        MPI_Request_free(&req[RIGHT]);
    }

    // Recv from down
    if(colRank != ny - 1)
    {
        int offset = (YmemAllocSize-haloSize)*XmemAllocSize+haloSize;
        MPI_Recv(localParams.data()+offset, 1, MPI_HALO_ROW_F, colRank+1, NO_TAG, MPI_COL_COMM, MPI_STATUSES_IGNORE);
    }

    // Recv from up
    if(colRank != 0)
    {
        int offset = haloSize;
        MPI_Recv(localParams.data()+offset, 1, MPI_HALO_ROW_F, colRank-1, NO_TAG, MPI_COL_COMM, MPI_STATUSES_IGNORE);
    }

    // Recv from right
    if(rowRank != nx - 1)
    {
        int offset = haloSize*XmemAllocSize+XmemAllocSize-haloSize;
        MPI_Recv(localParams.data()+offset, 1, MPI_HALO_COL_F, rowRank+1, NO_TAG, MPI_ROW_COMM, MPI_STATUSES_IGNORE);
    }

    // Recv from left
    if(rowRank != 0)
    {
        int offset = haloSize*XmemAllocSize;
        MPI_Recv(localParams.data()+offset, 1, MPI_HALO_COL_F, rowRank-1, NO_TAG, MPI_ROW_COMM, MPI_STATUSES_IGNORE);
    }

    // MAP
    // Send up
    if(colRank != 0)
    {
        int offset = XmemAllocSize*haloSize+haloSize;
        MPI_Isend(localMap.data()+offset, 1, MPI_HALO_ROW_I, colRank-1, NO_TAG, MPI_COL_COMM, &req[UP]);
        MPI_Request_free(&req[UP]);
    }

    // Send down
    if(colRank != ny - 1)
    {
        int offset = (YmemAllocSize-2*haloSize)*XmemAllocSize+haloSize;
        MPI_Isend(localMap.data()+offset, 1, MPI_HALO_ROW_I, colRank+1, NO_TAG, MPI_COL_COMM, &req[DOWN]);
        MPI_Request_free(&req[DOWN]);
    }

    // Send left
    if(rowRank != 0)
    {
        int offset = XmemAllocSize*haloSize+haloSize;
        MPI_Isend(localMap.data()+offset, 1, MPI_HALO_COL_I, rowRank-1, NO_TAG, MPI_ROW_COMM, &req[LEFT]);
        MPI_Request_free(&req[LEFT]);
    }

    // Send right
    if(rowRank != nx - 1)
    {
        int offset = haloSize*XmemAllocSize+XmemAllocSize-2*haloSize;
        MPI_Isend(localMap.data()+offset, 1, MPI_HALO_COL_I, rowRank+1, NO_TAG, MPI_ROW_COMM, &req[RIGHT]);
        MPI_Request_free(&req[RIGHT]);
    }

    // Recv from down
    if(colRank != ny - 1)
    {
        int offset = (YmemAllocSize-haloSize)*XmemAllocSize+haloSize;
        MPI_Recv(localMap.data()+offset, 1, MPI_HALO_ROW_I, colRank+1, NO_TAG, MPI_COL_COMM, MPI_STATUSES_IGNORE);
    }

    // Recv from up
    if(colRank != 0)
    {
        int offset = haloSize;
        MPI_Recv(localMap.data()+offset, 1, MPI_HALO_ROW_I, colRank-1, NO_TAG, MPI_COL_COMM, MPI_STATUSES_IGNORE);
    }

    // Recv from right
    if(rowRank != nx - 1)
    {
        int offset = haloSize*XmemAllocSize+XmemAllocSize-haloSize;
        MPI_Recv(localMap.data()+offset, 1, MPI_HALO_COL_I, rowRank+1, NO_TAG, MPI_ROW_COMM, MPI_STATUSES_IGNORE);
    }

    // Recv from left
    if(rowRank != 0)
    {
        int offset = haloSize*XmemAllocSize;
        MPI_Recv(localMap.data()+offset, 1, MPI_HALO_COL_I, rowRank-1, NO_TAG, MPI_ROW_COMM, MPI_STATUSES_IGNORE);
    }
}

void ParallelHeatSolver::initHaloEx()
{
    if(m_simulationProperties.IsRunParallelRMA())
    {
        MPI_Win_create(localDataB.data(), XmemAllocSize*YmemAllocSize*sizeof(float), sizeof(float), MPI_INFO_NULL, MPI_ROW_COMM, &rowWin);
        MPI_Win_create(localDataB.data(), XmemAllocSize*YmemAllocSize*sizeof(float), sizeof(float), MPI_INFO_NULL, MPI_COL_COMM, &colWin);
        MPI_Win_fence(0, colWin);
        MPI_Win_fence(0, rowWin);
    }
}

void ParallelHeatSolver::sendHalo(float* tmpA, float* tmpB)
{
    if(m_simulationProperties.IsRunParallelP2P())
    {
        // Send up
        if(colRank != 0)
        {
            int offset = XmemAllocSize*haloSize+haloSize;
            MPI_Isend(tmpB+offset, 1, MPI_HALO_ROW_F, colRank-1, NO_TAG, MPI_COL_COMM, &req[UP]);
            MPI_Request_free(&req[UP]);
        }

        // Send down
        if(colRank != ny - 1)
        {
            int offset = (YmemAllocSize-2*haloSize)*XmemAllocSize+haloSize;
            MPI_Isend(tmpB+offset, 1, MPI_HALO_ROW_F, colRank+1, NO_TAG, MPI_COL_COMM, &req[DOWN]);
            MPI_Request_free(&req[DOWN]);
        }

        // Send left
        if(rowRank != 0)
        {
            int offset = XmemAllocSize*haloSize+haloSize;
            MPI_Isend(tmpB+offset, 1, MPI_HALO_COL_F, rowRank-1, NO_TAG, MPI_ROW_COMM, &req[LEFT]);
            MPI_Request_free(&req[LEFT]);
        }

        // Send right
        if(rowRank != nx - 1)
        {
            int offset = haloSize*XmemAllocSize+XmemAllocSize-2*haloSize;
            MPI_Isend(tmpB+offset, 1, MPI_HALO_COL_F, rowRank+1, NO_TAG, MPI_ROW_COMM, &req[RIGHT]);
            MPI_Request_free(&req[RIGHT]);
        }
    }
    else if(m_simulationProperties.IsRunParallelRMA())
    {
        // Send up
        if(colRank != 0)
        {
            int offsetOrigin = XmemAllocSize*haloSize+haloSize;
            int offsetTarget = (YmemAllocSize-haloSize)*XmemAllocSize+haloSize;
            MPI_Put(tmpB+offsetOrigin, 1, MPI_HALO_ROW_F, colRank-1, offsetTarget, 1, MPI_HALO_ROW_F, colWin);
        }

        // Send down
        if(colRank != ny - 1)
        {
            int offsetOrigin = (YmemAllocSize-2*haloSize)*XmemAllocSize+haloSize;
            int offsetTarget = haloSize;
            MPI_Put(tmpB+offsetOrigin, 1, MPI_HALO_ROW_F, colRank+1, offsetTarget, 1, MPI_HALO_ROW_F, colWin);
        }

        // Send left
        if(rowRank != 0)
        {
            int offsetOrigin = XmemAllocSize*haloSize+haloSize;
            int offsetTarget = haloSize*XmemAllocSize+XmemAllocSize-haloSize;
            MPI_Put(tmpB+offsetOrigin, 1, MPI_HALO_COL_F, rowRank-1, offsetTarget, 1, MPI_HALO_COL_F, rowWin);
        }

        // Send right
        if(rowRank != nx - 1)
        {
            int offsetOrigin = haloSize*XmemAllocSize+XmemAllocSize-2*haloSize;
            int offsetTarget = haloSize*XmemAllocSize;
            MPI_Put(tmpB+offsetOrigin, 1, MPI_HALO_COL_F, rowRank+1, offsetTarget, 1, MPI_HALO_COL_F, rowWin);
        }
    }
}

void ParallelHeatSolver::recvHalo(float* tmpA, float* tmpB)
{
    if(m_simulationProperties.IsRunParallelP2P())
    {
        // Recv from down
        if(colRank != ny - 1)
        {
            int offset = (YmemAllocSize-haloSize)*XmemAllocSize+haloSize;
            MPI_Recv(tmpB+offset, 1, MPI_HALO_ROW_F, colRank+1, NO_TAG, MPI_COL_COMM, MPI_STATUSES_IGNORE);
        }

        // Recv from up
        if(colRank != 0)
        {
            int offset = haloSize;
            MPI_Recv(tmpB+offset, 1, MPI_HALO_ROW_F, colRank-1, NO_TAG, MPI_COL_COMM, MPI_STATUSES_IGNORE);
        }

        // Recv from right
        if(rowRank != nx - 1)
        {
            int offset = haloSize*XmemAllocSize+XmemAllocSize-haloSize;
            MPI_Recv(tmpB+offset, 1, MPI_HALO_COL_F, rowRank+1, NO_TAG, MPI_ROW_COMM, MPI_STATUSES_IGNORE);
        }

        // Recv from left
        if(rowRank != 0)
        {
            int offset = haloSize*XmemAllocSize;
            MPI_Recv(tmpB+offset, 1, MPI_HALO_COL_F, rowRank-1, NO_TAG, MPI_ROW_COMM, MPI_STATUSES_IGNORE);
        }
    }
    else if(m_simulationProperties.IsRunParallelRMA())
    {
        MPI_Win_fence(0, colWin);
        MPI_Win_fence(0, rowWin);
    }
}

void ParallelHeatSolver::finalizeHaloEx()
{
    if(m_simulationProperties.IsRunParallelRMA())
    {
        MPI_Win_free(&rowWin);
        MPI_Win_free(&colWin);
    }
}

void ParallelHeatSolver::allocVectorsForLocalData()
{
    // Calculate size of local edge
    localXedgeSize = m_materialProperties.GetEdgeSize() / nx;
    localYedgeSize = m_materialProperties.GetEdgeSize() / ny;

    // Calculate total size to allocate
    XmemAllocSize = localXedgeSize + 2*haloSize;
    YmemAllocSize = localYedgeSize + 2*haloSize;

    // Allocate memory
    localDataA.resize(XmemAllocSize*YmemAllocSize);
    localDataB.resize(XmemAllocSize*YmemAllocSize);
    localParams.resize(XmemAllocSize*YmemAllocSize);
    localMap.resize(XmemAllocSize*YmemAllocSize);

    // Buffer for storing
    dataToStore.resize(localXedgeSize*localYedgeSize);

    fill(localDataA.begin(), localDataA.end(), NAN);
    fill(localDataB.begin(), localDataB.end(), NAN);
    fill(localParams.begin(), localParams.end(), NAN);
    fill(localMap.begin(), localMap.end(), -1);
    fill(dataToStore.begin(), dataToStore.end(), NAN);
}

void ParallelHeatSolver::createMPIdataTypes()
{
    if(m_rank == ROOT_RANK)
    {
        MPI_Type_vector(localYedgeSize, localXedgeSize, m_materialProperties.GetEdgeSize(), MPI_FLOAT, &MPI_TILE_MAT_F);
        MPI_Type_commit(&MPI_TILE_MAT_F);    
        MPI_Type_create_resized(MPI_TILE_MAT_F, 0, localXedgeSize * sizeof(float), &MPI_TILE_MAT_RES_F);
        MPI_Type_commit(&MPI_TILE_MAT_RES_F);
        
        MPI_Type_vector(localYedgeSize, localXedgeSize, m_materialProperties.GetEdgeSize(), MPI_INT, &MPI_TILE_MAT_I);
        MPI_Type_commit(&MPI_TILE_MAT_I);    
        MPI_Type_create_resized(MPI_TILE_MAT_I, 0, localXedgeSize * sizeof(int), &MPI_TILE_MAT_RES_I);
        MPI_Type_commit(&MPI_TILE_MAT_RES_I);
    }

    MPI_Type_vector(localYedgeSize, localXedgeSize, XmemAllocSize, MPI_FLOAT, &MPI_LOCAL_TILE_MAT_F);
    MPI_Type_commit(&MPI_LOCAL_TILE_MAT_F);

    MPI_Type_vector(localYedgeSize, localXedgeSize, XmemAllocSize, MPI_INT, &MPI_LOCAL_TILE_MAT_I);
    MPI_Type_commit(&MPI_LOCAL_TILE_MAT_I);

    MPI_Type_vector(haloSize, localXedgeSize, XmemAllocSize, MPI_FLOAT, &MPI_HALO_ROW_F);
    MPI_Type_commit(&MPI_HALO_ROW_F);

    MPI_Type_vector(haloSize, localXedgeSize, XmemAllocSize, MPI_INT, &MPI_HALO_ROW_I);
    MPI_Type_commit(&MPI_HALO_ROW_I);

    MPI_Type_vector(localYedgeSize, haloSize, XmemAllocSize, MPI_FLOAT, &MPI_HALO_COL_F);
    MPI_Type_commit(&MPI_HALO_COL_F);

    MPI_Type_vector(localYedgeSize, haloSize, XmemAllocSize, MPI_INT, &MPI_HALO_COL_I);
    MPI_Type_commit(&MPI_HALO_COL_I);
}

void ParallelHeatSolver::freeMPIdataTypes()
{
    if(m_rank == ROOT_RANK)
    {
        MPI_Type_free(&MPI_TILE_MAT_F);
        MPI_Type_free(&MPI_TILE_MAT_RES_F);
        MPI_Type_free(&MPI_TILE_MAT_I);
        MPI_Type_free(&MPI_TILE_MAT_RES_I);
    }
    MPI_Type_free(&MPI_LOCAL_TILE_MAT_F);
    MPI_Type_free(&MPI_LOCAL_TILE_MAT_I);

    MPI_Type_free(&MPI_HALO_ROW_F);
    MPI_Type_free(&MPI_HALO_ROW_I);
    MPI_Type_free(&MPI_HALO_COL_F);
    MPI_Type_free(&MPI_HALO_COL_I);
}

void ParallelHeatSolver::prepForScatterOrGather()
{
    sendCounts.resize(m_size);
    displs.resize(m_size);

    for(int y = 0; y < ny; y++)
    {
        for(int x = 0; x < nx; x++)
        {
            sendCounts[y*nx + x] = 1;
            displs[y*nx + x] =  y * nx * localYedgeSize + x;
        }
    }

    offsetToCenterOffTile = XmemAllocSize*2+haloSize;
}

void ParallelHeatSolver::scatterData()
{
    MPI_Scatterv(m_materialProperties.GetInitTemp().data(),     sendCounts.data(), displs.data(), MPI_TILE_MAT_RES_F, localDataA.data()+offsetToCenterOffTile,  1, MPI_LOCAL_TILE_MAT_F, ROOT_RANK, MPI_COMM_WORLD);
    MPI_Scatterv(m_materialProperties.GetDomainParams().data(), sendCounts.data(), displs.data(), MPI_TILE_MAT_RES_F, localParams.data()+offsetToCenterOffTile, 1, MPI_LOCAL_TILE_MAT_F, ROOT_RANK, MPI_COMM_WORLD);
    MPI_Scatterv(m_materialProperties.GetDomainMap().data(),    sendCounts.data(), displs.data(), MPI_TILE_MAT_RES_I, localMap.data()+offsetToCenterOffTile,    1, MPI_LOCAL_TILE_MAT_I, ROOT_RANK, MPI_COMM_WORLD);
    copy(localDataA.begin(), localDataA.end(), localDataB.begin());
}

void ParallelHeatSolver::gatherDataTo(float* outResult)
{
    MPI_Gatherv(localDataA.data()+offsetToCenterOffTile, 1, MPI_LOCAL_TILE_MAT_F, outResult, sendCounts.data(), displs.data(), MPI_TILE_MAT_RES_F, ROOT_RANK, MPI_COMM_WORLD);
}

void ParallelHeatSolver::createMPIcomms()
{
    MPI_Comm_split(MPI_COMM_WORLD, m_rank / nx, m_rank % ny, &MPI_ROW_COMM);
    MPI_Comm_split(MPI_COMM_WORLD, m_rank % nx, m_rank / ny, &MPI_COL_COMM);

    MPI_Comm_rank(MPI_ROW_COMM, &rowRank);
    MPI_Comm_rank(MPI_COL_COMM, &colRank);

    int colIndex = m_rank % nx;
    int midColIndex = nx/2;
    if(colIndex == midColIndex)
    {
        MPI_Comm_split(MPI_COMM_WORLD, m_rank % nx, m_rank / ny, &MPI_MID_COMM);
    }
    else
    {
        MPI_Comm_split(MPI_COMM_WORLD, MPI_UNDEFINED, m_rank / ny, &MPI_MID_COMM);
    }
}

void ParallelHeatSolver::freeMPIcomms()
{
    MPI_Comm_free(&MPI_ROW_COMM);
    MPI_Comm_free(&MPI_COL_COMM);
    if(MPI_MID_COMM != MPI_COMM_NULL)
    {
        MPI_Comm_free(&MPI_MID_COMM);
    }
}