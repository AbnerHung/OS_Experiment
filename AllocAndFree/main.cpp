#include <bits/stdc++.h>
#define MEMORY_SIZE 640
#define MAX_PRO 20
#define ALLOCED 1
#define FREE 0

using namespace std;

struct MyPartitionNode
{
    int pid;
    int state;
    int addr;
    int nsize;

    MyPartitionNode * pre;
    MyPartitionNode * next;
};

MyPartitionNode * head;
MyPartitionNode * cur;

string STATE[2] = {"FREE","ALLOCED"};
int OP[2][11] = {{1,2,3,0,4,0,0,5,6,7,0},{130,60,100,2,200,3,1,140,60,50,6}};

MyPartitionNode * initPartitionList()
{
    MyPartitionNode * p = (MyPartitionNode *)malloc(sizeof(struct MyPartitionNode)); // 分配一块空间

    p->pid = -1;
    p->state = FREE; // 0
    p->addr = 0;
    p->nsize = MEMORY_SIZE; // 640

    p->pre = nullptr;
    p->next = nullptr;
}

int alloc(int pid, int psize, MyPartitionNode * p)
{
    if(p==nullptr) {
        return 0;
    }

    if(p->nsize == psize) { // 刚好等于，全部分配
        p->pid = pid;
        p->state = ALLOCED; // 1
    } else {                // 分割，插入到原来结点后面
        MyPartitionNode * newPartitionNode = (MyPartitionNode *)malloc(sizeof(struct MyPartitionNode));
        newPartitionNode->pid = -1;
        newPartitionNode->state = FREE;
        newPartitionNode->addr = p->addr + psize;
        newPartitionNode->nsize = p->nsize - psize;

        p->pid = pid;
        p->state = ALLOCED;
        p->nsize = psize;

        newPartitionNode->next = p->next;
        p->next = newPartitionNode;
        newPartitionNode->pre = p;
    }
    return 1;
}

int free(int pid)
{
    MyPartitionNode *p = head;

    while(p!=nullptr) {
        if(p->pid == pid) {
            break;
        }
        p = p->next;
    }

    if(p==nullptr) {   // 未找到
        return 0;
    }

    if(p != head && p->pre->state == FREE && p->pre->addr + p->pre->nsize == p->addr) {
        // 前一分区为空闲分区
        MyPartitionNode *preNode = p->pre;
        MyPartitionNode *nextNode = p->next;

        // 逻辑删除
        preNode->nsize += p->nsize;
        preNode->next = p->next;
        nextNode->pre = preNode;

        // 如果后面也是空闲的，直接和前面的也合并了
        if(p->next->state == FREE && p->addr + p->nsize == p->next->addr) {
            preNode->nsize += nextNode->nsize;
            preNode->next = nextNode->next;
            nextNode->next->pre = preNode;
            free(nextNode); // 释放
        }
        free(p); // 释放
    } else {
        if(p->next != nullptr && p->next->state == FREE && p->addr + p->nsize == p->next->addr) {
            // 前面分区不空闲，或者就是头结点，但后面分区是空闲的
            MyPartitionNode *nextNode = p->next;
            p->pid = -1;
            p->state = FREE;             // 当前分区置为空
            p->nsize += nextNode->nsize; // 后面的空闲分区合并到当前分区去
            p->next = nextNode->next;    // 逻辑删除后面的结点
            nextNode->next->pre = p;
            free(nextNode);              // 释放
        } else {
            // 否则只清空自己
            p->pid = -1;
            p->state = FREE;
        }
    }

    return 1;
}

MyPartitionNode *ffFindFreeArea(int psize)
{
	MyPartitionNode *p = head;

	while (p) {
		if (p->state == FREE && p->nsize >= psize) {
            return p;
		}
		p = p->next;
	}

	return nullptr;
}


int ffAlloc(int pid, int psize)
{
	return alloc(pid, psize, ffFindFreeArea(psize));
}

MyPartitionNode *bfFindFreeArea(int psize)
{
	MyPartitionNode *p = head;
	MyPartitionNode *pMin = nullptr;
	int minSize = MEMORY_SIZE + 1;

	while (p) {
		if (p->state == FREE && p->nsize >= psize) {
			if (p->nsize < minSize) {
				minSize = p->nsize;
				pMin = p;
			}
		}
		p = p->next;
	}

	return pMin;
}

int bfAlloc(int pid, int psize)
{
	return alloc(pid, psize, bfFindFreeArea(psize));
}

void print()
{
    MyPartitionNode *p = head;
    cout<<"-------------------------------------------\n";
    cout<<"pid\t状态 \t首地址\t末地址\t长度 \n";
    while(p!=nullptr) {
        cout<<p->pid<<"\t"<<STATE[p->state]<<"\t"<<p->addr<<"\t"<<p->addr+p->nsize<<"\t"<<p->nsize<<"\n";
        p = p->next;
    }
    cout<<"-------------------------------------------\n";
}

void programStart(int op[2][11], int mode)//默认best fit
{
    for(int i = 0; i < 11; i++) {
        if(op[0][i]==0) {
            free(op[1][i]);
            print();
            continue;
        }
        if(mode==1) {
            ffAlloc(op[0][i],op[1][i]);
            print();
            continue;
        }
        bfAlloc(op[0][i],op[1][i]);
        print();
    }
}

int main()
{
    head = initPartitionList();
    cout<<"First Fit Algo\n";
    programStart(OP, 1);
    head = initPartitionList();
    cout<<"Best Fit Algo\n";
    programStart(OP, 0);
    return 0;
}
