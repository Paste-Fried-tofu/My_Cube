#include "Cube.h"
#include <cstring>
#include <fstream>

namespace rubik_cube{
	cube::cube(){
		for(int i=0; i<8; ++i){
			cp[i] = i;
			co[i] = 0;
		}
		for(int i=0; i<12; ++i){
			ep[i] = i;
			eo[i] = 0;
		}
	}

	cube::cube(const cube&  t){
		for(int i=0; i<8; ++i){
			cp[i] = t.cp[i];
			co[i] = t.co[i];
		}
		for(int i=0; i<12; ++i){
			ep[i] = t.ep[i];
			eo[i] = t.eo[i];
		}
	}

	cube::cube(block_info_t corner, block_info_t edge){
		for(int i=0; i<8; ++i){
			cp[i] = corner.first[i];
			co[i] = corner.second[i];
		}
		for(int i=0; i<12; ++i){
			ep[i] = edge.first[i];
			eo[i] = edge.second[i];
		}
	}


	cube& cube::operator = (const cube& t){
		for(int i=0; i<8; ++i){
			cp[i] = t.cp[i];
			co[i] = t.co[i];
		}
		for(int i=0; i<12; ++i){
			ep[i] = t.ep[i];
			eo[i] = t.eo[i];
		}
		return *this;
	}


	block_info_ct cube::getCornerBlock() const{
		return {cp, co};
	}

	block_info_ct cube::getEdgeBlock() const{
		return {ep, eo};
	}

	void cube::rotate(face_t face, int count){
		static const int corner_rotate_map[2][6][4] = 
		{
			{
				// clockwise
				{ 4, 5, 6, 7 }, // top
				{ 3, 2, 1, 0 }, // bottom
				{ 7, 6, 2, 3 }, // front
				{ 5, 4, 0, 1 }, // back
				{ 4, 7, 3, 0 }, // left
				{ 6, 5, 1, 2 }  // right
			}, {
				// counterclockwise
				{ 7, 6, 5, 4 }, // top
				{ 0, 1, 2, 3 }, // bottom
				{ 7, 3, 2, 6 }, // front
				{ 5, 1, 0, 4 }, // back
				{ 4, 0, 3, 7 }, // left
				{ 6, 2, 1, 5 }  // right
			}
		};

		static const int edge_rotate_map[2][6][4] = 
		{
			{
				// clockwise
				{ 4, 5, 6, 7 },   // top
				{ 11, 10, 9, 8 }, // bottom
				{ 6, 2, 10, 3 },  // front
				{ 4, 0, 8, 1 },   // back
				{ 7, 3, 11, 0 },  // left
				{ 5, 1, 9, 2 }    // right
			}, {
				// counterclockwise
				{ 7, 6, 5, 4 },   // top
				{ 8, 9, 10, 11 }, // bottom
				{ 3, 10, 2, 6 },  // front
				{ 1, 8, 0, 4 },   // back
				{ 0, 11, 3, 7 },  // left
				{ 2, 9, 1, 5 }    // right
			}
		};

		count = (count % 4 + 4) & 3;

		if(count == 2)
		{
			const int *C = corner_rotate_map[0][int(face)];

			swap2(cp[C[0]], cp[C[2]]);
			swap2(cp[C[1]], cp[C[3]]);
			swap2(co[C[0]], co[C[2]]);
			swap2(co[C[1]], co[C[3]]);

			const int *E = edge_rotate_map[0][int(face)];
			swap2(ep[E[0]], ep[E[2]]);
			swap2(ep[E[1]], ep[E[3]]);
			swap2(eo[E[0]], eo[E[2]]);
			swap2(eo[E[1]], eo[E[3]]);
		} else {
			// rotate the corners
			const int *C = corner_rotate_map[count >> 1][int(face)];
			swap(cp, C); swap(co, C);

			// rotation of top and bottom face does not change the orientation
			if(int(face) >= 2) 
			{
				if(++co[C[0]] == 3) co[C[0]] = 0;
				if(++co[C[2]] == 3) co[C[2]] = 0;
				if(--co[C[1]] == -1) co[C[1]] = 2;
				if(--co[C[3]] == -1) co[C[3]] = 2;
			}

			// rotate the edges
			const int *E = edge_rotate_map[count >> 1][int(face)];
			if(int(face) >= 4)
			{
				eo[E[0]] ^= 1;
				eo[E[1]] ^= 1;
				eo[E[2]] ^= 1;
				eo[E[3]] ^= 1;
			}

			swap(ep, E); swap(eo, E);
		}
	}
    void cube::set_cube(std::string filename){
		std::ifstream in(filename, std::ios::in); 
        if(in.fail()){
            printf("error: Can't open %s. Please check your path and file!\n", filename.c_str());
            in.close();
            exit(1);
        }
		std::string str;
        for(int i=0; i<4; ++i){
            getline(in, str);
            if(i == 0){
                int k=0;
                for(size_t i=0; i<str.length(); ++i){
                    if(str[i] == ' ')continue;
                    this->cp[k++] = str[i]-48;
                }
            }
            else if(i == 1){
                int k=0;
                for(size_t i=0; i<str.length(); ++i){
                    if(str[i] == ' ')continue;
                    this->co[k++] = str[i]-48;
                }
            }
            else if(i == 2){
                int k=0;
                for(size_t i=0; i<str.length(); ++i){
                    if(str[i] == ' ')continue;
                    this->ep[k++] = str[i]-48;
                }
            }
            else {
                int k=0;
                for(size_t i=0; i<str.length(); ++i){
                    if(str[i] == ' ')continue;
                    this->eo[k++] = str[i]-48;
                }
            }
        }
        in.close();
	}
	void cube::save(std::string filename){
		std::ofstream out(filename, std::ios::out);
		for(int i=0; i<4; i++){
			switch (i)
			{
			case 0:
				for(int i=0; i<8; ++i){
					out << (int)cp[i] << ' ';
				}
				out << std::endl;
				break;
			case 1:
				for(int i=0; i<8; ++i){
					out << (int)co[i] << ' ';
				}
				out << std::endl;
				break;
			case 2:
				for(int i=0; i<12; ++i){
					out << (int)ep[i] << ' ';
				}
				out << std::endl;
				break;
			case 3:
				for(int i=0; i<12; ++i){
					out << (int)eo[i] << ' ';
				}
				out << std::endl;
				break;
			default:
				break;
			}
		}
		out.close();
	}

	void cube::show(){
		for(int i=0; i<12; ++i){
			if(i < 8){
				printf("position:%d block:%u orientation:%u\n", i, cp[i], co[i]);
			}
			printf("position:%d block:%u orientation:%u\n", i, ep[i], eo[i]);
		}
		putchar('\n');
	}

}