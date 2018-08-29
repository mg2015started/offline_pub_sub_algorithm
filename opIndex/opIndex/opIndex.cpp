#include "opIndex.h"
#include <algorithm>

void opIndex::insert(Sub &x)
{
    int att = getMinFre(x);
    isPivot[att] = true;
    for (int i = 0; i < x.size; i++)
    {
        ConElement e;
        e.subID = x.id;
        e.att = x.constraints[i].att;
        e.val = x.constraints[i].value;

        if (x.constraints[i].op == 0)
        {
            data[x.size-1][att][0][e.att % act_seg].push_back(e);
            sig[x.size-1][att][0][e.att % act_seg][signatureHash1(e.att, e.val)] = true;
        } else
        {
            data[x.size-1][att][x.constraints[i].op][e.att % act_seg].push_back(e);
            sig[x.size-1][att][x.constraints[i].op][e.att % act_seg][signatureHash2(e.att)] = true;
        }
    }
}
void opIndex::match(Pub &pub, int &matchSubs, vector<Sub>& subList, vector<double> &matchDetailPub, vector<int> &matchDetailSub)
{
    Timer t;
    initCounter(subList);
    int ps = pub.size;
    int sig1[ps];
    int sig2[ps];
    for (int i=0; i<ps; ++i)
    {
        int piv_att = pub.pairs[i].att;
        sig1[piv_att] = signatureHash1(piv_att,pub.pairs[i].value);
        sig2[piv_att] = signatureHash2(piv_att);
    }
    for (int siIndex = 0; siIndex < sizeDom && siIndex < ps; ++siIndex)
    {

        //cout << "se" << siIndex << endl;
        for (int i = 0; i < pub.size; i++)
        {
            //cout << "match pair " << i << endl;
            int piv_att = pub.pairs[i].att;
            if (!isPivot[piv_att])
                continue;

            for (int j = 0; j < pub.size; j++)
            {
                int att = pub.pairs[j].att % act_seg, value = pub.pairs[j].value;
                int pa = pub.pairs[j].att;
                int hashValue = sig1[pa];

                vector<ConElement> &data_0 = data[siIndex][piv_att][0][att];
                int data_0_size = data_0.size();
                if (sig[siIndex][piv_att][0][att][hashValue]) {
                    for (int k=0; k<data_0_size; ++k) {
                        ConElement &ce = data_0[k];
                        if (ce.att == pub.pairs[j].att && ce.val == value) {
                            --counter[ce.subID];
                            if (counter[ce.subID] == 0) {
                                //matchDetailPub.push_back(t.elapsed_nano() / 1000000.0);
                                //matchDetailSub.push_back(ce.subID);
                                ++matchSubs;
                            }
                        }
                    }
                }

                hashValue = sig2[pa];
                vector<ConElement> &data_1 = data[siIndex][piv_att][1][att];
                int data_1_size = data_1.size();
                if (sig[siIndex][piv_att][1][att][hashValue]) {
                    for (int k=0; k<data_1_size; ++k) {
                        ConElement &ce = data_1[k];
                        if (ce.att == pub.pairs[j].att && ce.val >= value) {
                            --counter[ce.subID];

                            if (counter[ce.subID] == 0) {
                                //matchDetailPub.push_back(t.elapsed_nano() / 1000000.0);
                                //matchDetailSub.push_back(ce.subID);
                                ++matchSubs;
                            }
                        }
                    }
                }

                vector<ConElement> &data_2 = data[siIndex][piv_att][2][att];
                int data_2_size = data_2.size();
                if (sig[siIndex][piv_att][2][att][hashValue]) {
                    for (int k=0; k<data_2_size; ++k) {
                        ConElement &ce = data_2[k];
                        if (ce.att == pub.pairs[j].att && ce.val <= value) {
                            --counter[ce.subID];
                            if (counter[ce.subID] == 0) {
                                //matchDetailPub.push_back(t.elapsed_nano() / 1000000.0);
                                //matchDetailSub.push_back(ce.subID);
                                ++matchSubs;
                            }
                        }
                    }
                }

            }
        }
    }
}

void opIndex::initCounter(vector<Sub> &subList)
{
    for (int i = 0; i < subList.size(); i++)
        counter[i] = subList[i].size;
}

void opIndex::initCounter(vector<IntervalSub> &subList)
{
    for (int i = 0; i < subList.size(); i++)
        counter[i] = subList[i].size << 1;
}

int opIndex::getMinFre(Sub x)
{
    int tmp = x.constraints.at(0).att;
    for (int i = 1; i < x.size; i++)
        if (fre[x.constraints[i].att] < fre[tmp])
            tmp = x.constraints[i].att;
    return tmp;
}

int opIndex::getMinFre(IntervalSub x)
{
    int tmp = x.constraints.at(0).att;
    for (int i = 1; i < x.size; i++)
        if (fre[x.constraints[i].att] < fre[tmp])
            tmp = x.constraints[i].att;
    return tmp;
}

