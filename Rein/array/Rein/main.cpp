#include <iostream>
#include "rein.h"
#include "oldrein.h"
using namespace std;
int main(int argc, char **argv)
{
    int subs;           // Number of subscriptions.
    int pubs;           // Number of publications.
    int attDom;           // Total number of attributes, i.e. dimensions.
    int k;           // Number of constraints(predicates) in one sub.
    int m;              // Number of constraints in one pub.
    int valDom;         // Cardinality of values.
    int subAttDis;         // The distribution of attributes in subs and pubs. 0:uniform distribution | 1:Zipf distribution
    int subValDis;         // The distribution of values in subs and pubs. 0:uniform
    int pubAttDis;         // The distribution of attributes in subs and pubs. 0:uniform distribution | 1:Zipf distribution
    int pubValDis;         // The distribution of values in subs and pubs. 0:uniform
    double subAttalpha;       // Parameter for Zipf distribution.
    double pubAttalpha;
    double subValalpha;
    double pubValalpha;
    double width;       // Width of a predicate.
    double equalRatio;
    int old_buck_num;
    int new_buck_num;

    ifstream parafile;
    parafile.open(string("./paras/paras_")+string(argv[1])+string(".txt"));
    parafile >> subs >> pubs >> k >> m >> subAttDis >> subValDis >> pubAttDis >> pubValDis >> attDom >> valDom >> subAttalpha >>subValalpha >>  pubAttalpha >> pubValalpha >> width >> equalRatio >> old_buck_num >> new_buck_num;
    parafile.close();

    //m = atts;           // Note that Rein requires m == atts.
    vector<double> insertTimeList;
    vector<double> matchTimeList;
    vector<double> matchSubList;
    vector<int> matchDetailSub;
    vector<double> matchDetailPub;
    vector<double> totalDetail;
    vector<double> old_subMatchtime[subs];
    vector<double> new_subMatchtime[subs];
    vector<double> old_pubMatchtime[pubs];
    vector<double> new_pubMatchtime[pubs];

    // Initiate generator
    intervalGenerator gen(subs, pubs, k, m, subAttDis, subValDis, pubAttDis, pubValDis, attDom, valDom, subAttalpha ,subValalpha ,pubAttalpha, pubValalpha, width, equalRatio);
    gen.GenSubList();
    gen.GenPubList();
    cout << "gen datalist finished" << endl;

    oldRein old_Rein(valDom, old_buck_num);
    Rein new_Rein(valDom,k, new_buck_num);

    ofstream fileStream;

/*
    for (int i=0; i<pubs; ++i)
    {
        fileStream.open(string("../oldpubDetail/") + Util::Int2String(i+1) + string(".txt"));
        fileStream.close();
        fileStream.open(string("../newpubDetail/") + Util::Int2String(i+1) + string(".txt"));
        fileStream.close();
    }
    for (int i=0; i<subs; ++i)
    {
        fileStream.open(string("../oldsubDetail/") + Util::Int2String(i+1) + string(".txt"));
        fileStream.close();
        fileStream.open(string("../newsubDetail/") + Util::Int2String(i+1) + string(".txt"));
        fileStream.close();
    }
*/
    fileStream.open("./Detail/pretime0.txt");fileStream.close();
    fileStream.open("./Detail/searchtime0.txt");fileStream.close();
    fileStream.open("./Detail/traversetime0.txt");fileStream.close();
    fileStream.open("./Detail/tagtime0.txt");fileStream.close();
    fileStream.open("./Detail/pretime1.txt");fileStream.close();
    fileStream.open("./Detail/searchtime1.txt");fileStream.close();
    fileStream.open("./Detail/traversetime1.txt");fileStream.close();
    fileStream.open("./Detail/tagtime1.txt");fileStream.close();

    // insert
    for (int i = 0; i < subs; i++)
    {
        Timer subStart;

        old_Rein.insert(gen.subList[i]);                       // Insert sub[i] into data structure.

        int64_t insertTime = subStart.elapsed_nano();   // Record inserting time in nanosecond.
        insertTimeList.push_back((double) insertTime / 1000000);
    }



    // match
    for (int i = 0; i < pubs; i++)
    {
        if (!((i+1)%100)) cout << "matching " << i+1 << "..." << endl;
        int matchSubs = 0;                              // Record the number of matched subscriptions.
        matchDetailPub.clear();
        matchDetailSub.clear();
        Timer matchStart;

        old_Rein.match(gen.pubList[i], matchSubs, gen.subList, matchDetailPub, matchDetailSub);

        int64_t eventTime = matchStart.elapsed_nano();  // Record matching time in nanosecond.
        matchTimeList.push_back((double) eventTime / 1000000);
        matchSubList.push_back(matchSubs);
        /*
         old_pubMatchtime[i].insert(old_pubMatchtime[i].end(), matchDetailPub.begin(), matchDetailPub.end());
         for(int j=0; j<matchDetailSub.size(); ++j)
             old_subMatchtime[matchDetailSub[j]].push_back(matchDetailPub[j]);
         if (!((i+1)%3000))
         {
             Util::storeOldResult(old_pubMatchtime, pubs, old_subMatchtime, subs);
         }
          */
    }

    cout << "matching old_Rein finished" << endl;


    // output
    string outputFileName = "./Detail/old_rein.txt";
    string content = Util::Int2String(subs) + "\t" + Util::Double2String(Util::Average(insertTimeList)) + "\t" +
                     Util::Double2String(Util::Average(matchTimeList)) + "\t" +
                     Util::Double2String(Util::Average(matchSubList));
    Util::WriteData(outputFileName.c_str(), content);



    fileStream.open(string("./totaldata/totaldata_")+string(argv[1])+string(".txt"));
    fileStream << content << endl;
    fileStream.close();

    //Util::writeOldtp(pubs,subs,string("./totaldata/totaldata_")+string(argv[1])+string(".txt"));


    insertTimeList.clear();
    matchTimeList.clear();
    matchSubList.clear();

    matchDetailPub.clear();
    matchDetailSub.clear();
    totalDetail.clear();

    // insert
    for (int i = 0; i < subs; i++)
    {
        Timer subStart;

        new_Rein.insert(gen.subList[i]);                       // Insert sub[i] into data structure.

        int64_t insertTime = subStart.elapsed_nano();   // Record inserting time in nanosecond.
        insertTimeList.push_back((double) insertTime / 1000000);
    }

    cout << "insert new_Rein finished" << endl;


    // match
    for (int i = 0; i < pubs; i++)
    {
        if (!((i+1)%100)) cout << "matching " << i+1 << "..." << endl;
        int matchSubs = 0;                              // Record the number of matched subscriptions.
        matchDetailPub.clear();
        matchDetailSub.clear();
        Timer matchStart;

        new_Rein.match(gen.pubList[i], matchSubs, gen.subList, matchDetailPub, matchDetailSub);

        int64_t eventTime = matchStart.elapsed_nano();  // Record matching time in nanosecond.
        matchTimeList.push_back((double) eventTime / 1000000);
        matchSubList.push_back(matchSubs);
        /*
        new_pubMatchtime[i].insert(new_pubMatchtime[i].end(), matchDetailPub.begin(), matchDetailPub.end());
        for(int j=0; j<matchDetailSub.size(); ++j)
            new_subMatchtime[matchDetailSub[j]].push_back(matchDetailPub[j]);
        if (!((i+1)%3000))
        {
            Util::storeNewResult(new_pubMatchtime, pubs, new_subMatchtime, subs);
        }
        */
    }

    cout << "matching new_Rein finished" << endl;


    // output
    outputFileName = "./Detail/new_rein.txt";
    content = Util::Int2String(subs) + "\t" + Util::Double2String(Util::Average(insertTimeList)) + "\t" +
                     Util::Double2String(Util::Average(matchTimeList)) + "\t" +
                     Util::Double2String(Util::Average(matchSubList));
    Util::WriteData(outputFileName.c_str(), content);



    fileStream.open(string("./totaldata/totaldata_")+string(argv[1])+string(".txt"),ios::app);
    fileStream << content << endl;
    fileStream.close();

    //Util::writeNewtp(pubs,subs,string("./totaldata/totaldata_")+string(argv[1])+string(".txt"));

    //Util::disdetail(string(argv[1]));




    // check
    //Util::Check(gen, "./Detail/check_rein.txt");

    return 0;
}
