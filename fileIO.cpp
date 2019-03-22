//
//  fileIO.cpp
//  hmmexam
//
//  Created by Gred on 2017. 12. 9..
//  Copyright © 2017년 Gred. All rights reserved.
//

#include "fileIO.h"

using namespace std;
FileIO::FileIO() {
    this->resultSet = map<string, vector<string>>();
}
vector<string> FileIO::readRef2Test(string filePath) {
    vector<string> fileList;
    string line;
    int index;
    ifstream openFile(filePath);
    
    if(openFile.is_open()) {
        while(getline(openFile, line)) {
            if (line == ".")
                continue;
            else {
                index = line.find_last_of(".lab");
                if(index != string::npos) {
                    line = line.substr(1, index-4);
                    line += ".txt";
                    fileList.emplace_back(line);
                }
            }
        }
        openFile.close();
    }
    return fileList;
}

map<string, testData> FileIO::readTestData(vector<string> testFileList) {
    map<string, testData> testDataMap;
    vector<string> tmp;
    vector<double> tmp_double;
    string line;
    int dataSize;
    int dataDimension;
    
    for(string fileName:testFileList) {
        ifstream openFile("/Users/gred/Desktop/hmmexam/hmmexam/"+fileName);
        if(openFile.is_open()) {
            //get first line (ex:389 39)
            getline(openFile, line);
            tmp = stringSplit(line, " ");
            dataSize = stoi(tmp[0]);
            dataDimension = stoi(tmp[1]);
            
            tmp.clear();
            
            //get double data
            testData tmpData;
            tmpData.reserve(dataSize);
            while(getline(openFile, line)){
//                s(line, tmp, ' ');
                tmp = stringSplit(line, " ");
                vector<double> cur_obs;
                for (int j = 0; j < 39; j++) {
                    cur_obs.emplace_back(stod(tmp[j]));
                }
                tmpData.emplace_back(cur_obs);
            }
            testDataMap[fileName] = tmpData;
            openFile.close();
        }
    }
    return testDataMap;
}

void FileIO::add_result(string filename, vector<string> word_result) {
    int end_idx = filename.find_last_of(".txt");
    if (end_idx != string::npos) {
        filename = filename.substr(0, end_idx - 3);
        filename += ".rec";
    }
    this->resultSet[filename] = word_result;
}
void FileIO::write_to_file(string filepath) {
    ofstream outFile = ofstream(filepath);
    outFile << "#!MLF!#" << endl;;
    map<string, vector<string>>::iterator result;
    for (result = this->resultSet.begin(); result != this->resultSet.end(); result++) {
        outFile << "\"" << result->first << "\"" << endl;
        for (int w = 0; w < result->second.size(); w++) {
            outFile << result->second[w] << endl;
        }
        outFile << "." << endl;
    }
    
    outFile.close();
}


