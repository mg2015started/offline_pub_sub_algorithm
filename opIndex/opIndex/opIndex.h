#ifndef _OPINDEX_H
#define _OPINDEX_H
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

const int MAX_ATTS = 101;
const int MAX_SUBS = 1000001;
const int SEGMENTS = 100;
const int MAX_SIGNATURE = 2000;
const int MAX_CONS = 20;
class opIndex {
    vector<ConElement> data[MAX_CONS][MAX_ATTS][3][SEGMENTS];
    bool sig[MAX_CONS][MAX_ATTS][3][SEGMENTS][MAX_SIGNATURE];
    int counter[MAX_SUBS];
    int matchTimes[MAX_SUBS];
    bool isPivot[MAX_ATTS];
    int fre[MAX_ATTS];
    int act_seg;
    int act_sig;
    //vector<conIndex> subPosition[MAX_SUBS];

    void initCounter(vector<Sub> &subList);
    void initCounter(vector<Pub> &pubList);

    void initCounter(vector<IntervalSub> &subList);

    int getMinFre(Sub x);

    int getMinFre(IntervalSub x);

    int signatureHash1(int att, int val);       // for == operation
    int signatureHash2(int att);                // for <= and >= operation

    int sizeDom;
public:

    opIndex(int k, int segs, int sigs)
    {
        sizeDom = k;
        act_seg = segs;
        act_sig = sigs;
        memset(isPivot, 0, sizeof(isPivot));
        memset(sig, 0, sizeof(sig));
    }

    void calcFrequency(vector<Sub> &subList);

    void calcFrequency(vector<IntervalSub> &subList);

    void insert(Sub &x);

    void getOrder(Pub &pub, vector<att_times> &attOrder);
    void getOrder(Pub &pub, int piv_att, vector<seg_times> &segOrder);

    void insert(IntervalSub &x);
    void virtual_insert(IntervalSub &x);

    void remove(IntervalSub &x);
    void virtual_remove(IntervalSub &x);

    void clearMatchTimes()
    {
        memset(matchTimes, 0, MAX_SUBS);
    }

    void match(Pub &pub, int &matchSubs, vector<Sub> &subList,vector<double> &matchDetailPub, vector<int> &matchDetailSub);

    void match(Pub &pub, int &matchSubs, vector<IntervalSub>& subList, vector<double> &matchDetailPub, vector<int> &matchDetailSub, double &moditime,bool beforefirst, bool needModify, double &firsttime);
    void writeCounter();

    void checksomething(string filename);
    void writesegCounter();

/*
    int getSize()
    {
        return size;
    }
    int getmovetimes()
    {
        return movetimes;
    }
    */
};

#endif
