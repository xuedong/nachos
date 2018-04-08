/* ----------------------------------------

         Test of all Nachos system calls
	 and library functions

  ---------------------------------------- */

// For Nachos system calls
#include "userlib/syscall.h"

// For nachos library functions
#include "userlib/libnachos.h"

#define Dim 	20	/* sum total of the arrays doesn't fit in 
			 * physical memory 
			 */
int A[Dim][Dim];
int B[Dim][Dim];
int C[Dim][Dim];

void err(char *msg,int code_err) {
  if (code_err == -1) {
    PError(msg);
    Exit(0);
  }
}


/** Signale une erreur ATTENDUE */
void expect_err(char *msg, int code_err) {
  if (code_err == -1) {
    char *prefix = "Expected error for %s";
    char s[n_strlen(prefix)+n_strlen(msg)]; // Stack dyn alloc = GCC extension
    n_snprintf(s, sizeof(s), prefix, msg);
    PError(s);
  } else {
    n_printf("Expected error for %s: No error detected!\n", msg);
    Exit(0);
  }
}

/* Attend que l'utilisateur tape entree */
void pause()
{
  char c;
  n_printf("[PAUSE] Hit <ENTER> to continue...\n");
  do {
    Read(&c, 1, ConsoleInput);
  } while (c != '\n');
}


void f(void)
{
  static int k = 0;
  int i;

  n_printf("   Thread %d started ... Yielding \n",k++);
  i = k;
  Yield();	
  n_printf("   Thread restarted %d \n",i);
}

#define TAILLE 5

int Info[TAILLE];
SemId NbVide, NbPlein, FinFils;
ThreadId TProc1,TProc2;

// Nombre de productions et de consommation
const int NbProd=9;

int Producteur(int arg)
{  int iVide=TAILLE-1;
   int i=0;
   
   n_printf("   Beginning production\n");
   for (i=0; i<NbProd; i++)
   {
     err("tst_sema(PPvide)",P(NbVide));
     iVide=(iVide+1)%TAILLE;
     Info[iVide]='a'+i;
     n_printf("   Producing : %c\n",Info[iVide]);
     err("tst_sema(Vplein)",V(NbPlein));
   }

   err("tst_sema(Vfinfils)",V(FinFils));
   return 0;
}

int Consommateur(int arg)
{  int iPlein=TAILLE-1;
   int i=0;
   
   n_printf("   Beginning consumption\n");
   for (i=0; i<NbProd; i++)
   {   
     err("tst_sema(Pplein)",P(NbPlein));
     iPlein=(iPlein+1)%TAILLE;
     n_printf("   Consumming : %c\n",Info[iPlein]);
     err("tst_sema(Vvide)",V(NbVide));
   }
   err("tst_sema(Vfinfils)",V(FinFils));
   return 0;
}

LockId lid;
SemId fin;

void f1(void)
{
  n_printf("   Thread t1 started\n");
  LockAcquire(lid);
  n_printf("   I'm t1, and I have the lock, Yielding ...\n");
  Yield();
  LockRelease(lid);
  n_printf("   Finishing t1\n");
  V(fin);
}

void f2(void)
{
  n_printf("   Thread t2 started\n");
  LockAcquire(lid);
  n_printf("   I'm t2, and I should no be there is t1 has not finished\n");
  LockRelease(lid);
  n_printf("   Finishing t2\n");
  V(fin);
}

CondId cid;

void cf2(void)
{
  n_printf("   Testing conditions: thread t2 started\n");
  n_printf("   In t2, about to wait for condition\n");
  CondWait(cid);
  n_printf("   Restarting (and finishing) t2\n");
  V(fin);
}

void cf1(void)
{
  n_printf("   Testing conditions: thread t1 started\n");
  n_printf("   In t1, about to signal condition\n");
  CondSignal(cid);
  n_printf("   In t1, condition signalled\n");
  n_printf("   Finishing t1\n");
  V(fin);
}


