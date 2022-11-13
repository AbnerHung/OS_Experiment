#include<stack>
#include<iostream>
#include<iomanip>
#include<queue>
#include <fstream>
#include "DiskManager.h"
#include "Folder.h"
#include "FileType.h"
#include "FAT.h"
#include <unistd.h>
#include <sys/stat.h>

const string ACCESS[] = {"readonly", "modify", "executable"};
const string rootPath = "/home";
queue<FCB *> persistQueue;//持久化队列
FAT fat;
string blocks[N];
ofstream *out = NULL;
ifstream *in = NULL;
#if (defined(_WIN32) || defined(__WIN32__))
#define mkdir(A, B) mkdir(A)
#endif

using namespace std;


void DiskManager::DiskWrite(File *file) {
    //文件输出流
    printf("%s\n", file->path.c_str());
    out = new ofstream(file->path.c_str());
    if (out->is_open()) {
        out->close();
    }
}

bool DiskManager::DiskMkdir(string dirName) {
    printf("%s\n", dirName.c_str());
    return mkdir(dirName.c_str(), S_IRWXU) == 0;
}

bool DiskManager::DiskRmdir(string dirName) {
    return rmdir(dirName.c_str()) == 0;
}

bool DiskManager::DiskCkdir(string dirName) {

    if (access(dirName.c_str(), 0) == -1) {
        return mkdir(dirName.c_str(), S_IRWXU) == 0;
    }
    return false;
}

void DiskManager::DiskRmdir(Folder *f) {
    //DFS删除
    for (int i = 0; i < f->child.size(); i++) {
        //判断文件类型
        if (f->child[i]->type == DOCUMENT) {
            printf("%s\n", f->child[i]->path.c_str());
            remove(f->child[i]->path.c_str());
            delete f->child[i];
        } else {
            this->DiskRmdir((Folder *) f->child[i]);
        }
    }
    printf("%s\n", f->path.c_str());
    this->DiskRmdir(f->path.c_str());
}

DiskManager::DiskManager() {
    fat.init(blocks);
    root = new Folder(rootPath, FileType::FOLDER);
    root->path = rootPath;
    this->DiskMkdir(rootPath);
    //设置磁盘根为目录
    //设置根节点的父节点为自身
    root->father = root;
    // cout << "------------输入help获得帮助-----------" << endl;
    cout << "\nroot@localhost:" + rootPath + "$ ";
    string opear, cmd;
    while (cin >> cmd) {
        if (cmd == "format") {
            this->format(blocks);
        } else if (cmd == "mkdir") {
            this->Mkdir();
        } else if (cmd == "rmdir") {
            this->Rmdir();
        } else if (cmd == "ls") {
            this->ls();
        } else if (cmd == "cd") {
            this->cd();
        } else if (cmd == "create") {
            this->create();
        } else if (cmd == "open") {
            this->open();
        } else if (cmd == "close") {
            this->close();
        } else if (cmd == "rm") {
            this->rm();
        } else if (cmd == "exit") {
            //fat.save(blocks);
            printf("%s\n", "now exit!");
            break;
        } else if (cmd == "help") {
            cout << "\n format: format the file storage.\n" <<
                 " mkdir: to create subdirectory\n" <<
                 " rmdir : to delete the subdirectory\n" <<
                 " ls : display the dir\n" <<
                 " cd : change the dir\n" <<
                 " create : create the file\n" <<
                 " open : open the file\n" <<
                 " close : close the file\n" <<
                 " write : write file\n" <<
                 " read : read file\n" <<
                 " rm : delete file\n" <<
                 " exit : exit the system\n"
                 << endl;
        } else {
            cout << "No command '"<<cmd<<"' found, please try again" << endl;
        }
        cout << "\nroot@localhost:" + this->root->path + "$ ";
    }
}

DiskManager::~DiskManager() {

}

void DiskManager::format(string *blocks) {
    fat.init(blocks);

    //回退到根目录
    while (root->father != root) {
        this->root = (Folder *) (this->root->father);
    }
    this->DiskRmdir(this->root);
    root->child.clear();
    printf("%s\n", "formated successfully");
}

void DiskManager::Mkdir() {
    string name;
    cin >> name;
    Folder *childFile = new Folder(name, FileType::FOLDER);
    //设置父节点
    childFile->father = (this->root);
    childFile->path = this->root->path + "/"+ name ;
    //判断是否文件重复
    if (this->root->count(childFile)) {
        //文件重复报错
        cout << "failed to create, duplicate file name !" << endl;
    } else {
        cout << "folder created successfully!" << endl;
        this->DiskMkdir(childFile->path);
        this->root->addChild(childFile);
    }
}

