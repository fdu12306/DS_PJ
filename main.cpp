#include"./include/function.h"

int main(){
    function p;
   clock_t start,end;
   start=clock();
   p.compress("testcase08Speed\\1.csv");
   //p.decompress("test.huf");
   end=clock();
   cout<<end-start<<endl;
    system("pause");
    return 0;
}