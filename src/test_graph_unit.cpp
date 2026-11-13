//c++ libraries
#include <iostream>
// SAMC
#include "graph.hpp"

void test_coord_to_index(){
    std::cout<<"making graph\n";
    Graph graph;
    graph.resize(5,5,5);
    std::cout<<"graph = "<<graph<<"\n";
    std::cout<<"indices = \n";
    for(int i=0; i<5; ++i){
        for(int j=0; j<5; ++j){
            for(int k=0; k<5; ++k){
                std::vector<int> v={i,j,k};
                std::cout<<"index("<<i<<","<<j<<","<<k<<") = "<<graph.index(v)<<"\n";
            }
        }
    }
}

void test_index_to_coord(){
    std::cout<<"making graph\n";
    Graph graph;
    graph.resize(5,5,5);
    std::cout<<"graph = "<<graph<<"\n";
    std::cout<<"indices = \n";
    for(int n=0; n<graph.size(); ++n){
        const std::vector<int> v=graph.index(n);
        std::cout<<"index("<<n<<") = ("<<v[0]<<","<<v[1]<<","<<v[2]<<")\n";
    }
}

void test_rand(){
    std::cout<<"making graph\n";
    Graph graph;
    graph.resize(5,5,5);
    std::cout<<"graph = "<<graph<<"\n";
    std::cout<<"filling graph randomly\n";
    Graph::fill_rand(graph);
    std::cout<<"writing graph\n";
    Graph::write_data(graph,"graph_rand.data");
}

void test_snake(){
    std::cout<<"making graph\n";
    Graph graph;
    graph.resize(5,5,5);
    std::cout<<"graph = "<<graph<<"\n";
    std::cout<<"filling graph with snake\n";
    Graph::fill_snake(graph);
    std::cout<<"writing graph\n";
    Graph::write_data(graph,"graph_snake.data");
}

void test_snake_broken(){
    std::cout<<"making graph\n";
    Graph graph;
    graph.resize(5,5,5);
    std::cout<<"graph = "<<graph<<"\n";
    std::cout<<"filling graph with snake - broken (8)\n";
    Graph::fill_snake_broken(graph,8);
    std::cout<<"writing graph\n";
    Graph::write_data(graph,"graph_snake_broken.data");
}

void test_degree(){
    std::vector<int> degree;
    Graph graph;
    graph.resize(5,5,5);
    std::cout<<"rand\n";
    Graph::fill_rand(graph);
    Graph::degree(graph,degree);
    for(int i=0; i<degree.size(); ++i){
        std::cout<<"degree["<<i<<"] = "<<degree[i]<<"\n";
    }
    std::cout<<"snake\n";
    Graph::fill_snake(graph);
    Graph::degree(graph,degree);
    for(int i=0; i<degree.size(); ++i){
        std::cout<<"degree["<<i<<"] = "<<degree[i]<<"\n";
    }
    std::cout<<"snake_broken 8\n";
    Graph::fill_snake_broken(graph,8);
    Graph::degree(graph,degree);
    for(int i=0; i<degree.size(); ++i){
        std::cout<<"degree["<<i<<"] = "<<degree[i]<<"\n";
    }
}

int main(int argc, char* argv[]){

    test_coord_to_index();
    test_index_to_coord();
    test_rand();
    test_snake();
    test_snake_broken();
    test_degree();
    
    return 0;
}