void test_lib_nachos(void)
{
  float f;
  double d;
  char str[80], str2[80];
  int i;

  n_printf("Testing the libnachos library\n");
  n_printf("--------------------------------\n");
  n_printf  ("   Testing different formats for n_printf (c,s,d,x,f)\n");
  n_printf  ("   Character (c) = (%c)\n",'c');
  n_printf  ("   String (toto) = (%s)\n","toto");
  n_printf  ("   Decimal (0) = (%d)\n",0);
  n_printf  ("   Decimal (1) = (%d)\n",1);
  n_printf  ("   Decimal (-1) = (%d)\n",-1);
  n_printf  ("   Decimal (5544) = (%d)\n",5544);
  n_printf  ("   Decimal (-5544) = (%d)\n",-5544);
  n_printf  ("   Decimal (2147483647) = (%d)\n",2147483647);
  n_printf  ("   Decimal (-21483648) = (%d)\n",-21483648);
  n_printf  ("   Hexadecimal (0xffff64) = (%x)\n",0xffff64);
  n_printf  ("   Float (0.0) = %f\n", 0.0);
  n_printf  ("   Float (678.12345) = %f\n", 678.12345);

  d = 12.3456789;
  f = d / 3.29;
  d = f * -2.27;
  n_printf  ("   Double/Float conv. (3.752485987 => arrondi 3.75249) = %f\n", f);
  n_printf  ("   Double/Float/Double conv. (-8.518143190 => arrondi -8.51814) = %f\n", d);

  n_printf  ("   Format specials (percent) = %% + (percent k) = %k\n");
  n_printf  ("   String (x) = (%s) + decimal (4) = (%d)\n","x",4);

  if (n_strcmp("toto","toto")) {
    n_printf("   String comparison NOK\n");
  }
  n_snprintf(str, 10, "Toto et %s sont sur un %s\n", "titi", "bateau");
  if (str[9] != '\0') {
    n_printf("   String format truncated NOK\n");
  }
  n_snprintf(str, sizeof(str), "Toto et %s sont sur un %s\n", "titi", "bateau");
  if (n_strcmp(str, "Toto et titi sont sur un bateau\n")) {
    n_printf("   String format NOK\n");
  }  
  n_printf("%s\n","String to be copied");
  n_strcpy(str,"String to be copied");
  if (n_strcmp(str,"String to be copied")) {
    n_printf("   String copy NOK\n");
    n_printf("   Len(str) %d len(source) %d\n",n_strlen(str),n_strlen("String to be copied"));
    n_printf("   car 0 %c\n",str[0]);
  }
  if (n_strlen("toto") != 4) {
    n_printf("   strlen NOK\n");
  }
  n_strcpy(str,"Message ");
  n_printf("Message %s\n",str);
  n_strcat(str,"1");
  if (n_strcmp(str,"Message 1")) {
    n_printf("   strcat NOK\n");
    n_printf("%s\n",str);
  }
  if (n_toupper('c') != 'C') {
    n_printf("   toupper NOK\n");
  }
  if (n_tolower('C') != 'c') {
    n_printf("   tolower NOK\n");
  }
  if (n_atoi("-144") != -144) {
    n_printf("   atoi NOK\n");
  }
  n_memcpy(str,str2,80);
  if (n_memcmp(str,str2,80)) {
    n_printf("   bcopy or bcmp NOK\n");
  }
  n_memset(str,0,80);
  for (i=0;i<80;i++) {
    if (str[i]!=0) {
      n_printf("   bzero NOK\n");
    }
  }
  n_printf("Entrer un entier: \n");
  i = n_read_int();
  n_printf("   Entier lu %d\n",i);
  pause();
}

void test_time(void)
{
  Nachos_Time t1,t2;
  int i,j;

  n_printf  ("3. Testing time management\n");
  n_printf("----------------------------------------\n");
  SysTime (&t1);
  n_printf("   Time t1 %d sec %d nanos\n",t1.seconds,t1.nanos);
  for (i=0;i<10;i++) j=6*i;
  SysTime (&t2);
  n_printf("   Time t2 %d sec %d nanos\n",t2.seconds,t2
.nanos);
  pause();
}

