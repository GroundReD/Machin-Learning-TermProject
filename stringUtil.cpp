//
//  stringUtil.cpp
//  hmmexam
//
//  Created by Gred on 2017. 12. 10..
//  Copyright © 2017년 Gred. All rights reserved.
//

#include "stringUtil.hpp"

vector<string> stringSplit(string strTarget, string strTok)
{
    int     nCutPos;
    int     nIndex     = 0;
    vector<string> strResult;
    
    while ((nCutPos = strTarget.find_first_of(strTok)) != strTarget.npos)
    {
        if (nCutPos > 0)
        {
            strResult.emplace_back(strTarget.substr(0, nCutPos));
            //            strResult[nIndex++] = strTarget.substr(0, nCutPos);
        }
        strTarget = strTarget.substr(nCutPos+1);
    }
    
    if(strTarget.length() > 0)
    {
        strResult.emplace_back(strTarget.substr(0, nCutPos));
        //        strResult[nIndex++] = strTarget.substr(0, nCutPos);
    }
    
    return strResult;
}
