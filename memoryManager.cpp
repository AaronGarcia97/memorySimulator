#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>

using namespace std;

// You can change this variables during compilation
#ifndef PAGE_SIZE
#define PAGE_SIZE 16
#endif

#ifndef REAL_SIZE
#define REAL_SIZE 2048
#endif

#ifndef RESERVE_SIZE
#define RESERVE_SIZE 4096
#endif

int swapIns = 0;
int swapOuts = 0;
int pageFaults = 0;

queue<int> paginasDispM;
queue<int> paginasDispS;

struct ProcessInfo{
  int pid;
  bool bitRef;
  int timeStamp, pageFaults;
  vector <int> pagesUsed;

  //Dummy constructor, to test data
  ProcessInfo(int _pid, bool _bitRef, int _timeStamp, int _pageFaults){
    pid = _pid;
    bitRef = _bitRef;
    timeStamp = _timeStamp;
    pageFaults = _pageFaults;
  }

};

unordered_map<int, ProcessInfo> tablaMem;

int main(int argc, char *argv[]){

  ProcessInfo *p = new ProcessInfo(1, 0, 0, 0);
  cout << p->pid << " " << p->bitRef << " " << p->timeStamp << " " << p->pageFaults  << endl;

  cout << PAGE_SIZE << " " << REAL_SIZE << " " << RESERVE_SIZE << endl;

  return 0;
}