void test_process(void)
{  
  ThreadId threadid;

  n_printf("4. Testing process exec/join\n");
  n_printf("----------------------------------------\n");
  err("tst_process(Exec)",threadid = Exec("/testnewpss"));
  n_printf("   Waiting for process to finish\n");
  err("Join", Join(threadid));
  expect_err("Exec(invalid file name)", Exec("/existepas"));
  pause();

}

void test_thread(void)
{
  int i;
  ThreadId tid[10];
 
  n_printf("Testing multithreading\n");
  n_printf("----------------------\n");
  for (i=0;i<10;i++) {
    err("tst_thread(threadCreate)",tid[i] = 
	threadCreate("mt_test", (VoidNoArgFunctionPtr)&f));
    n_printf("Id of thread number %d = %d\n",i,tid[i]);
  }
  Yield();
  Yield();
  pause();
}
void test_mmap(void) {
  OpenFileId fid;
  int i;
  int buf[1];

  // Test writing into file
  n_printf("   Test of file contents: writing file\n");
  err("tst_file(Create)",Create("mmap",1024));
  err("tst_file(Open)",fid = Open("mmap"));
  for (i=0;i<100;i++) {
    buf[0] = i;
    err("tst_file(Write)",Write((char *)buf,sizeof(int),fid));
  }
  n_printf("   Checking contents\n");
  err("tst_file(Close)",Close(fid));
  err("tst_file(Open)",fid = Open("mmap"));
  for (i=0;i<100;i++) {
    err("tst_file(Read)",Read((char *)buf,sizeof(int),fid));
    n_printf("%d ",buf[0]);
    if (buf[0]!=i) n_printf("   Contents NOK\n");
  }
  err("tst_file(Close)",Close(fid));
  n_printf("   Contents OK\n");

  // Test mapped files
  err("tst_file(Open)",fid = Open("mmap"));
  int *addr = (int *)Mmap(fid,100*sizeof(int));
  n_printf("   Mapping file in memory\n");
  n_printf("   Address map %x\n",addr);
  if ((int)addr ==-1) {
    n_printf("Argh ... Mmap failed\n"); return;
  }
  n_printf("Writing into mapped file:\n");
  for (i=0;i<100;i++) {
    addr[i] = -1*i;
  }
  n_printf("   Checking contents\n");
  for (i=0;i<100;i++) {
    if (addr[i] != -1*i) {
      n_printf("Contents NOK\n"); return;
    }
    else n_printf("%d ",addr[i]);
  }
  n_printf("   Contents OK\n");

  // Re-test from file itself
  
  n_printf("   Test of file contents: writing file\n");
  err("tst_file(Open)",fid = Open("mmap"));
  n_printf("   Checking contents\n");
  err("tst_file(Close)",Close(fid));
  err("tst_file(Open)",fid = Open("mmap"));
  for (i=0;i<100;i++) {
    err("tst_file(Read)",Read((char *)buf,sizeof(int),fid));
    n_printf("%d ",buf[0]);
    if (buf[0]!=-1*i) n_printf("   Contents NOK\n");
  }
  err("tst_file(Close)",Close(fid));
  n_printf("   Contents OK\n");

  pause();
}

