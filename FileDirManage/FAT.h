
#pragma once

#include<stack>
#include<algorithm>

const static int BLOCK_SIZE = 4096 / sizeof(bool);

using namespace std;

class FAT {
public:

    stack<int> freeDiskBlock;//�մ��̿�ջ

    stack<int> fullDiskBlock;//ռ�ô��̿�

    void init(string blocks[]);

    int getBlock();

    void addBlock(int block, string blocks[]);//��ȡһ���յĴ��̿�

    void save(string blocks[]);

};
