#include "config.h"
#include "noise.h"
#include "world.h"

void create_world(int p, int q, world_func func, void *arg) {
    //Different heights to generate different trees at
    int low_level = 12;
    int mid_level = 37;
    int high_level = 56;

    int pad = 1;
    for (int dx = -pad; dx < CHUNK_SIZE + pad; dx++) {
        for (int dz = -pad; dz < CHUNK_SIZE + pad; dz++) {
            int flag = 1;
            if (dx < 0 || dz < 0 || dx >= CHUNK_SIZE || dz >= CHUNK_SIZE) {
                flag = -1;
            }
            int x = p * CHUNK_SIZE + dx;
            int z = q * CHUNK_SIZE + dz;
            float f = simplex2(x * 0.01, z * 0.01, 4, 0.5, 2);
            float g = simplex2(-x * 0.01, -z * 0.01, 2, 0.9, 2);
            int mh = g * 32 + 16;

            //use a random number generator to determine if a moutain will
            //be made or not set to 90 so moutains are rare
            int num = (rand() % (100 - 1 + 1)) + 1;
            int h = f * mh *2;

            int w = 1;
            int t = 12;
            //player height is .75 so brick player is standing on is height -.75
            //sand starts at brick 12 which is the value of t
            //int oldh = h;

            /**
                Grass is 1
                Sand  is 2
                Stone is 6
            */
            //if the height is less than ground level (12) then use sand brick
            if (h <= t+1/*t is a hardcoded ground level*/) {
                w = 2;
            }


            //if height is 2 less than ground level then use stone brick
            //if the level is over 28 then use the stone block. This means
            //a moutain is being formed
            if (h <= t-2 || h>= 32) {
                //h = t;
                w = 6;

                if (num >=95)
                {
                	w = 1;
                }

                if (h>=55)
                {
                	//white wool looks like snow brick
                	//use that brick if the height is moutain high
                	w=61;
                }
            }
            //terrain
            for (int y = 0; y < h; y++) {
                // this is what generates the map

                func(x, y, z, w * flag, arg);
            }
            // /*
            if (w == 1) {
                if (SHOW_PLANTS) {
                    // grass
                    if (simplex2(-x * 0.1, z * 0.1, 4, 0.8, 2) > 0.6) {
                        func(x, h, z, 17 * flag, arg);
                    }
                    // flowers
                    if (simplex2(x * 0.05, -z * 0.05, 4, 0.8, 2) > 0.7) {
                        int w = 18 + simplex2(x * 0.1, z * 0.1, 4, 0.8, 2) * 7;
                        func(x, h, z, w * flag, arg);
                    }
                }

                //Generate standard trees at mid_level
                if (h >= low_level + 7 && h <= mid_level) {
                  int ok = SHOW_TREES;
                  //Lower tree_frequency for more trees
                  float tree_frequency = 0.78;
                  if (dx - 4 < 0 || dz - 4 < 0 ||
                      dx + 4 >= CHUNK_SIZE || dz + 4 >= CHUNK_SIZE)
                  {
                      ok = 0;
                  }
                  if (ok && simplex2(x, z, 6, 0.5, 2) > tree_frequency) {
                      for (int y = h + 3; y < h + 8; y++) {
                          for (int ox = -3; ox <= 3; ox++) {
                              for (int oz = -3; oz <= 3; oz++) {
                                  int d = (ox * ox) + (oz * oz) +
                                      (y - (h + 4)) * (y - (h + 4));
                                  if (d < 11) {
                                      func(x + ox, y, z + oz, 15, arg);
                                  }
                              }
                          }
                      }
                      for (int y = h; y < h + 7; y++) {
                          func(x, y, z, 5, arg);
                      }
                  }
                }

                //Generate palm trees at low_level
                if (h <= low_level + 2 && h >= low_level) {
                  //Add a random number for variety in palm tree appearances
                  int r_num = 1;
                  int ok = SHOW_TREES;
                  //Lower tree_frequency for more trees
                  float tree_frequency = 0.69 + 0.01;
                  if (dx - 4 < 0 || dz - 4 < 0 ||
                      dx + 4 >= CHUNK_SIZE || dz + 4 >= CHUNK_SIZE)
                  {
                      ok = 0;
                  }
                  if (ok && simplex2(x, z, 6, 0.5, 2) > tree_frequency) {
                      //Generate the trunk
                      func(x, h, z, 5, arg);
                      if ((rand() % 10) + 1 <= 5) {
                        r_num = -1;
                      }
                      func(x + r_num, h, z, 5, arg);

                      r_num = 1;
                      if ((rand() % 10) + 1 <= 5) {
                        r_num = -1;
                      }
                      func(x, h, z + r_num, 5, arg);

                      for(int y = h + 1; y < h + 4; y++) {
                        func(x, y, z, 5, arg);
                      }

                      r_num = 1;
                      if ((rand() % 10) + 1 <= 5) {
                        r_num = -1;
                      }

                      r_num = 1;
                      if ((rand() % 10) + 1 <= 5) {
                        r_num = -1;
                      }
                      func(x, h + 4, z += r_num, 5, arg);
                      x += 1;
                      for(int i = 5; i < 7; i++) {
                        func(x, h + i, z, 5, arg);
                      }

                      r_num = 1;
                      if ((rand() % 10) + 1 <= 5) {
                        r_num = -1;
                      }
                      for(int i = 7; i < 10; i++) {
                        func(x, h + i, z + r_num, 5, arg);
                      }

                      //Generate the leaves
                      //ox, oy, oz represent coordinates for the top of the trunk
                      int ox = x;
                      int oy = h + 8;
                      int oz = z + r_num;
                      func(ox, oy + 1, oz, 15, arg);
                      func(ox, oy, oz, 15, arg);
                      func(ox + 1, oy, oz, 15, arg);
                      func(ox - 1, oy, oz, 15, arg);
                      func(ox, oy, oz + 1, 15, arg);
                      func(ox, oy, oz - 1, 15, arg);
                      func(ox + 1, oy, oz + 1, 15, arg);
                      func(ox + 1, oy, oz - 1, 15, arg);
                      func(ox - 1, oy, oz + 1, 15, arg);
                      func(ox - 1, oy, oz - 1, 15, arg);
                      func(ox + 2, oy--, oz + 1, 15, arg);
                      func(ox - 2, oy, oz - 1, 15, arg);
                      func(ox + 2, oy, oz - 1, 15, arg);
                      func(ox, oy, oz - 2, 15, arg);
                      func(ox + 2, oy, oz + 1, 15, arg);
                      func(ox - 2, oy, oz - 2, 15, arg);
                      func(ox, oy, oz + 2, 15, arg);
                      func(ox, oy - 1, oz + 3, 15, arg);
                      func(ox - 2, oy, oz + 1, 15, arg);
                      func(ox - 2, oy - 1, oz + 1, 15, arg);
                      func(ox - 3, oy - 1, oz - 1, 15, arg);
                      func(ox + 3, oy - 1, oz - 1, 15, arg);
                      func(ox, oy - 1, oz - 3, 15, arg);
                      func(ox + 2, oy - 1, oz + 2, 15, arg);
                  }

                }

                //Generate conifers at high_level
                if (h >= mid_level && h <= high_level + 7) {
                  int tree_type = 0;
                  if ((rand() % 10) + 1 <= 5) {
                    tree_type = 1;
                  }
                  int ok = SHOW_TREES;
                  //Lower tree_frequency for more trees
                  float tree_frequency = 0.62;
                  if (dx - 4 < 0 || dz - 4 < 0 ||
                      dx + 4 >= CHUNK_SIZE || dz + 4 >= CHUNK_SIZE)
                  {
                      ok = 0;
                  }
                  if (ok && simplex2(x, z, 6, 0.5, 2) > tree_frequency) {
                    if(tree_type) {
                      for (int y = h; y < h + 9; y++) {
                          func(x, y, z, 5, arg);
                      }

                      //Generate leaves
                      int block = 15;
                      int y = h + 1;
                      int i = 0;
                      while(i < 4) {
                        for(int i = -1; i < 1; i++) {
                          for(int j = -1; j < 1; j++) {
                            func(x + i, y, z + j, block, arg);
                            func(x - i, y, z + j, block, arg);
                            func(x - i, y, z - j, block, arg);
                            func(x - i, y, z + j, block, arg);
                          }
                        }
                        func(x - 1, y, z + 1, block, arg);
                        y++;
                        if (y >= 58 && i == 3) {
                          block = 61;
                        }
                        func(x - 1, y, z, block, arg);
                        func(x + 1, y, z, block, arg);
                        func(x, y, z - 1, block, arg);
                        func(x, y, z + 1, block, arg);
                        y++;
                        i++;
                      }
                      func(x, y++, z, block, arg);
                      block = 15;
                    } else {
                      //Generate tree with wider base
                      for (int y = h; y < h + 8; y++) {
                          func(x, y, z, 5, arg);
                      }
                      int block = 15;
                      //Base leaf layer
                      for(int i = -2; i < 3; i++) {
                        for(int j = -2; j < 3; j++) {
                          if(abs(i) + abs(j) != 4){
                            func(x + i, h + 1, z + j, block, arg);
                          }
                        }
                      }

                      //Second leaf layer
                      for(int i = -1; i < 2; i++) {
                        for(int j = -1; j < 2; j++) {
                          func(x + i, h + 3, z + j, block, arg);
                        }
                      }

                      //Third leaf layer
                      for(int i = -1; i < 2; i++) {
                        for(int j = -1; j < 2; j++) {
                          if(abs(i) + abs(j) != 2){
                            func(x + i, h + 5, z + j, block, arg);
                          }
                        }
                      }

                      //Top leaf layer
                      if (h + 7 >= 58) {
                        block = 61;
                      }
                      for(int i = -1; i < 2; i++) {
                        for(int j = -1; j < 2; j++) {
                          if(abs(i) + abs(j) != 2){
                            func(x + i, h + 7, z + j, block, arg);
                          }
                        }
                      }
                      func(x, h + 8, z, block, arg);

                    }
                  }
                }


          }
             //clouds
            if (SHOW_CLOUDS) {
                for (int y = 64; y < 72; y++) {
                    if (simplex3(
                       x * 0.01, y * 0.1, z * 0.01, 8, 0.5, 2) > 0.75)
                    {
                      func(x, y, z, 16 * flag, arg);
                   }
               }
            }//*/

        }
    }
}
