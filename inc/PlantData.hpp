
#ifndef CS354_PROJECT2_PLANT_DATA_HPP
#define CS354_PROJECT2_PLANT_DATA_HPP

#include <stddef.h>

namespace cs354 {
    extern struct BasicModel {
        size_t size;
        float *vertex, *color, *texture;
        int *elements;
        void (*drawColor)(BasicModel *);
        void (*drawTexture)(BasicModel *);
    } _n_leaf[4], _n_branch;
}

#endif
