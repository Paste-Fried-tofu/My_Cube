#ifndef __GET_MOVE_H__
#define __GET_MOVE_H__

#include "Cube.h"
#include <string>
#include <map>
#include <vector>
#include <fstream>
#include <iostream>

namespace rubik_cube{

    typedef std::pair<face_t, int> move_step_t;
    typedef std::map<std::string, move_step_t> move_map_fore;
    typedef std::map<move_step_t, std::string> move_map_back;
    typedef std::vector<move_step_t> move_vct;

    static move_map_fore move_table_f = { 
        {"R", move_step_t(right, (int)1)}, 
        {"R2", move_step_t(right, (int)2)},
        {"R'", move_step_t(right, (int)3)},
        {"L", move_step_t(left, (int)1)},
        {"L2", move_step_t(left, (int)2)},
        {"L'", move_step_t(left, (int)3)},
        {"U", move_step_t(top, (int)1)},
        {"U2", move_step_t(top, (int)2)},
        {"U'", move_step_t(top, (int)3)},
        {"D", move_step_t(down, (int)1)},
        {"D2", move_step_t(down, (int)2)},
        {"D'", move_step_t(down, (int)3)},
        {"F'", move_step_t(front, (int)1)},
        {"F2'", move_step_t(front, (int)2)},
        {"F''", move_step_t(front, (int)3)},
        {"B", move_step_t(back, (int)1)},
        {"B2", move_step_t(back, (int)2)},
        {"B'", move_step_t(back, (int)3)}
    };
    static move_map_back move_table_b = {
        {move_step_t(right, (int)1),  "R"}, 
        {move_step_t(right, (int)2), "R2" },
        {move_step_t(right, (int)3), "R'"},
        {move_step_t(left, (int)1), "L" },
        {move_step_t(left, (int)2), "L2"},
        {move_step_t(left, (int)3), "L'"},
        {move_step_t(top, (int)1), "U"},
        {move_step_t(top, (int)2), "U2"},
        {move_step_t(top, (int)3), "U'"},
        {move_step_t(down, (int)1), "D"},
        {move_step_t(down, (int)2), "D2"},
        {move_step_t(down, (int)3), "D'"},
        {move_step_t(front, (int)1), "F"},
        {move_step_t(front, (int)2), "F2"},
        {move_step_t(front, (int)3), "F'"},
        {move_step_t(back, (int)1), "B"},
        {move_step_t(back, (int)2), "B2"},
        {move_step_t(back, (int)3), "B'"}
    };

    
    move_vct get_move_steps(std::string filename);
    void standardization(move_vct& moves);
    void save_moves(move_vct& moves, std::string filename);

}

#endif