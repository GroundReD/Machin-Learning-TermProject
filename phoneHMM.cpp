//
//  phoneHMM.cpp
//  hmmexam
//
//  Created by Gred on 2017. 12. 10..
//  Copyright © 2017년 Gred. All rights reserved.
//

#include "phoneHMM.hpp"
#include "stringUtil.hpp"

using namespace std;

PhoneHMM::PhoneHMM() {
    this->phoneHMMDict = map<string, vector<string> >();
    this->phoneIndexTable = map<string, int>();
}

void PhoneHMM::makePhoneDict(map<string, string> dict) {
    string number;
    vector<string> phonesVector;
    string phones;
    
        for (map<string,string>::iterator iter = dict.begin(); iter != dict.end() ; ++iter) {
            number = iter->first;
            phones = iter->second;
            phonesVector = stringSplit(phones, " ");
            
            this->phoneHMMDict[number] = phonesVector;
            this->phoneSize[number] = phonesVector.size();
        }
}

map<string, vector<string>> * PhoneHMM::getPhoneHMMDict() {
    return &(this->phoneHMMDict);
}

int PhoneHMM::getPhoneSize(string number) {
        return this->phoneSize[number];
}

void PhoneHMM::createIndextable() {
    this->phoneIndexTable= map<string, int>();
    string word;
    for (int i = 0; i < 21; i++) {
        word = phones[i].name;
        this->phoneIndexTable[word] = i;
    }
}

hmmType PhoneHMM::getPhoneModel(string phone) {
    int modelIndex = this->phoneIndexTable[phone];
    hmmType mModel = phones[modelIndex];
    return mModel;
}

int PhoneHMM::getStateSize(string phone) {
    int modelIndex = this->phoneIndexTable[phone];
    hmmType mModel = phones[modelIndex];
    int size= sizeof(mModel.state) / sizeof(mModel.state[0]);
    return size;
}

double PhoneHMM::calculateObservationProb(stateType* state, vector<double> inputSpeech) {

    double result;
    double variance;
    double numer;
    const double ConstValue = 1.0 / sqrt(2 * 3.14);
    result = 0;

    for (pdfType pt:state->pdf) {
        numer = 0;
        variance = 0;

        for (int v = 0; v < 39; v++) {
            numer+=pow(inputSpeech[v] - pt.mean[v], 2);
            variance += pt.var[v];
        }
        result += pt.weight * (ConstValue / sqrt(variance) * exp(numer / (-2 * variance)));
    }
    return result;
}

double PhoneHMM::getTransProb(string phone, int i, int j) {
    hmmType currentModel = this->getPhoneModel(phone);
    return currentModel.tp[i + 1][j + 1];
}
int PhoneHMM::getPhoneIndex(string phone) {
    return this->phoneIndexTable[phone];
}



