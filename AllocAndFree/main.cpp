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
    MyPartitionNode * p = (MyPartitionNode *)malloc(sizeof(struct MyPartitionNode)); // ����һ��ռ�

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

    if(p->nsize == psize) { // �պõ��ڣ�ȫ������
        p->pid = pid;
        p->state = ALLOCED; // 1
    } else {                // �ָ���뵽ԭ��������
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

    if(p==nullptr) {   // δ�ҵ�
        return 0;
    }

    if(p != head && p->pre->state == FREE && p->pre->addr + p->pre->nsize == p->addr) {
        // ǰһ����Ϊ���з���
        MyPartitionNode *preNode = p->pre;
        MyPartitionNode *nextNode = p->next;

        // �߼�ɾ��
        preNode->nsize += p->nsize;
        preNode->next = p->next;
        nextNode->pre = preNode;

        // �������Ҳ�ǿ��еģ�ֱ�Ӻ�ǰ���Ҳ�ϲ���
        if(p->next->state == FREE && p->addr + p->nsize == p->next->addr) {
            preNode->nsize += nextNode->nsize;
            preNode->next = nextNode->next;
            nextNode->next->pre = preNode;
            free(nextNode); // �ͷ�
        }
        free(p); // �ͷ�
    } else {
        if(p->next != nullptr && p->next->state == FREE && p->addr + p->nsize == p->next->addr) {
            // ǰ����������У����߾���ͷ��㣬����������ǿ��е�
            MyPartitionNode *nextNode = p->next;
            p->pid = -1;
            p->state = FREE;             // ��ǰ������Ϊ��
            p->nsize += nextNode->nsize; // ����Ŀ��з����ϲ�����ǰ����ȥ
            p->next = nextNode->next;    // �߼�ɾ������Ľ��
            nextNode->next->pre = p;
            free(nextNode);              // �ͷ�
        } else {
            // ����ֻ����Լ�
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
    cout<<"pid\t״̬ \t�׵�ַ\tĩ��ַ\t���� \n";
    while(p!=nullptr) {
        cout<<p->pid<<"\t"<<STATE[p->state]<<"\t"<<p->addr<<"\t"<<p->addr+p->nsize<<"\t"<<p->nsize<<"\n";
        p = p->next;
    }
    cout<<"-------------------------------------------\n";
}

void programStart(int op[2][11], int mode)//Ĭ��best fit
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
