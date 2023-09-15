#include <Game/EventBuffer.hpp>

#include <memory.h>

namespace Core
{
    namespace Game
    {
        void InputBuffer::push(GameInput event) {
            if (size == capacity) {
                capacity *= 2;
                GameInput* newEvents = new GameInput[capacity];
                memcpy(newEvents, events, size);
                delete[] events;
                events = newEvents;
            }
            events[size++] = event;
        }

        InputStream InputBuffer::getStream() {
            return { events, size };
        }

        void InputBuffer::clear() {
            size = 0;
        }
    }
}