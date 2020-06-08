// Grava.c
// Gravar tudo que a Caixa Preta envia quando no modo Opera 8
// #[m ... m]# --> Campo com dados do MPU
// #[g ... g]# --> Campo com dados do GPS
// #[l ... l]# --> Campo com dados da calibração ao ligar
// #[l ... f]# --> Campo com dados da calibração de fábrica
// f]# --> Marca o final do arquivo

// Automaticamente gera arquivo com nome dxxx$.txt

// Receptor serial
// Usa a API do Win32-API

// https://www.xanthium.in/Serial-Port-Programming-using-Win32-API
// https://github.com/xanthium-enterprises/Serial-Programming-Win32API-C/commit/3aa6c7cedc30863dce94cf950ef2267c9ee27bc8


// https://docs.microsoft.com/pt-br/windows/win32/api/fileapi/nf-fileapi-readfile
// https://docs.microsoft.com/pt-br/dotnet/api/system.io.ports.serialport.getportnames?view=dotnet-plat-ext-3.1
// https://stackoverflow.com/Questions/1388871/how-do-i-get-a-list-of-available-serial-ports-in-win32

// Definição dos nomes dos arquivos
// dxxx$.txt --> Contém tudo, incluindo { #[m ... #[g ... #]f }.
// dxxxa.txt --> { ax ay az } Acelerômetro
// dxxxg.txt --> { gx gy gz } Giroscópio
// dxxxh.txt --> { hx hy hz } Magnetômetro
// dxxxn.txt --> { ... linha gps 128 bytes } GPS = navegador
// dxxxp.txt --> { Lat N/S Long E/W } Só latitude e longitude 
// dxxxv.txt --> {velocidade km/h} Só a velocidade 

#include<windows.h>
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<conio.h>

#define CR 0xD		//Carriage Return
#define LF 0xA		//Line Feed

//#define BR CBR_256000	//Baudrate
//#define BR CBR_128000	//Baudrate
#define CBR CBR_115200	//Baudrate

FILE *abre_arq(int nr, char tipo);
void separa_arq(int nr);
int  arq_str(FILE *fp, char *buf, unsigned int limite);
char serial_str(char *buf, unsigned int limite);
char busca_nr(void);		//Retorna primeiro número de arquivo livre dxxxt.txt
char serial_rd(void);		//Ler um byte serial, prende execução
int  sopen(char *nome, long br); 
void sclose(void);

HANDLE hComm;

#include <stdio.h>
void main(int argc, char *argv[]){
	char nome[10];		//Nome da porta serial COM1, COM2, ...
	char nome_arq[10]="dxxx$.txt"; //Gerar nome do arquivo a ser aberto
	char msg[256];	//Receber as strings que chegam pela porta serial
	char aux[100];
	char c1,c2,c3;
	int  nr;			//Número dos arquivos
	int  ok;
	int  i=0;
	char dado;
	char *com="com18";
	FILE *fpt=NULL;

	printf("\n\nPrograma <Grava> - Receber dados CXP no modo Opera 8.\n\n");
	
	// Verificar se programa Grava foi chamada com argumentos
	nome[0]='\0';
	if (argc>1){
		strcpy(nome,argv[1]);
	}
	else{
		nome[0]='C';
		nome[1]='O';
		nome[2]='M';
		printf("Informe numero da porta [COM??]: ");
		scanf("%s",&nome[3]);
	}
	
	if (sopen(nome,CBR) == TRUE)
		printf("\nAbriu porta serial [%s] com %d bps.\n",nome,CBR);
	else{
		printf("\nErro ao abrir porta serial [%s]\n",nome);
		exit(0);
	}
	
	//Laço Principal
	while(TRUE){

		// Abrir porta serial
		nr=busca_nr();	//Buscar nr para gerar nome dos arquivos
		printf("nr = %d esta vazio.\n",nr);
		sprintf(aux,"%05d",nr);
		nome_arq[1]=aux[2];
		nome_arq[2]=aux[3];
		nome_arq[3]=aux[4];
		fpt=fopen(nome_arq,"w+");
		if (fpt == NULL){
			printf("Nao pode abrir [%s].\n",nome_arq);
			printf("Programa interrompido.\n");
			sclose();
			exit(0);	//Interromper
		}
		printf("Abriu [%s].\n",nome_arq);
		
		// #[m --> Esperar para iniciar a gravação
		c1=c2=c3='\0';
		while(TRUE){
			c1=c2;
			c2=c3;
			c3=serial_rd();
			putchar(c3);
			if (c1=='#' && c2=='[' && c3=='m')	break;
		}
		printf("\n\nAchou #[m. Inicando Gravacao\n\n");

		// f]# --> Esperar para finalizar
		fputc(c1,fpt);
		fputc(c2,fpt);
		fputc(c3,fpt);
		while(TRUE){
			c1=c2;
			c2=c3;
			c3=serial_rd();
			putchar(c3);
			fputc(c3,fpt);
			if (c1=='f' && c2==']' && c3=='#')	break;
		}
		fclose(fpt);
		printf("\nAchou f]#. Terminou a gravação.\n");
		printf("Use tecla X para finalizar. Qq outra tecla, repete.\n");
		dado=getch();
		if (dado=='x' || dado=='X')	break;
	}
	sclose();
	printf("\nPrograma <Grava> finalizado.\n");
}

