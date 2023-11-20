#ifndef HUFFMAN_H
#define HUFFMAN_H

#include<iostream>
#include<string>
#include<queue>
#include<fstream>
using namespace std;

//哈夫曼结点
class HuffmanNode{
private:
    unsigned char ascii;
    long long int weight;
    string code;
    HuffmanNode* lchild;
    HuffmanNode* rchild;
public:
    HuffmanNode(){};
    HuffmanNode(unsigned char _ascii,long long int weight);//构造函数
    void set_ascii(int _ascii);//设置ascii值
    unsigned char get_ascii();//获取ascii值
    void set_weight(long long int _weight);//设置权重
    void add_weight();//权重加一
    long long int get_weight();//获取权重
    void set_code(string _code);//设置哈夫曼编码
    string get_code();//获取哈夫曼编码
    void set_lchild(HuffmanNode* _lchild);//设置左孩子节点
    HuffmanNode* get_lchild();//获取左孩子节点
    void set_rchild(HuffmanNode* _rchild);//设置右孩子节点
    HuffmanNode* get_rchild();//获取右孩子节点
    friend bool operator < (HuffmanNode A,HuffmanNode B){//利用重载的友元函数定义优先数列的优先级
        return A.get_weight()>B.get_weight();
    }
};

//哈夫曼树
class HuffmanTree{
private:
    priority_queue<HuffmanNode> HuffmanQueue;//优先队列实现哈夫曼结点的排序
public:
    HuffmanNode* root;//根节点
    HuffmanNode HuffmanArray[256];//用来存储256个ascii码相关的信息
    HuffmanTree();//构造函数
    void getweight(string path);//获得ascii的权重
    void creatHuffmanTree();//创建哈夫曼树
    void getHuffmanCode(HuffmanNode* node,string code);//获得哈夫曼编码
    void HuffmanCompress(string path,ofstream &fout);//哈夫曼压缩
    void HuffmanDecompress(string filename,ifstream& fin,long long int len);//哈夫曼解压
};


#endif