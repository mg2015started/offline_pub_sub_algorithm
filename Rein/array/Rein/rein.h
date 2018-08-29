#ifndef _REIN_H
#define _REIN_H
#include<vector>
#include <cstring>
#include "generator.h"
#include "chrono_time.h"
#include "util.h"
#include "data_structure.h"
#include <math.h>

const int MAX_SUBS = 2000001;
const int MAX_ATTS = 101;
const int MAX_BUCKS = 1000;
const int MAX_CONS = 25;

class Rein{
    int valDom, Dom;
    vector<kCombo> data[MAX_CONS][MAX_ATTS][2][MAX_BUCKS];    // 0:left parenthesis, 1:right parenthesis
    vector<int> LvSize[MAX_CONS];
    int LvBuck[MAX_CONS];
    int LvBuckStep[MAX_CONS];
public:
    Rein(int _valDom, int k, int buck_num){
        valDom = _valDom;
        Dom =k;
        for (int i=0; i<Dom; ++i)
        {
            LvBuck[i] = int(buck_num * sqrt(i+1));
            LvBuckStep[i] = (valDom - 1) / LvBuck[i] + 1;
            LvBuck[i] = (valDom - 1) / LvBuckStep[i] + 1;
            //cout << LvBuck[i] << endl;
        }
        //buckStep = (valDom - 1) / MAX_BUCKS + 1;
        //bucks = (valDom - 1) / buckStep + 1;
    }
    void insert(Sub &sub);
    void insert(IntervalSub &sub);
    void match(const Pub &pub, int &matchSubs, const vector<Sub> &subList);
    void match(const Pub &pub, int &matchSubs, const vector<IntervalSub> &subList);
    void match(const Pub &pub, int &matchSubs, const vector<IntervalSub> &subList , vector<double> &matchDetailPub, vector<int> &matchDetailSub);
    void setBucketSize();
};

#endif