void test_file(void)
{
  OpenFileId fid;
  int i;
  int buf[1];

  n_printf("Testing file system operations\n");
  n_printf("------------------------------\n");
  expect_err("tst_file(Create invalid dir)",Create("/inexist/toto",200));
  expect_err("tst_file(Mkdir invalid dir)",Mkdir("/inexist/toto"));

  err("tst_file(MkDir)",Mkdir("/titi"));
  n_printf("   Dir /titi created\n");
  expect_err("tst_file(MkDir exist dir)",Mkdir("/titi"));
  expect_err("tst_file(Create exist dir)",Create("/titi",200));

  err("tst_file(Create)",Create("/titi/toto",200));
  n_printf("   File /titi/toto created\n");
  expect_err("tst_file(Create exist file)",Create("/titi/toto",200));
  expect_err("tst_file(MkDir exist file)",Mkdir("/titi/toto"));

  err("tst_file(Open)",fid = Open("/titi/toto"));
  n_printf("   File /titi/toto opened\n");
  for (i=0;i<100;i++) {
    buf[0] = i;
    err("tst_file(Write)",Write((char *)buf,sizeof(int),fid));
  }
  n_printf("   File /titi/toto filled\n");
  err("tst_file(Seek)",Seek(0,fid));
  n_printf("   Seek OK\n");
  for (i=0;i<100;i++) {
    err("tst_file(Read)",Read((char*)buf,sizeof(int),fid));
    if (buf[0]!=i) {
      n_printf("   Argh, read data is not correct byte %d = %d\n",i,buf[0]);
    }
  }
  n_printf("   Read OK\n");
  err("tst_file(Close)",Close(fid));
  n_printf("   File /titi/toto closed\n");
  expect_err("Remove should fail (file does not exist)",Remove("/tata"));
  FSList();

  expect_err("tst_file(Remove dir)",Remove("/titi"));
  expect_err("tst_file(Rmdir non empty dir)",Rmdir("/titi"));

  expect_err("tst_file(Rmdir file)",Rmdir("/titi/toto"));
  err("tst_file(Remove)",Remove("/titi/toto"));

  expect_err("tst_file(Remove empty dir)",Remove("/titi"));
  err("tst_file(Rmdir empty dir)",Rmdir("/titi"));
  n_printf("   Printing the contents of the file system\n");

  FSList();
  //pause();

  // Test writing into file
  n_printf("   Test of file contents: writing file\n");
  err("tst_file(Create)",Create("toto",1024));
  err("tst_file(Open)",fid = Open("toto"));
  for (i=0;i<100;i++) {
    buf[0] = i;
    err("tst_file(Write)",Write((char *)buf,sizeof(int),fid));
  }
  n_printf("   Checking contents\n");
  err("tst_file(Close)",Close(fid));
  err("tst_file(Open)",fid = Open("toto"));
  for (i=0;i<100;i++) {
    err("tst_file(Read)",Read((char *)buf,sizeof(int),fid));
    n_printf("%d %x ",buf[0],buf[0]);
  }
  err("tst_file(Close)",Close(fid));
  n_printf("   Contents OK\n");

}

void test_sema(void)
{
    /* ------------------------------------ */
    n_printf("Testing semaphores\n");
    n_printf("---------------------\n");

    err("tst_sema(Create)",NbVide=SemCreate("NbVide", TAILLE));
    err("tst_sema(Create)",NbPlein=SemCreate("NbPlein", 0));
    err("tst_sema(Create)",FinFils=SemCreate("FinFils", 0));
  
    err("tst_sema(Exec)",TProc1=threadCreate("prod", (VoidNoArgFunctionPtr)Producteur));
    err("tst_sema(Exec)",TProc2=threadCreate("cons", (VoidNoArgFunctionPtr)Consommateur));
    Yield();
    err("tst_sema(P)",P(FinFils));
    err("tst_sema(p)",P(FinFils));
    err("tst_sema(destroy)",SemDestroy(NbVide));
    err("tst_sema(destroy)",SemDestroy(NbPlein));
    err("tst_sema(destroy)",SemDestroy(FinFils));
    pause();
 }

