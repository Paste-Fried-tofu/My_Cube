#include "get_move.h"
#include <fstream>
#include <cstdio>
#include <iostream>



namespace rubik_cube{

    move_vct get_move_steps(std::string filename){
        // init_move_table();
        move_vct move;
        std::ifstream in(filename, std::ios::in); // every step should be splited by ' '
        if(in.fail()){
            printf("error: Can't open %s. Please check your path and file!\n", filename.c_str());
            return move;
        }
        std::string step_str;
        
        move_step_t step;
        while(getline(in, step_str, ' ')){
            // std::cout << step_str << ' ';

            step = std::make_pair(move_table_f.find(step_str)->second.first, move_table_f.find(step_str)->second.second);

            // for(auto i = move_table_f.begin(); i!=move_table_f.end(); ++i){
            //     std::cout<< i->first << ' ';
            //     std::cout << i->second.first << ' ' << i->second.second << std::endl;
            // }
            move.push_back(step);
        }
        // for(size_t i=0; i<move.size(); i++){
        //     printf("%d %d\n", move[i].first, move[i].second);
        // }
        in.close();
        return move;
    }

    void standardization(move_vct& moves){
        for(auto& x:moves){
            x.second = (x.second%4+4)&3;
        }
    }


    void save_moves(move_vct& moves, std::string filename){
    std::ofstream out(filename, std::ios::out);
    std::string op_str;
    for(size_t i=0; i<moves.size(); ++i){
        op_str = move_table_b.find(moves[i])->second;
        for(size_t j=0; j<op_str.length(); ++j){
            out << op_str[j];
        }
        out << ' ';
    }
    out.close();
}
}
