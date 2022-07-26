#include "kociemba.h"
#include <algorithm>
#include <functional>
#include <cstring>

namespace rubik_cube{

    void kociemba::set_thread(int num){
        thread_num = num;
    }


    kociemba::kociemba(){
        thread_num = 1;

        std::vector<cube> states;

        // 初始化第二阶段的表
        std::memset(phrase2_corners, 0xff, sizeof(phrase2_corners));
        init_heuristic<true>(phrase2_corners, &kociemba::encode_phrase2_corners);// 初始化角块位置第二阶段的表

        std::memset(phrase2_edges1, 0xff, sizeof(phrase2_edges1));
        init_heuristic<true>(phrase2_edges1, &kociemba::encode_phrase2_edges1);// 初始化8个棱块位置的搜索表

        std::memset(phrase2_edges2, 0xff, sizeof(phrase2_edges2));
        init_heuristic<true, true>(phrase2_edges2, &kociemba::encode_phrase2_edges2, &states); // 初始化中间层棱块(4个)位置的搜索表

        for(cube& c : states)
        {
            // fix the orientation
            int8_t *eo = const_cast<int8_t*>(c.getEdgeBlock().second);
            eo[0] = eo[1] = eo[2] = eo[3] = 0;
        }// 以所有G1群中的魔方为初始状态.

        std::memset(phrase1_edges, 0xff, sizeof(phrase1_edges));
        init_heuristic<false>(phrase1_edges, &kociemba::encode_phrase1_edges, nullptr, states);

        states.clear();

        cube cube0;
        int8_t *cube0_cp = const_cast<int8_t*>(cube0.getCornerBlock().first);
        init_permutation<8>(0, cube0_cp, [&]() {
            states.push_back(cube0);
        }, (1 << 8) - 1 );// 所有非中间层的方向为0的魔方棱块的排列

        std::memset(phrase1_co, 0xff, sizeof(phrase1_co));
        init_heuristic<false>(phrase1_co, &kociemba::encode_phrase1_co, nullptr, states);

        states.clear();
        cube0 = cube();

        int8_t *cube0_ep = const_cast<int8_t*>(cube0.getEdgeBlock().first);

        init_permutation<12>(4, cube0_ep, [&]() {
            states.push_back(cube0);
        }, ((1 << 8) - 1) << 4 );   //获取除中间层外的所有方向为0的棱块的排列, 并以此为初始状态

        std::memset(phrase1_eo, 0xff, sizeof(phrase1_eo));
        init_heuristic<false>(phrase1_eo, &kociemba::encode_phrase1_eo, nullptr, states);
    }

    template<int N, int S>
    inline int kociemba::encode_perm(const int8_t *p, const int *k) 
        {
            int pos[N], elem[N];
            for(int i = 0; i != N; ++i)
                pos[i] = elem[i] = i;

            int v = 0, t;
            for(int i = 0; i != S; ++i)
            {
                t = pos[p[i]];
                v += k[i] * t;
                pos[elem[N - i - 1]] = t;
                elem[t] = elem[N - i - 1];
            }
            return v;
        }


    int kociemba::encode_phrase1_edges(const cube& c){
        block_info_ct eb = c.getEdgeBlock();
        int code = 0;
        int8_t perm[4];
        for(int i = 0; i < 12; ++i){
            if(eb.first[i] < 4){
                perm[eb.first[i]] = i;
                code |= eb.second[i] << eb.first[i];
            }
        }
        return code | (encode_perm<12, 4>(perm, factorial_12) << 4);
    }
    int kociemba::encode_phrase1_co(const cube& c){
        static const int base[] = { 1, 3, 9, 27, 81, 243, 729, 2187 };
        block_info_ct cb = c.getCornerBlock();
        int code = 0;
        for(int i=0; i<8; ++i){
            code += cb.second[i] * base[i];
        }
        return code;
    }
    int kociemba::encode_phrase1_eo(const cube& c){
        block_info_ct eb = c.getEdgeBlock();
        int code = 0;
        for(int i=0; i<8; i++){
            code |= eb.second[i+4] << i;
        }
        return code;
    }
    int kociemba::encode_phrase2_corners(const cube& c){
        block_info_ct cb = c.getCornerBlock();
        return encode_perm<8, 7>(cb.first, factorial_8);
    }
    int kociemba::encode_phrase2_edges1(const cube& c){
        block_info_ct eb = c.getEdgeBlock();
        int8_t perm[8];
        for(int i = 4; i != 12; ++i)
            perm[i - 4] = eb.first[i] - 4;

        return encode_perm<8, 7>(perm, factorial_8);
    }
    int kociemba::encode_phrase2_edges2(const cube& c){
        block_info_ct eb = c.getEdgeBlock();
        return encode_perm<4, 3>(eb.first, factorial_4);
    }
    template <int Phrase>
    int kociemba::estimate(const cube& c)const{
        if(Phrase == 1){
            return estimate_phrase1(c);
        }
        return estimate_phrase2(c);
    }


