/**
 * @File nbody.cu
 *
 * Implementation of the N-Body problem
 *
 * Paralelní programování na GPU (PCG 2022)
 * Projekt c. 1 (cuda)
 * Login: xsladk07
 */

#include <cmath>
#include <cfloat>
#include "nbody.h"

/**
 * CUDA kernel to calculate gravitation velocity
 * @param p       - particles
 * @param tmp_vel - temp array for velocities
 * @param N       - Number of particles
 * @param dt      - Size of the time step
 */
__global__ void calculate_gravitation_velocity(t_particles p, t_velocities tmp_vel, int N, float dt)
{
  float r, dx, dy, dz;
  float vx, vy, vz;
  float r3, G_dt_r3, Fg_dt_m2_r;

  float tmp_x = 0;
  float tmp_y = 0;
  float tmp_z = 0;

  float isComputable = 0;

  int index = blockDim.x * blockIdx.x + threadIdx.x;

  for(int i = 0; i < N; i++)
  {
    // (index != i) <- make sure, you are not computing effects of particle on itself
    // (r > COLLISION_DISTANCE) <- because it was in the cpu implementation
    isComputable = (index != i) && (r > COLLISION_DISTANCE);

    dx = p.pos_x[index] - p.pos_x[i];
    dy = p.pos_y[index] - p.pos_y[i];
    dz = p.pos_z[index] - p.pos_z[i];

    r = sqrt(dx*dx + dy*dy + dz*dz);

    r3 = r * r * r + FLT_MIN;
    G_dt_r3 = -G * dt / r3;
    Fg_dt_m2_r = G_dt_r3 * p.weight[i];

    vx = Fg_dt_m2_r * dx;
    vy = Fg_dt_m2_r * dy;
    vz = Fg_dt_m2_r * dz;

    tmp_x += vx * isComputable;
    tmp_y += vy * isComputable;
    tmp_z += vz * isComputable;
  }
  
  tmp_vel.vel_x[index] += tmp_x;
  tmp_vel.vel_y[index] += tmp_y;
  tmp_vel.vel_z[index] += tmp_z;
}// end of calculate_gravitation_velocity
//----------------------------------------------------------------------------------------------------------------------

/**
 * CUDA kernel to calculate collision velocity
 * @param p       - particles
 * @param tmp_vel - temp array for velocities
 * @param N       - Number of particles
 * @param dt      - Size of the time step
 */
__global__ void calculate_collision_velocity(t_particles p, t_velocities tmp_vel, int N, float dt)
{
  float r, dx, dy, dz;
  float vx, vy, vz;

  float tmp_x = 0;
  float tmp_y = 0;
  float tmp_z = 0;

  float isComputable = 0;

  int index = blockDim.x * blockIdx.x + threadIdx.x;

  for(int i = 0; i < N; i++)
  {
    isComputable = (index != i) && (r < COLLISION_DISTANCE) && (r > 0.0f);

    dx = p.pos_x[index] - p.pos_x[i];
    dy = p.pos_y[index] - p.pos_y[i];
    dz = p.pos_z[index] - p.pos_z[i];

    r = sqrt(dx*dx + dy*dy + dz*dz);

    vx = ((p.weight[index] * p.vel_x[index] - p.weight[i] * p.vel_x[index] + 2 * p.weight[i] * p.vel_x[i]) / (p.weight[index] + p.weight[i])) - p.vel_x[index];
    vy = ((p.weight[index] * p.vel_y[index] - p.weight[i] * p.vel_y[index] + 2 * p.weight[i] * p.vel_y[i]) / (p.weight[index] + p.weight[i])) - p.vel_y[index];
    vz = ((p.weight[index] * p.vel_z[index] - p.weight[i] * p.vel_z[index] + 2 * p.weight[i] * p.vel_z[i]) / (p.weight[index] + p.weight[i])) - p.vel_z[index];

    tmp_x += vx * isComputable;
    tmp_y += vy * isComputable;
    tmp_z += vz * isComputable;
  }

  tmp_vel.vel_x[index] += tmp_x;
  tmp_vel.vel_y[index] += tmp_y;
  tmp_vel.vel_z[index] += tmp_z;

}// end of calculate_collision_velocity
//----------------------------------------------------------------------------------------------------------------------

/**
 * CUDA kernel to update particles
 * @param p       - particles
 * @param tmp_vel - temp array for velocities
 * @param N       - Number of particles
 * @param dt      - Size of the time step
 */
