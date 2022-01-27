#include "system.h"

static void sighandler(int signo){
  if (signo == SIGINT){
    printf(".\n");
    exit(0);
  }
}

int ss_rem(struct cons *ssray, int *ssnum, int index){
  int pid = ssray[index].kid;
  int i;
  for (i = index; i < *ssnum-1; i ++){
    ssray[i] = ssray[i+1];
  }
  *ssnum = *ssnum - 1;
  return pid;
}

int server_setup() {
  system("clear");
  printf("S: Initiating!\n"); /**/

  struct addrinfo * hints, * results;
  hints = calloc(1, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM;
  hints->ai_flags = AI_PASSIVE;
  getaddrinfo(NULL, "12947", hints, &results);

  int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

  bind(sd, results->ai_addr, results->ai_addrlen);

  free(hints);
  freeaddrinfo(results);

  listen(sd, CLIENTNUM);
  printf("S: Online!\n"); /**/
  return sd;
}

void clienthandler(int client_socket, int ss2s[2], int s2ss[2]){
  close(ss2s[READ]);
  close(s2ss[WRITE]);

  char id[12] = {};
  sprintf(id, "%d", getpid());
  printf("\tSS (%s): Forked!\n", id); /**/


  printf("\tSS (%s): S and C Connected!\n", id); /**/
  write(client_socket, id, 12);
  char *ready = "->";
  char msg[USERLEN+2+INPLEN];
  printf("\tSS (%s): Online!\n", id); /**/

  while(1){
    memset(msg, 0, USERLEN+2+INPLEN);
    read(s2ss[READ], msg, USERLEN+2+INPLEN);
    printf("\tSS (%s): Received [%s] from S\n", id, msg); /**/

    if (write(client_socket, msg, strlen(msg)+1) == -1){
      printf("\tSS (%s): Connection to C Lost; Exiting\n", id); /**/
      exit(0);
    }
    printf("\tSS (%s): Sent [%s] to C\n", id, msg); /**/

    if (strchr(msg, ':')){
      write(ss2s[WRITE], ready, 2);
      printf("\tSS (%s): Sent [%s] to S\n", id, ready);
    }else if (!strcmp(id, msg)){ // this pid
      memset(msg, 0, USERLEN+2+INPLEN);
      read(client_socket, msg, USERLEN+2+INPLEN);
      if (!strlen(msg)){
	printf("\tSS (%s): Connection to C Lost; Exiting\n", id); /**/
	write(ss2s[WRITE], "\0", 1);
	exit(0);
      }
      printf("\tSS (%s): Received [%s] from C\n", id, msg); /**/

      write(ss2s[WRITE], msg, strlen(msg));
      printf("\tSS (%s): Sent [%s] to S\n", id, msg); /**/
    }
  }
}

void *server_connect(void *ead) {
  struct thr *ough = (struct thr *) ead;
  int client_socket;
  socklen_t sock_size;
  struct sockaddr_storage client_address;
  sock_size = sizeof(client_address);

  // Accept

  while (1){
    client_socket = accept(*(ough->sd), (struct sockaddr *) &client_address, &sock_size);
    if (client_socket == -1)
      printf("Error %d: %s", errno, strerror(errno)); /**/
    else
      printf("S: Received connection!\n"); /**/

    int ss2s[2] = {};
    int s2ss[2] = {};
    pipe(ss2s);
    pipe(s2ss);
    int pid = fork();
    if (!pid) // ss
      clienthandler(client_socket, ss2s, s2ss);
    else{ // s
      close(ss2s[WRITE]);
      close(s2ss[READ]);
      (ough->ssray[*(ough->ssnum)]).kid = pid;
      (ough->ssray[*(ough->ssnum)]).s2ss = s2ss[WRITE];
      (ough->ssray[*(ough->ssnum)]).ss2s = ss2s[READ];
      *(ough->ssnum) = *(ough->ssnum) + 1;
    }
  }
  return 0;
}

void *chathandler(void *ead){
  struct thr *ough = (struct thr *) ead;
  int i = -1;
  char pid[12];
  char ready[2];
  char msg[USERLEN+2+INPLEN];

  while (1){
    while (*(ough->ssnum) > 0){
      int i = (i+1) % *(ough->ssnum);
      memset(pid, 0, 12);
      sprintf(pid, "%d", (ough->ssray[i]).kid);
      if (write((ough->ssray[i]).s2ss, pid, strlen(pid)) == -1){
	printf("S: Removing SS [%d]\n", (ough->ssray[i]).kid); /**/
	ss_rem(ough->ssray, ough->ssnum, i);
	i --;
	continue;
      }
      printf("S: Sent [%s], a PID, to [%s]\n", pid, pid); /**/
      //memset(ready, 0, 2);
      //read((ough->ssray[i]).ss2s, ready, 2);
      //printf("S: SS (%s) Ready: %s\n", pid, ready);
      
      memset(msg, 0, USERLEN+2+INPLEN);
      read((ough->ssray[i]).ss2s, msg, USERLEN+2+INPLEN);
      if (!strlen(msg)){
	printf("S: Removing SS [%d]\n", (ough->ssray[i]).kid); /**/
	ss_rem(ough->ssray, ough->ssnum, i);
	i --;
	continue;
      }
      printf("S: Received [%s] from [%s]\n", msg, pid); /**/

      int j;
      for (j = i+1; j != i; j ++){
        j %= *(ough->ssnum);
        if (j == i)
          break;
        if (write((ough->ssray[j]).s2ss, msg, strlen(msg)) == -1){
	  printf("S: Removing SS [%d]\n", (ough->ssray[j]).kid); /**/
	  ss_rem(ough->ssray, ough->ssnum, j);
	  if (i > j)
	    i --;
	  j --;
	  continue;
	}
	memset(ready, 0, 2);
	read((ough->ssray[j]).ss2s, ready, 2);
	printf("S: Received [%s] from [%s]\n", ready, pid);
        memset(pid, 0, 12);
        sprintf(pid, "%d", (ough->ssray[j]).kid);
        printf("S: Sent [%s] to [%s]\n", msg, pid); /**/
      }
    }
  }
  return 0;
}

int main() {
  signal(SIGINT, sighandler);
  int *sd = calloc(1, sizeof(int));
  *sd = server_setup();

  struct cons *ssray = calloc(1, sizeof(struct cons) * CLIENTNUM);
  int *ssnum = calloc(1, sizeof(int));

  struct thr *ead = calloc(1, sizeof(struct thr));
  ead->sd = sd;
  ead->ssray = ssray;
  ead->ssnum = ssnum;

  pthread_t connthread, chatthread;

  pthread_create(&connthread, NULL, server_connect, ead);
  pthread_create(&chatthread, NULL, chathandler, ead);
  pthread_join(connthread, NULL);
  pthread_join(chatthread, NULL);

  free(ssnum);
  free(ssray);
  return 0;
}
