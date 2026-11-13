#pragma once
#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "graph.hpp"

class Config{
private:
    Graph graph_;
    std::vector<int> ep_;
public:
    //==== constructors/destructors ====
    Config(){}
    ~Config(){}

    //==== access ====
    Graph& graph(){return graph_;}
    const Graph& graph()const{return graph_;}
    std::vector<int>& ep(){return ep_;}
    const std::vector<int>& ep()const{return ep_;}
};

#endif