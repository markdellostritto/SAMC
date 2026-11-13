#pragma once
#ifndef GRID_HPP
#define GRID_HPP

//c++ libraries
#include <iostream>
#include <vector>

#ifndef ROW_MAJOR
#define ROW_MAJOR
#endif

//******************************************************
// Graph
//******************************************************

class Graph{
private:
    int dim_{0};//dimension
    int size_{0};//size of array storing vertices
    std::vector<int> len_;//lengths along each dimension
    std::vector<int> v_;//vertices
    std::vector<std::vector<int> > e_;//edges
    void resize();
public:
    //==== constructors/destructors ====
    Graph(){}
    ~Graph(){}

    //==== operators ====
    friend std::ostream& operator<<(std::ostream& out, const Graph& graph);

    //==== access ====
    //graph data
    const int& dim()const{return dim_;}
    const int& size()const{return size_;}
    const int& len(int i)const{return len_[i];}
    //vertex indices
    int index(const std::vector<int>& v)const;
    std::vector<int> index(int i)const;
    //vertices
    int& v(int i){return v_[i];}
    const int& v(int i)const{return v_[i];}
    //edges
    int nedges(int i)const{return e_[i].size();}
    int edge(int i, int j)const{return e_[i][j];}
    
    //==== member functions ====
    //resizing
    void resize(int d, const std::vector<int>& l);
    void resize(int l1);
    void resize(int l1, int l2);
    void resize(int l1, int l2, int l3);
    //modifying edges
    void edge_a(int i, int j);
    void edge_r(int i, int j);
    void edge_c(int i);
    void edge_c();
    bool edge_f(int i, int j)const;
    //neighbor
    std::vector<int> neighbor(int i)const;
    //comparison
    int edge_diff(const Graph& graph)const;
    
    //==== static functions ====
    static Graph& fill_rand(Graph& graph);
    static Graph& fill_snake(Graph& graph);
    static Graph& fill_snake_broken(Graph& graph, int n);
    static void write_data(const Graph& graph, const char* fname);
    static void write_data(const Graph& graph, FILE* writer);
    static std::vector<int>& degree(const Graph& graph, std::vector<int>& degree);
    static int edge_diff(const Graph& graph1, const Graph& graph2);
};

#endif