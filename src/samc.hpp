#pragma once
#ifndef SAMC_HPP
#define SAMC_HPP

// c++ libraries
#include <random>
// local
#include "config.hpp"

//******************************************************
// SAMC
//******************************************************

class SAMC{
private:
    // random
    std::random_device rd_;
    std::mt19937 gen_;
    std::uniform_int_distribution<> dist_;
    // mc
    int nstep_{0};
    int naccept_{0};
    int nreject_{0};
public:
    //==== constructors/destructors ====
    SAMC(){}
    ~SAMC(){}

    //==== access ====
    const int& nstep()const{return nstep_;}
    const int& naccept()const{return naccept_;}
    const int& nreject()const{return nreject_;}

    //=== member functions ====
    void init(Config& config);
    void step(Config& config);
};

#endif