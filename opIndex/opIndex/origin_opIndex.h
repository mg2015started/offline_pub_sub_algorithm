#ifndef _origin_opIndex_H
#define _origin_opIndex_H
#include <ctime>
#include "data_structure.h"
#include "generator.h"
#include "printer.h"
#include "chrono_time.h"
#include <string>
#include <cstring>
#include <list>
#include <set>
#include <tuple>
const int MAX_ATTS_origin = 101;
const int MAX_SUBS_origin = 1000001;
const int SEGMENTS_origin = 100;
const int MAX_SIGNATURE_origin = 2000;

class origin_opIndex {
    vector<originConElement> data[MAX_ATTS_origin][3][SEGMENTS_origin];
    bool sig[MAX_ATTS_origin][3][SEGMENTS_origin][MAX_SIGNATURE_origin];
    int counter[MAX_SUBS_origin];
    bool isPivot[MAX_ATTS_origin];
    int fre[MAX_ATTS_origin];
    int act_seg;
    int act_sig;
    void initCounter(vector<Sub> &subList);

    void initCounter(vector<IntervalSub> &subList);
    void initCounter(vector<Pub> &pubList);

    int getMinFre(Sub x);

    int getMinFre(IntervalSub x);

    int signatureHash1(int att, int val);       // for == operation
    int signatureHash2(int att);                // for <= and >= operation

public:
    origin_opIndex(int segs, int sigs)
    {
        act_seg = segs;
        act_sig = sigs;
        memset(isPivot, 0, sizeof(isPivot));
        memset(sig, 0, sizeof(sig));
    }

    void calcFrequency(vector<Sub> &subList);

    void calcFrequency(vector<IntervalSub> &subList);

    void insert(Sub &x);

    void insert(IntervalSub &x);

    void match(Pub &pub, int &matchSubs, vector<Sub> &subList, vector<double> &matchDetailPub, vector<int> &matchDetailSub);
    void match(Pub &pub, int &matchSubs, vector<IntervalSub> subList, vector<double> &matchDetailList, vector<int> &matchDetailSub, double &moditime);
    void match(Pub &pub, int &matchSubs, vector<IntervalSub>& subList, vector<double> &matchDetailList, vector<int> &matchDetailSub, double &moditime, int *matchTimes);
    void remove(IntervalSub &x);
    void writeCounter();
};

#endif
