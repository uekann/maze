#ifndef DIRECTION_H
#define DIRECTION_H

#include <vector>
#include <tuple>
#include <stdexcept>

enum class Direction{
    UP,
    RIGHT,
    DOWN,
    LEFT,
    END
};

inline Direction begin(Direction){ return Direction::UP; }
inline Direction end(Direction){ return Direction::END; }
inline Direction operator*(Direction d){ return d; }
inline Direction operator++(Direction& d){
    return d = Direction(std::underlying_type<Direction>::type(d) + 1);
//    switch (d){
//        case Direction::UP:
//            return d = Direction::RIGHT;
//        case Direction::RIGHT:
//            return d = Direction::DOWN;
//        case Direction::DOWN:
//            return d = Direction::LEFT;
//        case Direction::LEFT:
//            return d = Direction::UP;
//        default:
//            throw std::invalid_argument("invalid direction");
//    }
}

std::tuple<int, int> translate(std::tuple<int, int> p, Direction d); // pをd方向に1だけ移動した点を返す
Direction getRelative(Direction d1, Direction d2); // d1から見たd2の相対的な方向を返す
Direction getDiffDirection(std::tuple<int, int> p1, std::tuple<int, int> p2); // p1からp2に向かうときに移動する方向を返す
Direction getOpposite(Direction d); // dの反対方向を返す

#endif
