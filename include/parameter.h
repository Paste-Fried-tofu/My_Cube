#ifndef __PARA_H__
#define __PARA_H__

#include "Cube.h"

namespace rubik_cube{

    // 阶乘数组
    constexpr static int factorial_4[] = { 1, 4, 12, 24 };
    constexpr static int factorial_8[] = { 1, 8, 56, 336, 1680, 6720, 20160, 40320 };
    constexpr static int factorial_12[] = { 1, 12, 132, 1320, 11880, 95040, 665280, 3991680, 19958400, 79833600, 239500800, 479001600 };
    
	constexpr static int disallow_faces[6] = { -1, 0, -1, 2, -1, 4 };

    // 两个阶段的魔方各状态总数
    static const int phrase2_corners_size = 40320; // 8!
    static const int phrase2_edges1_size = 40320;  // 8!
    static const int phrase2_edges2_size = 24;     // 4!
    static const int phrase1_edges_size = 12 * 11 * 10 * 9 * 16;
    static const int phrase1_co_size = 6561; //3^8
    static const int phrase1_eo_size = 1 << 8;

}

#endif