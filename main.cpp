#include "direction.h"
#include "maze.h"
#include <unistd.h>
#include "make_maze_sample.cpp"

#define SLEEP_MS(x) usleep((x)*1000)

int main(){
//    std::vector<std::vector<Wall>> walls_vertical = {
//            {Wall::NOT_EXIST, Wall::EXIST, Wall::NOT_EXIST},
//            {Wall::NOT_EXIST, Wall::EXIST, Wall::NOT_EXIST},
//            {Wall::EXIST, Wall::EXIST, Wall::NOT_EXIST},
//            {Wall::EXIST, Wall::NOT_EXIST, Wall::NOT_EXIST}
//    };
//
//    std::vector<std::vector<Wall>> walls_horizontal = {
//            {Wall::EXIST, Wall::NOT_EXIST, Wall::NOT_EXIST, Wall::EXIST},
//            {Wall::NOT_EXIST, Wall::NOT_EXIST, Wall::NOT_EXIST, Wall::NOT_EXIST},
//            {Wall::NOT_EXIST, Wall::NOT_EXIST, Wall::EXIST, Wall::NOT_EXIST}
//    };
//
//    std::map<std::string, std::vector<std::vector<Wall>>> walls = {
//            {"vertical", walls_vertical},
//            {"horizontal", walls_horizontal}
//    };
//
//    Maze maze_robot(4, 4);
//    maze_robot.setStart({3, 0});
//    maze_robot.setGoal({0, 3});
//    Maze maze_real(4, 4);
//    maze_real.setStart({3, 0});
//    maze_real.setGoal({0, 3});
//    maze_real.setWalls(walls);
    const int height = 10;
    const int width = 10;
    Maze maze_robot(height, width);
    Maze maze_real = make_maze(height, width);
    maze_robot.setStart({height-1, 0});
    maze_robot.setGoal({0, width-1});
    maze_real.setStart({height-1, 0});
    maze_real.setGoal({0, width-1});

    std::tuple<int, int> now = maze_robot.start;
    std::tuple<int, int> next;

    std::cout << "The maze is" << std::endl;
    std::cout << maze_real.toString() << std::endl;
    std::cout << "Start Search" << std::endl;

    while(now != maze_robot.goal){
        std::cout << "now : (" << std::get<0>(now) << ", " << std::get<1>(now) << ")" << std::endl;
        for(const auto d : Direction()){
            next = translate(now, d);
            if((!maze_robot.isInMaze(next)) || maze_robot.getWallBetweenPoints(now, next) == Wall::EXIST) continue;
            if(maze_real.getWallBetweenPoints(now, next) == Wall::EXIST) maze_robot.setWallBetweenPoints(now, next, Wall::EXIST);
            else maze_robot.setWallBetweenPoints(now, next, Wall::NOT_EXIST);
        }
        std::cout << maze_robot.toString() << std::endl;
        maze_robot.calcDist();
        now = maze_robot.getPointToMove(now);
//        SLEEP_MS(500);
    }
    std::cout << "now : (" << std::get<0>(now) << ", " << std::get<1>(now) << ")" << std::endl;
}