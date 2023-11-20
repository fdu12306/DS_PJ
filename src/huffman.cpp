#include"huffman.h"

HuffmanNode::HuffmanNode(unsigned char _ascii,long long int _weight){
    ascii=_ascii;
    weight=_weight;
    lchild=nullptr;
    rchild=nullptr;
}

void HuffmanNode::set_ascii(int _ascii){
    ascii=_ascii;
}

unsigned char HuffmanNode::get_ascii(){
    return ascii;
}

void HuffmanNode::set_weight(long long int _weight){
    weight=_weight;
}

void HuffmanNode::add_weight(){
    weight++;
}

long long int HuffmanNode::get_weight(){
    return weight;
}

void HuffmanNode::set_code(string _code){
    code=_code;
}

string HuffmanNode::get_code(){
    return code;
}

void HuffmanNode::set_lchild(HuffmanNode* _lchild){
    lchild=_lchild;
}

HuffmanNode* HuffmanNode::get_lchild(){
    return lchild;
}

void HuffmanNode::set_rchild(HuffmanNode* _rchild){
    rchild=_rchild;
}

HuffmanNode* HuffmanNode::get_rchild(){
    return rchild;
}

HuffmanTree::HuffmanTree(){
    for(int i=0;i<256;i++){
        HuffmanArray[i].set_ascii(i);
        HuffmanArray[i].set_weight(0);
        HuffmanArray[i].set_code("");
    }
}

void HuffmanTree::getweight(string filepath){
    ifstream fin(filepath,ios::binary);
    int ch;
    ch=fin.get();
    while(!fin.eof()){
        HuffmanArray[ch].add_weight();
        ch=fin.get();
    }
    fin.close();
}

void HuffmanTree::creatHuffmanTree(){
    for(int i=0;i<256;i++){
        if(HuffmanArray[i].get_weight()>0){
            HuffmanNode temp(i,HuffmanArray[i].get_weight());
            HuffmanQueue.push(temp);
        }
    }
    while(HuffmanQueue.size()>1){
        HuffmanNode* templeft=new HuffmanNode(HuffmanQueue.top());
        HuffmanQueue.pop();
        HuffmanNode* tempright=new HuffmanNode(HuffmanQueue.top());
        HuffmanQueue.pop();
        HuffmanNode node(0,templeft->get_weight()+tempright->get_weight());
        node.set_lchild(templeft);
        node.set_rchild(tempright);
        HuffmanQueue.push(node);
    }
    root=new HuffmanNode(HuffmanQueue.top());
}

void HuffmanTree::getHuffmanCode(HuffmanNode* node,string code){
    if(node->get_lchild()==nullptr&&node->get_rchild()==nullptr){
        int i=(int)node->get_ascii();
        HuffmanArray[i].set_code(code);
    }
    else if(node->get_lchild()&&node->get_rchild()){
        getHuffmanCode(node->get_lchild(),code+"0");
        getHuffmanCode(node->get_rchild(),code+"1");
    }
}

void HuffmanTree::HuffmanCompress(string path,ofstream &fout){
    long long int filelength=0;
    fout.write((char*)&filelength,sizeof(long long int));
    ifstream fins(path,ios::binary);
    int c=fins.get();
    //文件为空，写入文件的长度为0
    if(fins.eof()){
        fins.close();
    }
    //文件不为空
    else{
        fins.close();
        getweight(path);
        creatHuffmanTree();
        getHuffmanCode(new HuffmanNode(HuffmanQueue.top()),"");
        //写入每个ascii码对应的权重
        for(int i=0;i<256;i++){
            long long int temp=HuffmanArray[i].get_weight();
            fout.write((char*)&temp,sizeof(long long int));
        }
        //写入哈夫曼编码，同时统计写入哈夫曼编码的长度
        ifstream fin(path,ios::binary);
        int ch=fin.get();
        int index=0;
        char temp=0;
        while(!fin.eof()){
            string str=HuffmanArray[ch].get_code();
            int len=str.length();
            for(int i=0;i<len;i++){
                if(str[i]=='1'){
                    char bit=1<<index;
                    temp=temp|bit;
                }
                index++;
                if(index==8){
                    fout.write((char*)&temp,sizeof(char));
                    filelength++;
                    index=0;
                    temp=0;
                }
            }
            ch=fin.get();
        }
        fin.close();
        if(index>0){
            fout.write(&temp,sizeof(char));
            filelength++;
        }
        //定位到filelength的位置修改其值
        long long int count1=0;
        long long int count2=0;
        count1=-(filelength+2056);
        count2=filelength+2048;
        fout.seekp(count1,ios::cur);
        fout.write((char*)&filelength,sizeof(long long int));
        fout.seekp(count2,ios::cur);
    }
}

void HuffmanTree::HuffmanDecompress(string filename,ifstream& fin,long long int len){
    ofstream fout(filename,ios::binary);
    //文件为空
    if(len==0){
        fout.close();
    }
    //文件不为空
    else{
        //读取ascii数组的权重构建哈夫曼树
        for(int i=0;i<256;i++){
            long long int temp;
            fin.read((char*)&temp,sizeof(long long int));
            HuffmanArray[i].set_weight(temp);
        }
        creatHuffmanTree();
        getHuffmanCode(new HuffmanNode(HuffmanQueue.top()),"");
        //根据哈夫曼编码解压
        int ch=fin.get();
        len--;
        int index=0;
        HuffmanNode* node;
        while(len>0){
            node=root;
            while(true){
                if(node->get_lchild()==nullptr&&node->get_rchild()==nullptr){
                    unsigned char temp=node->get_ascii();
                    fout.write((char*)&temp,sizeof(char));
                    node=root;
                    break;
                }
                //左移
                if(ch&(1<<index)){
                    node=node->get_rchild();
                }
                //右移
                else{
                    node=node->get_lchild();
                }
                index++;
                //index=8则再次读取，哈夫曼编码长度-1
                if(index==8){
                    ch=fin.get();
                    len--;
                    index=0;
                }
            }
        }
        fout.close();
    }
}








