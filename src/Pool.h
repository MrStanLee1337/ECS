#pragma once
#include <unordered_map>
#include <memory>
#include <typeindex>

using Entity = uint32_t;
class BasePool { public: virtual ~BasePool() = default; };

template<typename T>
class Pool : public BasePool 
{
public:
    std::unordered_map<Entity, std::unique_ptr<T>> data;
    void add(Entity entity, std::unique_ptr<T> comp) { data[entity] = std::move(comp); }
    bool has(Entity entity) { return data.count(entity) != 0; }
    void remove(Entity entity) { data.erase(entity); }
    T* get(Entity entity) 
    {
        auto it = data.find(entity);
        if (it == data.end()) return nullptr;
        return it->second.get(); 
    }
};