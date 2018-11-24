#include <iostream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <string>

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

#ifndef TECHNIQUE
#define TECHNIQUE "FIFO"
#endif

int swapIns;              // Holds the count of every swapIn
int swapOuts;             // Holds count of every swapOut
int pageFaults;           // Holds count of every pageFault
int tStamp;               // Holds current cirtual tStamp

queue<int> paginasDispM;  // Holds available pages for use in Real Memory
queue<int> paginasDispS;  // Holds available pages for use in Reserve

vector<int> M;            // Real Memory
vector<int> S;            // Reserve Memory

class ProcessInfo{

  public:

    int pid;
    bool bitRef;
    int timeStamp, pageFaults, timeStampLRU;
    vector <int> pagesUsed;

    // Empty default constructor
    ProcessInfo(){

    }

    //Dummy constructor, to test data
    ProcessInfo(int pid, bool bitRef, int timeStamp, int pageFaults, int timeStampLRU){
      this->pid = pid;
      this->bitRef = bitRef;
      this->timeStamp = timeStamp;
      this->timeStampLRU = timeStampLRU;
      this->pageFaults = pageFaults;
    }

    void printProcessInfo(){
      cout << "Process (PID=" << this->pid << ") info: "  << endl;
      cout << "\tRef Bit: " << this->bitRef << endl;
      cout << "\tTimestampFIFO: " << this->timeStamp << endl;
      cout << "\tTimestampLRU: " << this->timeStampLRU << endl;
      cout << "\tPage Faults: " << this->pageFaults << endl;
      cout << "\tPages currently used(index): ";
      for( int i = 0; i < this->pagesUsed.size(); i++){
        cout << pagesUsed[i] << ":";
        if( this->bitRef == 0 ) cout << M[pagesUsed[i]];
        else cout << S[pagesUsed[i]];
        cout << " ";
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
  swapIns = swapOuts = pageFaults = tStamp = 0;
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

/*
 Returns size of process in bytes.
 Since we init memory with PAGE_SIZE and decrement it if we insert process,
 we need to return the difference of PAGE_SIZE - remainingBytes of Page.
*/
int getUsedBytesOfPage(int& pageIndex){
  cout << "pageIndex: " << pageIndex << " = " << M[pageIndex] << endl;
  return PAGE_SIZE - M[pageIndex];
}

/*
  Returns which PID you should remove in FIFO technique.
  Iterate through every PID which is in Real Memory
  and return the one with lowest timeStamp.
  (((Inefficient method)))
*/
int getPIDtoRemove(string s){

  if( s != "FIFO" || s != "LRU" ) {
    cout << "Unexistent technique..." << endl;
    return -1;
  }

  bool isFifo;
  int actualProcessTimeStamp = 0;
  int minTimeStamp = 0;
  int pidMinTimestamp = -666;
  unordered_map<int, ProcessInfo*>::iterator itr = tablaMem.begin();

  isFifo = (s == "FIFO") ? true : false;

  while( itr != tablaMem.end() ){

    ProcessInfo *process = itr->second;
    actualProcessTimeStamp = (isFifo) ? process->timeStamp : process->timeStampLRU;

    if( process->bitRef == 0 ) { //If process is in real memory

      if( pidMinTimestamp != -666 )
        minTimeStamp = isFifo ? tablaMem[pidMinTimestamp]->timeStamp : tablaMem[pidMinTimestamp]->timeStampLRU;

      if( pidMinTimestamp == -666 ) pidMinTimestamp = process->pid;

      else if( actualProcessTimeStamp < minTimeStamp ){
        // Get PID of min timestamp
        pidMinTimestamp = process->pid;

      }
    }
    ++itr;
  }

  return pidMinTimestamp;
}

/*
  Function which loads a process into Reserve memory. Similar to loadProcess()
  function in this same doc.
*/
int sendToReserve(int &pid, int &n){
  //loadProcess but to reserve instead of real memory
  cout << "Size of process(PID=" << pid << ") to be removed: " << n << "." << endl;
  int sizeAvailable = getSSizeAvailable();

  // Validations
  if( n > sizeAvailable ) {
    cout << "Not enough memory, can't save on Reserve." << endl;
    return -1;
  } else if( n < 1 ) {
    cout << "A process with size less than 1, can't exist." << endl;
    return -1;
  }

  cout << "Moving process (PID=" << pid << ") to Reserve Memory..." << endl;

  // If it arrives here, it means there's enough memory
  ProcessInfo *pi = tablaMem[pid];
  int pagesNeeded = ceil(double(n)/double(PAGE_SIZE));

  // Clear past pages
  pi->pagesUsed.clear();

  // Allocate in Reserve Memory
  // Iterate until pagesNeeded-1
  cout << "Assigning pages(index): ";
  for( int i = 0; i < pagesNeeded-1; i++) {
    // Get next available memory location
    int pageNum = paginasDispS.front();

    // Add pageIndex to process info
    pi->pagesUsed.push_back(pageNum);

    // Indicate bytes used
    S[pageNum] -= PAGE_SIZE;

    paginasDispS.pop();

    cout << pageNum << " ";
  }
  cout << endl;
  // Fill last memory location
  int lastPage = paginasDispS.front();
  int lastBytesUsed = n%PAGE_SIZE;
  paginasDispS.pop();

  cout << "Last page of process: " << lastPage << endl;

  // If its 0 it means it used whole page, else only bytes left
  // If processSize = 17, it will subtract 1
  // If processSize = 16 = page_size, then it will subtract page_size
  if( lastBytesUsed == 0 ) S[lastPage] -= PAGE_SIZE;
  else S[lastPage] -= lastBytesUsed;

  // Add pageIndex to process info
  pi->pagesUsed.push_back(lastPage);

  // Make reference bit 1, telling that it's in Reserve Memory
  pi->bitRef = 1;

  return 1;
}

/*
  Function which fressSpace using FIFO technique,
  First In First Out...
*/
int freeSpace(string technique){

  if( technique != "FIFO" || technique != "LRU" ) {
    cout << "Unexistent technique..." << endl;
    return -1;
  }

  int pidToRemove = getPIDtoRemove(technique);
  int sizeOfProcessToRemove = 0;
  // We makeing bad assumption taht pid returned is that of a process who exists
  ProcessInfo *pi = tablaMem[pidToRemove];

  cout << "Removing process " << pidToRemove << endl;
  pi->printProcessInfo();

  // For every page used for that process add it back
  // to queue of available memory
  cout << "Freeing pages: ";
  for( int i = 0; i < pi->pagesUsed.size(); i++ ) {
    int pageIndex = pi->pagesUsed[i];

    // Get bytes of process to be sent to Reserve
    sizeOfProcessToRemove += getUsedBytesOfPage(pageIndex);
    // Give back memory of that page
    M[pageIndex] = PAGE_SIZE;
    cout << pageIndex << " ";
    // Add it to available memory pages queue
    paginasDispM.push(pageIndex);
  }
  cout << endl;

  // SEND pidToRemove process to reserve
  if ( sendToReserve(pidToRemove, sizeOfProcessToRemove) > 0 ){
    cout << "Process: " << pidToRemove << " succesfully sent to Reserve." << endl;
    swapOuts++;
  }
  else {
    // An error happened, erase process...
    cout << "Process: " << pidToRemove << " wasn't sent to Reserve." << endl;
    tablaMem.erase(pidToRemove);
  }

  return 1;
}

/*
  Function that liberates pages of a process who is in Reserve Memory.
*/
int liberateReserveProcess(ProcessInfo* &process){
  double secondsPassed = 0;
  int bitRef = process->bitRef;

  // Validate process is in Reserve
  if( bitRef == 0 ) {
    cout << "Process (PID=" << process->pid << ") is in Real Memory";
    return -1;
  }

  int pagesUsedSize = process->pagesUsed.size();

  cout << "Liberating pages from Process(PID=" << process->pid << ") in Reserve: " << endl;

  for( int i = pagesUsedSize-1; i >= 0; i-- ){
    int pageIndex = process->pagesUsed[i];

    // Push page back to availablePages queue
    paginasDispS.push(pageIndex);

    // Give bytes back
    S[pageIndex] = PAGE_SIZE;

    // Pop page from process vector pagesUsed
    process->pagesUsed.pop_back();

    cout << "\tPage (" << pageIndex << ") liberated." << endl;
    secondsPassed += 0.1;

  }

  cout << "Process (PID=" << process->pid << ") pages succesfully liberated." << endl;
  cout << "Total seconds elapsed: " << secondsPassed << endl;

  return 1;

}

/*
  Function which loads process into realMemory
    Receives:
      n = size in bytes
      pid = process ID
   If process doesn't fit in realMemory, sends a process to Reserve
   Memory to free off space.
*/
int loadProcess(int &n, int &pid, bool isSendingFromReserveToMemory){
  int sizeAvailable = getMSizeAvailable();
  int amountOfPageFaults = 0;

  // Validates that process is not bigger than memory's size
  if( n > REAL_SIZE ) {
    cout << "\tCan't allocate more memory than " << REAL_SIZE << " bytes..." << endl;
    return -1;
  }

  // Validates that process is greater than 0 bytes
  if( n < 1 ) {
    cout << "\tCan't allocates less than 1 bytes..." << endl;
    return -1;
  }

  // Validate that pid doesn't exist yet
  if( tablaMem.find(pid) != tablaMem.end() && !isSendingFromReserveToMemory ) {
    cout << "\tProcess with PID=" << pid << " already exists..." << endl;
    return -1;
  }

  cout << "Pages available: " << paginasDispM.size() << endl;

  while( n > sizeAvailable ) {
      // Free memory sending it to reserve
      // Take account of pageFaults that happened during this
      // Only if process is being loaded for first time
      if ( freeSpace(TECHNIQUE) > 0  && !isSendingFromReserveToMemory ) {
        amountOfPageFaults++;
      }
      // Get size updated
      sizeAvailable = getMSizeAvailable();
      cout << "Real Memory Size Available: " << sizeAvailable << endl;
  }

  // Enough memory was freed
  // Load process into real memory
  double tmp = double(n)/double(PAGE_SIZE);
  int pagesNeeded = ceil(tmp);
  cout << "\tPages needed: " << pagesNeeded << endl;

  ProcessInfo *pi;

  // Initialize process info and create entry in memory
  if( !isSendingFromReserveToMemory ) { // If process doesn't exist already

    pi = new ProcessInfo(pid, 0, tStamp, 0, tStamp);
    tablaMem[pid] = pi;

    // Validate process was inserted
    if (tablaMem.find(pid) == tablaMem.end()){
      cout << "\tProcess insertion failed..." << endl;
      return -1;
    }

  }

  // Get Process pointer we are working with
  pi = tablaMem[pid];

  // Liberate pages from process from reserve, si falla, return error
  if( isSendingFromReserveToMemory ) {
    if( liberateReserveProcess(pi) < 0 ) return -1;
    else pi->bitRef = 0;
  }

  // Fill amount of bytes used and which pages the process uses
  cout << "\tPages Assigned(index): ";
  for(int i = 0; i < pagesNeeded; i++){
    int pageNum = paginasDispM.front();

    pi->pagesUsed.push_back(pageNum);

    if( i+1 == pagesNeeded && n%PAGE_SIZE != 0) {
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
  cout << endl;

  cout << "\tProcess " << pid << " sucesfully inserted." << endl;
  pi->printProcessInfo();

  // Add page faults to process, if it has
  pi->pageFaults += amountOfPageFaults;
  pageFaults += amountOfPageFaults;

  return 1;

}

/*
  Function which receives process and address, and prints virtual memory
  address and real memory address.
*/
int showAddresses(int &address, ProcessInfo* &process){

  int realAddress;
  int virtualAddress;

  // Gets you page in which address is located on its own virtual memory
  virtualAddress = address / PAGE_SIZE;

  // Gives you page in which address is located on real memory
  realAddress = process->pagesUsed[virtualAddress];

  cout << "Virtual address: " << virtualAddress << "." << endl;
  cout << "Real Memory address: " << realAddress << "." << endl;

  return 1;

}

int getUsedBytesOfProcess(ProcessInfo* &process){

  int size = 0;
  int pagesUsedSize = process->pagesUsed.size();
  int lastPage = process->pagesUsed[pagesUsedSize-1];

  size += (pagesUsedSize - 1)*PAGE_SIZE + getUsedBytesOfPage(lastPage);

  return size;
}

/*
  Function which receives process to be sent from Reserve to Real Memory
*/
int sendProcessToMemory(ProcessInfo* &process){

  int size = 0;

  size = getUsedBytesOfProcess(process);

  if ( loadProcess(size, process->pid, true) > 0 ) { //Means process was sent succesfully to memory

    // Update timestamp of process
    process->timeStamp = tStamp;
    return 1;

  } else {
    return -1;
  }

  return 1;

}

/*
  Function which receives an address to be accessed and a pid of a process,
  whose address is going to be accessed.
*/
int accessProcess(int &address, int &pid){

  // Check that process is in memory
  if( tablaMem.find(pid) == tablaMem.end() ){
    cout << "Process(PID=" << pid << ") doesn't exist." << endl;
    return -1;
  }


  ProcessInfo *process = tablaMem[pid];
  int bitRef = process->bitRef;

  // Check if process is in Reserve or Real memory
  if( bitRef == 0 ){ // Real

    cout << "Process(PID=" << pid << ") is in REAL Memory" << endl;
    showAddresses(address, process);

  } else { // Reserve

    cout << "Process(PID=" << pid << ") is in RESERVE Memory" << endl;
    if( sendProcessToMemory(process) > 0 ) {
      showAddresses(address, process);
      // Since process was swappedIn succesfully, we increment swapIn
      // and pageFault, both local and global
      process->pageFaults++;
      swapIns++;
      pageFaults++;
    }

  }

  return 1;
}


/*
  Function which liberates the pages used by a process located in Real
  Memory. Receives a Process Info pointer.
*/
int liberateRealMemoryProcess(ProcessInfo* &process){

  int pagesSize = process->pagesUsed.size();
  double secondsPassed = 0;

  cout << "Liberating process(PID=" << process->pid << ") from Real Memory." << endl;

  // For every page used for that process add it back
  // to queue of available memory
  for( int i = pagesSize-1; i >= 0; i-- ) {

    int pageIndex = process->pagesUsed[i];

    // Add it back to availablePages queue
    paginasDispM.push(pageIndex);

    // Give bytes back
    M[pageIndex] = PAGE_SIZE;

    // Pop page from process vector pagesUsed
    process->pagesUsed.pop_back();

    cout << "\tPage (" << pageIndex << ") liberated." << endl;
    secondsPassed += 0.1;
  }

  cout << "Process (PID=" << process->pid << ") pages succesfully liberated." << endl;
  cout << "Total seconds elapsed: " << secondsPassed << endl;

  return 1;
}

/*
  Function that liberates process with received pid, if is in reserve it
  takes differente actions as if it was on Real Memory.
*/
int liberateProcess(int &pid) {

  // Check that process is in memory
  if( tablaMem.find(pid) == tablaMem.end() ){
    cout << "Process(PID=" << pid << ") doesn't exist." << endl;
    return -1;
  }

  ProcessInfo *process = tablaMem[pid];
  int bitRef = process->bitRef;

  if ( bitRef == 0 ) { // Process is in Real Memory
    return liberateRealMemoryProcess(process);
  } else {             // Process is in Reserve Memory
    return liberateReserveProcess(process);
  }

  return -1;

}

int main(int argc, char *argv[]){

  int n, pid, address, type;
  string line = "";
  char action = 'E';

  cout << "Technique used: " << TECHNIQUE << endl;

  init();

  cout << "Memory sizes (bytes): " << endl;
  cout << "Page\tReal\tReserve" << endl;
  cout << PAGE_SIZE << '\t' << REAL_SIZE << '\t' << RESERVE_SIZE << endl;

  do{
    cout << "---------------------------------------------------------" << endl;
    cin >> action;

    switch(action) {
    case 'P' :  // Loads process (P n p) => (action size pid)
      cin >> n >> pid;
      cout << "Loading process..." << endl;
      cout << action << " " << n << " " << pid << endl;
      cout << "Assign " << n << " bytes to process " << pid << "." << endl;
      loadProcess(n, pid, false);
      break;

    case 'A' :  // Access process (A d p m) => (action virtualAddress pid type)
      cin >> address >> pid >> type;
      cout << "Acessing process..." << endl;
      accessProcess(address, pid);
      break;

    case 'L' :  // Free every page of process (L p) => (action pid)
      cin >> pid;
      cout << "Liberating process..." << endl;
      liberateProcess(pid);
      break;

    case 'C' :  // Comment line entered (C l) => (action lineComment)
      getline(cin, line);
      cout << "Comment(time=" << tStamp << "):" << line << endl;
      break;

    }

    // Increment by 1 global timestamp
    tStamp++;
    cout << "GLOBAL timestamp incremented to: " << tStamp << "." << endl;


  }while( action != 'E' );

  cout << "Program ends here..." << endl;
  destroyMapPointers();
  cout << "Goodbye :)" << endl;

  return 0;
}