    int kociemba::estimate_phrase1(const cube& c)const{
        return std::max(
            phrase1_edges[encode_phrase1_edges(c)],
            std::max(
                phrase1_eo[encode_phrase1_eo(c)],
                phrase1_co[encode_phrase1_co(c)])
        );
    }
    int kociemba::estimate_phrase2(const cube& c)const{
        return std::max(
            phrase2_corners[encode_phrase2_corners(c)],
            std::max(
                phrase2_edges1[encode_phrase2_edges1(c)],
                phrase2_edges2[encode_phrase2_edges2(c)]
            )
        );
    }

    
    template<int Phrase>
    bool kociemba::search_phrase(const search_info& s) const{
        
        if(s.thread_id >= 0 && *s.result_id >= 0){
            return true;
        }

        search_info t = s;
        t.g += 1;

        for(int i=0; i<6; i++){
            if(i == s.face || disallow_faces[i] == s.face) continue;

            cube cb = s.t_cube;

            for(int j=1; j<=3; ++j){
                if(Phrase == 1){
                    cb.rotate(face_t(i), 1);
                }
                else {
                    if(i >= 2 && j != 2)continue;
                    cb.rotate(face_t(i), i < 2 ? 1 : j);
                }

                int h = estimate<Phrase>(cb);
                if(h + s.g + 1 <= s.depth){
                    (*s.move)[s.g] = move_step_t{face_t(i),j};

                    if(h == 0){
                        if(s.thread_id >= 0) *s.result_id = s.thread_id;
                        return true;
                    }
                    
                    t.t_cube = cb;
                    t.face = i;

                    if(search_phrase<Phrase>(t))
                        return true;
                }
            }
        }
        return false;
    }
    template<int Size, typename PushFunc>
    void kociemba::init_permutation(int now, int8_t* v, const PushFunc& push, int avail)
    {
        if(!avail) return push();

        for(int i = 0; i != Size; ++i)
        {
            if((avail >> i) & 1)
            {
                avail ^= 1 << i;
                v[now] = i;
                init_permutation<Size>(now + 1, v, push, avail);
                avail ^= 1 << i;
            }
        }
    }

    move_vct kociemba::solve(cube &cb) {
        move_vct solution;
        t_cube = cb;

        // phrase 1
        for(int depth = 0; ; ++depth)
        {
            move_vct moves(depth);

            search_info s;
            s.t_cube    = t_cube;
            s.g     = 0;
            s.move   = &moves;
            s.face  = 6;
            s.depth = depth;

            if(depth < 7 || thread_num == 1) 
            {
                s.thread_id = -1;
                if(search_phrase<1>(s))
                {
                    solution = *s.move;
                    break;
                }
            } else {
                using namespace std::placeholders;
                if(search_multi_thread(thread_num, s, std::bind(&kociemba::search_phrase<1>, this, _1)))
                {
                    solution = *s.move;
                    break;
                }
            }
        }

        for(move_step_t& step : solution)
            t_cube.rotate(step.first, step.second);

        // phrase 2
        for(int depth = 0; ; ++depth)
        {
            move_vct seq(depth);

            search_info s;
            s.t_cube    = t_cube;
            s.g     = 0;
            s.move    = &seq;
            s.face  = 6;
            s.depth = depth;
            s.thread_id   = -1;

            if(search_phrase<2>(s))
            {
                if(seq.front().first == solution.back().first)
                {
                    // merge rotation of same faces;
                    seq[0].second = (seq[0].second + solution.back().second) % 4;
                    solution.pop_back();
                }  

                for(move_step_t step : seq)
                    solution.push_back(step);
                break;
            }
        }

        for(move_step_t& step : solution)
            if(step.second == 3)
                step.second = -1;

        return solution;
    }

    move_vct kociemba::solve(){
        move_vct solution;

        // phrase 1
        for(int depth = 0; ; ++depth)
        {
            move_vct moves(depth);

            search_info s;
            s.t_cube    = this->t_cube;
            s.g     = 0;
            s.move   = &moves;
            s.face  = 6;
            s.depth = depth;

            if(depth < 7 || thread_num == 1) 
            {
                s.thread_id = -1;
                if(search_phrase<1>(s))
                {
                    solution = *s.move;
                    break;
                }
            } else {
                using namespace std::placeholders;
                if(search_multi_thread(thread_num, s, std::bind(&kociemba::search_phrase<1>, this, _1)))
                {
                    solution = *s.move;
                    break;
                }
            }
        }

        for(move_step_t& step : solution)
            t_cube.rotate(step.first, step.second);

        // phrase 2
        for(int depth = 0; ; ++depth)
        {
            move_vct seq(depth);

            search_info s;
            s.t_cube    = this->t_cube;
            s.g     = 0;
            s.move    = &seq;
            s.face  = 6;
            s.depth = depth;
            s.thread_id   = -1;

            if(search_phrase<2>(s))
            {
                if(seq.front().first == solution.back().first)
                {
                    // merge rotation of same faces;
                    seq[0].second = (seq[0].second + solution.back().second) % 4;
                    solution.pop_back();
                }  

                for(move_step_t step : seq)
                    solution.push_back(step);
                break;
            }
        }

        for(move_step_t& step : solution)
            if(step.second == 3)
                step.second = -1;

        return solution;
    }

}

