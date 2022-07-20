#ifndef __SEARCH_HPP__
#define __SEARCH_HPP__

#include "Cube.h"
#include "get_move.h"
#include <thread>
#include <future>
#include <atomic>
#include <mutex>
#include <condition_variable>

namespace rubik_cube{
    struct search_info{
        cube t_cube;
        int g;
        int face;
        int depth;

        move_vct* move;

        int thread_id;
        std::atomic<int>* result_id;
    };

    template<typename SearchFunc>
    bool search_multi_thread(
        int thread_num,
        const search_info& s,
        SearchFunc search){
            search_info infos[18];
            move_vct moves[18];

            // 设置互斥量和条件变量
            std::mutex cv_m;
            std::condition_variable cv;
            int working_thread = 0;

            std::future<bool> results[18];
            std::atomic<int> result_id;
            result_id = -1;

            for(int i=0; i<6; ++i){
                cube cb = s.t_cube;
                for (int j = 1; j <= 3; ++j){
                    int id = i*3 + j - 1;// id: 0~18
                    cb.rotate(face_t(i), 1);

                    moves[id].resize(s.depth);
                    moves[id][0] = move_step_t(face_t(i), j);

                    infos[id] = s;
                    infos[id].thread_id = id;
                    infos[id].t_cube = cb;
                    infos[id].move = moves + id;
                    infos[id].g = 1;
                    infos[id].face = i;
                    infos[id].result_id = &result_id;

                    std::packaged_task<bool()> task{
                        [&, id]()->bool{
                            // PV 操作
                            std::unique_lock<std::mutex> lk(cv_m);// P
                            cv.wait(lk, [&]{
                                return working_thread < thread_num;
                            });

                            ++working_thread;
                            lk.unlock();// V

                            bool ret = search(infos[id]);

                            {
                                std::lock_guard<std::mutex> lk(cv_m);
                                --working_thread;
                            }

                            cv.notify_one();
                            return ret;
                        }
                    };

                    results[id] = task.get_future();

                    std::thread{std::move(task)}.detach();
                }
            }
            cv.notify_all();

            for(auto& fu:results){
                fu.wait();
            }

            if(result_id >= 0){
                *s.move = moves[result_id];
                return true;
            }
            else return false;
        }
    

}



#endif