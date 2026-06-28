#pragma once
class Registry;  

class System 
{
public:
    virtual ~System() = default;
    virtual void update(float dt, Registry& registry) = 0;
};


