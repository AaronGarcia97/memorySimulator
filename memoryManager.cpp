#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>

using namespace std;

/* You can change these variables during compilation
   for testing purposes, with following command:
     Changing REAL_SIZE to 80:
        g++ memoryManager.cpp -DREAL_SIZE=80 -o mem
     Changing REAL_SIZE to 80 && RESERVE_SIZE to 160:
        g++ memoryManager.cpp -DREAL_SIZE=80 -DRESERVE_SIZE=160 -o mem
*/
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

class ProcessInfo{

  public:

    int pid;
    bool bitRef;
    int timeStamp, pageFaults;
    vector <int> pagesUsed;

    // Empty default constructor
    ProcessInfo(){

    }

    //Dummy constructor, to test data
    ProcessInfo(int pid, bool bitRef, int timeStamp, int pageFaults){
      this->pid = pid;
      this->bitRef = bitRef;
      this->timeStamp = timeStamp;
      this->pageFaults = pageFaults;
    }

    void printProcessInfo(){
      cout << "PID: " << this->pid << endl;
      cout << "\tRef Bit: " << this->bitRef << endl;
      cout << "\tTimestamp: " << this->timeStamp << endl;
      cout << "\tPage Faults: " << this->pageFaults << endl;
      cout << "\tPages currently used(index): ";
      for( int i = 0; i < this->pagesUsed.size(); i++){
        cout << pagesUsed[i] << " ";
      }
      cout << endl;
    }

};


unordered_map<int, ProcessInfo*> tablaMem;   // Table which will have info of each process

// Function that destroys pointers to void memory leaks
void destroyMapPointers(){
  cout << "Destroying pointers..." << endl;
  unordered_map<int, ProcessInfo*>::iterator itr = tablaMem.begin();
  while( itr != tablaMem.end()){
    ProcessInfo *pi = itr->second;
    delete pi;
    ++itr;
  }
  cout << "DONE..." << endl;
}

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
  destroyMapPointers();
  tablaMem.clear();
  cout << "DONE..." << endl;

};


// Returns size available in Real Memory
int getMSizeAvailable(){
  return paginasDispM.size() * PAGE_SIZE;
}


// Returns size available in Reserve Memory
int getSSizeAvailable(){
  return paginasDispS.size() * PAGE_SIZE;
}


/* Function which loads process into realMemory
   Receives:
    n = size in bytes
    pid = process ID
   If process doesn't fit in realMemory, sends a process to Reserve
   Memory to free off space.
*/
int loadProcess(int &n, int &pid){
  int sizeAvailable = getMSizeAvailable();

  if( n > REAL_SIZE ) {
    cout << "\tCan't allocate more memory than " << REAL_SIZE << " bytes..." << endl;
    return -1;
  }

  if( n < 1 ) {
    cout << "\tCan't allocates less than 1 bytes..." << endl;
    return -1;
  }

  while( n > sizeAvailable ) {
      // Free memory sending it to reserve
      // freeSpace();
      // Get size updated
      sizeAvailable = getMSizeAvailable();
  }

  // Enough memory was freed
  // Load process into real memory
  double tmp = double(n)/double(PAGE_SIZE);
  int pagesNeeded = ceil(tmp);
  cout << "\tPages needed: " << pagesNeeded << endl;

  // Initialize process info and create entry in memory
  ProcessInfo *pi = new ProcessInfo(pid, 0, 0, 0);
  tablaMem[pid] = pi;

  // Validate process was inserted
  if (tablaMem.find(pid) == tablaMem.end()){
    cout << "\tProcess insertion failed..." << endl;
    return -1;
  }

  // Get ProcessInfo pointer we are working with
  pi = tablaMem[pid];

  // Fill amount of bytes used and which pages the process uses
  cout << "\tPages Assigned(index): ";
  for(int i = 0; i < pagesNeeded; i++){
    int pageNum = paginasDispM.front();

    pi->pagesUsed.push_back(pageNum);

    if( i+1 == pagesNeeded ) {
      // If is last page used, take only required bytes
      int lastBytesOfInfo = n%PAGE_SIZE;
      M[pageNum] -= lastBytesOfInfo;
    } else {
      // If is not last page, take the whole page
      M[pageNum] -= PAGE_SIZE;
    }

    cout << pageNum << " ";

    // Pop page used
    paginasDispM.pop();
  }

  cout << "\n\tProcess " << pid << " sucesfully inserted." << endl;
  pi->printProcessInfo();
  return 1;

}

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
      cout << action << " " << n << " " << pid << endl;
      cout << "Assign " << n << " bytes to process " << pid << "." << endl;
      loadProcess(n, pid);
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
  destroyMapPointers();
  cout << "Goodbye :)" << endl;

  return 0;
}
