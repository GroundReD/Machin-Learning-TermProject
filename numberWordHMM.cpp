//
//  NumberWordHMM.cpp
//  hmmexam
//
//  Created by Gred on 2017. 12. 10..
//  Copyright © 2017년 Gred. All rights reserved.
//

#include "numberWordHMM.hpp"
#include <iostream>

NumberHMM::NumberHMM() {
    this->phoneHMM = new PhoneHMM;
    this->bigram = new map <pair<string,string>, double>();
    this->stateTransProbMap = StateTransProbMap();
}

NumberHMM::NumberHMM(PhoneHMM *phoneHMM, map<pair<string,string>, double>* bigram) {
    this->phoneHMM = phoneHMM;
    this->bigram = bigram; 
    this->stateTransProbMap = StateTransProbMap();
}

void NumberHMM::constructWordHMM(PhoneHMM *phm) {
    
    map<string, vector<string>> *phoneHMMDict;
    hmmType currentModel;
    int stateSize;
    double state_in_prob = 0.0;
    double state_out_prob = 0.0;
    double pre_state_out_prob = 0.0;

    StateInfo currentState;
    StateInfo preState;
    StateInfo nextState;

    vector<StateInfo> stateInfoVec;
    map<string,StateInfo> lastPhoneMap;
    
    phoneHMMDict = this->phoneHMM->getPhoneHMMDict();
    
    
    
    // phone HMM -> word HMM
    for (map<string, vector<string>>::iterator iter = phoneHMMDict->begin(); iter != phoneHMMDict->end() ; ++iter) {
        string word = iter->first;
        vector<string> phones = iter->second;
        
        int order = 0;
        bool isFirst = true;
        
        for (string phone:phones) {
            cout<<phone<<"\n";
            currentModel = phm->getPhoneModel(phone);
            stateSize = phm->getStateSize(phone);
            
            // before state info
            if (!isFirst) {
                preState = currentState;
                string first;
                int second, third, forth;
                tie(first, second, third, forth) = preState;
                cout<< iter->first <<" : "<<first << " " << second << " " <<third <<" " <<forth <<"\n";
                pre_state_out_prob = state_out_prob;
            }
            
            state_in_prob = currentModel.tp[0][1];
            state_out_prob = currentModel.tp[3][4];
            
            int phoneIndex = phm->getPhoneIndex(phone);
            cout<<"phone Idx : "<<phoneIndex<<"\n";
            for(int i = 1 ; i<=stateSize ; ++i) {
                cout<<"\n";
                currentState = StateInfo(word, phoneIndex, i, order);
                cout<<"state : "<<i<<"\n";
                cout<<"order : "<<order<<"\n";
                // pre->current probability
                if (!isFirst) {
                    this->stateTransProbMap[preState][currentState] = pre_state_out_prob * state_in_prob;
                } else if (i == 1){
                    firstPhoneMap[word] = currentState;
                }
                
                for (int j = 1; j<=stateSize ; ++j) {
                    if (currentModel.tp[i][j] != 0) {
                        nextState = StateInfo(word, phoneIndex, j, order);
                        cout<<"state : "<<j<<"\n";
                        this->stateTransProbMap[currentState][nextState] = currentModel.tp[i][j];
                    }
                }
            }
            isFirst = false;
            if (phone.compare("sp") == 0) {
                string temp_word = get<0>(preState);
                cout<<"last phone : "<< get<1>(preState)<<"\n";
                lastPhoneMap[temp_word] = preState;
            }
            order++;
        }
    }
    
    string word;
    StateInfo lastPhone;
    StateInfo firstPhone;
    int lastPhoneIndex, firstPhoneIndex;
    
    float sil_in_prob = phm->getPhoneModel("sp").tp[0][1];
    float sil_skip_prob = phm->getPhoneModel("sp").tp[0][2];
    float sil_out_prob = phm->getPhoneModel("sp").tp[1][2];
    
    hmmType tmpModel;

    for (map<string, vector<string>>::iterator iter = phoneHMMDict->begin(); iter != phoneHMMDict->end() ; ++iter) {
        lastPhone = lastPhoneMap[iter->first];
        lastPhoneIndex = get<1>(lastPhone);
        currentModel = phones[lastPhoneIndex];
        state_out_prob = currentModel.tp[3][4];
        
        for (map<string, vector<string>>::iterator iter = phoneHMMDict->begin(); iter != phoneHMMDict->end() ; ++iter) {
            firstPhone = firstPhoneMap[iter->first];
            firstPhoneIndex = get<1>(firstPhone);
            tmpModel = phones[firstPhoneIndex];
            state_in_prob = tmpModel.tp[0][1];
            pair<string,string> key = make_pair(currentModel.name, tmpModel.name);
            if (bigram->count(key) == 0) {
                continue;
            }
            double bigram_out_prob = this->bigram->at(key);
            this->stateTransProbMap[lastPhone][firstPhone] = state_in_prob * state_out_prob * sil_skip_prob * bigram_out_prob;
            
            int phoneIndex = phm->getPhoneIndex("sp");
            if(iter->first != "<s>") {
                this->stateTransProbMap[StateInfo(word, phoneIndex, 0, get<3>(lastPhone)+1)][firstPhone] = sil_in_prob * sil_out_prob * bigram_out_prob;
            }
        }
    }
}

StateTransProbMap* NumberHMM::getStateTransProbMap() {
    return &this->stateTransProbMap;
}
map<string, StateInfo>* NumberHMM::getFirstPhone() {
    return &this->firstPhoneMap;
}

PhoneHMM* NumberHMM::getPhoneHMM() {
    return this->phoneHMM;
}


