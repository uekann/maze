#ifndef MAKE_MAZE
#define MAKE_MAZE

#include "maze.h"

#include <random>

inline Maze make_maze(int height, int width){
    /* 穴掘り法による迷路の生成
     *
     * アルゴリズム概要
     * 迷路を、全て壁あり、全て未到達で初期化。
     * 現在地点からランダムな方向を向く。
     * その方向に壁があり、壁の先のマスに未到達なら、その壁を掘り1マス進む。
     * どの方向も上の条件を満たさない場合、この条件を満たすような方向を持つマスにランダムでワープする。
     * ワープする先がなくなったら終了*/

    // 迷路の高さと幅は2以上でなければならない
    if(height < 2 || width < 2) throw std::invalid_argument("height and width must be greater than 1");
    Maze maze(height, width);

    // 迷路の壁をすべて壁ありにする
    maze.setWalls({
        {"horizontal", std::vector<std::vector<Wall>>(height, std::vector<Wall>(width-1, Wall::EXIST))},
        {"vertical", std::vector<std::vector<Wall>>(height-1, std::vector<Wall>(width, Wall::EXIST))}
    });

    std::vector<std::vector<bool>> reached(height, std::vector<bool>(width, false)); // 探索済みの点を記録する配列
    std::tuple<int, int> now = std::make_tuple(0, 0);
    reached[0][0] = true;

    std::vector<std::tuple<std::tuple<int, int>, Direction>> can_search; // 探索可能な点とその点からの移動方向を記録する配列
    while(true){
        std::vector<Direction> moveable; // 現在位置から移動可能な方向を記録する配列
        for(auto const d: Direction()){
            std::tuple<int, int> next = translate(now, d);
            if(!maze.isInMaze(next)) continue;
            if(reached[std::get<0>(next)][std::get<1>(next)]){
                // 方向dの先のマスnextが到達済みなら、nextから~dへの探索可能フラグを削除
                can_search.erase(std::remove(can_search.begin(), can_search.end(), std::make_tuple(next, getOpposite(d))), can_search.end());
            } else {
                // 方向dの先のマスnextが未到達なら、dを探索可能な方向として記録
                moveable.push_back(d);
            }
        }
        Direction d;
        if(moveable.empty()){
            // 探索可能な方向がない場合、探索可能な点をランダムに選び、そこからランダムな方向にワープする
            if(can_search.empty()) break;
            std::mt19937 mt{std::random_device{}()};
            std::uniform_int_distribution<int> rand(0, can_search.size()-1);
            int i = rand(mt);
            now = std::get<0>(can_search[i]);
            d = std::get<1>(can_search[i]);
        } else {
            // 探索可能な方向がある場合、その中からランダムに選び、そこに進む
            std::mt19937 mt{std::random_device{}()};
            std::uniform_int_distribution<int> rand(0, moveable.size()-1);
            int i = rand(mt);
            d = moveable[i];
            moveable.erase(moveable.begin()+i);
        }

        std::tuple<int, int> next = translate(now, d);
        maze.setWallByPoint(now, d, Wall::NOT_EXIST);
        reached[std::get<0>(next)][std::get<1>(next)] = true;
        for(auto const d: moveable){
            can_search.push_back(std::make_tuple(now, d));
        }
        now = next;
    }
    return maze;
}

//int main(){
//    Maze maze = make_maze(20, 20);
//    std::cout << maze.toString() << std::endl;
//}

#endif