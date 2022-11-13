#include <bits/stdc++.h>

using namespace std;

struct DATA
{
    int pageNum;
    int blockNum;
    int time;
};

struct BLOCKNODE
{
    DATA data;
    struct BLOCKNODE* next;
};

BLOCKNODE* block1;
BLOCKNODE* block2;
BLOCKNODE* block3;
BLOCKNODE* block4;
BLOCKNODE* p;

float cnt = 0;
int pageAddr[320];
int instrAddr[320];

typedef int (*func) (int, int, int);
func mode;

void setPageNum(BLOCKNODE* block, int num)
{
    block->data.pageNum = num;
}

void setBlockNum(BLOCKNODE* block, int num)
{
    block->data.blockNum = num;
}

void setTime(BLOCKNODE* block, int num)
{
    block->data.time = num;
}

void makeLink(BLOCKNODE* blockA, BLOCKNODE* blockB)
{
    blockA->next = blockB;
}
/*
void addrInit()
{
    for(int i = 0; i < 320; ) //初始化地址流
    {
        int m = rand() % 320;
        instrAddr[i] = m + 1;
        pageAddr[i] = instrAddr[i] / 10;
        i++;

        int m1 = rand() % (m - 1);
        instrAddr[i] = m1;
        pageAddr[i] = m1 / 10;
        i++;

        instrAddr[i] = m1 + 1;
        pageAddr[i] = instrAddr[i] / 10;
        i++;

        int m2 = rand() % (319 - m1 - 1) + m1 + 2;
        instrAddr[i] = m2;
        pageAddr[i] = m2 / 10;
        i++;

        instrAddr[i] = m2 + 1;
        pageAddr[i] = instrAddr[i] / 10;
        i++;
    }
}*/

void addrInit()
{	int i=1;
    int m;
	pageAddr[0]=rand()%(319 - 0 + 1) + 0 ;
	m=pageAddr[0];
	while(i<320)
	{
	    instrAddr[i]=++m;
	    pageAddr[i] = instrAddr[i]/10;
	    i++;
	    if(i>=320)break;
	    instrAddr[i]=rand()%(m-1 -0 +1) + 0;
	    m=instrAddr[i];
	    pageAddr[i] = instrAddr[i]/10;
	    i++;
	    if(i>=320)break;
	    instrAddr[i]=++m;
	    pageAddr[i] = instrAddr[i]/10;
	    i++;
	    if(i>=320)break;
	    instrAddr[i]=rand()%(319 -m +2) + m+1;
	    m=instrAddr[i];
	    pageAddr[i] = instrAddr[i]/10;
	    i++;

	}
}

void init()
{
    cnt = 0;

    block1 = (BLOCKNODE*)malloc(sizeof(BLOCKNODE));
    block2 = (BLOCKNODE*)malloc(sizeof(BLOCKNODE));
    block3 = (BLOCKNODE*)malloc(sizeof(BLOCKNODE));
    block4 = (BLOCKNODE*)malloc(sizeof(BLOCKNODE));
    p = block1;

    setPageNum(block1, -1);
    setPageNum(block2, -1);
    setPageNum(block3, -1);
    setPageNum(block4, -1);

    setBlockNum(block1, 0);
    setBlockNum(block2, 1);
    setBlockNum(block3, 2);
    setBlockNum(block4, 3);

    setTime(block1, 0);
    setTime(block2, 0);
    setTime(block3, 0);
    setTime(block4, 0);

    makeLink(block1, block2);
    makeLink(block2, block3);
    makeLink(block3, block4);
    makeLink(block4, block1);
}

void clean()
{
    free(block1);
    free(block2);
    free(block3);
    free(block4);
}

void printBlock()
{
    BLOCKNODE* p = block1;
    for(int i = 0; i<4; i++) {
        cout<<p->data.pageNum<<" \t";
        p = p->next;
    }
    cout<<"\n";
}

void printTime()
{
    BLOCKNODE* p = block1;
    for(int i = 0; i<4; i++) {
        cout<<p->data.time<<" \t";
        p = p->next;
    }
    cout<<"\n";
}

int FIFO(int pageNum,int addr, int id)
{
    BLOCKNODE* head = p;

    for(int i = 0; i < 4; i++) {
        // 指令未装入内存
        if(p->data.pageNum == -1) {
            p->data.pageNum = pageNum;
            cnt++;
            cout<<"---------------------------------------------------------------------------------\n";
            cout<<"从第0块到第3块的页号\n";
            printBlock();
            cout<<"\n当前id: "<<id<<"  指令地址： "<<addr<<"  第"<<pageNum<<"页  第";
            cout<<(addr % 10)<<"条  物理地址: 第"<<p->data.blockNum<<"块  第"<<(addr % 10)<<"条\n";
            p = block1;
            return 1;
        }

        // 指令已在内存中
        if(p->data.pageNum == pageNum) {
            p = head;
            return 1;
        }

        p = p->next;
    }
    // 指令未装入内存且内存块已满
    p->data.pageNum = pageNum;
    cnt++;
    cout<<"---------------------------------------------------------------------------------\n";
    cout<<"从第0块到第3块的页号\n";
    printBlock();
    cout<<"\n当前id: "<<id<<"  指令地址： "<<addr<<"  第"<<pageNum<<"页  第";
    cout<<(addr % 10)<<"条  物理地址: 第"<<p->data.blockNum<<"块  第"<<(addr % 10)<<"条\n";
    p = p->next;

    return 1;
}

