#include "cube.h"
#include <string>
#include <iostream>
#include <sstream>

namespace ose4g{
    const int CLOCKWISE=1, ANTICLOCKWISE=-1, DOUBLE=2;

    void Cube::rotateFace(int direction, Cube::FaceName faceName)
    {
        Face& face = faceMap.at(faceName);

        // if it is a double rorate then switch the opposite sides and corners
        if(direction == DOUBLE){
            std::swap(face[0][1], face[2][1]);
            std::swap(face[1][0], face[1][2]);
            std::swap(face[0][0],face[2][2]);
            std::swap(face[0][2],face[2][0]);
        }
        else{
            // if not rotate the array as required.
            std::array<std::array<int,2>,4> corners{{{0,0},{0,2},{2,2},{2,0}}};
            std::array<std::array<int,2>,4> sides{{{0,1},{1,2},{2,1},{1,0}}};

            int start = 0;
            for(int i = 0; i < 3; i++)
            {
                int prev = (start + 4 - direction) % 4;
                auto c1 = corners[prev];
                auto c2 = corners[start];
                auto s1 = sides[prev];
                auto s2 = sides[start];

                std::swap(face[c1[0]][c1[1]], face[c2[0]][c2[1]]);
                std::swap(face[s1[0]][s1[1]], face[s2[0]][s2[1]]);
                start -= direction;
                start +=4;
                start %= 4;
            }
        }

        // rotate the sides on the border of face.

        auto belt = faceBorderMap[faceName];
        int steps = direction == CLOCKWISE ? 3 : direction == ANTICLOCKWISE ? -3 : 6;
        int swaps = direction == DOUBLE ? 6  : 9;

        int start = 0;
        for(int i = 0; i < swaps; i++)
        {
            int prev = (start - steps + 12) % 12;
            auto prevVal=  belt[prev];
            auto currVal = belt[start];

            start = steps < 0 ? start+1 : start-1;
            start += 12;
            start %= 12;

            std::swap(faceMap.at(prevVal.d_facename)[prevVal.d_row][prevVal.d_col], faceMap.at(currVal.d_facename)[currVal.d_row][currVal.d_col]);
        }
        return;
        
    }

    Cube::Cube(){
        
        reset();
        using enum FaceName;

        // set the values in facemap
        faceMap.insert({FRONT, front});
        faceMap.insert({BACK, back});
        faceMap.insert({LEFT, left});
        faceMap.insert({RIGHT, right});
        faceMap.insert({UP, up});
        faceMap.insert({DOWN, down});

        letterFaceMap['R'] = RIGHT;
        letterFaceMap['L'] = LEFT;
        letterFaceMap['U'] = UP;
        letterFaceMap['D'] = DOWN;
        letterFaceMap['F'] = FRONT;
        letterFaceMap['B'] = BACK;


        // set the borders of each face.

        faceBorderMap[FRONT] = {
            {UP, 2, 0}, {UP, 2, 1}, {UP, 2, 2}, 
            {RIGHT, 0, 0}, {RIGHT, 1, 0}, {RIGHT, 2, 0},
            {DOWN, 0, 2}, {DOWN, 0, 1}, {DOWN, 0, 0},
            {LEFT, 2, 2}, {LEFT, 1, 2}, {LEFT, 0, 2}
        }; 

        faceBorderMap[BACK] = {
            {UP, 0, 2}, {UP, 0, 1}, {UP, 0, 0}, 
            {LEFT, 0, 0}, {LEFT, 1, 0}, {LEFT, 2, 0},
            {DOWN, 2, 0}, {DOWN, 2, 1}, {DOWN, 2, 2},
            {RIGHT, 2, 2}, {RIGHT, 1, 2}, {RIGHT, 0, 2}
        }; 

        faceBorderMap[LEFT] = {
            {UP, 0, 0}, {UP, 1, 0}, {UP, 2, 0}, 
            {FRONT, 0, 0}, {FRONT, 1, 0}, {FRONT, 2, 0},
            {DOWN, 0, 0}, {DOWN, 1, 0}, {DOWN, 2, 0},
            {BACK, 2, 2}, {BACK, 1, 2}, {BACK, 0, 2}
        }; 

        faceBorderMap[RIGHT] = {
            {UP, 2, 2}, {UP, 1, 2}, {UP, 0, 2}, 
            {BACK, 0, 0}, {BACK, 1, 0}, {BACK, 2, 0},
            {DOWN, 2, 2}, {DOWN, 1, 2}, {DOWN, 0, 2},
            {FRONT, 2, 2}, {FRONT, 1, 2}, {FRONT, 0, 2}
        }; 

        faceBorderMap[UP] = {
            {BACK, 0, 2}, {BACK, 0, 1}, {BACK, 0, 0}, 
            {RIGHT, 0, 2}, {RIGHT, 0, 1}, {RIGHT, 0, 0},
            {FRONT, 0, 2}, {FRONT, 0, 1}, {FRONT, 0, 0},
            {LEFT, 0, 2}, {LEFT, 0, 1}, {LEFT, 0, 0}
        }; 

        faceBorderMap[DOWN] = {
            {FRONT, 2, 0}, {FRONT, 2, 1}, {FRONT, 2, 2}, 
            {RIGHT, 2, 0}, {RIGHT, 2, 1}, {RIGHT, 2, 2},
            {BACK, 2, 0}, {BACK, 2, 1}, {BACK, 2, 2},
            {LEFT, 2, 0}, {LEFT, 2, 1}, {LEFT, 2, 2}
        }; 
        


    }

