//c++ libraries
#include <stdexcept>
#include <random>
#include <stack>
// samc
#include "graph.hpp"
#include "math.hpp"

//******************************************************
// Graph
//******************************************************

//==== operators ====

std::ostream& operator<<(std::ostream& out, const Graph& graph){
    out<<" d "<<graph.dim()<<" s "<<graph.size()<<" l";
    for(int i=0; i<graph.dim(); ++i){
        out<<" "<<graph.len(i);
    }
    return out;
}

//==== access ====

int Graph::index(const std::vector<int>& v)const{
    #ifdef ROW_MAJOR
        //row-major order
        int i=v[0];
        for(int d=1; d<dim_; ++d){
            i*=len_[d];
            i+=v[d];
        }
    #else
        //column-major order
        int i=v[dim_-1];
        for(int d=dim_-2; d>=0; --d){
            i*=len_[d];
            i+=v[d];
        }
    #endif
    return i;
}

std::vector<int> Graph::index(int i)const{
    std::vector<int> v(dim_);
    #ifdef ROW_MAJOR
        //row-major order
        for(int d=dim_-1; d>=0; --d){
            v[d]=math::mod(i,len_[d]);
            i/=len_[d];
        }
    #else
        //column-major order
        for(int d=0; d<dim_; ++d){
            v[d]=math::mod(i,len_[d]);
            i/=len_[d];
        }
    #endif
    return v;
}

//==== member functions ====

//resizing 

void Graph::resize(){
    //compute sizes
    size_=1;
    for(int d=0; d<dim_; ++d){
        size_*=len_[d];
    }
    //resize graph data
    v_.resize(size_,0);
    e_.resize(size_);
}

void Graph::resize(int d, const std::vector<int>& l){
    //check parameters
    if(d<=0) throw std::invalid_argument("Invalid dimension: must be greater than zero.");
    if(d!=l.size()) throw std::invalid_argument("Invalid dimension: must match lengths.");
    for(int i=0; i<l.size(); ++i){
        if(l[i]<=0) throw std::invalid_argument("Invalid length: must be greater than zero.");
    }
    //set graph size
    dim_=d;
    len_=l;
    //resize graph data
    resize();
}

void Graph::resize(int l1){
    //check parameters
    if(l1<=0) throw std::invalid_argument("Invalid length: must be greater than zero.");
    //set graph size
    dim_=1;
    len_.resize(dim_);
    len_[0]=l1;
    //resize graph data
    resize();
}

void Graph::resize(int l1, int l2){
    //check parameters
    if(l1<=0) throw std::invalid_argument("Invalid length: must be greater than zero.");
    if(l2<=0) throw std::invalid_argument("Invalid length: must be greater than zero.");
    //set graph size
    dim_=2;
    len_.resize(dim_);
    len_[0]=l1;
    len_[1]=l2;
    //resize graph data
    resize();
}

void Graph::resize(int l1, int l2, int l3){
    //check parameters
    if(l1<=0) throw std::invalid_argument("Invalid length: must be greater than zero.");
    if(l2<=0) throw std::invalid_argument("Invalid length: must be greater than zero.");
    if(l3<=0) throw std::invalid_argument("Invalid length: must be greater than zero.");
    //set graph size
    dim_=3;
    len_.resize(dim_);
    len_[0]=l1;
    len_[1]=l2;
    len_[2]=l3;
    //resize graph data
    resize();
}

//modifying edges

void Graph::edge_a(int i, int j){
    e_[i].push_back(j);
    e_[j].push_back(i);
}

void Graph::edge_r(int i, int j){
    for(int n=0; n<e_[i].size(); ++n){
        if(e_[i][n]==j){
            e_[i][n]=e_[i].back();
            e_[i].pop_back();
            break;
        }
    }
    for(int n=0; n<e_[j].size(); ++n){
        if(e_[j][n]==i){
            e_[j][n]=e_[j].back();
            e_[j].pop_back();
            break;
        }
    }
}

void Graph::edge_c(int i){
    e_[i].clear();
}

void Graph::edge_c(){
    for(int i=0; i<size_; ++i){
        e_[i].clear();
    }
}

