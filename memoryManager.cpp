#include <iostream>
#include <vector>

using namespace std;

int swapIns = 0;
int swapOuts = 0;
int pageFaults = 0;

struct ProcessInfo{
  int pid;
  bool bitRef;
  int timeStamp, pageFaults;
  vector <int> pages;

  //Dummy constructor, to test data
  ProcessInfo(int _pid, bool _bitRef, int _timeStamp, int _pageFaults){
    pid = _pid;
    bitRef = _bitRef;
    timeStamp = _timeStamp;
    pageFaults = _pageFaults;
  }

};

int main(int argc, char *argv[]){

  ProcessInfo *p = new ProcessInfo(1, 0, 0, 0);
  cout << p->pid << " " << p->bitRef << " " << p->timeStamp << " " << p->pageFaults << " " << endl;

  return 0;
}
