#include <iostream>
#include "opIndex.h"
#include "origin_opIndex.h"
#include <algorithm>
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
    int old_seg;
    int new_seg;
    int old_sig;
    int new_sig;

    // read paras
    ifstream parafile;
    parafile.open(string("./paras/paras_")+string(argv[1])+string(".txt"));
    parafile >> subs >> pubs >> k >> m >> subAttDis >> subValDis >> pubAttDis >> pubValDis >> attDom >> valDom
             >> subAttalpha >>subValalpha >>  pubAttalpha >> pubValalpha >> width >> equalRatio>>old_seg >> old_sig>>new_seg>>new_sig;
    parafile.close();

    // define test vectors
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
    double moditime;
    double firsttime;
    double secondtime;
    vector<double> totalmodi;
    vector<double> firsttimevec;
    ofstream fileStream;

    srand(time(NULL));

    // Initiate generator
    generator gen(subs, pubs, k, m, subAttDis, subValDis, pubAttDis, pubValDis, attDom, valDom, subAttalpha ,subValalpha ,pubAttalpha, pubValalpha, width, equalRatio);

    gen.GenSubList();
    gen.GenPubList();

    cout << "gen datalist finished" << endl;

    // initiate opindex
    origin_opIndex oldop(old_seg,old_sig);
    oldop.calcFrequency(gen.subList);
    opIndex newop(k,new_seg,new_sig);
    newop.calcFrequency(gen.subList);
    //origin_opIndex
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
    // insert
    for (int i = 0; i < subs; i++)
    {
        Timer subStart;

        oldop.insert(gen.subList[i]);                       // Insert sub[i] into data structure.

        int64_t insertTime = subStart.elapsed_nano();   // Record inserting time in nanosecond.
        insertTimeList.push_back((double) insertTime / 1000000);
    }

    cout << "insert origin_opIndex finished" << endl;

    // match

    fileStream.open("./Detail/origin_matchDetailsList.txt");
    fileStream << "\n";
    fileStream.close();


    for (int i = 0; i < pubs; i++)
    {
        if (!((i+1)%100)) cout << "matching " << i+1 << "..." << endl;
        int matchSubs = 0;                              // Record the number of matched subscriptions.
        matchDetailPub.clear();
        matchDetailSub.clear();
        Timer matchStart;
        oldop.match(gen.pubList[i], matchSubs, gen.subList, matchDetailPub, matchDetailSub);

        int64_t eventTime = matchStart.elapsed_nano();  // Record matching time in nanosecond.
        matchTimeList.push_back((double) eventTime / 1000000);

        matchSubList.push_back(matchSubs);
        /*
        old_pubMatchtime[i].insert(old_pubMatchtime[i].end(), matchDetailPub.begin(), matchDetailPub.end());
        for(int j=0; j<matchDetailSub.size(); ++j)
            old_subMatchtime[matchDetailSub[j]].push_back(matchDetailPub[j]);
        if (!((i+1)%2000))
        {
            Util::storeOldResult(old_pubMatchtime, pubs, old_subMatchtime, subs);
        }
         */
    }
    cout << "matching origin_opIndex finished" << endl;


    fileStream.open("./Detail/origin_matchsubs.txt");
    for (size_t i=0; i<matchSubList.size(); ++i)
        fileStream << matchSubList[i] << "\n";
    fileStream.close();


    // output
    string outputFileName = "./Detail/origin_opIndex.txt";
    string content = Util::Int2String(subs) + "\t" + Util::Int2String(pubs) + "\t" + Util::Double2String(Util::Average(insertTimeList)) + "\t" +
                     Util::Double2String(Util::Average(matchTimeList)) + "\t" +
                     Util::Double2String(Util::Average(matchSubList));
    Util::WriteData(outputFileName.c_str(), content);

    //a.writeCounter();


    fileStream.open(string("./totaldata/totaldata_")+string(argv[1])+string(".txt"));
    fileStream << content << endl;
    fileStream.close();

    //Util::writeOldtp(pubs,subs,string("./totaldata/totaldata_")+string(argv[1])+string(".txt"));

    //clear vectors

    insertTimeList.clear();
    matchTimeList.clear();
    matchSubList.clear();

    matchDetailPub.clear();
    matchDetailSub.clear();
    totalDetail.clear();
    totalmodi.clear();
    //new_opindex

    // insert
    for (int i = 0; i < subs; i++)
    {
        Timer subStart;

        newop.insert(gen.subList[i]);                       // Insert sub[i] into data structure.

        int64_t insertTime = subStart.elapsed_nano();   // Record inserting time in nanosecond.
        insertTimeList.push_back((double) insertTime / 1000000);
    }

    cout << "insert new_opIndex finished" << endl;

    // match

    fileStream.open("./Detail/new_matchDetailsList.txt");
    fileStream << "\n";
    fileStream.close();


    for (int i = 0; i < pubs; i++)
    {
        if (!((i+1)%100)) cout << "matching " << i+1 << "..." << endl;
        int matchSubs = 0;                              // Record the number of matched subscriptions.
        matchDetailPub.clear();
        matchDetailSub.clear();

        Timer matchStart;
        newop.match(gen.pubList[i], matchSubs, gen.subList, matchDetailPub, matchDetailSub);

        int64_t eventTime = matchStart.elapsed_nano();  // Record matching time in nanosecond.
        matchTimeList.push_back((double) eventTime / 1000000);
        matchSubList.push_back(matchSubs);
        //Printer::SaveDetails("new_matchDetailsList.txt",matchDetailPub, i);
        /*
        new_pubMatchtime[i].insert(new_pubMatchtime[i].end(), matchDetailPub.begin(), matchDetailPub.end());
        for(int j=0; j<matchDetailSub.size(); ++j)
            new_subMatchtime[matchDetailSub[j]].push_back(matchDetailPub[j]);
        if (!((i+1)%2000))
        {
            Util::storeNewResult(new_pubMatchtime, pubs, new_subMatchtime, subs);
        }
         */
    }
    cout << "matching new_opIndex finished" << endl;

    //newop.writeCounter();
    //newop.writesegCounter();

    fileStream.open("./Detail/new_matchsubs.txt");
    for (size_t i=0; i<matchSubList.size(); ++i)
        fileStream << matchSubList[i] << "\n";
    fileStream.close();

    // output
    outputFileName = "./Detail/new_opindex.txt";
    content = Util::Int2String(subs) + "\t" + Util::Int2String(pubs) + "\t" + Util::Double2String(Util::Average(insertTimeList)) + "\t" +
              Util::Double2String(Util::Average(matchTimeList)) + "\t" +
              Util::Double2String(Util::Average(matchSubList));
    Util::WriteData(outputFileName.c_str(), content);


    fileStream.open(string("./totaldata/totaldata_")+string(argv[1])+string(".txt"),ios::app);
    fileStream << '\n' << content << endl;
    fileStream.close();

    //Util::writeNewtp(pubs,subs,string("./totaldata/totaldata_")+string(argv[1])+string(".txt"));


    // check
    //Util::Check(gen);

    return 0;
}
