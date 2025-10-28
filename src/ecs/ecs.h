#ifndef EXPLORE_ECS_ECS_H_
#define EXPLORE_ECS_ECS_H_

#include <bitset>
#include <vector>

#include "../common.h"

static constexpr u32 MAX_COMPONENTS{32};

namespace explore::ecs {

// use a bitset to keep track of which components
// an entity has, and also helps keep track of which entities
// a given system is interested in.
typedef std::bitset<MAX_COMPONENTS> Signature;

struct BaseComponent {
   protected:
    static u32 _next_id;
};

template <typename TComponent>
class Component : public BaseComponent {
   public:
    // assign unique id to a TComponent
    static const u32 get_id() {
        // created once per unique TComponent
        static auto id{_next_id++};
        return id;
    }
};

class Entity {
   private:
    u32 _id;

   public:
    Entity(u32 id) : _id(id) {}

   public:
    [[nodiscard]] u32 get_id() const;

    bool operator==(const Entity &other) const;
    bool operator!=(const Entity &other) const;
    bool operator<(const Entity &other) const;
    bool operator>(const Entity &other) const;
};

class System {
   private:
    Signature _component_signature;
    std::vector<Entity> _entities;

   public:
    System() = default;
    ~System() = default;

    void add_entity(Entity entity);
    bool remove_entity(Entity entity);

    const std::vector<Entity> &get_entities() const;
    const Signature &get_comp_signature() const;

    template <typename TComponent>
    void require_component();
};

template <typename TComponent>
void System::require_component() {
    const auto id{Component<TComponent>::get_id()};
    _component_signature.set(id);
}

class IPool {
   public:
    virtual ~IPool() {}
};

template <typename T>
class Pool : public IPool {
   private:
    std::vector<T> _data;

   public:
    Pool(u32 size = 100) { _data.resize(size); }

    virtual ~Pool() = default;

    bool empty() const { return _data.empty(); }

    u32 size() const { return _data.size(); }

    void resize(u32 n) { _data.resize(n); }

    void clear() { _data.clear(); }

    void add(T object) { _data.push_back(object); }

    void set(u32 index, T object) { _data[index] = object; }

    T &get(u32 index) { return _data[index]; }

    T &operator[](u32 index) { return _data[index]; }
};
}  // namespace explore::ecs

#endif  // EXPLORE_ECS_ECS_H_
