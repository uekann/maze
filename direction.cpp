#include "direction.h"

std::tuple<int, int> translate(std::tuple<int, int> p, Direction d){
    // pをd方向に1だけ移動した点を返す
    switch (d){
        case Direction::UP:
            return std::make_tuple(std::get<0>(p)-1, std::get<1>(p));
        case Direction::DOWN:
            return std::make_tuple(std::get<0>(p)+1, std::get<1>(p));
        case Direction::LEFT:
            return std::make_tuple(std::get<0>(p), std::get<1>(p)-1);
        case Direction::RIGHT:
            return std::make_tuple(std::get<0>(p), std::get<1>(p)+1);
        default:
            throw std::invalid_argument("invalid direction");
    }
}

Direction getRelative(Direction d1, Direction d2){
    // d1から見たd2の相対的な方向を返す
    return static_cast<Direction>(
            ((static_cast<int>(d2) - static_cast<int>(d1)) + 4) % 4
    );
}

Direction getDiffDirection(std::tuple<int, int> p1, std::tuple<int, int> p2){
    for(auto d : Direction()){
        if(translate(p1, d) == p2) return d;
    }
    throw std::invalid_argument("p1 and p2 are not adjacent");
}

Direction getOpposite(Direction d){
    // dの反対方向を返す
    switch (d){
        case Direction::UP:
            return Direction::DOWN;
        case Direction::DOWN:
            return Direction::UP;
        case Direction::LEFT:
            return Direction::RIGHT;
        case Direction::RIGHT:
            return Direction::LEFT;
        default:
            throw std::invalid_argument("invalid direction");
    }
}