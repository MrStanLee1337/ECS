#pragma once
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <memory>
#include "Pool.h"
#include "System.h"

class Registry 
{
    int nextId = 1;
    std::unordered_map<std::type_index, std::unique_ptr<BasePool>> pools;
    std::vector<std::unique_ptr<System>> systems;

public:
    Entity create() { return nextId++; }

    template<typename T, typename... Args>
    void addSystem(Args&&... args) {
        systems.push_back(std::make_unique<T>(std::forward<Args>(args)...));
    }

    template<typename T>
    Pool<T>& getPool() {
        std::type_index key(typeid(T));
        auto it = pools.find(key);
        if (it == pools.end()) {
            auto pool = std::make_unique<Pool<T>>();
            Pool<T>* raw = pool.get();
            pools[key] = std::move(pool);
            return *raw;
        }
        return *static_cast<Pool<T>*>(it->second.get());
    }

    template<typename T>
    void set(Entity e, std::unique_ptr<T> comp) {
        getPool<T>().add(e, std::move(comp));
    }

    template<typename T>
    T* get(Entity e) {
        return getPool<T>().get(e);
    }

    template<typename T>
    bool has(Entity e) {
        return getPool<T>().has(e);
    }

    template<typename T>
    std::vector<Entity> view() {
        std::vector<Entity> vec;
        for (const auto& pair : getPool<T>().data) {
            vec.push_back(pair.first);
        }
        return vec;
    }

    template<typename T>
    void remove(Entity e) {
        getPool<T>().remove(e);
    }

    void update(float dt) {
        for (auto& sys : systems) {
            sys->update(dt, *this);
        }
    }
};