void test_lock(void)
{
  ThreadId id1,id2;

    n_printf("Testing locks\n");
    n_printf("------------------------\n");

    fin =SemCreate("fin", 0);
    if (fin == -1) {
      n_printf("   Argh, sema creation failed\n");
    }
    lid=LockCreate("lock");
    if (lid == -1) {
      n_printf("   Argh, lock creation failed\n");
    }
    n_printf("   Lock created lid = %d\n",lid);
    id1=threadCreate("lck1", (VoidNoArgFunctionPtr)&f1);
    if (id1 == -1) {
      n_printf("   Argh, thread creation failed\n");
    }
    n_printf("   Thread 1 created\n");
    id2=threadCreate("lck2", (VoidNoArgFunctionPtr)&f2);
    if (id2 == -1) {
      n_printf("   Argh, thread creation failed\n");
    }
    n_printf("   Thread 2 created\n");
    Yield(); // Let the threads a chance to execute before
    // removing their lock
    P(fin);
    P(fin);
    if (LockDestroy(lid) == -1) {
       n_printf("   Argh, lock destruction failed\n");
    }
    pause();
}

void test_cond(void)
{
  ThreadId id1,id2;

    n_printf("Testing conditions\n");
    n_printf("-------------------\n");

    fin =SemCreate("fin", 0);
    if (fin == -1) {
      n_printf("   Argh, sema creation failed\n");
    }
    cid = CondCreate("cond");
    if (cid == -1) {
      n_printf("   Argh, condition creation failed\n");
    }
    n_printf("   Condition created cid = %d\n",cid);
    id2=threadCreate("lck2", (VoidNoArgFunctionPtr)&cf2);
    if (id2 == -1) {
      n_printf("   Argh, thread creation failed\n");
    }
    n_printf("   Thread 2 created\n");
    id1=threadCreate("lck1", (VoidNoArgFunctionPtr)&cf1);
    if (id1 == -1) {
      n_printf("   Argh, thread creation failed\n");
    }
    n_printf("   Thread 1 created\n");
    P(fin);
    P(fin);
}

void test_acia(void)
{
  int i,res;
  ThreadId threadid;
  char msg[10];

  n_printf("Testing the ACIA\n");
  n_printf("--------------------------\n");
    
  /* Start the receiver */
  threadid = Exec("/testnewpss");
  if (threadid == -1) {
    n_printf("   Process creation failed...\n");
  }
  else n_printf("   Receiver created\n");
  n_strcpy(msg,"message ");
  for (i=0;i<10;i++) {
    msg[7] = (char)('0'+i);
    n_printf("   Envoi de %s\n",msg);
    res = TtySend((char *)msg);
    n_printf("   J'ai envoye (8) = %i caracteres\n",res);
  }
  n_printf("Join ...\n");
  Join(threadid);
  pause();
}

void test_console() 
{
  char s[128];

    n_printf("Testing the console\n");
    n_printf("-----------------------\n");
    n_printf("   Enter a string (end with <ENTER>) :\n> ");
    n_memset(s, 0, sizeof(s)); // To make sure the \0 will be present
    Read(s, sizeof(s)-1, ConsoleInput);
    n_printf("   Your string was «%s»\n", s);
    n_printf("   True ???\n");
    pause();
}

void test_vm(void)
{int i,j,k;

    n_printf("Stressing virtual memory management\n");
    n_printf("-----------------------------------\n");
    n_printf("   Starting matmult\n");
    
      for (i = 0; i < Dim; i++)		/* first initialize the matrices */
	for (j = 0; j < Dim; j++) {
	  A[i][j] = i;
	  B[i][j] = j;
	  C[i][j] = 0;
	}

      for (i = 0; i < Dim; i++)		/* then multiply them together */
	for (j = 0; j < Dim; j++)
	  for (k = 0; k < Dim; k++)
	    C[i][j] += A[i][k] * B[k][j];


      n_printf("End matmult\n");
      pause();

}

void test_directory(void)
{
    n_printf("Testing directory management\n");
    n_printf("-----------------------------\n");

    if (Mkdir("/dir1") == -1) {
      n_printf("   Argh, cannot create directory dir1\n");
    }
    n_printf("   /dir1 created\n");
    if (Mkdir("/dir1/dir2") == -1) {
      n_printf("   Argh, cannot create directory dir1/dir2\n");
    }
    n_printf("   /dir1/dir2 created\n");
    FSList();
    if (Rmdir("/dir1") != -1) {
      n_printf("   Argh, should not have been able to delete dir (non empty)\n");
    }
    if (Rmdir("/dir1/dir2") == -1) {
      n_printf("   Argh, should have been able to delete dir dir1/dir2)\n");
    }
    n_printf("   /dir1/dir2 removed\n");
    Rmdir("/dir1");
    n_printf("   /dir1 removed\n");
    FSList();
    pause();
}

