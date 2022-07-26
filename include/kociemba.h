# ifndef __KOCIEMBA_H__
# define __KOCIEMBA_H__

#include "Cube.h"
#include "get_move.h"
#include "parameter.h"
#include "search.hpp"
#include "queue"

namespace rubik_cube{

    class kociemba{
    private:
        cube t_cube;
        int thread_num;
        // 编码模板
        template<int N, int S>
        static inline int encode_perm(const int8_t *p, const int *k) ;
        template<bool IsGroup1, bool RecordState = false>
        static inline void init_heuristic(
            int8_t *buf,
            int(*encoder)(const cube&),
            std::vector<cube>* recoder = nullptr,
            const std::vector<cube>& init_state = {cube()}){
                std::queue<std::pair<cube, uint8_t>> q;

                for(const cube& c : init_state){
                    buf[(*encoder)(c)] = 0;
                    q.push({c, 0 | (6 << 4)}); // 0000 0110 -> 0110 0000 | 0000 0000 -> 0110 0000
                }

                while (!q.empty()){
                    auto u = q.front();
                    int face = u.second >> 4; // 0110 0000 -> 0000 0110 6
                    int step = u.second & 0xf;//0110 0000 & 0000 1111 -> 0000 0000 0

                    for(int i=0; i<6; ++i){
                        if(i == face || disallow_faces[i] == face)
                            continue;
                        cube c = u.first;
                        for(int j = 0; j<3; j++){
                            if(IsGroup1 && i>=2){ // G1 = <U, D, F2, B2, L2, R2>
                                if(j) break;
                                c.rotate(face_t(i), 2);
                            }
                            else {
                                c.rotate(face_t(i), 1); // G0 = <U, D, F, B, L, R>
                            }

                            int code = (*encoder)(c);

                            if(buf[code] == -1){
                                buf[code] = step + 1;
                                q.push({c, (step+1) | (i<<4)});
                                if(RecordState) recoder->push_back(c);
                            }
                        }
                    }
                    q.pop();
                }
            }

        template<int Size, typename PushFunc>
	    static void init_permutation(int, int8_t*, const PushFunc&, int);

        // 魔方各个状态到达目标阶段的所用步数
        int8_t phrase2_corners[phrase2_corners_size];
        int8_t phrase2_edges1[phrase2_edges1_size];
        int8_t phrase2_edges2[phrase2_edges2_size];
        int8_t phrase1_edges[phrase1_edges_size];
        int8_t phrase1_co[phrase1_co_size];
        int8_t phrase1_eo[phrase1_eo_size];

        // 对魔方状态编码函数
        static int encode_phrase1_edges(const cube&);
	    static int encode_phrase1_co(const cube&);
	    static int encode_phrase1_eo(const cube&);
	    static int encode_phrase2_corners(const cube&);
	    static int encode_phrase2_edges1(const cube&);
	    static int encode_phrase2_edges2(const cube&);

        
        template<int Phrase>
        bool search_phrase(const search_info&) const;

        // 两阶段的评估(取最大步数)
        template<int Phrase>
        int estimate(const cube&)const;
        int estimate_phrase1(const cube&)const;
        int estimate_phrase2(const cube&)const;

    public:
        kociemba();
        kociemba(cube&);
        ~kociemba() = default;
        move_vct solve();
        move_vct solve(cube&);
        void set_thread(int num);
    };

}





#endif