bool Graph::edge_f(int a, int b)const{
    bool edge=false;
    for(int n=0; n<e_[a].size(); ++n){
        if(e_[a][n]==b){
            edge=true;
            break;
        } 
    }
    return edge;
}

//neighbor

std::vector<int> Graph::neighbor(int i)const{
    std::vector<int> nn;
    std::vector<int> v=index(i);
    std::vector<int> vn;
    for(int d=0; d<dim_; ++d){
        vn=v; vn[d]=math::mod(vn[d]+1,len_[d]);
        nn.push_back(index(vn));
        vn=v; vn[d]=math::mod(vn[d]-1,len_[d]);
        nn.push_back(index(vn));
    }
    return nn;
}

//comparison

int Graph::edge_diff(const Graph& graph)const{
    if(graph.dim()!=dim_) throw std::invalid_argument("Graph must be the same dim to take diff.");
    if(graph.size()!=size_) throw std::invalid_argument("Graph must be the same size to take diff.");
    for(int d=0; d<dim_; ++d){
        if(graph.len(d)!=len_[d]) throw std::invalid_argument("Graph must be the same len to take diff.");
    }
    int nEdgeDiff=0;
    for(int i=0; i<size_; ++i){
        for(int j=0; j<e_[i].size(); ++j){
            if(!graph.edge_f(i,graph.edge(i,j))) nEdgeDiff++;
        }
    }
    return nEdgeDiff;
}

//==== static functions ====

Graph& Graph::fill_rand(Graph& graph){
    //fill each vertex
    for(int i=0; i<graph.size(); ++i){
        graph.v(i)=1;
    }
    //reset the bonds
    for(int i=0; i<graph.size(); ++i){
        graph.edge_c(i);
    }
    //fill bonds randomly
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dist(0.0,1.0);
    for(int i=0; i<graph.size(); ++i){
        std::vector<int> v=graph.index(i);
        for(int d=0; d<graph.dim(); ++d){
            std::vector<int> w=v;
            w[d]=math::mod(w[d]+1,graph.len(d));
            const double r=dist(gen);
            if(r<0.5) graph.edge_a(i,graph.index(w));
        }    
    }
    //return graph
    return graph;
}

Graph& Graph::fill_snake(Graph& graph){
    //fill each vertex
    for(int i=0; i<graph.size(); ++i){
        graph.v(i)=1;
    }
    //reset the bonds
    for(int i=0; i<graph.size(); ++i){
        graph.edge_c(i);
    }
    //loop over all vertices 
    std::vector<int> current={0,0,0};
    int npt=1;
    do{
        //get next point
        for(int d=graph.dim()-1; d>=0; --d){
            std::vector<int> next=current;
            next[d]=math::mod(next[d]+1,graph.len(d));
            const int iNext=graph.index(next);
            if(graph.nedges(iNext)==0){
                graph.edge_a(graph.index(current),iNext);
                current=next;
                break;
            }
        }
        //increment npt
        npt++;
    }while(npt<graph.size());
    //return graph
    return graph;
}

Graph& Graph::fill_snake_broken(Graph& graph, int n){
    //check args
    if(n<1) throw std::invalid_argument("Can't split polymer into less than 1 component.");
    //fill snake
    Graph::fill_snake(graph);
    //break snake
    const int len=std::round((1.0*graph.size())/n);
    std::vector<bool> visited(graph.size(),false);
    visited[0]=true;
    std::vector<int> current={0,0,0};
    int npt=1;
    do{
        //get next point
        const int iCurrent=graph.index(current);
        for(int i=0; i<graph.nedges(iCurrent); ++i){
            const int iNext=graph.edge(iCurrent,i);
            if(!visited[iNext]){
                visited[iNext]=true;
                if(math::mod(npt,len)==0 && graph.nedges(iNext)>1){
                    graph.edge_r(iCurrent,iNext);
                }
                current=graph.index(iNext);
            }
        }
        //increment npt
        npt++;
    }while(npt<graph.size());
    //return graph
    return graph;
}

void Graph::write_data(const Graph& graph, const char* fname){
    FILE* writer=fopen(fname,"w");
    if(writer==NULL) throw std::runtime_error("Could not open file for writing.");
    write_data(graph,writer);
    fclose(writer);
    writer=NULL;
}

