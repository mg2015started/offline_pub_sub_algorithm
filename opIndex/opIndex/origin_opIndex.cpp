#include "origin_opIndex.h"
void origin_opIndex::insert(Sub &x)
{
    int att = getMinFre(x);
    isPivot[att] = true;
    for (int i = 0; i < x.size; i++)
    {
        //cout << "insert" << i << endl;
        originConElement e;
        e.subID = x.id;
        e.att = x.constraints[i].att;
        e.val = x.constraints[i].value;

        if (x.constraints[i].op == 0)
        {
            data[att][0][e.att % act_seg].push_back(e);
            sig[att][0][e.att % act_seg][signatureHash1(e.att, e.val)] = true;
        } else
        {
            data[att][x.constraints[i].op][e.att % act_seg].push_back(e);
            sig[att][x.constraints[i].op][e.att % act_seg][signatureHash2(e.att)] = true;
        }
    }
}

void origin_opIndex::match(Pub &pub, int &matchSubs, vector<Sub>& subList, vector<double> &matchDetailPub, vector<int> &matchDetailSub)
{
    Timer t;
    initCounter(subList);
    for (int i = 0; i < pub.size; i++)
    {
        //cout << "match pair " << i << endl;
        int piv_att = pub.pairs[i].att;
        if (!isPivot[piv_att])
            continue;

        for (int j = 0; j < pub.size; j++)
        {
            int att = pub.pairs[j].att % act_seg, value = pub.pairs[j].value;

            int hashValue = signatureHash1(pub.pairs[j].att, pub.pairs[j].value);

            vector<originConElement> &data_0 = data[piv_att][0][att];
            int data_0_size = data_0.size();
            if (sig[piv_att][0][att][hashValue]) {
                for (int k=0; k<data_0_size; ++k) {

                    originConElement &ce = data_0[k];
                    if (ce.att == pub.pairs[j].att && ce.val == value) {
                        --counter[ce.subID];
                        if (counter[ce.subID] == 0) {
                            matchDetailPub.push_back(t.elapsed_nano() / 1000000.0);
                            matchDetailSub.push_back(ce.subID);
                            ++matchSubs;
                        }
                    }
                }
            }


            hashValue = signatureHash2(pub.pairs[j].att);
            vector<originConElement> &data_1 = data[piv_att][1][att];
            int data_1_size = data_1.size();
            if (sig[piv_att][1][att][hashValue]) {
                for (int k=0; k<data_1_size; ++k) {
                    originConElement &ce = data_1[k];
                    if (ce.att == pub.pairs[j].att && ce.val >= value) {
                        --counter[ce.subID];

                        if (counter[ce.subID] == 0) {
                            matchDetailPub.push_back(t.elapsed_nano() / 1000000.0);
                            matchDetailSub.push_back(ce.subID);
                            ++matchSubs;
                        }
                    }
                }
            }

            vector<originConElement> &data_2 = data[piv_att][2][att];
            int data_2_size = data_2.size();
            if (sig[piv_att][2][att][hashValue]) {
                for (int k=0; k<data_2_size; ++k) {
                    originConElement &ce = data_2[k];
                    if (ce.att == pub.pairs[j].att && ce.val <= value) {
                        --counter[ce.subID];

                        if (counter[ce.subID] == 0) {
                            matchDetailPub.push_back(t.elapsed_nano() / 1000000.0);
                            matchDetailSub.push_back(ce.subID);
                            ++matchSubs;
                        }
                    }
                }
            }


        }
    }
}


void origin_opIndex::initCounter(vector<Sub> &subList)
{
    for (int i = 0; i < subList.size(); i++)
        counter[i] = subList[i].size;
}

void origin_opIndex::initCounter(vector<IntervalSub> &subList)
{
    for (int i = 0; i < subList.size(); i++)
        counter[i] = subList[i].size << 1;
}

int origin_opIndex::getMinFre(Sub x)
{
    int tmp = x.constraints.at(0).att;
    for (int i = 1; i < x.size; i++)
        if (fre[x.constraints[i].att] < fre[tmp])
            tmp = x.constraints[i].att;
    return tmp;
}

int origin_opIndex::getMinFre(IntervalSub x)
{
    int tmp = x.constraints.at(0).att;
    for (int i = 1; i < x.size; i++)
        if (fre[x.constraints[i].att] < fre[tmp])
            tmp = x.constraints[i].att;
    return tmp;
}

void origin_opIndex::calcFrequency(vector<Sub> &subList)
{
    memset(fre, 0, sizeof(fre));
    for (int i = 0; i < subList.size(); i++)
        for (int j = 0; j < subList[i].size; j++)
            ++fre[subList[i].constraints[j].att];
}

void origin_opIndex::calcFrequency(vector<IntervalSub> &subList)
{
    memset(fre, 0, sizeof(fre));
    for (int i = 0; i < subList.size(); i++)
        for (int j = 0; j < subList[i].size; j++)
            ++fre[subList[i].constraints[j].att];
}

int origin_opIndex::signatureHash1(int att, int val)
{
    return att * val % act_sig;
}

int origin_opIndex::signatureHash2(int att)
{
    return att * att % act_sig;
}

/*
void origin_opIndex::insert(IntervalSub &x)
{
    int att = getMinFre(x);
    int seg, signi;
    isPivot[att] = true;
    for (int i = 0; i < x.size; i++)
    {
        originConElement e;
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
    }
    //size++;

}

void origin_opIndex::match(Pub &pub, int &matchSubs, vector<IntervalSub>& subList, vector<double> &matchDetailList, vector<int> &matchDetailSub, double &moditime)
{
    Timer t;
    initCounter(subList);
    for (int i = 0; i < pub.size; i++)
    {
        //cout << "match pair " << i << endl;
        int piv_att = pub.pairs[i].att;
        if (!isPivot[piv_att])
            continue;

        for (int j = 0; j < pub.size; j++)
        {
            int att = pub.pairs[j].att % act_seg, value = pub.pairs[j].value;

            int hashValue = signatureHash2(pub.pairs[j].att);
            vector<originConElement> &data_0 = data[piv_att][0][att][hashValue];

            if (sig[piv_att][0][att][hashValue]) {
                for (int k=0; k<data_0.size(); ++k) {

                    originConElement &ce = data_0[k];
                    if (ce.att == pub.pairs[j].att && ce.val <= value) {
                        --counter[ce.subID];
                        if (counter[ce.subID] == 0) {
                            matchDetailList.push_back(t.elapsed_nano() / 1000000.0);
                            matchDetailSub.push_back(ce.subID);
                            ++matchSubs;
                        }
                        //++ce.counter;
                    }
                }
                //visited_set.push_back(make_tuple(piv_att, 0, att, hashValue));
            }

            vector<originConElement> &data_1 = data[piv_att][1][att][hashValue];

            if (sig[piv_att][1][att][hashValue]) {
                for (int k=0; k<data_1.size(); ++k) {
                    originConElement &ce = data_1[k];
                    if (ce.att == pub.pairs[j].att && ce.val >= value) {
                        --counter[ce.subID];

                        if (counter[ce.subID] == 0) {
                            matchDetailList.push_back(t.elapsed_nano() / 1000000.0);
                            matchDetailSub.push_back(ce.subID);
                            ++matchSubs;
                        }
                        //++ce.counter;
                    }
                }
                //visited_set.push_back(make_tuple(piv_att, 1, att, hashValue));
            }
        }
    }

    Timer newtimer;

    moditime = newtimer.elapsed_nano()/ 1000000.0;

}
*/