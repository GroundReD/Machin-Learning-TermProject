//
//  NumberWordHMM.cpp
//  hmmexam
//
//  Created by Gred on 2017. 12. 10..
//  Copyright © 2017년 Gred. All rights reserved.
//

#include "numberWordHMM.hpp"


RNet::RNet() {
    this->wordSet = new WordHMMSet();
    this->biGram = new map <string,map<string, double>>();
    this->transTable = TransTable();
}
RNet::RNet(WordHMMSet* wordSet, map<string, map<string, double>>* biGram) {
    this->wordSet = wordSet;
    this->biGram = biGram;
    this->transTable = TransTable();
}

void RNet::construct_network(HMMManager* hmmManager) {
    //collect all state
    vector<WordHMM>* model_list;
    string word,phone;
    vector<string> phoneSet;
    hmmType cur_model;
    double entry_prob, exit_prob;
    int total_phone,total_state;
    int is_progress = 0;
    float *tpList;
    StateKey preKey, curKey, nextKey;
    int phone_idx;
    
    model_list=this->wordSet->getWordSet();
    for (int i = 0; i < model_list->size(); i++) {
        is_progress = 0;
        word=model_list->at(i).getWord();
        phoneSet = model_list->at(i).getPhoneSet();
        total_phone= phoneSet.size();
        for (int j = 0; j < total_phone; j++) {
            phone = phoneSet[j];
            cur_model = hmmManager->get_model(phone);
            total_state= sizeof(cur_model.state) / sizeof(cur_model.state[0]);
            if (phone == "sp") total_state = 1;
            entry_prob = cur_model.tp[0][1];
            // connect pre exit - cur entry
            if(is_progress==1){
                phone_idx = hmmManager->get_phone_idx(phone);
                curKey = StateKey(word, phone_idx, 0,j);
                this->transTable[preKey][curKey] = exit_prob*entry_prob;
            }
            // connect state within model
            for (int k = 0; k < total_state; k++) {
                is_progress = 1;
                phone_idx = hmmManager->get_phone_idx(phone);
                curKey = StateKey(word, phone_idx, k,j);
                tpList = cur_model.tp[k+1];
                for (int l = 0; l < total_state; l++) {
                    if (*(tpList+(l + 1)) == 0) continue;
                    phone_idx = hmmManager->get_phone_idx(phone);
                    nextKey = StateKey(word, phone_idx, l,j);
                    this->transTable[curKey][nextKey] = *(tpList+(l+1));
                }
                // last state of phone
                if (k == total_state - 1) {
                    exit_prob = *(tpList+(total_state + 1));
                    preKey = curKey;
                }
            }
        }
    }
    //connect word
    //collect word border
    map<string,StateKey> wordLastPhone;
    StateKey firstPhone, lastPhone;
    string comp_word;
    int last_idx;
    for (int i = 0; i < model_list->size(); i++) {
        word = model_list->at(i).getWord();
        phoneSet = model_list->at(i).getPhoneSet();
        total_phone = phoneSet.size();
        phone_idx = hmmManager->get_phone_idx(phoneSet[0]);
        this->wordFirstPhone[word] = StateKey(word, phone_idx, 0,0);
        if (total_phone < 2) last_idx = 0;
        else last_idx = total_phone - 2;
        phone_idx = hmmManager->get_phone_idx(phoneSet[last_idx]);
        wordLastPhone[word]=StateKey(word, phone_idx, 2,last_idx);
        //wordLastPhone.emplace_back(StateKey(word, "sp", 0));
    }
    //connect word
    hmmType comp_model;
    double inter_prob,sp_prob,bi_prob;
    map<string, double> bi_map;
    for (int i = 0; i < model_list->size(); i++) {
        word = model_list->at(i).getWord();
        lastPhone = wordLastPhone[word];
        cur_model = phones[get<1>(lastPhone)];
        exit_prob = cur_model.tp[3][4];
        inter_prob = hmmManager->get_model("sp").tp[0][2];
        sp_prob = hmmManager->get_model("sp").tp[1][2];
        for (int j = 0; j < model_list->size(); j++) {
            comp_word= model_list->at(j).getWord();
            firstPhone = this->wordFirstPhone[comp_word];
            comp_model = phones[get<1>(firstPhone)];
            entry_prob = comp_model.tp[0][1];
            bi_map = this->biGram->at(word);
            if (bi_map.find(comp_word) == bi_map.end()) {
                continue;
            }
            else bi_prob = bi_map[comp_word];
            this->transTable[lastPhone][firstPhone] = exit_prob*inter_prob*entry_prob*bi_prob;
            phone_idx = hmmManager->get_phone_idx("sp");
            if(word!="<s>")    this->transTable[StateKey(word, phone_idx, 0,get<3>(lastPhone)+1)][firstPhone] = sp_prob*entry_prob*bi_prob;
        }
    }
}

TransTable* RNet::get_transTable() {
    return &this->transTable;
}
map<string, StateKey>* RNet::getFirstPhone() {
    return &this->wordFirstPhone;
}

WordHMMSet* RNet::getWordSet() {
    return this->wordSet;
}
