//
//  main.cpp
//  hmmexam
//
//  Created by Gred on 2017. 12. 5..
//  Copyright © 2017년 Gred. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "fileIO.h"
#include "hmm.h"
#include "mdictionary.h"
#include "phoneHMM.hpp"
#include "numberWordHMM.hpp"
#include "viterbi.hpp"


using namespace std;

int main(int argc, const char * argv[]) {
    
    
    FileIO fileIO;
    vector<string> testDataName = fileIO.readRef2Test("/Users/gred/Desktop/hmmexam/hmmexam/reference.txt");
    map<string, testData> testDataMap = fileIO.readTestData(testDataName);
    
    cout << testDataName.size() <<"\n";
    string tmp = testDataName.at(0);
    cout << testDataMap.size() << "\n";
    
    PhoneHMM pHMM;
    pHMM.createIndextable();
    pHMM.makePhoneDict(dictionary);
    map<string,vector<string>> *mDict = pHMM.getPhoneHMMDict();
    
    NumberHMM nHMM;
    nHMM = NumberHMM(&pHMM, &bigram);
    nHMM.constructWordHMM(&pHMM);
    
    // print testData
    for (map<string,testData>::iterator iter = testDataMap.begin(); iter != testDataMap.end() ; ++iter) {
        cout<< iter->first << "\n";
    }
    
    // print word Dictionary
    for (map<string,vector<string>>::iterator iter = mDict->begin(); iter != mDict->end() ; ++iter) {
        cout<<iter->first<< " : ";
        for(string v:iter->second) {
            cout<< v << " ";
        }
        cout<<endl;
    }
    
    // print Firts Phone
    for (map<string,string>::iterator iter = dictionary.begin(); iter != dictionary.end() ; ++iter) {
        string first;
        int second, third, forth;
        tie(first, second, third, forth) = nHMM.getFirstPhone()->at(iter->first);
        cout<< iter->first <<" : "<<first << " " << second << " " <<third <<" " <<forth <<"\n";
    }
    
    Viterbi recognizer;
    recognizer = Viterbi(&nHMM, &pHMM, &unigram);
    
    stack<StateInfo> result;
    vector<string> word_result;
    
    testData cur_data;
    int count = 0;
    for (map<string, testData>::iterator node = testDataMap.begin(); node != testDataMap.end(); node++) {
        count++;
        cur_data = node->second;
        result = recognizer.viterbiProcess(cur_data);
        word_result=recognizer.word_decoding(result);
        fileIO.add_result(node->first, word_result);
        
        cout<<count<<"\n";
        if (count % 100 == 0) cout << count<<" repeat"<< endl;
    }
    
    fileIO.write_to_file("/Users/gred/Desktop/hmmexam/hmmexam/tst/result.txt");
    
//
//    hmmType temp = phones[0];
//
//    cout<< temp.name << "\n";
//
//    for (map<string,string>::iterator iter = dictionary.begin(); iter != dictionary.end() ; ++iter) {
//        cout<< iter->first <<" "<< iter->second << "\n";
//    }
//
//    for (map<pair<string,string>,double>::iterator iter = bigram.begin(); iter != bigram.end() ; ++iter) {
//        if (iter->first.first == "zero" || iter->first.second == "<s>"){
//            cout<< iter->first.first <<" "<< iter->first.second <<" "<< iter->second<<"\n";
//        }
//    }
//
//    print_hmm(temp);
    
    return 0;
}
//
//void print_hmm(hmmType h) {
//    cout<< h.name << "\n";
//    for (int i = 0 ; i<5 ; i++) {
//        for (int j = 0 ; j < 5 ; i++ ) {
//            cout<<h.tp[i][j]<< " ";
//        }
//        cout<<"\n";
//    }
//}

