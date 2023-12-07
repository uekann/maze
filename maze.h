#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <tuple>
#include <queue>
#include <map>
#include <iostream>
#include <stdexcept>
#include <string>
#include "direction.h"

enum Wall{
    EXIST,
    NOT_EXIST,
    UNKNOWN
};


class Maze{
public:
    const int height; // 迷路の高さ
    const int width; // 迷路の幅
    std::vector<std::vector<int>> dist; // マスがgoalからどれだけ離れているかを保持する配列 height x width
    std::tuple<int, int> start; // スタート
    std::tuple<int, int> goal; // ゴール

private:
    std::map<std::string, std::vector<std::vector<Wall>>> walls;

public:
    Maze(int height, int width) : height(height), width(width) {
        walls = {
                {"vertical", std::vector<std::vector<Wall>>(height, std::vector<Wall>(width-1, Wall::UNKNOWN))},
                {"horizontal", std::vector<std::vector<Wall>>(height-1, std::vector<Wall>(width, Wall::UNKNOWN))}
        };
    }
    std::string toString();
    void setStart(std::tuple<int, int> p);
    void setGoal(std::tuple<int, int> p);
    void setWalls(std::map<std::string, std::vector<std::vector<Wall>>> walls);
    void setWallByPoint(std::tuple<int, int> p, Direction d, Wall w);
    void setWallBetweenPoints(std::tuple<int, int> p1, std::tuple<int, int> p2, Wall w);
    void calcDist(); // distを更新する
    std::vector<std::tuple<int, int>> getMoveable(std::tuple<int, int> p); // pの上下左右の移動可能な点をかえす
    Direction getDireToMove(std::tuple<int, int> p); // pからゴールに向かって移動するときに移動する方向をかえす
    std::tuple<int, int> getPointToMove(std::tuple<int, int> p); // pからゴールに向かって移動するときに移動する点をかえす
    bool isInMaze(std::tuple<int, int> p);
    Wall getWallByPoint(std::tuple<int, int> p, Direction d);
    Wall getWallBetweenPoints(std::tuple<int, int> p1, std::tuple<int, int> p2);

private:
    std::tuple<std::string, int, int> getWallIdByPoint(std::tuple<int, int> p, Direction d); // pとdから壁のポインタをかえす
    std::tuple<std::string, int, int> getWallIdBetweenPoints(std::tuple<int, int> p1, std::tuple<int, int> p2); // p1とp2から壁のポインタをかえす
};

#endif