void Graph::write_data(const Graph& graph, FILE* writer){
    //compute the number of atoms
    int natoms=0;
    for(int i=0; i<graph.size(); ++i){
        if(graph.v(i)==1) ++natoms;
    }
    //compute the bonds
    std::vector<std::vector<int> > bonds;
    std::vector<bool> visited(graph.size(),false);
    std::stack<int> nodes;
    for(int n=0; n<graph.size(); ++n){
        if(!visited[n]) nodes.push(n);
        while(!nodes.empty()){
            const int current=nodes.top(); nodes.pop();
            visited[current]=true;
            for(int i=0; i<graph.nedges(current); ++i){
                if(!visited[graph.edge(current,i)]){
                    const int next=graph.edge(current,i);
                    const int ii=current+1;
                    const int jj=next+1;
                    std::vector<int> bond={ii,jj};
                    bonds.push_back(bond);
                    nodes.push(next);
                }
            }
        }
    }
    //write comment
    fprintf(writer,"# graph\n");
    fprintf(writer,"\n");
    //write atoms
    fprintf(writer,"%i atoms\n",natoms);
    fprintf(writer,"%i bonds\n",bonds.size());
    //write types
    fprintf(writer,"1 atom types\n");
    fprintf(writer,"1 bond types\n");
    fprintf(writer,"\n");
    //write limits
    fprintf(writer,"0.0 %f xlo xhi\n",graph.len(0)*1.0);
    fprintf(writer,"0.0 %f ylo yhi\n",graph.len(1)*1.0);
    fprintf(writer,"0.0 %f zlo zhi\n",graph.len(2)*1.0);
    fprintf(writer,"\n");
    //write masses
    fprintf(writer,"Masses\n\n");
    fprintf(writer,"1 1.0\n");
    fprintf(writer,"\n");
    //write atoms
    fprintf(writer,"Atoms\n\n");
    int iAtom=0;
    const int tAtom=1;
    for(int i=0; i<graph.len(0); ++i){
        for(int j=0; j<graph.len(1); ++j){
            for(int k=0; k<graph.len(2); ++k){
                std::vector<int> v={i,j,k};
                if(graph.v(graph.index(v))==1) fprintf(writer,"%i %i %f %f %f\n",++iAtom,tAtom,i*1.0,j*1.0,k*1.0);
            }
        }
    }
    fprintf(writer,"\n");
    //write bonds
    fprintf(writer,"Bonds\n\n");
    const int tBond=1;
    for(int i=0; i<bonds.size(); ++i){
        fprintf(writer,"%i %i %i %i\n",i+1,tBond,bonds[i][0],bonds[i][1]);
    }
    fprintf(writer,"\n");
}

std::vector<int>& Graph::degree(const Graph& graph, std::vector<int>& degree){
    //clear and resize vector
    degree.clear();
    degree.resize(graph.size(),0);
    //compute the degree of each node
    for(int i=0; i<graph.size(); ++i){
        for(int n=0; n<graph.nedges(i); ++n){
            degree[i]++;
            degree[graph.edge(i,n)]++;
        }
    }
    for(int i=0; i<graph.size(); ++i){
        degree[i]/=2;
    }
    //return degree
    return degree;
}

int Graph::edge_diff(const Graph& graph1, const Graph& graph2){
    if(graph1.dim()!=graph2.dim()) throw std::invalid_argument("Graphs must be the same dim to take diff.");
    if(graph1.size()!=graph2.size()) throw std::invalid_argument("Graphs must be the same size to take diff.");
    const int dim=graph1.dim();
    for(int d=0; d<dim; ++d){
        if(graph1.len(d)!=graph2.len(d)) throw std::invalid_argument("Graphs must be the same len to take diff.");
    }
    int nEdgeDiff=0;
    const int size=graph1.size();
    for(int i=0; i<size; ++i){
        const std::vector<int> v=graph1.index(i);
        for(int d=0; d<dim; ++d){
            std::vector<int> w=v;
            w[d]=(w[d]+1)%graph1.len(d);
            const int j=graph1.index(w);
            const bool edge1=graph1.edge_f(i,j);
            const bool edge2=graph2.edge_f(i,j);
            if(edge1!=edge2) nEdgeDiff++;
        }
    }
    return nEdgeDiff;
}