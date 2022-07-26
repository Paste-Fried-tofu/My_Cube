#include "Cube.h"
#include "get_move.h"
#include "kociemba.h"
#include "parameter.h"
#include <iostream>
#include <cstdio>
#include <fstream>
#include <ctime>
#include <string>

using namespace rubik_cube;

int main(){
    cube a ;


    /* 通过文件获取魔方状态 */
    // a.set_cube("example_files/example_cube.txt");

    /* 打印魔方状态(可注释掉) */
    a.show();

    move_vct moves;

    // /* 通过文件获取魔方打乱方式 */
    // //moves =  rubik_cube::get_move_steps("example_files/example_disrupted_steps.txt");


    /* 随机打乱魔方 */
    int times = 200;
    printf("Input the random rotation times to generate a cube: ");
    std::cin >> times;

    moves = get_random_moves(times);

    // /* 保存魔方打乱方式 */
    // // save_moves(moves, "example_files/example_disrupted_steps.txt");
    
    for(size_t i=0; i<moves.size(); i++){
        std::cout << move_table_b.find(moves[i])->second << " " ;
        a.rotate(moves[i].first, moves[i].second);
    }
    putchar('\n');

    a.save("example_files/example_cube.txt");
    kociemba sovler;
    int thread_nums = 1;

    printf("Input thread_nums(<= 30): ");
    std::cin>>thread_nums;
    if(thread_nums > 30){
        printf("too many threads!\n");
        return 1;
    }
    

    sovler.set_thread(thread_nums);

    move_vct ans =  sovler.solve(a);

    standardization(ans);

    putchar('\n');

    printf("Steps: %ld\n", ans.size());
    for(auto x:ans){
        std::cout << move_table_b.find(x)->second << ' ';
        a.rotate(x.first, x.second);
    }

    putchar('\n');

    return 0;
}