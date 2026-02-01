//
// Created by dew on 2/1/26.
//

#ifndef MAIN_UPSR_BOUNDARY_H
#define MAIN_UPSR_BOUNDARY_H
// Clamp coordinate to valid image range
static inline int upsr_clamp(int v , int lo , int hi) {
    if (v < lo ) return lo;
    if (v > hi) return hi;
    return v;
}


#endif //MAIN_UPSR_BOUNDARY_H