void test_perror(void)
{
  OpenFileId fid;
  n_printf(" Testing perror system call\n");
  n_printf("----------------------------\n");
  
  n_printf("Should print \"No error\"\n");
  PError("Test PError");
  n_printf("Should print an error message\n");
  fid =  Open("/existepas");
  PError("Test PError");
  pause();
}

void test_typeid(void)
{
  n_printf("Testing the typeId syscall checks\n");
  n_printf("---------------------------------\n");
    
  expect_err("syscall(Join invalid thread)", Join(0x424242));
  expect_err("syscall(Read invalid file)", Read((void*)0, 0, 0x424242));
  expect_err("syscall(Write invalid file)", Write((void*)0, 0, 0x424242));
  expect_err("syscall(Seek invalid file)", Seek(0, 0x424242));
  expect_err("syscall(Close invalid file)", Close(0x424242));
  expect_err("syscall(P invalid sema)", P(0x424242));
  expect_err("syscall(V invalid sema)", V(0x424242));
  expect_err("syscall(Destroy invalid sema)", SemDestroy(0x424242));
  expect_err("syscall(Lock invalid mutex)", LockAcquire(0x424242));
  expect_err("syscall(Release invalid mutex)", LockRelease(0x424242));
  expect_err("syscall(Destroy invalid mutex)", LockDestroy(0x424242));
  expect_err("syscall(Wait invalid cond)", CondWait(0x424242));
  expect_err("syscall(Signal invalid cond)", CondSignal(0x424242));
  expect_err("syscall(Bcast invalid cond)", CondBroadcast(0x424242));
  expect_err("syscall(Destroy invalid cond)", CondDestroy(0x424242));
  pause();
}

void test_segv(void)
{
  int *ptrint;
  n_printf("Test access to an invalid address (Should exit Nachos)\n");
  ptrint = (int*)4;
  *ptrint = 42;
}

void test_accessviolation(void)
{
  int *ptrint;
  n_printf("Test write access to a read-only address (Should exit Nachos)\n");
  ptrint = (int*)test_accessviolation;
  *ptrint = 42;
}

int main(void)
{
  char c;
  n_printf("Test of Nachos\n");
  n_printf("--------------\n\n");
  n_printf("Segv (g) \tAccViolation (V) \tLibnachos (n) \tTime (i)\n");
  n_printf("Process (p) \tThread (t) \tFiles (f) \tMMap (m) \tSemaphores (s)\n");
  n_printf("Locks (l) \tConditions (c) \tConsole (C) \tVM (v)\n");
  n_printf("Directory (d) \tPerror (e) \tTypeID (I) \tACIA (a)\n");
  n_printf("Enter a char: \n");
  Read(&c, 1, ConsoleInput);

  switch (c) {
  case 'g': test_segv(); break;
  case 'V': test_accessviolation(); break;
  case 'n': test_lib_nachos(); break;
  case 'i': test_time(); break;
  case 'p': test_process(); break;
  case 't': test_thread(); break;
  case 'f': test_file(); break;
  case 'm': test_mmap(); break;
  case 's': test_sema(); break;
  case 'l': test_lock(); break;
  case 'c': test_cond(); break;
  case 'C': test_console(); break;
  case 'v': test_vm(); break;
  case 'a': test_acia(); break;
  case 'd': test_directory(); break;
  case 'e': test_perror(); break;
  case 'I': test_typeid(); break;
  default: n_printf("Unknown feature\n");
  }

  n_printf("That's all folks !\n");
  Halt();

  return 0; // nonsense
}
