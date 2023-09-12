#include <UI/Node.hpp>

#include <Draw/Quad.hpp>

namespace UI
{
    bool Node::bounds(float x, float y)
    {
        if (x >= position.x &&
            y >= position.y &&
            x <= position.x + size.x &&
            y <= position.y + size.y)
        {
            return true;
        }

        return false;
    }

    // Node management
    void Node::addChild(Node *newNode)
    {
        // Add node to intrusive list 
        if (first_child == nullptr)
        {
            first_child = newNode;
            return;
        }

        Node *current = first_child;
        while (current->next_node != nullptr)
            current = current->next_node;
        
        current->next_node = newNode;
    }

    Node *Node::getLastChild()
    {
        Node *current = first_child;
        while (current->next_node != nullptr)
            current = current->next_node;

        return current;
    }

    void Node::removeChild(Node *toRemove)
    {
        // Node is first and only
        if (first_child == toRemove)
        {
            if (toRemove->next_node == nullptr)
                first_child = nullptr;
            else
                first_child = first_child->next_node;
            return;
        }

        for (Node *current = first_child; current != nullptr; current = current->next_node)
        {

            if (current->next_node == toRemove)
            {
                current->next_node = toRemove->next_node;
                return;
            }
        }
    }

    // Node Functionality
    void Node::renderTree()
    {
        render();
        Node *child = first_child;
        while (child)
        {
            child->renderTree();
            child = child->next_node;
        }
    }

    void Node::click(float x, float y) {
        if (bounds(x,y)) {
            if (this->onClick)
                this->onClick();

            Node *child = first_child;
            while (child)
            {
                child->click(x,y);
                child = child->next_node;
            }
        }
    }
}