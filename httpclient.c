#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#define PORT 80
#define MAX_DATA_SIZE 1024

void startClient(char *pIp, char *pResource)
{
	int clientSocket, numBytes;
	char readBuffer[MAX_DATA_SIZE];
	struct sockaddr_in webServer;

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (clientSocket == -1)
	{
		printf("Error al crear el socket de conexion principal" );
		exit(-1);
	}

	webServer.sin_family = AF_INET;
	webServer.sin_port = htons(PORT);
	webServer.sin_addr.s_addr = inet_addr(pIp);

	if (connect(clientSocket, (struct sockaddr*)&webServer, sizeof(struct sockaddr)) == -1)
	{
		printf("Se ha producido un error al intentar conectar con el servidor externo.");
		exit(-1);
	}

	char *msg = calloc(MAX_DATA_SIZE, sizeof(char));
	strcpy(msg, "GET ");
	strcat(msg, pResource);
	strcat(msg, " HTTP/1.1\n");
	char *endHeader = "\n";
	numBytes = send(clientSocket, msg, strlen(msg), 0);
	if (numBytes == -1)
	{
		printf("No se han podido enviar los datos al servidor.");
		exit(-1);
	}
	free(msg);
	numBytes = send(clientSocket, endHeader, strlen(endHeader), 0);
	if (numBytes == -1)
	{
		printf("No se han podido enviar los datos al servidor.");
		exit(-1);
	}

	numBytes = recv(clientSocket, readBuffer, MAX_DATA_SIZE, 0);
	if (numBytes == -1)
	{
		printf("No se ha podido extraer los datos del servidor.");
		exit(-1);
	}
	readBuffer[numBytes] = '0';
	close(clientSocket);
}

void parseUrlDirection(char *pUrlDirection)
{
	char *auxPtr, *auxPtr2, *auxPtr3;
	int len;
	auxPtr = strchr(pUrlDirection, '/');
	auxPtr += 2;
	auxPtr2 = strchr(auxPtr, '/');
	if(auxPtr2 == NULL)
	{
		auxPtr2 = strchr(auxPtr, '\0');
		len = auxPtr2 - auxPtr;
		auxPtr3 = calloc(len, sizeof(char));
		strncpy(auxPtr3, auxPtr, len);
		startClient(auxPtr3, "/");
		free(auxPtr3);
	}
	else
	{
		len = auxPtr2 - auxPtr;
		char *auxPtr3 = calloc(len, sizeof(char));
		strncpy(auxPtr3, auxPtr, len);
		auxPtr = strchr(auxPtr2, '\0');
		len = auxPtr - auxPtr2;
		auxPtr = calloc(len, sizeof(char));
		strncpy(auxPtr, auxPtr2, len);
		startClient(auxPtr3, auxPtr);
		free(auxPtr);
	}
}

void initializeClient(int argc, char*argv[])
{
	int option;
	int controlFlag = 0;
	char urlDirection[50];
	while((option = getopt(argc,argv,"u:")) != -1)
	{
		controlFlag = 1;
		switch (option)
		{
			case 'u':
				strcpy(urlDirection, optarg);
				break;
		}
	}
	if(controlFlag == 1)
	{
		parseUrlDirection(urlDirection);
	}
	else
	{
		printf("Por favot digite correctamente los parametros de configuracion.\n");
	}
}

int main(int argc, char *argv[])
{
	initializeClient(argc, argv);
}