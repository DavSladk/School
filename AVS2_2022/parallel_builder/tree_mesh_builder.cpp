/**
 * @file    tree_mesh_builder.cpp
 *
 * @author  David Sladký <xsladk07@stud.fit.vutbr.cz>
 *
 * @brief   Parallel Marching Cubes implementation using OpenMP tasks + octree early elimination
 *
 * @date    15.12.2022
 **/

#include <iostream>
#include <math.h>
#include <limits>

#include "tree_mesh_builder.h"

TreeMeshBuilder::TreeMeshBuilder(unsigned gridEdgeSize)
    : BaseMeshBuilder(gridEdgeSize, "Octree")
{

}

unsigned TreeMeshBuilder::marchCubes(const ParametricScalarField &field)
{
    Vec3_t<float> minIndex(0);
    Vec3_t<float> maxIndex(mGridSize/*-1*/);
    Vec3_t<float> fieldSize = field.getSize();
    float edgeSize = std::max(fieldSize.x, std::max(fieldSize.y, fieldSize.z));
    int depthLevel = 0;
    unsigned int rtn = 0;

    #pragma omp parallel shared(rtn) firstprivate(minIndex, maxIndex, fieldSize, edgeSize, depthLevel)
    {
        #pragma omp master
        {
            rtn = taskGenerator(minIndex, maxIndex, edgeSize, depthLevel, field);
        }
    }

    return rtn;
}

