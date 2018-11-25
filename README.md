# memorySimulator

## Wiki

Default sizes:
- Page Size = 16
- Real Memory Size = 2048
- Reserve Memory Size = 4096

NOTE:
  **-D** flag changes those variables during compilation time...

  **RUN WITH FIFO TECHNIQUE**
```
  g++ memoryManager.cpp -o fifo
  ./fifo
```

  **RUN WITH LRU TECHNIQUE**
```
  g++ memoryManager.cpp -DTECHNIQUE=\"LRU\" -o lru
  ./lru
```

  **TESTING w Different Memory Values**
```
  g++ memoryManager.cpp -DPAGE_SIZE=16 -DREAL_SIZE=32 -o mem && ./mem
```


OUTPUT:
```
Initializing Stuff...
Variables...
DONE...
Clearing and initing memory...
DONE...
Emptying queues...
DONE...
Filling queues...
DONE...
Clearing map...
Destroying pointers...
DONE...
DONE...
Memory sizes (bytes):
Page	Real	Reserve
16	32	4096
---------------------------------------------------------
P 16 1
Loading process...
P 16 1
Assign 16 bytes to process 1.
QUEUE SIZE: 2
	Pages needed: 1
	Pages Assigned(index): 0
	Process 1 sucesfully inserted.
Process (PID=1) info:
	Ref Bit: 0
	Timestamp: 0
	Page Faults: 0
	Pages currently used(index): 0:0
GLOBAL timestamp incremented to: 1.
---------------------------------------------------------
P 32 2
Loading process...
P 32 2
Assign 32 bytes to process 2.
QUEUE SIZE: 1
Removing process 1
Process (PID=1) info:
	Ref Bit: 0
	Timestamp: 0
	Page Faults: 0
	Pages currently used(index): 0:0
Freeing pages: pageIndex: 0 = 0
0
Size of process(PID=1) to be removed: 16.
Moving process (PID=1) to Reserve Memory...
Assigning pages(index): 0
Process: 1 succesfully sent to Reserve.
SIZEAVAILABLE: 32
	Pages needed: 2
	Pages Assigned(index): 1 0
	Process 2 sucesfully inserted.
Process (PID=2) info:
	Ref Bit: 0
	Timestamp: 1
	Page Faults: 0
	Pages currently used(index): 1:0 0:0
GLOBAL timestamp incremented to: 2.
---------------------------------------------------------
P 8 3
Loading process...
P 8 3
Assign 8 bytes to process 3.
QUEUE SIZE: 0
Removing process 2
Process (PID=2) info:
	Ref Bit: 0
	Timestamp: 1
	Page Faults: 0
	Pages currently used(index): 1:0 0:0
Freeing pages: pageIndex: 1 = 0
1 pageIndex: 0 = 0
0
Size of process(PID=2) to be removed: 32.
Moving process (PID=2) to Reserve Memory...
Assigning pages(index): 1 2
Process: 2 succesfully sent to Reserve.
SIZEAVAILABLE: 32
	Pages needed: 1
	Pages Assigned(index): 1
	Process 3 sucesfully inserted.
Process (PID=3) info:
	Ref Bit: 0
	Timestamp: 2
	Page Faults: 0
	Pages currently used(index): 1:8
GLOBAL timestamp incremented to: 3.
---------------------------------------------------------
```
