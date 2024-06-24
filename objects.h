#include <SDL2/SDL.h>
#include <vector>
#include <iostream>

using namespace std;

class object
{
    private:
    int _posX, _posY, _width, _height;
    

    public:
    SDL_Rect rect;
    string _tag;
    int _health;

    object(int posX = 0, int posY = 0, int width = 0, int height = 0, string tag = "general", int health = 0)
    {
        _posX = posX;
        _posY = posY;
        _width = width;
        _height = height;
        _tag = tag;
        _health = health;
        rect = {posX, posY, width, height};
    }

    void updateX(int X)
    {
        _posX += X;
        rect = {_posX, _posY, _width, _height};
    }

    void updateY(int Y)
    {
        _posY += Y;
        rect = {_posX, _posY, _width, _height};
    }

    void updateXY(int X, int Y)
    {
        _posX += X;
        _posY += Y;
        rect = {_posX, _posY, _width, _height};
    }

    void updateWIDTH(int W)
    {
        _width += W;
        rect = {_posX, _posY, _width, _height};
    }

    void updateHEIGHT(int H)
    {
        _height += H;
        rect = {_posX, _posY, _width, _height};
    }
        
};

class hierarchy
{
    public:
    object *objects;

    hierarchy(int count)
    {
        objects = new object[count];
    }

    ~hierarchy()
    {
        delete[] objects;
    }

};
