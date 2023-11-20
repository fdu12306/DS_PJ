#include"function.h"

void function::getfilepath(string filename,vector<string> &filepaths){
    filesystem::path entry(filename);
    if(filesystem::is_empty(entry)){
        filepaths.push_back(entry.string());
    }
    else{
        for(auto& i : filesystem::directory_iterator(filename)){
            if(i.is_regular_file()){
                filepaths.push_back(i.path().string());
            }
            else{
                getfilepath(i.path().string(),filepaths);
            }
        }
    }
}

void function::filecompress(string filename){
    //自定义压缩后的文件名
    string newfilename;
    cout<<"please enter the new filename after compression!(No need to enter the suffix name, use .huf as the suffix name)"<<endl;
    cin>>newfilename;
    newfilename+=".huf";
    //string newfilename=filename;
   //newfilename+=".huf";
    //覆盖问题
    filesystem::path entry(newfilename);
    if(filesystem::exists(entry)){
        cout<<newfilename<<" already exists, whether to overwrite the original file. enter 0 to overwrite 1 to stop."<<endl;
        int flag;
        cin>>flag;
        if(flag==1){
            return;
        }
    }
    ofstream fout(newfilename,ios::binary);
    //以sign为标志，若sign=0则为文件的压缩；若sign=1则为文件夹的压缩
    int sign=0;
    fout.write((char*)&sign,sizeof(int));
    //写入文件名的长度以及文件名
    int filenamelength=filename.size();
    fout.write((char*)&filenamelength,sizeof(int));
    for(int i=0;i<filenamelength;i++){
        fout.write((char*)&filename[i],sizeof(char));
    }
    //写入文件的长度、ascii数组以及文件的哈夫曼编码
    HuffmanTree tree;
    tree.HuffmanCompress(filename,fout);
    fout.close();
    cout<<"End of compression!"<<endl;
}

void function::foldercompress(string filename){
    //获得所有文件以及空文件夹的路径
    vector<string> filepaths;
    getfilepath(filename,filepaths);
    //自定义压缩后的文件夹名
    string newfilename;
    cout<<"please enter the new foldername after compression!(No need to enter the suffix name, use .huf as the suffix name)"<<endl;
    cin>>newfilename;
    newfilename+=".huf";
    //string newfilename=filename;
    //newfilename+=".huf";
    //覆盖问题
    filesystem::path entry(newfilename);
    if(filesystem::exists(entry)){
        cout<<newfilename<<" already exists, whether to overwrite the original file. enter 0 to overwrite 1 to stop."<<endl;
        int flag;
        cin>>flag;
        if(flag==1){
            return;
        }
    }
    //sign=1作为文件夹的标志
    ofstream fout(newfilename,ios::binary);
    int sign=1;
    fout.write((char*)&sign,sizeof(int));
    //写入路径的总数以及各路径的长度以及其本身
    int filenum=filepaths.size();
    fout.write((char*)&filenum,sizeof(int));
    for(int i=0;i<filenum;i++){
        int len=filepaths[i].size();
        fout.write((char*)&len,sizeof(int));
        for(int j=0;j<len;j++){
            fout.write((char*)&filepaths[i][j],sizeof(char));
        }
    }
    //以for循环来压缩各文件
    for(int k=0;k<filenum;k++){
        int j=k+1;
        cout<<j<<"\\"<<filenum<<endl;
        //判断是否为文件
        if(filepaths[k].find('.')!=filepaths[k].npos){
            HuffmanTree tree;
            tree.HuffmanCompress(filepaths[k],fout);
        }
    }
    fout.close();
    cout<<"End of compression!"<<endl;
}

void function::compress(string filename){
    filesystem::path entry(filename);
    //单个文件的压缩
    if(filesystem::is_regular_file(entry)){
        filecompress(filename);
    }
    //文件夹的压缩
    else{
        foldercompress(filename);
    }
}

void function::decompress(string filename){
    //是我创建的压缩文件
    if(filename.find(".huf")!=filename.npos){
        ifstream fin(filename,ios::binary);
        int sign;
        fin.read((char*)&sign,sizeof(int));
        //sign=0,解压的是单个文件
        if(sign==0){
            //获得文件的名字
            int filenamelength;
            fin.read((char*)&filenamelength,sizeof(int));
            string filename("");
            while(filenamelength--){
                char ch=fin.get();
                filename+=ch;
            }
            filesystem::path entry(filename);
            if(filesystem::exists(entry)){
                cout<<filename<<" already exists, whether to overwrite the original file. enter 0 to overwrite 1 to stop."<<endl;
                int flag;
                cin>>flag;
                if(flag==1){
                    return;
                }
            }
            int m=filename.find_last_of('\\');
            string strf=filename.substr(0,m);
            filesystem::path ent(strf);
            if(!filesystem::exists(ent)){
                filesystem::create_directories(ent);
            }
            //读取哈夫曼编码的长度
            long long int filelength;
            fin.read((char*)&filelength,sizeof(long long int));
            HuffmanTree tree;
            //解压
            tree.HuffmanDecompress(filename,fin,filelength);
            fin.close();
        }
        //sign=1,解压的是文件夹
        else if(sign==1){
            //读取各个路径
            int filenum;
            fin.read((char*)&filenum,sizeof(int));
            vector<string> filepaths;
            while(filenum--){
                int filepath_length;
                fin.read((char*)&filepath_length,sizeof(int));
                string filepath("");
                while(filepath_length--){
                    char ch=fin.get();
                    filepath+=ch;
                }
                filepaths.push_back(filepath);
            }
            //对每个路径做解压
          //  for(int i=0;i<filepaths.size();i++){
            //    cout<<filepaths[i]<<endl;
            //}
            int num=filepaths.size();
            for(int i=0;i<num;i++){
                int j=i+1;
                cout<<j<<"\\"<<num<<endl;
                filesystem::path entry(filepaths[i]);
                if(filesystem::exists(entry)){
                    cout<<filepaths[i]<<" already exists, whether to overwrite the original file. enter 0 to overwrite 1 to stop."<<endl;
                    int flag;
                    cin>>flag;
                    if(flag==1){
                        return;
                    }
                }
                //文件
                if(filepaths[i].find('.')!=filepaths[i].npos){
                    //创建文件路径上的文件目录
                    int m=filepaths[i].find_last_of('\\');
                    string str=filepaths[i].substr(0,m);
                    filesystem::path entry(str);
                    if(!filesystem::exists(entry)){
                        filesystem::create_directories(entry);
                    }
                    long long int filelength=0;
                    fin.read((char*)&filelength,sizeof(long long int));
                    HuffmanTree tree;
                    tree.HuffmanDecompress(filepaths[i],fin,filelength);
                }
                //空文件夹
                else{
                    filesystem::path entry(filepaths[i]);
                    filesystem::create_directories(entry);
                }
            }
        }   
        fin.close();
        cout<<"End of decompression!"<<endl;
    }
    //不是我创建的压缩文件
    else{
        cout<<"This file was not created by me and cannot be decompressed!"<<endl;
    }
}







