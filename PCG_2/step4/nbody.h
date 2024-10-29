/**
 * @file      nbody.h
 *
 * @author    David Sladký \n
 *            xsladk07@stud.fit.vutbr.cz
 *
 * @brief     PCG Assignment 2
 *            N-Body simulation in ACC
 *
 * @version   2022
 *
 * @date      11 November  2020, 11:22 (created) \n
 * @date      15 November  2022, 14:10 (revised) \n
 *
 */

#ifndef __NBODY_H__
#define __NBODY_H__

#include <cstdlib>
#include <cstdio>
#include  <cmath>
#include "h5Helper.h"

/// Gravity constant
constexpr float G = 6.67384e-11f;

/// Collision distance threshold
constexpr float COLLISION_DISTANCE = 0.01f;

constexpr int TMP = 1;
constexpr int WRITE_TO_HOST = 2;

/**
 * @struct float4
 * Structure that mimics CUDA float4
 */
struct float4
{
  float x;
  float y;
  float z;
  float w;
};

/// Define sqrtf from CUDA libm library
#pragma acc routine(sqrtf) seq

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                       Declare following structs / classes                                          //
//                                  If necessary, add your own classes / routines                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Structure with particle data
 */
struct Particles
{
  // Fill the structure holding the particle/s data
  // It is recommended to implement constructor / destructor and copyToGPU and copyToCPU routines
  float *pos_x;
  float *pos_y;
  float *pos_z;
  float *vel_x;
  float *vel_y;
  float *vel_z;
  float *weight;

  size_t size;
  // size_t realSize;
  // int logValue;

  Particles(size_t N);
  Particles(const Particles &p);
  ~Particles();

  void allocCPUMemory();
  void allocGPUMemory();
  void setCPUMemToZero();

  void deleteCPUMemory();
  void deleteGPUMemory();

  void copyToGPU();
  void copyToCPU();
};// end of Particles
//----------------------------------------------------------------------------------------------------------------------

/**
 * @struct Velocities
 * Velocities of the particles
 */
struct Velocities
{
  // Fill the structure holding the particle/s data
  // It is recommended to implement constructor / destructor and copyToGPU and copyToCPU routines
  float *vel_x;
  float *vel_y;
  float *vel_z;

  size_t size;

  Velocities(size_t N);
  ~Velocities();

  void allocCPUMemory();
  void allocGPUMemory();

  void deleteCPUMemory();
  void deleteGPUMemory();

  void copyToGPU();
  void copyToCPU();

  void setZeroCPU();
  void setZeroGPU();
};// end of Velocities
//----------------------------------------------------------------------------------------------------------------------

/**
 * Compute gravitation velocity
 * @param [in]  p        - Particles
 * @param [out] tmp_vel  - Temporal velocity
 * @param [in ] N        - Number of particles
 * @param [in]  dt       - Time step size
 */
void calculate_gravitation_velocity(const Particles& p,
                                    Velocities&      tmp_vel,
                                    const int        N,
                                    const float      dt);

/**
 * Calculate collision velocity
 * @param [in]  p        - Particles
 * @param [out] tmp_vel  - Temporal velocity
 * @param [in ] N        - Number of particles
 * @param [in]  dt       - Time step size
 */
void calculate_collision_velocity(const Particles& p,
                                  Velocities&      tmp_vel,
                                  const int        N,
                                  const float      dt);

/**
 * Update particle position
 * @param [in]  p        - Particles
 * @param [out] tmp_vel  - Temporal velocity
 * @param [in ] N        - Number of particles
 * @param [in]  dt       - Time step size
 */
void update_particle(const Particles& p,
                     Velocities&      tmp_vel,
                     const int        N,
                     const float      dt);


/**
 * Combines above functions into one.
 * @param [in]  p_in        - Particles in
 * @param [out] p_out       - Particles out
 * @param [in ] N        - Number of particles
 * @param [in]  dt       - Time step size
 */
void calculate_velocity(const Particles& p_in,
                        Particles& p_out,
                        const int        N,
                        const float      dt);


/**
 * Compute center of gravity - implement in steps 3 and 4.
 * @param [in] p - Particles
 * @param [in] N - Number of particles
 * @return Center of Mass [x, y, z] and total weight[w]
 */
float4 centerOfMassGPU(const Particles& p,
                       const int        N);

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Compute center of mass on CPU
 * @param memDesc
 * @return centre of gravity
 */
float4 centerOfMassCPU(MemDesc& memDesc);

#endif /* __NBODY_H__ */
