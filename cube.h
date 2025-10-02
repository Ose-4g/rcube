#ifndef CUBE_H
#define CUBE_H

#include <array>
#include <iostream>


namespace ose4g{

    /**
     * This class represents a 3 by 3 rubiks cube
     */
    class Cube{
        public:
            // colors for each of the faces of the cube
            enum class Color{
                GREEN, 
                RED, 
                ORANGE, 
                BLUE, 
                YELLOW, 
                WHITE,
                NONE
            };

            // each face is a 3 by 3 array or colors
            using Face = std::array<std::array<Color, 3>, 3>;

            // names of each of the faces of the cibe
            enum class FaceName{
                FRONT,
                BACK,
                LEFT,
                RIGHT,
                UP,
                DOWN
            };

            // struct to represent a sticker face on a cube
            struct StickerPosition{
                FaceName d_facename;
                int d_row;
                int d_col;
            };

        private:
            // the 6 faces of the cube
            Face front, back, left, right, up, down;

            // map that maps the facebame to the actual face array
            std::unordered_map<FaceName, Face&> faceMap;

            // vector that maps a facename to the list of sticker faces that surround it and rotate with it. 
            // e.g when the UP face rotates, it also changes the faces of RIGHT, LEFT, FRONT and BACK
            std::unordered_map<FaceName, std::vector<StickerPosition>> faceBorderMap;

            // maps a letter to the face it rotates. 
            // e.g R maps to RIGHT and U to UP
            std::unordered_map<char, FaceName> letterFaceMap;
            
            // rotates one face oa a cube in a given direaction
            void rotateFace(int direction, Cube::FaceName faceName);

            // does an action on the cube
            void doMove(const std::string& move);

        public:
            Cube();
            // draws a single face of the cube
            void drawFace(FaceName& face) const;

            // given a string of moves in Cube notation, it rotates the cube.
            void doMoves(const std::string& moves);

            // draws the entire cube.
            void drawCube() const;

            // sets the cube to default solved state
            void reset();
    };

    // uses ansi code to draw a color for the cube sticker
    void drawColor(Cube::Color color, char c = ' ');

    // draws a row of colors for the cube
    template <typename T>
    requires std::is_same_v<T, std::array<Cube::Color, 3>> 
    ||  std::is_same_v<T, std::array<Cube::Color, 12>>
    void drawRow(T& row)
    {
        for(int i = 0; i < 3; i ++)
        {
            for(int j = 0; j < row.size()-1; j++)
            {
                drawColor(row[j]);std::cout<<"  ";
            } 
            drawColor(row.back()); std::cout<<"\n";
        }
        for(int j = 0; j < row.size()-1; j++)
        {
            drawColor(row[j]);std::cout<<"  ";
        } 
        drawColor(row.back()); std::cout<<"\n\n";
    }
}

#endif