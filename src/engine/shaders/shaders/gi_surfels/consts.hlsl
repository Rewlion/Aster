#ifndef SURFELS_CONSTS_HLSL
#define SURFELS_CONSTS_HLSL

#define CELLS_DIM                         21
#define CELLS_DIMS                        int3(CELLS_DIM, CELLS_DIM, CELLS_DIM)
#define CELLS_HALF_DIM                    int3((CELLS_DIMS - 1.0.xxx)/2)
#define CELLS_DIMS_F                      float3(CELLS_DIM, CELLS_DIM, CELLS_DIM)
#define CELL_SIZE                         1.0

#define SURFEL_COUNT_TOTAL                (250 * 1024)
#define SURFEL_COUNT_PER_CELL             10
#define SURFEL_RADIUS                     0.4

#define CASCADE_ZERO    0
#define CASCADE_X       1
#define CASCADE_MINUS_X 2
#define CASCADE_Y       3
#define CASCADE_MINUS_Y 4
#define CASCADE_Z       5
#define CASCADE_MINUS_Z 6
#define CASCADES_COUNT  CASCADE_MINUS_Z

#define CELLS_PER_CASCADE uint(CELLS_DIM * CELLS_DIM * CELLS_DIM)
#define CELLS_COUNT uint(CASCADES_COUNT * CELLS_DIM * CELLS_DIM * CELLS_DIM)

#define SPATIAL_STORAGE_CELL_PAYLOAD (1 + SURFEL_COUNT_PER_CELL) //[count,id,id,id,id...]

#define NO_AVAILABLE_SURFEL uint(-1)

#endif