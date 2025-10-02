#include <iostream>
#include "command-processor.h"
#include "cube.h"
#include <sstream>


/**
 * THING TO DO
 * move "moves"
 * solve
 * show-cube
 * show-face
 * replay scramble
 * replay solve
 * get-cross
 * set-face <face> colors
 * 
 */


 // rules or scrambel command
bool isValidMove(std::string s){
    int len = s.length();
    if(len == 0 || len > 2)
    {
        return false;
    }
    if(s[0] != 'R' 
    && s[0] != 'L'
    && s[0] != 'U'
    && s[0] != 'D'
    && s[0] != 'F'
    && s[0] != 'B'
    )
    {
        return false;
    }

    if(len == 2 && (s[1] != '\'' && s[1] != '2'))
    {
        return false;
    }
    return true;
};


int main(){
    ose4g::CommandProcessor cp("rcube");
    ose4g::Cube cube;


    ose4g::ArgCountRule<1,1> argumentsMustBe1;
    ose4g::UserRule mustBeValidScramble([](const ose4g::Args& args){
        std::stringstream ss(args[0]);

        std::string s;
        while(ss >> s)
        {
            if(!isValidMove(s))
            {
                return std::pair{false , s + " is not a valid cube move"};
            }
        }
        return std::pair{true, std::string()};
    });

    cp.add("move",[&cube](const ose4g::Args& args){
        cube.doMoves(args[0]);
    }, {&argumentsMustBe1, &mustBeValidScramble}, "Do specified actions on the cube. Sample usage:  move \"R U R' U'\"");
    cp.add("show-cube", [&cube](const ose4g::Args&){
        cube.drawCube();
    }, "print cube to screen");
    cp.run();
    
}