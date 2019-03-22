//
//  NumberWordHMM.hpp
//  hmmexam
//
//  Created by Gred on 2017. 12. 10..
//  Copyright © 2017년 Gred. All rights reserved.
//

#pragma once
#include <map>
#include "phoneHMM.hpp"

using namespace std;

typedef tuple<string, int, int, int> StateInfo; //word, phone, state, order
typedef map<StateInfo, map<StateInfo, double>> StateTransProbMap; // transition probability between phone, word

class NumberHMM {
private:
    PhoneHMM * phoneHMM;
    map<pair<string,string>, double>* bigram;
    StateTransProbMap stateTransProbMap;
    map<string, StateInfo> firstPhoneMap;
    
public:
    NumberHMM();
    NumberHMM(PhoneHMM* phoneHMM, map<pair<string,string>, double> *bigram);
    void constructWordHMM(PhoneHMM * phoneHMM);
    ::StateTransProbMap* getStateTransProbMap();
    map<string, StateInfo>* getFirstPhone();
    PhoneHMM* getPhoneHMM();
};

