#pragma once
struct Health
{
    int hp, maxHp; 
    Health(int hp, int maxHp) : hp(hp), maxHp(maxHp) {};
};
struct Transform
{
    float x, y, z; 
    Transform(float x, float y, float z) : x(x), y(y), z(z) {};
};