void opIndex::calcFrequency(vector<Sub> &subList)
{
    memset(fre, 0, sizeof(fre));
    for (int i = 0; i < subList.size(); i++)
        for (int j = 0; j < subList[i].size; j++)
            ++fre[subList[i].constraints[j].att];
}

void opIndex::calcFrequency(vector<IntervalSub> &subList)
{
    memset(fre, 0, sizeof(fre));
    for (int i = 0; i < subList.size(); i++)
        for (int j = 0; j < subList[i].size; j++)
            ++fre[subList[i].constraints[j].att];
    ofstream fileStream;
    fileStream.open("fre.txt");
    for (int i=0; i<MAX_ATTS; ++i)
        fileStream << i << '\t' << fre[i] << '\n';
    fileStream.close();
}

int opIndex::signatureHash1(int att, int val)
{
    return att * val % act_sig;
}

int opIndex::signatureHash2(int att)
{
    return att * att % act_sig;
}

/*
void opIndex::insert(IntervalSub &x)
{
    int att = getMinFre(x);
    int seg, signi;
    isPivot[att] = true;
    for (int i = 0; i < x.size; i++)
    {
        ConElement e;
        e.subID = x.id;
        e.att = x.constraints[i].att;
        e.val = x.constraints[i].lowValue;
        seg = e.att % act_seg;
        signi = signatureHash2(e.att);
        data[att][0][seg][signi].push_back(e);
        sig[att][0][seg][signi] = true;
        e.val = x.constraints[i].highValue;
        data[att][1][seg][signi].push_back(e);
        sig[att][1][seg][signi] = true;
        conIndex index1 = {att, 0, seg, signi, int(data[att][0][seg][signi].size())-1};
        subPosition[x.id].push_back(index1);
        conIndex index2 = {att, 1, seg, signi, int(data[att][1][seg][signi].size())-1};
        subPosition[x.id].push_back(index2);
    }
    //size++;
    attSize[att]++;
}

void opIndex::match(Pub &pub, int &matchSubs, vector<IntervalSub>& subList, vector<double> &matchDetailList, vector<int> &matchDetailSub, double &moditime, bool beforefirst, bool needModify, double &firsttime)
{
    Timer t;
    initCounter(subList);
    //Timer ftimer;
    //if (!beforefirst) first_level_opIndex.match(pub, matchSubs, subList, matchDetailList, matchDetailSub, moditime, counter, matchTimes, t);
    //first_time = ftimer.elapsed_nano() / 1000000.0;
    if (beforefirst)
    {
        for (int i = 0; i < pub.size; i++)
        {
            int piv_att = pub.pairs[i].att;
            if (!isPivot[piv_att])
                continue;
            for (int j = 0; j < pub.size; j++)
            {
                int att = pub.pairs[j].att % act_seg, value = pub.pairs[j].value;

                int hashValue = signatureHash2(pub.pairs[j].att);
                vector<ConElement> &data_0 = data[piv_att][0][att][hashValue];

                if (sig[piv_att][0][att][hashValue]) {
                    for (int k=0; k<data_0.size(); ++k) {
                        ConElement &ce = data_0[k];
                        if (ce.att == pub.pairs[j].att && ce.val <= value) {
                            --counter[ce.subID];
                            if (counter[ce.subID] == 0) {
                                matchDetailList.push_back(t.elapsed_nano() / 1000000.0);
                                matchDetailSub.push_back(ce.subID);
                                ++matchSubs;
                                ++matchTimes[ce.subID];
                            }
                        }
                    }
                }

                vector<ConElement> &data_1 = data[piv_att][1][att][hashValue];

                if (sig[piv_att][1][att][hashValue]) {
                    for (int k=0; k<data_1.size(); ++k) {
                        ConElement &ce = data_1[k];
                        if (ce.att == pub.pairs[j].att && ce.val >= value) {
                            --counter[ce.subID];

                            if (counter[ce.subID] == 0) {
                                matchDetailList.push_back(t.elapsed_nano() / 1000000.0);
                                matchDetailSub.push_back(ce.subID);
                                ++matchSubs;
                                ++matchTimes[ce.subID];
                            }
                        }
                    }
                }
            }
        }
    }
    else{
        Timer ftimer;
        vector<att_times> Attorder;
        getOrder(pub, Attorder);
        firsttime = ftimer.elapsed_nano() / 1000000.0;
        for (int i = 0; i < pub.size; i++)
        {
            //cout << "match pair " << i << endl;

            int piv_att = Attorder[i].att;
            if (!isPivot[piv_att])
                continue;
            for (int j = 0; j < pub.size; j++)
            {
                //vector<seg_times> segOrder;
                //getOrder(pub, piv_att, segOrder);
                //int att = segOrder[j].att % act_seg, value = segOrder[j].value;
                //int hashValue = signatureHash2(segOrder[j].att);
                int att = pub.pairs[j].att % act_seg, value =  pub.pairs[j].value;
                int hashValue = signatureHash2(pub.pairs[j].att);
                vector<ConElement> &data_0 = data[piv_att][0][att][hashValue];
                if (sig[piv_att][0][att][hashValue]) {
                    for (int k=0; k<data_0.size(); ++k) {
                        ConElement &ce = data_0[k];
                        if (ce.att == pub.pairs[j].att && ce.val <= value) {
                            --counter[ce.subID];
                            if (counter[ce.subID] == 0) {
                                matchDetailList.push_back(t.elapsed_nano() / 1000000.0);
                                matchDetailSub.push_back(ce.subID);
                                ++matchSubs;
                                ++matchTimes[ce.subID];
                            }
                        }
                    }
                }

                vector<ConElement> &data_1 = data[piv_att][1][att][hashValue];
                if (sig[piv_att][1][att][hashValue]) {
                    for (int k=0; k<data_1.size(); ++k) {
                        ConElement &ce = data_1[k];
                        if (ce.att == pub.pairs[j].att && ce.val >= value) {
                            --counter[ce.subID];
                            if (counter[ce.subID] == 0) {
                                matchDetailList.push_back(t.elapsed_nano() / 1000000.0);
                                matchDetailSub.push_back(ce.subID);
                                ++matchSubs;
                                ++matchTimes[ce.subID];
                            }
                        }
                    }
                }
            }
        }
    }

    //second_time = stimer.elapsed_nano() / 1000000.0;
    Timer newtimer;

    if (needModify)
    {
        memset(attCounter, 0, sizeof(attCounter));
        memset(matchratio, 0 ,sizeof(matchratio));
        memset(segCounter, 0 ,sizeof(segCounter));
        for (int i=0; i<subList.size(); ++i)
        {
            if (matchTimes[i]>0)
            {
                vector<conIndex> &pos = subPosition[i];
                int x = pos[0].att;
                attCounter[x]+=matchTimes[i];
                for (int j=0; j<pos.size(); ++j){
                    segCounter[x][pos[j].seg]+=matchTimes[i];
                }
            }
        }
        for (int i=0; i<MAX_ATTS; ++i)
        {
            if (isPivot[i])
            {
                matchratio[i] = double(attCounter[i])/200/attSize[i];
            }
        }
    }

    moditime = newtimer.elapsed_nano() / 1000000.0;
}

 void opIndex::writeCounter() {
    ofstream fileStream;
    fileStream.open("newopblocksize.txt");
    list<ConElement>::iterator Con_it;
    for (size_t i=0; i<MAX_ATTS; ++i) {
        if (isPivot[i]) {
            for (size_t j = 0; j < 2; ++j)
            {
                fileStream << "atts" << i << "\t" << "op" << j << '\t';
                for (size_t k = 0; k < act_seg; ++k)
                    for (size_t t = 0; t < act_sig; ++t)
                        if (sig[i][j][k][t]){
                            fileStream << data[i][j][k][t].size() << "\t";
                        }
                fileStream << endl;
            }
            fileStream << "atts" << i << "total: " << attSize[i] << "\t" << "matchnum: " << double(attCounter[i])/200 << "\t" << "matchratio: " << matchratio[i] << endl;
        }
    }
    fileStream.close();
}

void opIndex::writesegCounter() {
    ofstream fileStream;
    fileStream.open("segmentsize.txt");
    cout << "open segsize.txt" << endl;
    for (int i=0; i<MAX_ATTS; ++i)
        fileStream << i << "\t" << attCounter[i] << "\n";
    for (int i=0; i<MAX_ATTS; ++i)
    {
        for (int j=0;j<act_seg; ++j)
            fileStream << segCounter[i][j] << "\t";
        fileStream << "\n\n";
    }
    fileStream.close();
}

void opIndex::getOrder(Pub &pub, int piv_att,  vector<seg_times> &segOrder)
{
    for (int i=0; i<pub.size; ++i)
    {
        seg_times a = {pub.pairs[i].att, pub.pairs[i].value, segCounter[piv_att][pub.pairs[i].att%act_seg]};
        segOrder.push_back(a);
    }

    sort(segOrder.begin(),segOrder.end(),greater<seg_times>());
}

void opIndex::getOrder(Pub &pub,vector<att_times> &attOrder)
{
    for (int i=0; i<pub.size; ++i)
    {
        att_times a = {pub.pairs[i].att, attCounter[pub.pairs[i].att], matchratio[pub.pairs[i].att]};
        attOrder.push_back(a);
    }

    sort(attOrder.begin(),attOrder.end(),greater<att_times>());
}

void opIndex::checksomething(string filename) {
    ofstream fileStream;
    fileStream.open(filename);
    for (int i=0; i<MAX_SUBS; ++i)
        fileStream << matchTimes[i] << '\t';
    fileStream << '\n';
    fileStream.close();
}

*/