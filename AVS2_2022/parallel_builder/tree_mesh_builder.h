/**
 * @file    tree_mesh_builder.h
 *
 * @author  David Sladký <xsladk07@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP tasks + octree early elimination
 *
 * @date    15.12.2022
 **/

#ifndef TREE_MESH_BUILDER_H
#define TREE_MESH_BUILDER_H

#include <vector>
#include <climits>
#include "base_mesh_builder.h"

class TreeMeshBuilder : public BaseMeshBuilder
{
public:
    TreeMeshBuilder(unsigned gridEdgeSize);

protected:
    unsigned marchCubes(const ParametricScalarField &field);
    float evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field);
    void emitTriangle(const Triangle_t &triangle);
    const Triangle_t *getTrianglesArray() const { return mTriangles.data(); }

    unsigned taskGenerator(const Vec3_t<float> &minIndex, \
                           const Vec3_t<float> &maxIndex, \
                           const float edgeSize, \
                           const int depthLevel, \
                           const ParametricScalarField &field);

    std::vector<Triangle_t> mTriangles;

private:
    const int CUT_OFF_LEVEL = 3;
    const float SQRT_3_DIV_2 = 0.86602540378;
    const int MIDDLE_INDEX_CORNER = 6;
};

#endif // TREE_MESH_BUILDER_H