    void drawColor(Cube::Color color, char c){
        std::string code;
        if(color == Cube::Color::BLUE)
        {
            code = "44";
        }else if(color == Cube::Color::GREEN)
        {
            code = "42";
        }else if(color == Cube::Color::ORANGE)
        {   
            code = "48;5;208";
        }else if(color == Cube::Color::RED)
        {
            code = "41";
        }else if(color == Cube::Color::WHITE)
        {
            code = "47";
        }else if(color == Cube::Color::YELLOW)
        {
            code = "43";
        }
        if(code.length()==0)
        {   
            std::cout<<std::string(8,c)<<"";
        }
        else{
            std::cout<<"\033["<<code<<"m"<<std::string(8,c)<<"\033[0m";
        }
    }


    void Cube::drawFace(FaceName& facename) const {
        for(auto &row: faceMap.at(facename))
        {
            drawRow(row);
        }
    }

    void Cube::drawCube() const{
        
        std::array<std::array<Color, 12>, 9> bigCube;
        for(auto &row: bigCube)
        {
            row.fill(Color::NONE);
        }

        //second line

        //for first row
        for(int i = 0; i < 3; i ++)
        {
            int c = 3; 
            for(auto col: up[i])
            {
                bigCube[i][c] = col;
                c++;
            }
        }


        std::array faces = {left, front, right, back};
        for(int i = 0; i < 3; i ++)
        {
            int c = 0; 
            for(auto &f: faces)
            {
                for(auto col: f[i])
                {
                    bigCube[i+3][c] = col;
                    c++;
                }
            }
        }

         //for last row
         for(int i = 0; i < 3; i ++)
         {
             int c = 3; 
             for(auto col: down[i])
             {
                 bigCube[i+6][c] = col;
                 c++;
             }
         }
 

        for(auto &row: bigCube)
        {
            drawRow(row);
        }

    }

    void Cube::doMove(const std::string& move) {
        // assumption is that a valid move is being used. Validation will not be done in this method.
        auto faceName = letterFaceMap[toupper(move[0])];
        auto direction = move.length()==1 ? CLOCKWISE : move[1] == '2' ? DOUBLE : ANTICLOCKWISE;

        rotateFace(direction, faceName);
    }

    void Cube::doMoves(const std::string& moves){
        std::stringstream ss(moves);

        std::string s;
        while(ss >> s)
        {
            doMove(s);
        }
    }

    void Cube::reset() {
        static const auto fillColor = [this](Face& face, Color color){
            for(auto &row: face)
            {
                row.fill(color);
            }
        };

        // set the colors of the cube
        fillColor(front, Color::GREEN);
        fillColor(back, Color::BLUE);
        fillColor(left, Color::RED);
        fillColor(right, Color::ORANGE);
        fillColor(up, Color::YELLOW);
        fillColor(down, Color::WHITE);
    }

    
}