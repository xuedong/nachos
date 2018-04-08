#include "userlib/libnachos.h"
#include "userlib/syscall.h"

//sémaphore

SemId fullBuff; //Bloquant quand le buffer est plein
SemId emptyBuff; //Bloquant quand le buffer est vide

#define MAX_PATE 10

int buff[MAX_PATE] ; //qualité des patés de crabe au boeuf
int first_pate = 0;
int last_pate = 0;

int push(int pate){
    P(fullBuff);
    buff[last_pate]=pate;
    last_pate = (last_pate +1)% MAX_PATE;
    V(emptyBuff);
    return pate;    
}

int pull(){
    P(emptyBuff);
    int pate = buff[first_pate];
    first_pate = (first_pate +1) % MAX_PATE;
    V(fullBuff);
    return pate; 
}

int sleep(int n){
    int i;
    for(i=0; i<(1<<n); ++i);
    return i;
}

void threadSandy(){
    int pate;
    while(1){
        pate = pull() + sleep(17)%2 -1;
        n_printf("Sandy: It was great, Bob ! [%d]\n", pate);
    }
    return;
}

void threadGary(){
    int pate;
    while(1){
        pate = pull() + sleep(17)%2 -1;
        if(pate>=5){
            n_printf("Gary: Maaaoooow <3 [%d]\n", pate);
            push(pate);
        }else if (pate==1){
            n_printf("[Gary is leaving]\n");
            break;
        }else{
            n_printf("Gary: Maaow.\n");
        }
    }
    return;
}

void threadBob(){
    int pate = 1;
    while(1){
        sleep(18);
        pate = (pate*pate +1)% 10;
        push(pate);
        n_printf("Bob: Hehehe\n");
    }
}

typedef struct{
    SemId recv, send;
} thread_args;

int main(){
    fullBuff = SemCreate("fullBuff", MAX_PATE);
    emptyBuff = SemCreate("emptyBuff", 0);
    threadCreate("Sandy", threadSandy);
    threadCreate("Gary", threadGary);
    threadCreate("Bob", threadBob);
    return 0;
}
