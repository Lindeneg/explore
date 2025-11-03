#ifndef EXPLORE_EVENTS_BUS_H_
#define EXPLORE_EVENTS_BUS_H_

#include <functional>
#include <list>
#include <map>
#include <memory>
#include <typeindex>
#include <utility>

namespace explore::event {

class Event {
   public:
    Event() = default;
};

class ICallback {
   private:
    virtual void call(Event &e) = 0;

   public:
    virtual ~ICallback() = default;

    void execute(Event &e) { call(e); }
};

template <typename TOwner, typename TEvent>
class Callback : public ICallback {
   private:
    typedef void (TOwner::*CallbackFunction)(TEvent &);

    TOwner *_owner;
    CallbackFunction _callback_function;

    virtual void call(Event &e) override {
        std::invoke(_callback_function, _owner, static_cast<TEvent &>(e));
    }

   public:
    Callback(TOwner *owner, CallbackFunction callback_function)
        : _owner(owner), _callback_function(callback_function) {}

    virtual ~Callback() override = default;
};

typedef std::list<std::unique_ptr<ICallback>> HandlerList;

class Bus {
   private:
    std::map<std::type_index, std::unique_ptr<HandlerList>> _subscribers;

   public:
    Bus() = default;
    ~Bus() = default;

    template <typename TOwner, typename TEvent>
    void on(TOwner *owner, void (TOwner::*callbackFunction)(TEvent &)) {
        if (!_subscribers[typeid(TEvent)].get()) {
            _subscribers[typeid(TEvent)] = std::make_unique<HandlerList>();
        }

        auto subscriber =
            std::make_unique<Callback<TOwner, TEvent>>(owner, callbackFunction);

        _subscribers[typeid(TEvent)]->push_back(std::move(subscriber));
    }

    template <typename TEvent, typename... TArgs>
    void emit(TArgs &&...args) {
        auto handlers = _subscribers[typeid(TEvent)].get();
        if (!handlers) return;
        for (auto it = handlers->begin(); it != handlers->end(); it++) {
            auto handler = it->get();
            TEvent event(std::forward<TArgs>(args)...);
            handler->execute(event);
        }
    }
};

}  // namespace explore::event

#endif  // EXPLORE_EVENTS_BUS_H_
