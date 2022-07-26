#ifndef __CUBE_H__
#define __CUBE_H__

#include <cstdint>
#include <utility>
#include <string>

namespace rubik_cube
{
    enum face_t{
        top = 0,
        down = 1,
        front = 2,
        back = 3,
        left = 4,
        right = 5
    };

    struct block_t
    {
        int8_t top, bottom, front, back, left, right;
    };

    typedef std::pair<const int8_t*, const int8_t* > block_info_ct;
    typedef std::pair<int8_t*, int8_t* > block_info_t;


    class cube{
        int8_t cp[8], co[8];
        int8_t ep[12], eo[12];

        static void swap(int8_t *A, const int *C) {
            int tmp = A[C[3]];
            A[C[3]] = A[C[2]];
            A[C[2]] = A[C[1]];
            A[C[1]] = A[C[0]];
            A[C[0]] = tmp;
        };

        static void swap2(int8_t &a, int8_t &b) {
            int8_t t = a;
            a = b;
            b = t;
        };

    public:
        cube();
        cube(const cube& t);
        cube(block_info_t corner, block_info_t edge);
        cube& operator = (const cube&);
        void rotate(face_t, int count = 1);
        block_info_ct getCornerBlock() const;
        block_info_ct getEdgeBlock()const ;
        void set_cube(std::string filename);
        void save(std::string filename);
        void show();
    };
} // namespace rubik_cube





#endif