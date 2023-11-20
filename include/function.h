#ifndef FUNCTION_H
#define FUNCTION_H


#include<filesystem>
#include"huffman.h"

class function{
public:
    function(){};
    void getfilepath(string filename,vector<string> &filepaths);//获得文件夹中所有文件或空文件夹的路径
    void filecompress(string filename);//单个文件的压缩
    void foldercompress(string filename);//文件夹的压缩
    void compress(string filename);//压缩
    void decompress(string filename);//解压
};



#endif