__global__ void update_particle(t_particles p, t_velocities tmp_vel, int N, float dt)
{
  int index = blockDim.x * blockIdx.x + threadIdx.x;

  p.vel_x[index] += tmp_vel.vel_x[index];
  p.vel_y[index] += tmp_vel.vel_y[index];
  p.vel_z[index] += tmp_vel.vel_z[index];

  p.pos_x[index] += p.vel_x[index] * dt;
  p.pos_y[index] += p.vel_y[index] * dt;
  p.pos_z[index] += p.vel_z[index] * dt;
}// end of update_particle
//----------------------------------------------------------------------------------------------------------------------

/**
 * CUDA kernel to update particles
 * @param p_in    - particles input
 * @param p_out   - particles output
 * @param N       - Number of particles
 * @param dt      - Size of the time step
 */
__global__ void calculate_velocity(t_particles p_in, t_particles p_out, int N, float dt)
{
  float r, dx, dy, dz;
  float vx, vy, vz;
  float r3, G_dt_r3, Fg_dt_m2_r;

  float tmp_x = 0;
  float tmp_y = 0;
  float tmp_z = 0;

  float isComputable = 0;

  int index = blockDim.x * blockIdx.x + threadIdx.x;

  for(int i = 0; i < N; i++)
  {
    // GRAVITY


    dx = p_in.pos_x[index] - p_in.pos_x[i];
    dy = p_in.pos_y[index] - p_in.pos_y[i];
    dz = p_in.pos_z[index] - p_in.pos_z[i];

    r = sqrt(dx*dx + dy*dy + dz*dz);

    // (index != i) <- make sure, you are not computing effects of particle on itself
    // (r > COLLISION_DISTANCE) <- because it was in the cpu implementation
    isComputable = (index != i) && (r > COLLISION_DISTANCE);

    r3 = r * r * r + FLT_MIN;
    G_dt_r3 = -G * dt / r3;
    Fg_dt_m2_r = G_dt_r3 * p_in.weight[i];

    vx = Fg_dt_m2_r * dx;
    vy = Fg_dt_m2_r * dy;
    vz = Fg_dt_m2_r * dz;

    tmp_x += vx * isComputable;
    tmp_y += vy * isComputable;
    tmp_z += vz * isComputable;

    // COLLISION

    isComputable = (index != i) && (r < COLLISION_DISTANCE) && (r > 0.0f);

    vx = ((p_in.weight[index] * p_in.vel_x[index] - p_in.weight[i] * p_in.vel_x[index] + 2 * p_in.weight[i] * p_in.vel_x[i]) / (p_in.weight[index] + p_in.weight[i])) - p_in.vel_x[index];
    vy = ((p_in.weight[index] * p_in.vel_y[index] - p_in.weight[i] * p_in.vel_y[index] + 2 * p_in.weight[i] * p_in.vel_y[i]) / (p_in.weight[index] + p_in.weight[i])) - p_in.vel_y[index];
    vz = ((p_in.weight[index] * p_in.vel_z[index] - p_in.weight[i] * p_in.vel_z[index] + 2 * p_in.weight[i] * p_in.vel_z[i]) / (p_in.weight[index] + p_in.weight[i])) - p_in.vel_z[index];

    tmp_x += vx * isComputable;
    tmp_y += vy * isComputable;
    tmp_z += vz * isComputable;
  }

  p_out.vel_x[index] += tmp_x;
  p_out.vel_y[index] += tmp_y;
  p_out.vel_z[index] += tmp_z;

  p_out.pos_x[index] += p_out.vel_x[index] * dt;
  p_out.pos_y[index] += p_out.vel_y[index] * dt;
  p_out.pos_z[index] += p_out.vel_z[index] * dt;

}

/**
 * CUDA kernel to update particles
 * @param p       - particles
 * @param comX    - pointer to a center of mass position in X
 * @param comY    - pointer to a center of mass position in Y
 * @param comZ    - pointer to a center of mass position in Z
 * @param comW    - pointer to a center of mass weight
 * @param lock    - pointer to a user-implemented lock
 * @param N       - Number of particles
 */
__global__ void centerOfMass(t_particles p, float* comX, float* comY, float* comZ, float* comW, int* lock, const int N)
{

}// end of centerOfMass
//----------------------------------------------------------------------------------------------------------------------

/**
 * CPU implementation of the Center of Mass calculation
 * @param particles - All particles in the system
 * @param N         - Number of particles
 */
__host__ float4 centerOfMassCPU(MemDesc& memDesc)
{
  float4 com = {0 ,0, 0, 0};

  for(int i = 0; i < memDesc.getDataSize(); i++)
  {
    // Calculate the vector on the line connecting current body and most recent position of center-of-mass
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
}// enf of centerOfMassCPU
//----------------------------------------------------------------------------------------------------------------------