unsigned TreeMeshBuilder::taskGenerator(const Vec3_t<float> &minIndex, \
                                        const Vec3_t<float> &maxIndex, \
                                        const float edgeSize, \
                                        const int depthLevel, \
                                        const ParametricScalarField &field)
{
    // 1. jestli dojdu do cut off, tak projdi svuj sektor sekvencne
    if(depthLevel > CUT_OFF_LEVEL)
    {
        Vec3_t<float> tmp;
        float sum = 0;
        for(float z = minIndex.z; z < maxIndex.z + 1; z++)
        {
            for(float y = minIndex.y; y < maxIndex.y + 1; y++)
            {
                for(float x = minIndex.x; x < maxIndex.x + 1; x++)
                {
                    tmp.x = x;
                    tmp.y = y;
                    tmp.z = z;
                    sum += buildCube(tmp, field);
                }
            }
        }
        return sum;
    }
    // -----------------------------------------------------------

    // 2. Vypocitej rozsah kostek, ktery spravujes.
    int midIndex, midIndexX, midIndexY, midIndexZ;
    midIndex = maxIndex.x - minIndex.x;

    // 3. zda task spravuje uz jenom jednu kostku, negeneruj tasky a pocitej
    if(midIndex <= 0)
    {
        Vec3_t<float> tmp(minIndex.x, minIndex.y, minIndex.z);
        return buildCube(tmp, field);
    }

    // 4. Spocitej index prostredni kostky ve spravovanem prostoru
    midIndexX = minIndex.x + midIndex / 2;
    midIndexY = minIndex.y + midIndex / 2;
    midIndexZ = minIndex.z + midIndex / 2;

    // 5. dostane realne souradnice prostredni kostky
    Vec3_t<float> test(0);
    test.x = (midIndexX + sc_vertexNormPos[MIDDLE_INDEX_CORNER].x) * mGridResolution;
    test.y = (midIndexY + sc_vertexNormPos[MIDDLE_INDEX_CORNER].y) * mGridResolution;
    test.z = (midIndexZ + sc_vertexNormPos[MIDDLE_INDEX_CORNER].z) * mGridResolution;

    // 6. ziska hodnotu rohu uprostred sveho bloku - vyzkouset ruzne indexy - nejspis 7?
    float centerValue = evaluateFieldAt(test, field);
    float conditionValue = mIsoLevel + SQRT_3_DIV_2 * edgeSize;

    // 9. Vygeneruj 8 tasku jesli je splnena podminka
    if(centerValue <= conditionValue)
    {
        unsigned int one, two, three, four, five, six, seven, eight;
        Vec3_t<float> min, max;

        min.x = minIndex.x; max.x = midIndexX;
        min.y = minIndex.y; max.y = midIndexY;
        min.z = minIndex.z; max.z = midIndexZ;
        #pragma omp task firstprivate(min, max, edgeSize, depthLevel, field) shared(one)
        one   = taskGenerator(min, max, edgeSize / 2.0, depthLevel + 1, field);

        min.x = minIndex.x; max.x = midIndexX;
        min.y = minIndex.y; max.y = midIndexY;
        min.z = midIndexZ + 1; max.z = maxIndex.z;
        #pragma omp task firstprivate(min, max, edgeSize, depthLevel, field) shared(two)
        two   = taskGenerator(min, max, edgeSize / 2.0, depthLevel + 1, field);

        min.x = minIndex.x; max.x = midIndexX;
        min.y = midIndexY + 1; max.y = maxIndex.y;
        min.z = minIndex.z; max.z = midIndexZ;
        #pragma omp task firstprivate(min, max, edgeSize, depthLevel, field) shared(three)
        three = taskGenerator(min, max, edgeSize / 2.0, depthLevel + 1, field);

        min.x = minIndex.x; max.x = midIndexX;
        min.y = midIndexY + 1; max.y = maxIndex.y;
        min.z = midIndexZ + 1; max.z = maxIndex.z;
        #pragma omp task firstprivate(min, max, edgeSize, depthLevel, field) shared(four)
        four  = taskGenerator(min, max, edgeSize / 2.0, depthLevel + 1, field);

        min.x = midIndexX + 1; max.x = maxIndex.x;
        min.y = minIndex.y; max.y = midIndexY;
        min.z = minIndex.z; max.z = midIndexZ;
        #pragma omp task firstprivate(min, max, edgeSize, depthLevel, field) shared(five)
        five  = taskGenerator(min, max, edgeSize / 2.0, depthLevel + 1, field);

        min.x = midIndexX + 1; max.x = maxIndex.x;
        min.y = minIndex.y; max.y = midIndexY;
        min.z = midIndexZ + 1; max.z = maxIndex.z;
        #pragma omp task firstprivate(min, max, edgeSize, depthLevel, field) shared(six)
        six   = taskGenerator(min, max, edgeSize / 2.0, depthLevel + 1, field);

        min.x = midIndexX + 1; max.x = maxIndex.x;
        min.y = midIndexY + 1; max.y = maxIndex.y;
        min.z = minIndex.z; max.z = midIndexZ;
        #pragma omp task firstprivate(min, max, edgeSize, depthLevel, field) shared(seven)
        seven = taskGenerator(min, max, edgeSize / 2.0, depthLevel + 1, field);

        min.x = midIndexX + 1; max.x = maxIndex.x;
        min.y = midIndexY + 1; max.y = maxIndex.y;
        min.z = midIndexZ + 1; max.z = maxIndex.z;
        #pragma omp task firstprivate(min, max, edgeSize, depthLevel, field) shared(eight)
        eight = taskGenerator(min, max, edgeSize / 2.0, depthLevel + 1, field);
        
        // Pockej na vysledky tasku
        #pragma omp taskwait

        return one + two + three + four + five + six + seven + eight;
    }
    else
    {
        return 0;
    }
}

float TreeMeshBuilder::evaluateFieldAt(const Vec3_t<float> &pos, const ParametricScalarField &field)
{
    const Vec3_t<float> *pPoints = field.getPoints().data();
    const unsigned count = unsigned(field.getPoints().size());

    float value = std::numeric_limits<float>::max();

    for(unsigned i = 0; i < count; ++i)
    {
        float distanceSquared  = (pos.x - pPoints[i].x) * (pos.x - pPoints[i].x);
        distanceSquared       += (pos.y - pPoints[i].y) * (pos.y - pPoints[i].y);
        distanceSquared       += (pos.z - pPoints[i].z) * (pos.z - pPoints[i].z);

        value = std::min(value, distanceSquared);
    }

    return sqrt(value);
}

void TreeMeshBuilder::emitTriangle(const BaseMeshBuilder::Triangle_t &triangle)
{
    #pragma omp critical
    mTriangles.push_back(triangle);
}
