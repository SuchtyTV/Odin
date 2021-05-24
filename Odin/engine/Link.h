//
// Created by Niclas Schwalbe on 18.05.21.
//


#pragma once

#include <memory>
#include <utility>
#include "Node.h"

class Node;

class Link {
public:
    std::shared_ptr<Node> ptr;
    std::pair<int,int> move;
    Link(std::shared_ptr<Node> ptr,  std::pair<int,int> move);
};