// Buscar pelo primeiro número de arquivo livre
// dxxx$.txt, onde xxx = 1, 2, ..., 999
char busca_nr(void){
	unsigned int nr=0;
	char msg[10];		//Receber string do número
	char nome[10]="dxxx$.txt";
	FILE *fp=NULL;
		
	while(TRUE){
		nr++;
		sprintf(msg,"%05d",nr);
		nome[1]=msg[2];
		nome[2]=msg[3];
		nome[3]=msg[4];
		fp=fopen(nome,"r");
		if (fp == NULL)	break;
		fclose(fp);
		printf("Existe [%s]\n",nome);
	}
	return nr;
}

// Esperar chegar um byte pela porta serial
char serial_rd(void){
	DWORD qtd;	// Qtd de bytes lidos pelo ReadFile()
	BOOL  st;	//Resultado da leitura
	char  x;	// Temperory Character
	
	qtd=0;
	while(qtd == 0){
		st = ReadFile(hComm, &x, sizeof(x), &qtd, NULL);
	}
	return x;
}

// Abrir porta serial
int sopen(char *nome, long bps){
	BOOL Status;
	char com[20];
	com[0]='\0';
	strcat(com,"\\\\.\\");
	strcat(com,nome);
	//printf("Nome=%s\n",com);
	
/*--------------------- Opening the Serial Port -------------------------*/	
	hComm = CreateFile(com,                			//port name
                      GENERIC_READ | GENERIC_WRITE, //Read/Write
                      0,                            // No Sharing
                      NULL,                         // No Security
                      OPEN_EXISTING,				// Open existing port only
                      0,            				// Non Overlapped I/O
                      NULL);        				// Null for Comm Devices

	if (hComm == INVALID_HANDLE_VALUE){
		printf("Erro ao abrir [%s].\n",nome);
		return(FALSE);	
	}
	
/*------------- Setting the Parameters for the SerialPort --------------------*/	
  
	DCB dcbSerialParams = { 0 };                         // Initializing DCB structure
	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	Status = GetCommState(hComm, &dcbSerialParams);      //retreives  the current settings

	if (Status == FALSE){
		printf("\n    Error! in GetCommState()");
		return (FALSE);
	}
	//dcbSerialParams.BaudRate = bps;    		  // Setting BaudRate = 9600
	//dcbSerialParams.ByteSize = 8;             // Setting ByteSize = 8
	//dcbSerialParams.StopBits = ONESTOPBIT;    // Setting StopBits = 1
	//dcbSerialParams.Parity = NOPARITY;        // Setting Parity = None 

	Status = SetCommState(hComm, &dcbSerialParams);  //Configuring the port according to settings in DCB 

	if (Status == FALSE)
	{
		printf("\n    Error! in Setting DCB Structure");
		return(FALSE);
	}
	
	/*
	else //If Successfull display the contents of the DCB Structure
	{
		printf("\n\n    Setting DCB Structure Successfull\n");
		printf("\n       Baudrate = %d", dcbSerialParams.BaudRate);
		printf("\n       ByteSize = %d", dcbSerialParams.ByteSize);
		printf("\n       StopBits = %d", dcbSerialParams.StopBits);
		printf("\n       Parity   = %d", dcbSerialParams.Parity);
	}
	*/
	
/*--------------------- Setting Timeouts -------------------------*/	
	COMMTIMEOUTS timeouts = { 0 };
	timeouts.ReadIntervalTimeout         = 50;	//mseg
	timeouts.ReadTotalTimeoutConstant    = 50;	//mseg
	timeouts.ReadTotalTimeoutMultiplier  = 10;	//mseg
	timeouts.WriteTotalTimeoutConstant   = 50;	//mseg
	timeouts.WriteTotalTimeoutMultiplier = 10;	//mseg
			
	if (SetCommTimeouts(hComm, &timeouts) == FALSE){
		printf("\n\n    Error! in Setting Time Outs");
		return(FALSE);
	}
	//else	printf("\n\n    Setting Serial Port Timeouts Successfull");

	return(TRUE);
/*--------------------- Setting Receive Mask -------------------------*/	
	Status = SetCommMask(hComm, EV_RXCHAR); //Configure Windows to Monitor the serial device for Character Reception

	if (Status == FALSE)
		printf("\n\n    Error! in Setting CommMask");
	else
		printf("\n\n    Setting CommMask successfull");
}

// Fechar porta serial
void sclose(void){
	CloseHandle(hComm);//Closing the Serial Port
}