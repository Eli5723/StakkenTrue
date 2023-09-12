#pragma once

#include <glm/glm.hpp>

#include <Assets/Assets.hpp>

#include <string>

#include <functional>

namespace UI {
    class Node
    {
    public:
        glm::vec2 position;
        glm::vec2 size;
        
        Node *first_child = nullptr;
        Node *next_node = nullptr;

        Node() = default;
        Node(glm::vec2 position, glm::vec2 size) : position(position), size(size), first_child(nullptr), next_node(nullptr) {}
        bool bounds(float x, float y);

        // Node management
        void addChild(Node *newNode);
        Node *getLastChild();
        void removeChild(Node *toRemove);

        // Node Functionality+-
        void renderTree();
        virtual void render(){};

        // Event Propogation
        void click(float x, float y);
        
        // Event Listeners
        std::function<void()> onClick;
    };
}
