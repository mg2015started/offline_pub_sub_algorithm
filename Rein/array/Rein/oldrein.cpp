#include "oldrein.h"
/*
void oldRein::insert(Sub sub)
{
    for (int i = 0; i < sub.size; i++)
    {
        Cnt cnt = sub.constraints[i];
        Combo c;
        c.val = cnt.value;
        c.subID = sub.id;
        if (cnt.op == 0)        // ==
        {
            data[cnt.att][0][c.val / buckStep].push_back(c);
            data[cnt.att][1][c.val / buckStep].push_back(c);
        }
        else if (cnt.op == 1)   // >=
            data[cnt.att][0][c.val / buckStep].push_back(c);
        else                    // >=
            data[cnt.att][1][c.val / buckStep].push_back(c);
    }
}
 */

void oldRein::insert(IntervalSub &sub)
{
    for (int i = 0; i < sub.size; i++)
    {
        IntervalCnt cnt = sub.constraints[i];
        Combo c;
        c.val = cnt.lowValue;
        c.subID = sub.id;
        data[cnt.att][0][c.val / buckStep].push_back(c);
        c.val = cnt.highValue;
        data[cnt.att][1][c.val / buckStep].push_back(c);
    }
}
/*
void oldRein::match(const Pub &pub, int &matchSubs, const vector<Sub> &subList)
{
    vector<bool> bits (subList.size(), false);

    for (int i = 0; i < pub.size; i++)
    {
        int value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
        for (int k = 0; k < data[att][0][buck].size(); k++)
            if (data[att][0][buck][k].val > value)
                bits[data[att][0][buck][k].subID] = true;
        for (int j = buck + 1; j < bucks; j++)
            for (int k = 0; k < data[att][0][j].size(); k++)
                bits[data[att][0][j][k].subID] = true;

        for (int k = 0; k < data[att][1][buck].size(); k++)
            if (data[att][1][buck][k].val < value)
                bits[data[att][1][buck][k].subID] = true;
        for (int j = buck - 1; j >= 0; j--)
            for (int k = 0; k < data[att][1][j].size(); k++)
                bits[data[att][1][j][k].subID] = true;
    }

    for (int i = 0; i < subList.size(); i++)
        if (!bits[i])
            ++ matchSubs;
}
*/
void oldRein::match(const Pub &pub, int &matchSubs, const vector<IntervalSub> &subList , vector<double> &matchDetailPub, vector<int> &matchDetailSub)
{
    /*
    ofstream f0;
    ofstream f1;
    ofstream f2;
    ofstream f3;
    f0.open("./Detail/pretime0.txt",ios::app);
    f1.open("./Detail/searchtime0.txt",ios::app);
    f2.open("./Detail/traversetime0.txt",ios::app);
    f3.open("./Detail/tagtime0.txt",ios::app);
    double searchtime = 0;
    double travesetime = 0;
    double tagtime = 0;
    double pretime = 0;
     */
    Timer t;
    bool bits[subList.size()];
    memset(bits,0,sizeof(bits));

    for (int i = 0; i < pub.size; i++)
    {
        //Timer t0;
        int value = pub.pairs[i].value, att = pub.pairs[i].att, buck = value / buckStep;
        vector<Combo> &data_0 = data[att][0][buck];
        //pretime += t0.elapsed_nano()/1000000.0;

        //Timer t1;
        int data_0_size = data_0.size();
        for (int k = 0; k < data_0_size; k++)
            if (data_0[k].val > value)
                bits[data_0[k].subID] = true;
        //searchtime += t1.elapsed_nano()/1000000.0;

        //Timer t2;
        for (int j = buck + 1; j < bucks; j++){
            vector<Combo> &data_1 = data[att][0][j];
            int data_1_size = data_1.size();
            for (int k = 0; k < data_1_size; k++)
                bits[data_1[k].subID] = true;
        }
        //travesetime += t2.elapsed_nano()/1000000.0;

        //Timer t3;
        vector<Combo> &data_2 = data[att][1][buck];
        int data_2_size = data_2.size();
        for (int k = 0; k < data_2_size; k++)
            if (data_2[k].val < value)
                bits[data_2[k].subID] = true;
        //searchtime += t3.elapsed_nano()/1000000.0;

        //Timer t4;
        for (int j = buck - 1; j >= 0; j--)
        {
            vector<Combo> &data_3 = data[att][1][j];
            int data_3_size = data_3.size();
            for (int k = 0; k < data_3_size; k++)
                bits[data_3[k].subID] = true;
        }
        //travesetime += t4.elapsed_nano()/1000000.0;

    }

    //Timer t5;
    int b = subList.size();
    for (int i = 0; i < b; i++)
        if (!bits[i])
        {
            //matchDetailPub.push_back(t.elapsed_nano() / 1000000.0);
            //matchDetailSub.push_back(i);
            ++matchSubs;
        }

    //tagtime += t5.elapsed_nano()/1000000.0;
/*
    f0 << pretime << "\n";
    f1 << searchtime << "\n";
    f2 << travesetime << "\n";
    f3 << tagtime << "\n";
    f0.close();
    f1.close();
    f2.close();
*/
}
