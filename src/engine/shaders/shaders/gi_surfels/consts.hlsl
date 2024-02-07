#ifndef SURFELS_CONSTS_HLSL
#define SURFELS_CONSTS_HLSL

#define CELLS_DIM_X                       21
#define CELLS_DIM_Y                       CELLS_DIM_X
#define CELLS_DIM_Z                       CELLS_DIM_X
#define CELLS_DIMS                        uint3(CELLS_DIM_X, CELLS_DIM_Y, CELLS_DIM_Z)
#define CELLS_HALF_DIM                    uint3((CELLS_DIMS - 1.0.xxx)/2)
#define CELLS_DIMS_F                      float3(CELLS_DIM_X, CELLS_DIM_Y, CELLS_DIM_Z)
#define CELL_SIZE                         1.0

#define SURFEL_COUNT_TOTAL                (250 * 1024)
#define SURFEL_COUNT_PER_CELL             10
#define SURFEL_RADIUS                     0.3

#define CASCADE_ZERO    0
#define CASCADE_X       1
#define CASCADE_MINUS_X 2
#define CASCADE_Y       3
#define CASCADE_MINUS_Y 4
#define CASCADE_Z       5
#define CASCADE_MINUS_Z 6

#endif