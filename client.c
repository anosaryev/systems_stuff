#include "system.h"

char *end_null(char *str){
  if (str[strlen(str)-1] == '\n')
    str[strlen(str)-1] = 0;
  return str;
}

char *msgcat(char *msg, char *user, char *line){
  int i;
  for (i = 0; i < USERLEN+2+INPLEN; i ++)
    msg[i] = 0;
  for (i = 0; user[i]; i ++)
    msg[i] = user[i];
  msg[i] = ':';
  i ++;
  msg[i] = ' ';
  i ++;
  int j = i;
  for (i = 0; line[i]; i ++)
    msg[j+i] = line[i];
  return msg;
}

int client_connect() {
  // Preliminary
  struct addrinfo * hints, * results;
  hints = calloc(1, sizeof(struct addrinfo));
  hints->ai_family = AF_INET;
  hints->ai_socktype = SOCK_STREAM; // TCP
  getaddrinfo("127.0.0.1", "12947", hints, &results);  // Client sets node to Server IP

  // Socket
  int sd = socket(results->ai_family, results->ai_socktype, results->ai_protocol);

  // Connect
  connect(sd, results->ai_addr, results->ai_addrlen);

  free(hints);
  freeaddrinfo(results);

  return sd;
}

int main() {
  system("clear");

  char user[USERLEN+1] = {}; // extra for following while
  char line[INPLEN+1] = {};
  char msg[USERLEN+2+INPLEN] = {}; // user: line

  while(strlen(user) == 0 || strlen(user) > USERLEN){
    printf("Enter a username for this session.\n(up to %d characters, go beyond at your own peril):\n", USERLEN);
    memset(user, 0, USERLEN+1);
    end_null(fgets(user, USERLEN, stdin));
  }

  system("clear");
  printf("The rules are simple, %s!\n", user);
  printf("\nThe username at the bottom of the screen indicates who can currently type.\n");
  printf("When yours pops up, it's your turn!\nType your message or simply send an empty one!\n");
  printf("Don't hold on to the chatting ability too long, that's greedy!\n");
  printf("\nWelcome to the chat room, %s!\n\n", user);

  int sd = client_connect();
  char id[12] = {};
  read(sd, id, 12);
  printf("PID: %s\n\n\n", id); /**/

  while (1){
    memset(msg, 0, USERLEN+2+INPLEN);
    read(sd, msg, USERLEN+2+INPLEN);
    if (CLDEBUG)
      printf("\t\t\tC: Received [%s] from [%s]\n", msg, id); /**/
    if (strchr(msg, ':')){ // external chat msg
      printf("%s\n\n", msg);
      continue;
    }else if (!strcmp(id, msg)){ // matches SS PID
      printf("%s: ", user);
      memset(line, 0, INPLEN);
      end_null(fgets(line, INPLEN, stdin));
      printf("\n");
      msgcat(msg, user, line);
      write(sd, msg, strlen(msg));
      if (CLDEBUG)
	printf("\t\t\tC: Wrote [%s] to [%s]\n", msg, id); /**/
    }
  }
  return 0;
}
