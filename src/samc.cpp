#include <algorithm>
#include <string>
#include "samc.hpp"
#include "argparse.hpp"

//******************************************************
// SAMC
//******************************************************

//=== member functions ====

void SAMC::init(Config& config){
    gen_=std::mt19937(rd_());
    dist_=std::uniform_int_distribution<>(0,config.ep().size()-1);
}

void SAMC::step(Config& config){
    Graph& graph=config.graph();
    //randomly choose endpoint
    const int iA=dist_(gen_);
    const int A=config.ep()[iA];
    //generate neighbors
    std::vector<int> nnA=graph.neighbor(A);
    //shuffle neighbors
    std::shuffle(nnA.begin(),nnA.end(),gen_);
    //choose non-bonded neighbor of A
    int B=-1;
    for(int i=0; i<nnA.size(); ++i){
        B=nnA[i];
        if(!graph.edge_f(A,B)) break;
    }
    //choose bonded neighbor of B
    std::vector<int> nnB(graph.nedges(B));
    for(int i=0; i<graph.nedges(B); ++i){
        nnB[i]=graph.edge(B,i);
    }
    std::shuffle(nnB.begin(),nnB.end(),gen_);
    const int C=nnB[0];
    //ensure C is not an endpoint
    if(graph.nedges(C)>1){
        //accept move
        ++naccept_;
        //break B-C bond
        graph.edge_r(B,C);
        //add A-B bond
        graph.edge_a(A,B);
        //replace A with C in list of endpoints
        config.ep()[iA]=C;
    } else {
        //reject move
        ++nreject_;
    }
    //update step
    ++nstep_;
}

//********************************************************************
// Arg Parsing
//********************************************************************

struct MyArgs : public argparse::Args {
    int &nStep = kwarg("n,nstep", "Number of Monte Carlo steps.").set_default(0);
    int &nChain = kwarg("c,nchain", "Number of chains.").set_default(0);
    int &nWrite = kwarg("w,nwrite", "Stride for writing data files.").set_default(0);
    int &nPrint = kwarg("p,nprint", "Stride for writing data files.").set_default(0);
    int &nEdgeCorr = kwarg("r,ncorr", "Stride for writing data files.").set_default(0);
    int &nX = kwarg("x,nx", "Number of grid points in X direction.").set_default(0);
    int &nY = kwarg("y,ny", "Number of grid points in Y direction.").set_default(0);
    int &nZ = kwarg("z,nz", "Number of grid points in Z direction.").set_default(0);
	std::string &sFileOut = kwarg("o,out", "Output file.").set_default("NONE");
	bool &verbose  = flag("v,verbose", "A flag to toggle verbose").set_default(false);
};

//******************************************************
// Main
//******************************************************

int main(int argc, char* argv[]){
    //config
        Config config;
        Config config_init;
    //samc
        SAMC samc;
    //file i/o
        FILE* writer=NULL;
    //misc
        bool error=false;
    
    try{
		//check the number of arguments
		if(argc==1) throw std::invalid_argument("No arguments provided.");
		
		//read args
		std::cout<<"reading args\n";
		auto args = argparse::parse<MyArgs>(argc, argv);
		if (args.verbose) args.print();
        const int dim=3;
        const int nStep=args.nStep;
        const int nChain=args.nChain;
        const int nWrite=args.nWrite;
        const int nPrint=args.nPrint;
        const int nEdgeCorr=args.nEdgeCorr;
        const int nX=args.nX;
        const int nY=args.nY;
        const int nZ=args.nZ;
        std::string fileOut=args.sFileOut;

        //check the arguments
        std::cout<<"checking args\n";
        if(nStep<=0) throw std::invalid_argument("Invalid number of steps.");
        if(nPrint<0) throw std::invalid_argument("Invalid print stride.");
        if(nWrite<0) throw std::invalid_argument("Invalid write stride.");
        if(nEdgeCorr<0) throw std::invalid_argument("Invalid correlation stride.");
        if(nX<=1) throw std::invalid_argument("Invalid number of X points.");
        if(nY<=1) throw std::invalid_argument("Invalid number of Y points.");
        if(nZ<=1) throw std::invalid_argument("Invalid number of Z points.");
        if(nChain<1 || nChain>(nX*nY*nZ-1)/2) throw std::invalid_argument("Invalid number of chains.");

        //make the configuration
        std::cout<<"making config\n";
        config.graph().resize(nX,nY,nZ);
        Graph::fill_snake_broken(config.graph(),nChain);
        std::vector<int> degree(config.graph().size());
        Graph::degree(config.graph(),degree);
        for(int i=0; i<degree.size(); ++i){
            if(degree[i]==1) config.ep().push_back(i);
        }
        std::cout<<"graph = "<<config.graph()<<"\n";
        std::cout<<"nEP = "<<config.ep().size()<<"\n";
        config_init=config;
        
        //correlation
        std::vector<double> corr;
        if(nEdgeCorr>0){
            corr.resize(nStep/nEdgeCorr,0);
        }

        //init SAMC
        std::cout<<"initializing SAMC\n";
        samc.init(config);

        //run SAMC
        std::cout<<"running SAMC\n";
        int nCorr=0;
        for(int n=0; n<nStep; ++n){
            samc.step(config);
            if(nPrint>0 && n%nPrint==0){
                std::cout<<samc.nstep()<<" "<<samc.naccept()<<" "<<samc.nreject()<<"\n";
            }
            if(nWrite>0 && n%nWrite==0){
                std::string file=fileOut+"_n"+std::to_string(n)+".data";
                Graph::write_data(config.graph(),file.c_str());
            }
            if(nEdgeCorr>0 && n%nEdgeCorr==0){
                const int norm=dim*nX*nY*nZ-nChain;
                //const int nEdgeDiff=config.graph().edge_diff(config_init.graph());
                const int nEdgeDiff=Graph::edge_diff(config.graph(),config_init.graph());
                corr[nCorr++]=(norm-1.0*nEdgeDiff)/norm;
                //std::cout<<"nEdgeDiff = "<<nEdgeDiff<<"\n";
                //std::cout<<"norm = "<<norm<<"\n";
            }
        }

        //writing correlation
		writer=fopen("corr_edge.dat","w");
        if(writer!=NULL){
            fprintf(writer,"t corr\n");
            for(int i=0; i<corr.size(); ++i){
                fprintf(writer,"%i %f\n",i*nEdgeCorr,corr[i]);
            }
        } else {
            std::cout<<"Could not open correlation file.";
        }
    }catch(std::exception& e){
        std::cout<<"ERROR in SAMC::main(int,char**):\n";
		std::cout<<e.what()<<"\n";
		error=true;
	}
	
	if(error) return 1;
	else return 0;
}