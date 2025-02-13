

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#define TAMFILA      5
#define MAXNOMEHOST 30

main (int argc, char *argv[]) {
 int sock_escuta, sock_atende;
 unsigned int aux;
 char buffer[BUFSIZ + 1];
 struct sockaddr_in EnderecLocal, EnderecClient;
 struct hostent *RegistroDNS;
 char NomeHost[MAXNOMEHOST];

 if (argc != 2) {
  puts("Uso correto: servidor <porta>");
  exit(1);
 }

 gethostname (NomeHost, MAXNOMEHOST);

 if ((RegistroDNS = gethostbyname(NomeHost)) == NULL){
  puts ("Nao consegui meu proprio IP");
  exit (1);
 }	
	
 EnderecLocal.sin_port = htons(atoi(argv[1]));
 EnderecLocal.sin_family = AF_INET;		
 bcopy ((char *) RegistroDNS->h_addr, (char *) &EnderecLocal.sin_addr, RegistroDNS->h_length);

 if ((sock_escuta = socket(AF_INET,SOCK_STREAM,0)) < 0){
  puts ("Nao consegui abrir o socket");
  exit ( 1 );
 }	
	
 if (bind(sock_escuta, (struct sockaddr *) &EnderecLocal, sizeof(EnderecLocal)) < 0){
  puts ("Nao consegui fazer o bind");
  exit ( 1 );
 }		
 
 listen (sock_escuta, TAMFILA);
	
 for (;;){
  aux = sizeof(EnderecLocal);
  if ((sock_atende=accept(sock_escuta, (struct sockaddr *) &EnderecClient, &aux))<0){
   puts ("Nao consegui fazer conexao");
   exit (1);
  }
  memset(buffer,'\0',strlen(buffer));
  read(sock_atende, buffer, BUFSIZ);
  printf("Sou o servidor, recebi a mensagem----> %s\n", buffer);
  write(sock_atende, buffer, BUFSIZ);
 
  close(sock_atende);
 }
}
