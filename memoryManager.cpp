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

int swapIns;              // Holds the count of every swapIn
int swapOuts;             // Holds count of every swapOut
int pageFaults;           // Holds count of every pageFault

queue<int> paginasDispM;  // Holds available pages for use in Real Memory
queue<int> paginasDispS;  // Holds available pages for use in Reserve

vector<int> M;            // Real Memory
vector<int> S;            // Reserve Memory

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

// Function that clears and initializes every variable
void init(){
  cout << "Initializing Stuff..." << endl;

  // Initialize global variables to 0
  cout << "Variables..." << endl;
  swapIns = swapOuts = pageFaults = 0;
  cout << "DONE..." << endl;


  // Clearing vectors, and initializing them
  cout << "Clearing and initing memory..." << endl;
  M.clear();
  M.reserve(REAL_SIZE);
  M.assign(REAL_SIZE, PAGE_SIZE);
  S.clear();
  S.reserve(RESERVE_SIZE);
  S.assign(RESERVE_SIZE, PAGE_SIZE);
  cout << "DONE..." << endl;


  // Emptying queues
  cout << "Emptying queues..." << endl;
  while( !paginasDispM.empty() ) paginasDispM.pop();
  while( !paginasDispS.empty() ) paginasDispS.pop();
  cout << "DONE..." << endl;

  int totalPages = REAL_SIZE/PAGE_SIZE;

  // Add avaialable pages to queues
  cout << "Filling queues..." << endl;
  // Real Memory
  for(int i = 0; i < totalPages; i++){
    paginasDispM.push(i);
  }

  totalPages = RESERVE_SIZE/PAGE_SIZE;

  //Reserve memory
  for(int i = 0; i < totalPages; i++){
    paginasDispS.push(i);
  }
  cout << "DONE..." << endl;

  // Clearing map
  cout << "Clearing map..." << endl;
  tablaMem.clear();
  cout << "DONE..." << endl;

};


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
