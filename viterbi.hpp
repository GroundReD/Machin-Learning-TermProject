//
//  viterbi.hpp
//  hmmexam
//
//  Created by Gred on 2017. 12. 10..
//  Copyright © 2017년 Gred. All rights reserved.
//
#pragma once
#include <map>
#include <stack>
#include "numberWordHMM.hpp"
#include "phoneHMM.hpp"

using namespace std;

class Viterbi {
private:
    NumberHMM *numHMM;
    PhoneHMM *phoneHMM;
    map<string, double> *unigram;
public:
    Viterbi();
    Viterbi(NumberHMM *numHMM, PhoneHMM *phoneHMM, map<string, double> *unigram);
    stack<StateInfo> viterbiProcess(vector<vector<double>> testData);
    vector<string> word_decoding(stack<StateInfo> result);
};
