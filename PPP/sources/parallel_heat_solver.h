/**
 * @file    parallel_heat_solver.h
 * @author  xlogin00 <xlogin00@stud.fit.vutbr.cz>
 *
 * @brief   Course: PPP 2021/2022 - Project 1
 *
 * @date    2022-MM-DD
 */

#ifndef PARALLEL_HEAT_SOLVER_H
#define PARALLEL_HEAT_SOLVER_H

#include <cmath>
#include "base_heat_solver.h"

/**
 * @brief The ParallelHeatSolver class implements parallel MPI based heat
 *        equation solver in 2D using 1D and 2D block grid decomposition.
 */
class ParallelHeatSolver : public BaseHeatSolver
{
    //============================================================================//
    //                            *** BEGIN: NOTE ***
    //
    // Modify this class declaration as needed.
    // This class needs to provide at least:
    // - Constructor which passes SimulationProperties and MaterialProperties
    //   to the base class. (see below)
    // - Implementation of RunSolver method. (see below)
    // 
    // It is strongly encouraged to define methods and member variables to improve 
    // readability of your code!
    //
    //                             *** END: NOTE ***
    //============================================================================//
    
public:
    /**
     * @brief Constructor - Initializes the solver. This should include things like:
     *        - Construct 1D or 2D grid of tiles.
     *        - Create MPI datatypes used in the simulation.
     *        - Open SEQUENTIAL or PARALLEL HDF5 file.
     *        - Allocate data for local tile.
     *        - Initialize persistent communications?
     *
     * @param simulationProps Parameters of simulation - passed into base class.
     * @param materialProps   Parameters of material - passed into base class.
     */
    ParallelHeatSolver(SimulationProperties &simulationProps, MaterialProperties &materialProps);
    virtual ~ParallelHeatSolver();

    /**
     * @brief Run main simulation loop.
     * @param outResult Output array which is to be filled with computed temperature values.
     *                  The vector is pre-allocated and its size is given by dimensions
     *                  of the input file (edgeSize*edgeSize).
     *                  NOTE: The vector is allocated (and should be used) *ONLY*
     *                        by master process (rank 0 in MPI_COMM_WORLD)!
     */
    virtual void RunSolver(std::vector<float, AlignedAllocator<float> > &outResult);

    void printVectorF(std::vector<float, AlignedAllocator<float> > &vec, int x, int y)
    {
            std::cout << std::scientific;
    for(int i = 0; i < y; i++)
    {
        for(int j = 0; j < x; j++)
        {
            std::cout << vec.data()[i*x + j] << " ";

        }
        std::cout << std::endl;
    }
        std::cout << std::defaultfloat;
    return;
    }

protected:
    int m_rank;     ///< Process rank in global (MPI_COMM_WORLD) communicator.
    int m_size;     ///< Total number of processes in MPI_COMM_WORLD.

    const int haloSize = 2;
    const int ROOT_RANK = 0;
    const int NO_TAG = 0;

    int nx;
    int ny;

    std::vector<float, AlignedAllocator<float>> localDataA;
    std::vector<float, AlignedAllocator<float> > localDataB;
    std::vector<int, AlignedAllocator<int> > localMap;
    std::vector<float, AlignedAllocator<float> > localParams;

    int localXedgeSize;
    int localYedgeSize;

    int XmemAllocSize;
    int YmemAllocSize;

    void allocVectorsForLocalData();
    void createMPIdataTypes();
    void freeMPIdataTypes();
    void prepForScatterOrGather();
    void scatterData();
    void gatherDataTo(float* outResult);
    void createMPIcomms();
    void freeMPIcomms();
    void finalizeHaloEx();
    void recvHalo(float* tmpA, float* tmpB);
    void sendHalo(float* tmpA, float* tmpB);
    void initHaloEx();
    void exchangeParamsAndMap();
    void setOffsets();
    void calculateTile(float* outResult);
    void calculateHalos(float* tmpA, float* tmpB);
    void calculateCenter(float* tmpA, float* tmpB);
    float calculateAvgTemp();
    void storeData(size_t iteration, float *dataS, float *dataP);
    void ParallelStoreDataInto(hid_t fileHandle, size_t iteration, const float *data);

    void fillDummyData();

    MPI_Datatype MPI_TILE_MAT_F;
    MPI_Datatype MPI_TILE_MAT_RES_F;

    MPI_Datatype MPI_TILE_MAT_I;
    MPI_Datatype MPI_TILE_MAT_RES_I;

    MPI_Datatype MPI_LOCAL_TILE_MAT_F;
    MPI_Datatype MPI_LOCAL_TILE_MAT_I;

    MPI_Datatype MPI_HALO_ROW_F;
    MPI_Datatype MPI_HALO_COL_F;

    MPI_Datatype MPI_HALO_ROW_I;
    MPI_Datatype MPI_HALO_COL_I;

    std::vector<int> sendCounts;
    std::vector<int> displs;
    int offsetToCenterOffTile;

    MPI_Comm MPI_ROW_COMM;
    MPI_Comm MPI_COL_COMM;
    MPI_Comm MPI_MID_COMM;

    int rowRank;
    int colRank;

    MPI_Win rowWin;
    MPI_Win colWin;

    MPI_Request req[4];

    enum{
        UP = 0,
        DOWN,
        LEFT,
        RIGHT,
        CENTER
    };

    int offsetX[5];
    int offsetY[5];
    int sizeX[5];
    int sizeY[5];

    AutoHandle<hid_t> m_fileHandle;
    std::vector<float> dataToStore;

};

#endif // PARALLEL_HEAT_SOLVER_H