int LRU(int pageNum, int addr ,int id)
{
    int maxTime = -1;

    for(int i = 0; i < 4; i++) {
        // 指令未装入内存
        if(p->data.pageNum == -1) {
            p->data.pageNum = pageNum;
            cnt++;
            cout<<"---------------------------------------------------------------------------------\n";
            cout<<"从第0块到第3块的页号\n";
            printBlock();
            cout<<"时间计数器的值\n";
            printTime();
            cout<<"\n当前id: "<<id<<"  指令地址： "<<addr<<"  第"<<pageNum<<"页  第";
            cout<<(addr % 10)<<"条  物理地址: 第"<<p->data.blockNum<<"块  第"<<(addr % 10)<<"条\n";

            for(int i = 0; i < 4; i++) {
                p->data.time++;
                p = p->next;
            }

            p->data.time = 0;

            return 1;
        }

        // 指令已在内存中
        if(p->data.pageNum == pageNum) {
            for(int i = 0; i < 4; i++) {
                p->data.time++;
                p = p->next;
            }

            p->data.time = 0;

            return 1;
        }

        p = p->next;
    }
    // 指令未装入内存且内存块已满
    for(int i = 0; i < 4; i++) {
        maxTime = maxTime > p->data.time ? maxTime : p->data.time;
        p = p->next;
    }

    for(int i = 0; i < 4; i++) {
        if(p->data.time == maxTime) {
            for(int j = 0; j < 4; j++) {
                p->data.time++;
                p = p->next;
            }

            p->data.pageNum = pageNum;
            cnt++;
            cout<<"---------------------------------------------------------------------------------\n";
            cout<<"从第0块到第3块的页号\n";
            printBlock();
            cout<<"时间计数器的值\n";
            printTime();
            cout<<"\n当前id: "<<id<<"  指令地址： "<<addr<<"  第"<<pageNum<<"页  第";
            cout<<(addr % 10)<<"条  物理地址: 第"<<p->data.blockNum<<"块  第"<<(addr % 10)<<"条\n";
            p->data.time = 0;

            return 1;
        }
        p = p->next;
    }

    return 1;
}

int OPT(int pageNum, int addr, int pos)
{
    BLOCKNODE* q = block1;
    for(int i = 0; i < 4; i++) {
        if(q->data.pageNum == -1) {
            q->data.pageNum = pageNum;
            cnt++;
            cout<<"---------------------------------------------------------------------------------\n";
            cout<<"从第0块到第3块的页号\n";
            printBlock();
            cout<<"\n当前id: "<<pos<<"  指令地址： "<<addr<<"  第"<<pageNum<<"页  第";
            cout<<(addr % 10)<<"条  物理地址: 第"<<q->data.blockNum<<"块  第"<<(addr % 10)<<"条\n";
            return 1;
        }

        if(q->data.pageNum == pageNum) {
            return 1;
        }

        q = q->next;
    }

    int allBlockPageNum[4];

    for(int i = 0; i < 4; i++) {
        allBlockPageNum[i] = q->data.pageNum;
        q = q->next;
    }

    int nextAddr[4];

    for(int i = 0; i < 4; i++) {
        for(int j = pos; j < 320; j++) {
            if(allBlockPageNum[i] == pageAddr[j])
            {
                nextAddr[i] = j;
                break;
            } else if(j == 319 && allBlockPageNum[i] != pageAddr[j]) {
                nextAddr[i] = 666;
            }
        }
    }

    int temp = 0;
    int blockPos;

    for(int i = 0; i < 4; i++) {
        if(nextAddr[i] > temp)
        {
            temp = nextAddr[i];
            blockPos = i;
        }
    }

    for(int i = 0; i < 4; i++) {
        if(q->data.blockNum == blockPos) {
            q->data.pageNum = pageNum;
            cnt++;
            cout<<"---------------------------------------------------------------------------------\n";
            cout<<"从第0块到第3块的页号\n";
            printBlock();
            cout<<"下次访问在第几号指令\n";
            for(int i = 0; i < 4; i++) {
                cout<<nextAddr[i]<<" \t";
            }
            cout<<"\n";
            cout<<"\n当前id: "<<pos<<"  指令地址： "<<addr<<"  第"<<pageNum<<"页  第";
            cout<<(addr % 10)<<"条  物理地址: 第"<<q->data.blockNum<<"块  第"<<(addr % 10)<<"条\n";
        }

        q = q->next;
    }

    return 1;

}

/*void print()
{

}*/

void StartAlgo()
{
    //cout<<"第?条\t指令地址\t第?页\t第?条\t第?块\t第?条\t\n";
    for(int i = 0; i < 320;i++) {
        //cout<<"当前id\t\t用户指令\t物理地址\t\t\n";
        mode(pageAddr[i], instrAddr[i], i);
    }
    cout<<"---------------------------------------------------------------------------------\n";
    printf("缺页次数：%.0f\n", cnt);
    printf("计算得到的缺页率为：%.4f \n", cnt / 320);
    cout<<"=================================================================================\n";
}

/*
void OPTStartAlgo()
{
    for(int i = 0; i < 320;i++) {
        OPT(pageAddr[i], i);
    }
    printf("缺页次数：%.0f\n", cnt);
    printf("计算得到的缺页率为：%.4f \n", cnt / 320);
}*/


int main()
{
    cout<<"=====================================O=P=T=======================================\n";
    srand(40);
    addrInit();
    mode = OPT;
    init();
    StartAlgo();

    cout<<"\n\n\n\n\n=====================================L=R=U=======================================\n";
    srand(40);
    addrInit();
    mode = LRU;
    clean();
    init();
    StartAlgo();

    cout<<"\n\n\n\n\n====================================F=I=F=O=======================================\n";
    srand(30);
    addrInit();
    mode = FIFO;
    clean();
    init();
    StartAlgo();
    return 0;
}
