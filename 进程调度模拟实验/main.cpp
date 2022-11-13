#include <bits/stdc++.h>
#define MAXRESPONSETIME 100000
using namespace std;

string des[]= {"ID        ","PRIORITY  ","CPUTIME   ","ALLTIME  ","STARTBLOCK ","BLOCKTIME ","STATE     "};
string states[] = {"READY","BLOCK","RUNNING","FINISH"};

void print(int v[50][7]) {

    for(int i = 0; i < 7; i++) {
        if(i==6) {
            cout<<des[i]<<"\t";
            for(int j = 0; j < 5; j++) {
                cout<<states[v[j][i]]<<"\t";
            }
            cout<<"\n";
            break;
        }
        cout<<des[i]<<"\t";
        for(int j = 0; j < 5; j++) {
            cout<<v[j][i]<<"\t";
        }
        cout<<"\n";
    }

    cout<<"= = = = = = = = = = = = = = = = = = = = = = = = = = =\n";
}

bool cmp (std::pair<int,int> p1,std::pair<int,int> p2) {
    return p1.first > p2.first;
}

int main()
{
    int temp[50][7] = {{0,9,0,3,2,3,0},{1,38,0,3,-1,0,0},{2,30,0,6,-1,0,0},{3,29,0,3,-1,0},{4,0,0,4,-1,0,0}};
    print(temp);
    vector<pair<int,int> > readyQueue;
    vector<pair<int,int> > blockQueue;
    for(int i = 0; i < MAXRESPONSETIME; i++) {
        int maxPriority = -1, maxPriorityIndex = 0, isRunning = 0;
        for(int j = 0; j < 5; j++) {
            if(maxPriority <= temp[j][1] && temp[j][6] == 0){
                maxPriority = temp[j][1];
                maxPriorityIndex = j;
            }
        }
        if(maxPriority==-1) break;
        for(int j = 0; j < 5; j++) {
            if(temp[j][6]==0) {
                temp[j][1]++;
            } else if(temp[j][6]==1) {
                if(--temp[j][5] == 0) {
                    temp[j][6] = 0;
                }
            }
        }
        temp[maxPriorityIndex][2]++;
        temp[maxPriorityIndex][3]--;
        temp[maxPriorityIndex][1] -= 4;
        if(temp[maxPriorityIndex][3]==0) {
            temp[maxPriorityIndex][6]=3;
        } else {
            if(temp[maxPriorityIndex][4]!=-1) {
                temp[maxPriorityIndex][4]--;
            }
            if(temp[maxPriorityIndex][4]==0) {
                temp[maxPriorityIndex][6]=1;
            } else {
                temp[maxPriorityIndex][6]=2;
                isRunning = 1;
            }
        }
        for(int j = 0; j < 5; j++) {
            if(temp[j][6]==0) {
                readyQueue.push_back({temp[j][1],j});
            }
            if(temp[j][6]==1) {
                blockQueue.push_back({temp[j][1],j});
            }
        }
        sort(readyQueue.begin(),readyQueue.end(),cmp);
        sort(blockQueue.begin(),blockQueue.end(),cmp);
        cout<<"RUNNING PROG: "<<maxPriorityIndex<<"\nREADY-QUEUE: ";
        for(int j = 0; j<readyQueue.size(); j++) {
            cout<<"->"<<readyQueue[j].second;
        }
        cout<<"\nBLOCK-QUEUE: ";
        for(int j = 0; j<blockQueue.size(); j++) {
            cout<<"->"<<blockQueue[j].second;
        }
        cout<<"\n";
        print(temp);
        if(isRunning){
            temp[maxPriorityIndex][6]=0;
        }
        readyQueue.clear();
        blockQueue.clear();
    }

    return 0;
}
