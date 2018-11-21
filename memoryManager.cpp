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

vector<int> M(REAL_SIZE, 16);
vector<int> R(RESERVE_SIZE, 16);

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

// Function that initializes every variables
void init(){
  int totalPages = REAL_SIZE/PAGE_SIZE;

  cout << "Initializing Stuff... " << endl;

  // Add avaialable pages to queues
  // Real Memory
  for(int i = 0; i < totalPages; i++){
    paginasDispM.push(i);
  }

  totalPages = RESERVE_SIZE/PAGE_SIZE;

  //Reserve memory
  for(int i = 0; i < totalPages; i++){
    paginasDispS.push(i);
  }

};

unordered_map<int, ProcessInfo> tablaMem;

int main(int argc, char *argv[]){

  int n, pid, address, type;
  string line = "";
  char action = 'E';

  init();

  cout << "Memory sizes (bytes): " << endl;
  cout << "Page\tReal\tReserve" << endl;
  cout << PAGE_SIZE << '\t' << REAL_SIZE << '\t' << RESERVE_SIZE << endl;

  do{

    cin >> action;

    switch(action) {

    case 'P' :  // Loads process (P n p) => (action size pid)
      cin >> n >> pid;
      cout << "Loading process..." << endl;
      break;

    case 'A' :  // Access process (A d p m) => (action virtualAddress pid type)
      cin >> address >> pid >> type;
      cout << "Acessing process..." << endl;
      break;

    case 'L' :  // Free every page of process (L p) => (action pid)
      cin >> pid;
      cout << "Freeing process..." << endl;
      break;

    case 'C' :  // Comment line entered (C l) => (action lineComment)
      getline(cin, line);
      cout << "Loading comment..." << endl;
      break;
    }


  }while( action != 'E' );

  cout << "Program ends here..." << endl;

  return 0;
}
