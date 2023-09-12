#include <Game/EventBuffer.hpp>

#include <memory.h>

namespace Core
{
    namespace Game
    {
        void EventBuffer::push(GameEvent event) {
            if (size == capacity) {
                capacity *= 2;
                GameEvent* newEvents = new GameEvent[capacity];
                memcpy(newEvents, events, size);
                delete[] events;
                events = newEvents;
            }
            events[size++] = event;
        }

        EventStream EventBuffer::getStream() {
            return { events, size };
        }

        void EventBuffer::clear() {
            size = 0;
        }
    }
}