void DiskManager::Rmdir() {
    string name;
    cin >> name;
    Folder *childFile = new Folder(name, FOLDER);
    childFile = (Folder *) this->root->find(childFile);
    if (this->root->erase(childFile)) {
        //文件重复报错
        this->DiskRmdir(childFile);
        cout << "folder deleted successfully!" << endl;
    } else {
        cout << "no such dir, failed to delete!" << endl;
    }
}

void DiskManager::ls() {

    cout << setw(10) << "permission"<< setw(20) << "size"<< setw(25) << "last write time" << setw(20) << "name"<< endl;
    int size = this->root->size();
    for (int i = 0; i < size; i++) {
        cout << setw(10) << ACCESS[this->root->child[i]->access]
             << setw(20)
             << (this->root->child[i]->type != FOLDER ? ((File *) this->root->child[i])->toString(blocks).size() : 4096)
             << setw(25) << this->root->child[i]->modifyDate
             << setw(20) << this->root->child[i]->name
             << endl;
    }
}

void DiskManager::cd() {
    string name;
    cin >> name;
    if (name == "..") {
        this->root = (Folder *) (this->root->father);
    } else {
        if (this->root->count(new Folder(name, FOLDER))) {

            if (this->root->find(new Folder(name, FOLDER))->type != FOLDER) {
                cout << "no such folder" << endl;
            } else {
                root = (Folder *) this->root->find(new Folder(name, FOLDER));
            }
        } else {
            cout << "no such folder " << endl;
        }
    }

}

void DiskManager::create() {
    string name;
    cin >> name;

    File *childFile = new File(name, DOCUMENT, fat);
    //设置父节点
    childFile->father = (this->root);
    childFile->path = this->root->path + "/" + name;
    //判断是否文件重复
    if (this->root->count(childFile)) {
        //文件重复报错
        cout << "failed to create, duplicate file name !" << endl;
    } else {
        cout << "created the file!" << endl;
        this->root->addChild(childFile);
        this->DiskWrite(childFile);
    }
}

void DiskManager::open() {
    string name, cmd;
    cin >> name;

    File *file = (File *) this->root->find(new File(name, DOCUMENT, fat));
    if (file != NULL) {
        printf("%s\n", "open the read&write stream succeed");
        cout << "\nroot@localhost:" + this->root->path +  " -> "+ name + " $ ";
        while (cin >> cmd) {
            if (cmd == "write") {
                cout << "\nroot@localhost:" + this->root->path +  " -> "+ name + " $ ";
                this->write(file->path.c_str(), file);
            } else if (cmd == "read") {
                cout << "\nroot@localhost:" + this->root->path +  " -> "+ name + " $ ";
                this->read(file->path.c_str());
            } else if (cmd == "close") {
                cout << "\nroot@localhost:" + this->root->path +  " -> "+ name + " $ ";
                this->close();
                break;
            } else {
                cout << "error input! plz try again\n";
                cout << "\nroot@localhost:" + this->root->path +  " -> "+ name + " $ ";
            }
        }
    } else {
        printf("%s\n", "cannot open the read&write stream, or there's no such file!");
    }
}

void DiskManager::close() {
    if (out == NULL || in == NULL) {
        printf("%s\n", "there's no stream to close!");
    } else {
        out->close();
        in->close();
        printf("%s\n", "closed the read&write stream!");
    }
}

void DiskManager::write(const char *s, File *file) {
    string content;
    cin >> content;
    if (in != NULL)in->close();
    file->addContent(content.c_str(), blocks, fat);//添加内容到文件中
    content = file->toString(blocks);
    out = new ofstream(s);
    if (out->is_open()) {
        *out << content;
    }
    out->close();
}

void DiskManager::read(const char *s) {
    char *content = new char[N];
    if (out != NULL)out->close();
    in = new ifstream(s);
    if (in->is_open()) {
        *in >> content;
    }
    in->close();
    cout << content<<"\n";
    cout << "\nroot@localhost:" + this->root->path + "$ ";
}

void DiskManager::rm() {
    string name;
    cin >> name;
    File *childFile = new File(name, DOCUMENT, fat);
    if (this->root->count(childFile)) {
        //文件重复报错
        childFile = (File *) this->root->find(childFile);
        remove(childFile->path.c_str());
        childFile->release(fat, blocks);
        this->root->erase(childFile);
        cout << "file deleted successfully!" << endl;
    } else {
        cout << "no such file, failed to delete!" << endl;
    }
}
