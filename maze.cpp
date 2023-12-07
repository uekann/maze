#include "maze.h"

std::string Maze::toString(){
    std::string s;
    for(int j = 0; j < this->width; j++){
        s += " __";
    }
    s += " \n";
    for(int i = 0; i < this->height; i++){
        s += "|";
        for(int j = 0; j < this->width; j++){
            if(i == this->height-1 || getWallByPoint(std::make_tuple(i, j), Direction::DOWN) == Wall::EXIST) {
                s += "__";
            } else if(getWallByPoint(std::make_tuple(i, j), Direction::DOWN) == Wall::UNKNOWN){
                s += "..";
            } else {
                s += "  ";
            }
            if(j == this->width-1 || getWallByPoint(std::make_tuple(i, j), Direction::RIGHT) == Wall::EXIST){
                s += "|";
            } else if(getWallByPoint(std::make_tuple(i, j), Direction::RIGHT) == Wall::UNKNOWN){
                s += ":";
            } else {
                s += " ";
            }
        }
        s += "\n";
    }
    return s;
}

bool Maze::isInMaze(std::tuple<int, int> p){
    // pが迷路の点どうかを判別する
    if (!(std::get<0>(p) >= 0 && std::get<0>(p) < height)) return false;
    if (!(std::get<1>(p) >= 0 && std::get<1>(p) < width)) return false;
    return true;
}

Wall Maze::getWallByPoint(std::tuple<int, int> p, Direction d){
    // pとdから壁をかえす

    if(!isInMaze(p)) throw std::invalid_argument("p is not in maze");
    if(!isInMaze(translate(p, d))) throw std::invalid_argument("p is on edge of maze");

    std::tuple<std::string, int, int> wall_id = getWallIdByPoint(p, d);
    return walls[std::get<0>(wall_id)][std::get<1>(wall_id)][std::get<2>(wall_id)];
}

Wall Maze::getWallBetweenPoints(std::tuple<int, int> p1, std::tuple<int, int> p2) {
    // p1とp2の間の壁をかえす

    if(!isInMaze(p1)) throw std::invalid_argument("p1 is not in maze");
    if(!isInMaze(p2)) throw std::invalid_argument("p2 is not in maze");

    if(p1 == p2) throw std::invalid_argument("p1 and p2 are same");

    std::tuple<std::string, int, int> wall_id = getWallIdBetweenPoints(p1, p2);
    return walls[std::get<0>(wall_id)][std::get<1>(wall_id)][std::get<2>(wall_id)];
}

void Maze::setStart(std::tuple<int, int> p){
    if (!isInMaze(p)) throw std::invalid_argument("p is not in maze");
    start = p;
}

void Maze::setGoal(std::tuple<int, int> p){
    if(!isInMaze(p)) throw std::invalid_argument("p is not in maze");
    goal = p;
}

void Maze::setWalls(std::map<std::string, std::vector<std::vector<Wall>>> walls){
    this->walls = walls;
}

void Maze::setWallByPoint(std::tuple<int, int> p, Direction d, Wall w){
    // pとdから壁を作る

    if(!isInMaze(p)) throw std::invalid_argument("p is not in maze");
    if(!isInMaze(translate(p, d))) throw std::invalid_argument("p is on edge of maze");

    std::tuple<std::string, int, int> wall_id = getWallIdByPoint(p, d);
    walls[std::get<0>(wall_id)][std::get<1>(wall_id)][std::get<2>(wall_id)] = w;
}

void Maze::setWallBetweenPoints(std::tuple<int, int> p1, std::tuple<int, int> p2, Wall w){
    // p1とp2の間に壁を作る

    if(!isInMaze(p1)) throw std::invalid_argument("p1 is not in maze");
    if(!isInMaze(p2)) throw std::invalid_argument("p2 is not in maze");

    if(p1 == p2) throw std::invalid_argument("p1 and p2 are same");

    std::tuple<std::string, int, int> wall_id = getWallIdBetweenPoints(p1, p2);
    walls[std::get<0>(wall_id)][std::get<1>(wall_id)][std::get<2>(wall_id)] = w;
}

std::vector<std::tuple<int, int>> Maze::getMoveable(std::tuple<int, int> p){
    // pの上下左右の移動可能な点をかえす

    if(!isInMaze(p)) throw std::invalid_argument("p is not in maze");

    std::vector<std::tuple<int, int>> movable = {};
    for(const auto d : Direction()){
        std::tuple<int, int> p_ = translate(p, d);
        if (!isInMaze(p_)) continue;
        if (getWallBetweenPoints(p, p_) != Wall::EXIST) movable.push_back(p_);
    }
    return movable;
}

void Maze::calcDist(){
    // distを更新する
    // ここでは幅優先探索を用いる

    // distを初期化
    dist = std::vector<std::vector<int>>(height, std::vector<int>(width, -1));

    // 幅優先探索のためのキュー
    std::queue<std::tuple<int, int>> q;
    q.push(goal);
    dist[std::get<0>(goal)][std::get<1>(goal)] = 0;

    while(!q.empty()){
        std::tuple<int, int> p = q.front();
        q.pop();
        for(const auto& p_ : getMoveable(p)){
            if(dist[std::get<0>(p_)][std::get<1>(p_)] == -1){
                dist[std::get<0>(p_)][std::get<1>(p_)] = dist[std::get<0>(p)][std::get<1>(p)] + 1;
                q.push(p_);
            }
        }
    }
}

std::tuple<int, int> Maze::getPointToMove(std::tuple<int, int> p){
    // pからゴールに向かって移動するときに移動する点をかえす
    // ただし、pからゴールに向かって移動するときに移動する点がない場合はpをかえす

    if(!isInMaze(p)) throw std::invalid_argument("p is not in maze");

    if(dist.size() != height || dist[0].size() != width) throw std::invalid_argument("dist is not initialized");

    std::vector<std::tuple<int, int>> movable = getMoveable(p);
    if(movable.size() == 0) return p;

    std::tuple<int, int> p_ = movable[0];
    for(const auto& p__ : movable){
        if(dist[std::get<0>(p_)][std::get<1>(p_)] > dist[std::get<0>(p__)][std::get<1>(p__)]){
            p_ = p__;
        }
    }
    return p_;
}

Direction Maze::getDireToMove(std::tuple<int, int> p){
    // pからゴールに向かって移動するときに移動する方向をかえす
    return getDiffDirection(p, getPointToMove(p));
}

std::tuple<std::string, int, int> Maze::getWallIdByPoint(std::tuple<int, int> p, Direction d){
    // pとdから壁のポインタをかえす
    if(!isInMaze(p)) throw std::invalid_argument("p is not in maze");
    if(!isInMaze(translate(p, d))) throw std::invalid_argument("p is on edge of maze");

    if(d == Direction::UP || d == Direction::LEFT){
        p = translate(p, d);
        d = getOpposite(d);
    }

    if(d == Direction::DOWN){
        std::tuple<std::string ,int, int> wall_id = std::make_tuple(std::string("horizontal"), std::get<0>(p), std::get<1>(p));
        return wall_id;
    } else if (d == Direction::RIGHT){
        return std::make_tuple(std::string("vertical"), std::get<0>(p), std::get<1>(p));
    } else {
        throw std::invalid_argument("invalid direction");
    }
}

std::tuple<std::string, int, int> Maze::getWallIdBetweenPoints(std::tuple<int, int> p1, std::tuple<int, int> p2){
    return getWallIdByPoint(p1, getDiffDirection(p1, p2));
}