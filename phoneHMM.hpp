//
//  numberHMM.hpp
//  hmmexam
//
//  Created by Gred on 2017. 12. 10..
//  Copyright © 2017년 Gred. All rights reserved.
//
#pragma once
#include <vector>
#include <string>
#include <map>
#include <math.h>
#include "hmm.h"

using namespace std;

class PhoneHMM{
private:
    map<string, vector<string> > phoneHMMDict;
    map<string, int> phoneSize;
    map<string, int> phoneIndexTable;
public:
    PhoneHMM();
    void makePhoneDict(map<string, string> dictionary);
    int getPhoneSize(string number) ;
    map<string, vector<string>>* getPhoneHMMDict();
    void createIndextable();
    hmmType getPhoneModel(string phone);
    int getStateSize(string phone);
    double calculateObservationProb(stateType* state, vector<double> inputSpeech);
    double getTransProb(string phone, int i, int j);
    int getPhoneIndex(string phone);
//
};

