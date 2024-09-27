

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>

main(int argc, char *argv[]) 

 {  int sock_descr;
    int NumBytesRecebidos;
    struct sockaddr_in EnderecRemoto;
    struct hostent *RegistroDNS;
    char buffer[BUFSIZ+1];
    char *NomeHost;
    char *dados;

    if(argc != 4) {
      puts("Uso correto: <cliente> <nome-servidor> <porta> <dados>");
      exit(1);
    }

    NomeHost = argv[1];
    dados = argv[3];

    if((RegistroDNS = gethostbyname(NomeHost)) == NULL){
      puts("Nao consegui obter endereco IP do servidor.");
      exit(1);
    }

    bcopy((char *)RegistroDNS->h_addr, (char *)&EnderecRemoto.sin_addr, 
       RegistroDNS->h_length);
    EnderecRemoto.sin_family = AF_INET;
    EnderecRemoto.sin_port = htons(atoi(argv[2]));

    if((sock_descr=socket(AF_INET, SOCK_STREAM, 0)) < 0) {
      puts("Nao consegui abrir o socket.");
      exit(1);
    }

    if(connect(sock_descr, (struct sockaddr *) &EnderecRemoto, sizeof(EnderecRemoto)) < 0) {
      puts("Nao consegui conectar ao servidor");
      exit(1);
    } 

    if(write(sock_descr, dados, strlen(dados)) != strlen(dados)){
      puts("Nao consegui mandar os dados"); 
      exit(1);
    }

    read(sock_descr, buffer, BUFSIZ);
    printf("Sou o cliente, recebi: %s\n", buffer);
   
    close(sock_descr);
    exit(0);
}
