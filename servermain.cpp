#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/time.h>

/* You will to add includes here */
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>
#include <errno.h>
#include <stdint.h>

// Included to get the support library
#include <calcLib.h>

#include "protocol.h"

#define PORT "4950"

using namespace std;
/* Needs to be global, to be rechable by callback and main */
int loopCount=0;
int terminate=0;


/* Call back function, will be called when the SIGALRM is raised when the timer expires. */
void checkJobbList(int signum){
  // As anybody can call the handler, its good coding to check the signal number that called it.

  printf("Let me be, I want to sleep.\n");

  if(loopCount>20){
    printf("I had enough.\n");
    terminate=1;
  }

  return;
}


int main(int argc, char *argv[]){

  /* Do more magic */
  if (argc < 2)
  {
    printf("Not enough arguments\n");
    exit(0);
  }

  char currentAddr[100];
  char clientIP[100];
  struct addrinfo hints, * servInfo, * p;
  struct sockaddr_in clientAddr;
  socklen_t clientLength = sizeof(clientAddr);
  uint8_t socket;
  uint8_t returnValue;
  struct calcMessage* calcMsg;
  struct calcProtocol * calcProto;
  int numBytes;
  int currentClient = 1;
  void * ptrTempt[100];
  memset(&hints, 0, sizeof(hints));
  int clientIPLength = sizeof(clientIP);
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE;

  if ((returnValue = getaddrinfo(NULL, argv[1], &hints, &servInfo)) != 0)
  {
    fprintf(stderr, "[x] Error getaddrinfo\n");
    exit(0);
  }
  /*
     Prepare to setup a reoccurring event every 10s. If it_interval, or it_value is omitted, it will be a single alarm 10s after it has been set.
  */
  struct itimerval alarmTime;
  alarmTime.it_interval.tv_sec=10;
  alarmTime.it_interval.tv_usec=10;
  alarmTime.it_value.tv_sec=10;
  alarmTime.it_value.tv_usec=10;

  /* Regiter a callback function, associated with the SIGALRM signal, which will be raised when the alarm goes of */
  signal(SIGALRM, checkJobbList);
  setitimer(ITIMER_REAL,&alarmTime,NULL); // Start/register the alarm.


  while(terminate==0){
    printf("This is the main loop, %d time.\n",loopCount);
    sleep(1);
    loopCount++;
  }

  printf("done.\n");
  return(0);



}
