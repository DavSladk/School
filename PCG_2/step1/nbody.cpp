/**
 * @file      nbody.cpp
 *
 * @author    Jiri Jaros \n
 *            Faculty of Information Technology \n
 *            Brno University of Technology \n
 *            jarosjir@fit.vutbr.cz
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

#include <math.h>
#include <cfloat>
#include <cstring>
#include "nbody.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//                                       Declare following structs / classes                                          //
//                                  If necessary, add your own classes / routines                                     //
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// Compute gravitation velocity
void calculate_gravitation_velocity(const Particles& p,
                                    Velocities&      tmp_vel,
                                    const int        N,
                                    const float      dt)
{
  #pragma acc parallel present(p, tmp_vel)
  {
    // vynulování tmp_vel
    #pragma acc loop
    for(int i = 0; i < N; i++)
    {
      tmp_vel.vel_x[i] = 0.0f;
      tmp_vel.vel_y[i] = 0.0f;
      tmp_vel.vel_z[i] = 0.0f;
    }

    // Copy pasta CPU implementace s úpravami pro SoA a pragma acc
    float r, dx, dy, dz;
    float vx, vy, vz;

    #pragma acc loop
    for(int i = 0; i < N; i++)
    {
      #pragma acc loop
      for(int j = 0; j < N; i++)
      {
        dx = p.pos_x[i] - p.pos_x[j];
        dy = p.pos_y[i] - p.pos_y[j];
        dz = p.pos_z[i] - p.pos_z[j];

        r = sqrt(dx*dx + dy*dy + dz*dz);

        float F = -G * p.weight[i] * p.weight[j] / (r * r + FLT_MIN);
        float nx = F * dx/ (r + FLT_MIN);
        float ny = F * dy/ (r + FLT_MIN);
        float nz = F * dz/ (r + FLT_MIN);

        vx = nx * dt / p.weight[i];
        vy = ny * dt / p.weight[i];
        vz = nz * dt / p.weight[i];

        tmp_vel.vel_x[i] += (r > COLLISION_DISTANCE) ? vx : 0.0f;
        tmp_vel.vel_y[i] += (r > COLLISION_DISTANCE) ? vy : 0.0f;
        tmp_vel.vel_z[i] += (r > COLLISION_DISTANCE) ? vz : 0.0f;
      }
    }
  }
}// end of calculate_gravitation_velocity
//----------------------------------------------------------------------------------------------------------------------

void calculate_collision_velocity(const Particles& p,
                                  Velocities&      tmp_vel,
                                  const int        N,
                                  const float      dt)
{
  #pragma acc parallel present(p, tmp_vel)
  {
    // Copy pasta CPU implementace s úpravami pro SoA a pragma acc
    float r, dx, dy, dz;
    float vx, vy, vz;

    #pragma acc loop
    for(int i = 0; i < N; i++)
    {
      #pragma acc loop
      for(int j = 0; j < N; i++)
      {
        dx = p.pos_x[i] - p.pos_x[j];
        dy = p.pos_y[i] - p.pos_y[j];
        dz = p.pos_z[i] - p.pos_z[j];

        r = sqrt(dx*dx + dy*dy + dz*dz);

        vx = ((p.weight[i] * p.vel_x[i] - p.weight[j] * p.vel_x[i] + 2 * p.weight[j] * p.vel_x[j]) / (p.weight[i] + p.weight[j])) - p.vel_x[i];
        vy = ((p.weight[i] * p.vel_y[i] - p.weight[j] * p.vel_y[i] + 2 * p.weight[j] * p.vel_y[j]) / (p.weight[i] + p.weight[j])) - p.vel_y[i];
        vz = ((p.weight[i] * p.vel_z[i] - p.weight[j] * p.vel_z[i] + 2 * p.weight[j] * p.vel_z[j]) / (p.weight[i] + p.weight[j])) - p.vel_z[i];

        // jedna se o rozdilne ale blizke prvky
        if (r > 0.0f && r < COLLISION_DISTANCE)
        {
            tmp_vel.vel_x[i] += vx;
            tmp_vel.vel_y[i] += vy;
            tmp_vel.vel_z[i] += vz;
        }
      }
    }
  }
}// end of calculate_collision_velocity
//----------------------------------------------------------------------------------------------------------------------

/// Update particle position
void update_particle(const Particles& p,
                     Velocities&      tmp_vel,
                     const int        N,
                     const float      dt)
{
  // Copy pasta CPU implementace s úpravami pro SoA a pragma acc
  #pragma acc parallel present(p, tmp_vel)
  {
    #pragma acc loop
    for(int i = 0; i < N; i++)
    {
      p.vel_x[i] += tmp_vel.vel_x[i];
      p.vel_y[i] += tmp_vel.vel_y[i];
      p.vel_z[i] += tmp_vel.vel_z[i];

      p.pos_x[i] += p.vel_x[i] * dt;
      p.pos_y[i] += p.vel_y[i] * dt;
      p.pos_z[i] += p.vel_z[i] * dt;
    }
  }
}// end of update_particle
//----------------------------------------------------------------------------------------------------------------------

/// Compute center of gravity
float4 centerOfMassGPU(const Particles& p,
                       const int        N)
{

  return {0.0f, 0.0f, 0.0f, 0.0f};
}// end of centerOfMassGPU
//----------------------------------------------------------------------------------------------------------------------

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// Compute center of mass on CPU
float4 centerOfMassCPU(MemDesc& memDesc)
{
  float4 com = {0 ,0, 0, 0};

  for(int i = 0; i < memDesc.getDataSize(); i++)
  {
    // Calculate the vector on the line connecting points and most recent position of center-of-mass
    const float dx = memDesc.getPosX(i) - com.x;
    const float dy = memDesc.getPosY(i) - com.y;
    const float dz = memDesc.getPosZ(i) - com.z;

    // Calculate weight ratio only if at least one particle isn't massless
    const float dw = ((memDesc.getWeight(i) + com.w) > 0.0f)
                          ? ( memDesc.getWeight(i) / (memDesc.getWeight(i) + com.w)) : 0.0f;

    // Update position and weight of the center-of-mass according to the weight ration and vector
    com.x += dx * dw;
    com.y += dy * dw;
    com.z += dz * dw;
    com.w += memDesc.getWeight(i);
  }
  return com;
}// end of centerOfMassCPU
//----------------------------------------------------------------------------------------------------------------------

// PARTICLES ------------------------------------------

Particles::Particles(size_t N) : size(N)
{
  allocCPUMemory();
  allocGPUMemory();
}

Particles::~Particles()
{
  deleteGPUMemory();
  deleteCPUMemory();
}

void Particles::allocCPUMemory()
{
  pos_x  = new float[size];
  pos_y  = new float[size];
  pos_z  = new float[size];
  vel_x  = new float[size];
  vel_y  = new float[size];
  vel_z  = new float[size];
  weight = new float[size];
}

void Particles::allocGPUMemory()
{
  #pragma acc enter data copyin(this)
  #pragma acc enter data create(pos_x [size])
  #pragma acc enter data create(pos_y [size])
  #pragma acc enter data create(pos_z [size])
  #pragma acc enter data create(vel_x [size])
  #pragma acc enter data create(vel_y [size])
  #pragma acc enter data create(vel_z [size])
  #pragma acc enter data create(weight[size])
}

void Particles::deleteCPUMemory()
{
  delete [] pos_x ;
  delete [] pos_y ;
  delete [] pos_z ;
  delete [] vel_x ;
  delete [] vel_y ;
  delete [] vel_z ;
  delete [] weight;
}

void Particles::deleteGPUMemory()
{
  #pragma acc exit data delete(pos_x [size])
  #pragma acc exit data delete(pos_y [size])
  #pragma acc exit data delete(pos_z [size])
  #pragma acc exit data delete(vel_x [size])
  #pragma acc exit data delete(vel_y [size])
  #pragma acc exit data delete(vel_z [size])
  #pragma acc exit data delete(weight[size])
  #pragma acc exit data delete(this)
}

void Particles::copyToGPU()
{
  #pragma acc update device(pos_x [size])
  #pragma acc update device(pos_y [size])
  #pragma acc update device(pos_z [size])
  #pragma acc update device(vel_x [size])
  #pragma acc update device(vel_y [size])
  #pragma acc update device(vel_z [size])
  #pragma acc update device(weight[size])
}

void Particles::copyToCPU()
{
  #pragma acc update host(pos_x [size])
  #pragma acc update host(pos_y [size])
  #pragma acc update host(pos_z [size])
  #pragma acc update host(vel_x [size])
  #pragma acc update host(vel_y [size])
  #pragma acc update host(vel_z [size])
  #pragma acc update host(weight[size])
}

// VELOCITIES ------------------------------------------

Velocities::Velocities(size_t N) : size(N)
{
  this->allocCPUMemory();
  this->allocGPUMemory();
}

Velocities::~Velocities()
{
  this->deleteGPUMemory();
  this->deleteCPUMemory();
}

void Velocities::allocCPUMemory()
{
  vel_x = new float[size];
  vel_y = new float[size];
  vel_z = new float[size];
}

void Velocities::allocGPUMemory()
{
  #pragma acc enter data copyin(this)
  #pragma acc enter data create(vel_x[size])
  #pragma acc enter data create(vel_y[size])
  #pragma acc enter data create(vel_z[size])
}

void Velocities::deleteCPUMemory()
{
  delete [] vel_x;
  delete [] vel_y;
  delete [] vel_z;
}

void Velocities::deleteGPUMemory()
{
  #pragma acc exit data delete(vel_x[size])
  #pragma acc exit data delete(vel_y[size])
  #pragma acc exit data delete(vel_z[size])
  #pragma acc exit data delete(this)
}

void Velocities::copyToGPU()
{
  #pragma acc update device(vel_x[size])
  #pragma acc update device(vel_y[size])
  #pragma acc update device(vel_z[size])
}

void Velocities::copyToCPU()
{
  #pragma acc update host(vel_x[size])
  #pragma acc update host(vel_y[size])
  #pragma acc update host(vel_z[size])
}

void Velocities::setZeroCPU()
{
  memset(this->vel_x, 0, size * sizeof(float));
  memset(this->vel_y, 0, size * sizeof(float));
  memset(this->vel_z, 0, size * sizeof(float));
}

void Velocities::setZeroGPU()
{
  this->setZeroCPU();
  this->copyToGPU();
}
