#include "Cube.h"
#include "get_move.h"
#include "solver.h"
#include "parameter.h"
#include <iostream>
#include <cstdio>
#include <fstream>
#include <random>
#include <ctime>
#include <string>

using namespace rubik_cube;

std::random_device rd;
std::mt19937 mt(rd());

move_step_t get_random_step(){
    srand((unsigned int)time(nullptr));
    int face = mt()%6;
    int count = mt()%3+1;
    return {(face_t)face, count};
}

void show(block_info_ct& block_info, int n){
    for(int i=0; i<n; ++i){
        printf("position:%d block:%u orientation:%u\n", i, block_info.first[i], block_info.second[i]);
    }
    putchar('\n');
}



int main(){
    cube a ;


    /* 通过文件获取魔方状态 */
    // a.set_cube("example_files/example_cube.txt");


    /* 打印魔方状态(可注释掉) */
    block_info_ct corner = a.getCornerBlock();
    block_info_ct edge = a.getEdgeBlock();
    show(corner, 8);
    show(edge, 12);

    move_vct moves;

    /* 通过文件获取魔方打乱方式 */
    //moves =  rubik_cube::get_move_steps("example_files/example_disarrange_steps.txt");


    /* 随机打乱魔方 */
    int times = 200;
    printf("Input the random rotation times to generate a cube: ");
    std::cin >> times;

    for(int i=0; i<times; i++){
        moves.push_back(get_random_step());
    }

    /* 保存魔方打乱方式 */
    // save_moves(moves, "example_files/example_disarrange_steps.txt");
    
    for(size_t i=0; i<moves.size(); i++){
        std::cout << move_table_b.find(moves[i])->second << " " ;
        a.rotate(moves[i].first, moves[i].second);
    }

    
    putchar('\n');
    // putchar('\n');
    // show(corner, 8);
    // show(edge, 12);


    // a.save("example_files/example_cube.txt");


    solver s;


    int thread_nums = 1;

    printf("Input thread_nums(<= 30): ");
    if(thread_nums > 30){
        printf("too many threads!\n");
        return 1;
    }
    std::cin>>thread_nums;

    s.set_thread(thread_nums);

    move_vct ans =  s.solve(a);
    standardization(ans);
    putchar('\n');

    printf("Steps: %ld\n", ans.size());
    

    for(auto x:ans){
        std::cout << move_table_b.find(x)->second << ' ';
        a.rotate(x.first, x.second);
    }

    putchar('\n');
    // putchar('\n');
    // show(corner, 8);
    // show(edge, 12);

    return 0;
}
