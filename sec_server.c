#include "system.h"

void printe(int b){
  if (b == -1)
    printf("Error %d: %s\n", errno, strerror(errno));
}

static void sighandler(int signo){
  if (signo == SIGINT){
    printf(".\n");
    exit(0);
  }
}

int ss_add(struct cons *ssray, int *ssnum, struct cons newss){
  ssray[*ssnum] = newss;
  *ssnum ++;
  return 0;
}

int ss_rem(struct cons *ssray, int *ssnum, int index){
  int pid = ssray[index].kid;
  int i;
  for (i = index; i < *ssnum-1; i ++){
    ssray[i] = ssray[i+1];
  }
  *ssnum --;
  return pid;
}

int server_setup() {
  system("clear");
  printf("S: Initiating!\n"); /**/

  // Preliminary
  struct addrinfo * hints, * results;
  hints = calloc(1, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM; // TCP
  hints->ai_flags = AI_PASSIVE; // only needed on server
  getaddrinfo(NULL, "12957", hints, &results);  // Server sets node to NULL

  // Socket
  int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

  // Bind
  bind(sd, results->ai_addr, results->ai_addrlen);

  free(hints);
  freeaddrinfo(results);

  // Listen
  listen(sd, CLIENTNUM);
  printf("S: Online!\n"); /**/
  return sd;
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
    if (!pid){ // ss
      printf("\tSS: Forked!\n"); /**/
      struct conss *ssconss = calloc(1, 3*sizeof(int));
      ssconss->sd = client_socket;
      close(ss2s[READ]);
      close(s2ss[WRITE]);
      ssconss->ss2s = ss2s[WRITE];
      ssconss->s2ss = s2ss[READ];
      printf("\tSS: S and C Connected!\n"); /**/
      printf("\tSS: Online!\n"); /**/

      char msg[USERLEN+2+INPLEN];
      char id[12] = {};
      sprintf(id, "%d", getpid());
      write(ssconss->sd, id, 12);
      printf("oops\n");
      
      while(1){
	strcpy(msg, "");
	read(ssconss->s2ss, msg, USERLEN+2+INPLEN);
	if (strchr(msg, ':')){ // external chat msg
	  write(ssconss->sd, msg, strlen(msg)+1);
	  continue;
	}else{ // next chatter id
	  if (!strcmp(id, msg)){ // matches this process
	    write(ssconss->sd, msg, strlen(msg)+1);
	    memset(msg, 0, USERLEN+2+INPLEN);
	    read(ssconss->sd, msg, USERLEN+2+INPLEN);
	    printf("\tSS: Received: [%s]\n", msg); /**/
	    write(ssconss->ss2s, msg, strlen(msg)); //new
	  }
	}
      }
    }else{ // s
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
  char msg[USERLEN+2+INPLEN];
  while (1){
    while (*(ough->ssnum) > 0){
      int i = (i+1) % *(ough->ssnum);
      strcpy(pid, "");
      sprintf(pid, "%d", (ough->ssray[i]).kid);
      printf("S: Empowering PID %s\n", pid);
      if (write((ough->ssray[i]).s2ss, pid, strlen(pid)) == -1){
	ss_rem(ough->ssray, ough->ssnum, i);
	i --;
	continue;
      }
      strcpy(msg, "");
      if (read((ough->ssray[i]).ss2s, msg, USERLEN+2+INPLEN) == -1){
	ss_rem(ough->ssray, ough->ssnum, i);
	i --;
	continue;
      }
      printf("S: Received [%s]\n", msg);
      int j;
      for (j = i+1; j != i; j ++){
	j %= *(ough->ssnum);
	if (j == i)
	  break;
	if (write((ough->ssray[j]).s2ss, msg, strlen(msg)) == -1){
	  ss_rem(ough->ssray, ough->ssnum, j);
	  if (i > j)
	    i --;
	  j --;
	  continue;
	}
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
  //printf("up to\n");
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
