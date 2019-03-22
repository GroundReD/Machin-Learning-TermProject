//
//  fileIO.hpp
//  hmmexam
//
//  Created by Gred on 2017. 12. 9..
//  Copyright © 2017년 Gred. All rights reserved.
//
#pragma once
#include <stdio.h>
#include <map>
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include "stringUtil.hpp"

using namespace std;

typedef vector<vector<double>> testData;

class FileIO{
private:
    map<string, vector<string>> resultSet;
public:
    FileIO();
    vector<string> readRef2Test(string filePath);
    map<string, testData> readTestData(vector<string> testFileList);
    void add_result(string filename, vector<string> word_result);
    void write_to_file(string filepath);
};
