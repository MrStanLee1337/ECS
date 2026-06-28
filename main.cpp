#include <iostream>
#include <vector>
#include <unordered_map>
#include <memory>
#include <typeindex>
#include <format>
#include "src/LuaScripts.h"
#include "src/Registry.h"
#include "src/System.h"
#include "src/Components.h"

//для теста 
class MovementSystem : public System
{
public:
    void update(float dt, Registry& registry) override
    {
        for (Entity e : registry.view<Transform>()) {
            auto* tr = registry.get<Transform>(e);
            tr->x += 1, tr->y += 1, tr->z += 1;
        }
    }
};

int main()
{
    system("chcp 65001");//чтобы в консоли на русском было
    Registry world;
    world.addSystem<MovementSystem>();
    Entity e1 = world.create();
    
    world.set<Health>(e1, std::make_unique<Health>(100, 100));
    world.set<Transform>(e1, std::make_unique<Transform>(1.0f, 2.0f, 3.0f));

    Entity e2 = world.create();
    std::cout << "Создаем сущность 1 и сущность 2 с параметрами здоровья и вектором положения в пространстве.\n";
    world.set<Health>(e2, std::make_unique<Health>(50, 50));
    world.set<Transform>(e2, std::make_unique<Transform>(10.0f, 5.0f, 3.0f));

    for (Entity e : world.view<Health>()) {
        Health* h = world.get<Health>(e);
        std::cout << "Сущность " << e << ": здоровье " << h->hp << "/" << h->maxHp << "\n";
    }
    
    for (Entity e : world.view<Transform>()) {
        Transform* tr = world.get<Transform>(e);
        std::cout << "Сущность " << e << ": вектор " << std::format("{} {} {}", tr->x, tr->y, tr->z) << "\n";
    }
    
    std::cout << "Проверка: удалили здоровье у первой сущности и вектор у второй.\n";
    world.remove<Health>(e1); 
    world.remove<Transform>(e2);
    
    
    for (Entity e : world.view<Transform>()) {
        Transform* tr = world.get<Transform>(e);
        std::cout << "Сущность " << e << ": вектор " << std::format("{} {} {}", tr->x, tr->y, tr->z) << "\n";
    }
    
    for (Entity e : world.view<Health>()) {
        Health* h = world.get<Health>(e);
        std::cout << "Сущность " << e << ": здоровье " << h->hp << "/" << h->maxHp << "\n";
    }
    
    
    std::cout << std::format("Проверим update. Был вектор x = {}, y = {}, z = {}\n", 
        world.get<Transform>(e1)->x, world.get<Transform>(e1)->y, world.get<Transform>(e1)->z);
    
    world.update(0.01f); 
    std::cout << "Произошел update(), добавим вектор 1 1 1.\n";
    
    std::cout << std::format("После update. Стал вектор x = {}, y = {}, z = {}\n", 
        world.get<Transform>(e1)->x, world.get<Transform>(e1)->y, world.get<Transform>(e1)->z);
    
    
    
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    std::cout << "Проверим работу lua скрипта. С++ передает значения в Lua. Lua изменяет значения и возвращает результат.\n";
    std::cout << "Вызов в С++. Был вектор: " 
               << std::format("{} {} {}", 
                   world.get<Transform>(e1)->x,
                   world.get<Transform>(e1)->y,
                   world.get<Transform>(e1)->z) << "\n";

    lua_State* L = luaL_newstate();
    luaL_openlibs(L);
    
    //первое замыкание
    lua_pushlightuserdata(L, &world);   // upvalue 1
    lua_pushinteger(L, e1);              // upvalue 2
    lua_pushcclosure(L, lua_get_position, 2);
    lua_setglobal(L, "get_position");
   
    //второе замыкание
    lua_pushlightuserdata(L, &world);   
    lua_pushinteger(L, e1);              
    lua_pushcclosure(L, lua_set_position, 2);
    lua_setglobal(L, "set_position");
    
    const char* script = R"(
        local x, y, z = get_position()
        print(string.format("Вызов в Lua. вектор x = %d y = %d z = %d", x, y, z))
        print(string.format("Вызов в Lua. Возводим в квадрат."))
        -- меняем позицию
        set_position(x*x, y*y, z*z)
        
        local x2, y2, z2 = get_position()
        print(string.format("Вызов в Lua. вектор x = %d y = %d z = %d", x2, y2, z2))
    )";
    
    if (luaL_dostring(L, script) != LUA_OK) {
        std::cerr << "Lua error: " << lua_tostring(L, -1) << "\n";
        lua_pop(L, 1);
    }
    
    lua_close(L);
    
    std::cout << "Вызов в С++. Стал вектор " << 
    std::format("{} {} {}", world.get<Transform>(e1)->x,world.get<Transform>(e1)->y,world.get<Transform>(e1)->z) << "\n";
    
    return 0;
}