#include "modou.hpp"
int main(){
    //限制资源大小
    struct rlimit r;
    r.rlim_cur= 1024*1024*20;
    r.rlim_max= RLIM_INFINITY;
    setrlimit(RLIMIT_AS,&r);
    struct rlimit rl;
    rl.rlim_cur= 1;
    rl.rlim_max= RLIM_INFINITY;
    setrlimit(RLIMIT_CPU,&rl);
    int i=0;
    while(1){
      int* p=new int[1024*1024];
      i++;
      std::cout<<i<<std::endl;
      sleep(1);
    }
    return 0;
}//这个程序用来限制内存，超过就发送信号处理