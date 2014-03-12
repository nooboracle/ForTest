#include<iostream>
#include<fstream.h>
#include "stdio.h"
#include "stdlib.h"
using namespace std;

extern int answer[50000];
extern int questionType[50000];
extern string questionLabel[50000];
extern string twoAnswer[50000][2];
extern string questionImageFileName[50000];

static int openTxT(string filePath,int beginIndex){
    
    for (int i = 0 ; i < 50000; i++) {
        answer[i] = -1;
        questionLabel[i] = "";
        twoAnswer[i][0] = "";
        twoAnswer[i][1] = "";
        questionType[i] = -1;
        questionImageFileName[i] = "";
    }
    
    int counter = 0;
    string tmp0[50000];
    string tmp1[50000];
    
    ifstream fin;
    fin.open(filePath.c_str());
    for (int i = 0 ;; i++) {
        fin>>questionLabel[i];
        fin>>tmp0[i];
        fin>>tmp1[i];
        fin>>answer[i];
        fin>>questionType[i];
        if (questionType[i] == 0 || questionType[i] == 3) {    fin>>questionImageFileName[i];      }
        if (fin.eof()) {    break;      }
        counter++;
    }
    fin.close();
    
    for (int i = 0 ; i <=counter; i++) {
        twoAnswer[i][0] = tmp0[i];
        twoAnswer[i][1] = tmp1[i];
    }
    
    /*
     for (int i = 0 ; i <=counter ; i++) {
     cout<<i<<endl;
     cout<<questionLabel[i]<<" ";
     cout<<twoAnswer[i][0]<<" ";
     cout<<twoAnswer[i][1]<<" ";
     cout<<answer[i]<<" ";
     cout<<questionType[i]<<" ";
     cout<<questionImageFileName[i]<<endl;
     }
     */

    return counter;
}
