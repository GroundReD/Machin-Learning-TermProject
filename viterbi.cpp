//
//  viterbi.cpp
//  hmmexam
//
//  Created by Gred on 2017. 12. 10..
//  Copyright © 2017년 Gred. All rights reserved.
//

#include "viterbi.hpp"

Viterbi::Viterbi(){
    this->numHMM = new NumberHMM();
    this->phoneHMM = new PhoneHMM();
    this->unigram = new map<string, double>;
    
}

Viterbi::Viterbi(NumberHMM *numHMM, PhoneHMM *phoneHMM, map<string, double> *unigram) {
    this->numHMM = numHMM;
    this->phoneHMM = phoneHMM;
    this->unigram = unigram;
}

stack<StateInfo> Viterbi::viterbiProcess(vector<vector<double>> testDataVec) {
    
    StateTransProbMap transProbMap = *this->numHMM->getStateTransProbMap();
    map<StateInfo, double> stateTransProb;
    map<string, StateInfo> firstPhoneMap = *this->numHMM->getFirstPhone();
    StateInfo currentState;
    StateInfo preState;
    
    vector<map<StateInfo, double>> delta_state;
    vector<map<StateInfo, StateInfo>> psi_state;
    
    double pi_initialProb;
    double b_observeProb;
    double a_transProb;
    
    map<StateInfo,double> delta_i_PathProb;
    map<StateInfo, StateInfo> psi_i_PathNode;
    
    int i = 0;
    bool isFirst = true;
    
    StateInfo firstState = StateInfo("init", -1, 0, 0 );
    
    for(vector<double> testData:testDataVec) {
        
        // initiailize
        if (isFirst) {
            for (map<string, StateInfo>::iterator iter = firstPhoneMap.begin(); iter != firstPhoneMap.end(); ++iter) {
                currentState = iter->second;
                pi_initialProb = log(this->unigram->at(iter->first));
                
                stateType state = phones[get<1>(currentState)].state[get<2>(currentState)-1];
                b_observeProb = log(phoneHMM->calculateObservationProb(&state, testData));
                
                delta_i_PathProb[currentState] = b_observeProb + pi_initialProb;
                psi_i_PathNode[currentState] = firstState;
                
            }
            isFirst = !isFirst;
        } else {
            //recursion
            map<StateInfo, double> pre_delta_i = delta_state[i-1];
            for (map<StateInfo, double>::iterator iter = pre_delta_i.begin(); iter != pre_delta_i.end(); ++iter) {
                preState = iter->first;
                double preDeltaProb = iter->second;
                
                stateTransProb = transProbMap[preState];
                for (map<StateInfo, double>::iterator iter = stateTransProb.begin(); iter != stateTransProb.end(); ++iter) {
                    currentState = iter->first;
                    //a_ij
                    a_transProb = log(iter->second);
                    stateType state = phones[get<1>(currentState)].state[get<2>(currentState)-1];
                    //b observe
                    b_observeProb = log(phoneHMM->calculateObservationProb(&state, testData));
                    double currentDeltaProb = preDeltaProb + a_transProb + b_observeProb;
                    
                    if (psi_i_PathNode.count(currentState) == 0) {
                        delta_i_PathProb[currentState] = currentDeltaProb;
                        psi_i_PathNode[currentState] = preState;
                    }
                    
                    if (delta_i_PathProb[currentState] < currentDeltaProb) {
                        //max prob and arg max
                        delta_i_PathProb[currentState] = currentDeltaProb;
                        psi_i_PathNode[currentState] = preState;
                    }
                }
            }
        }
        
        delta_state.emplace_back(delta_i_PathProb);
        psi_state.emplace_back(psi_i_PathNode);
        ++i;
    }
    
    //Temination
    int lastPos = delta_state.size()-1;
    map<StateInfo, double> delta_P = delta_state[lastPos];
    StateInfo q_lastState;
    double max_delta_prob = -numeric_limits<int>::max();
    
    for (map<StateInfo, double>::iterator iter = delta_P.begin(); iter != delta_P.end(); ++iter) {
        if (max_delta_prob < iter->second) {
            max_delta_prob = iter->second;
            q_lastState = iter ->first;
        }
    }
    
    //backTracking
    stack<StateInfo> result;
    result.push(q_lastState);
    StateInfo result_state = q_lastState;
    
    int size = psi_state.size();
    for (int i = size - 1; i >= 0; i--) {
        result_state = psi_state[i][result_state];
        result.push(result_state);
    }
    
    return result;
}

vector<string> Viterbi::word_decoding(stack<StateInfo> result) {
    vector<string> word_list;
    StateInfo resultInfo;
    string cur_word;
    PhoneHMM *wordSet = this->numHMM->getPhoneHMM();
    
    int phone_order,phone_size,result_size;
    int word_progress=0,state_num;
    word_list.reserve(20);
    
    result_size = result.size();
    
    for (int j = 0; j < result_size; j++) {
        resultInfo = result.top();
        result.pop();
        cur_word = get<0>(resultInfo);
        if (cur_word == "<s>" || cur_word == "init") continue;
        phone_order = get<3>(resultInfo);
        state_num = get<2>(resultInfo);
        
        phone_size = wordSet->getPhoneSize(cur_word);
        if (phone_order>=phone_size-2 && state_num==2) {
            if (word_progress == 0) word_list.emplace_back(cur_word);
            word_progress = 1;
        }
        else word_progress = 0;
    }
    return word_list;
}
