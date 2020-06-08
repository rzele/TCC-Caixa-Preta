// Toca.c
// Processar aquivos gerados pelo programa Grava
// São os dados gerados pela Caixa Preta

// Arquivo processado tem o formato abaixo
// #[m ... m]# --> Campo com dados do MPU
// #[g ... g]# --> Campo com dados do GPS
// #[l ... l]# --> Campo com dados da calibração ao ligar
// #[l ... f]# --> Campo com dados da calibração de fábrica
// f]# --> Marca o final do arquivo

// Definição dos nomes dos arquivos
// dxxx$.txt --> Contém tudo, incluindo { #[m ... #[g ... f]# }.
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

// Códigos usados para indicar sim/não, passou/falhou
#define COD_SIM  0x5353     //(21.331) 2x ASCII(S) Afirmativo
#define COD_NAO  0x4E4E     //(20.046) 2x ASCII(N) Negativo

// Marcar posição de cada um dos parâmetros guardados em gps_dados[GPS_DADOS_TAM]
#define GPS_STATUS     0                 //2 bytes
#define GPS_HORA      (GPS_STATUS+2)    //11 bytes
#define GPS_DATA      (GPS_HORA+11)     //7 bytes
#define GPS_LAT       (GPS_DATA+7)      //11 bytes
#define GPS_NS        (GPS_LAT+11)      //2 bytes
#define GPS_LONG      (GPS_NS+2)        //12 bytes
#define GPS_EW        (GPS_LONG+12)     //2 bytes
#define GPS_VEL_NOS   (GPS_EW+2)        //8 bytes
#define GPS_CURSO     (GPS_VEL_NOS+8)   //8 bytes
#define GPS_PDOP      (GPS_CURSO+8)     //6 bytes
#define GPS_HDOP      (GPS_PDOP+6)      //6 bytes
#define GPS_VDOP      (GPS_HDOP+6)      //6 bytes
#define GPS_VEL_KPH   (GPS_VDOP+6)      //7 bytes
#define GPS_VEL_UN    (GPS_VEL_KPH+7)   //2 bytes
#define GPS_FIX       (GPS_VEL_UN+2)    //2 bytes
#define GPS_QTD_SAT   (GPS_FIX+2)       //3 bytes sem uso
#define GPS_ALT       (GPS_QTD_SAT+3)   //7 bytes
#define GPS_ALT_UN    (GPS_ALT+7)       //2 bytes
#define GPS_ADR_SRAM  (GPS_ALT_UN+2)    //5 bytes


void triagem(void);
void cal_fab(void);
void cal_lig(void);
void printa(void);
void inter(void);
void lista(void);
void cal_lig_prn(FILE *fp);

FILE *abre_arq(int nr, char tipo);
FILE *abre_arq_rd(int nr, char tipo);
int menu(void);
int escolha(int lim);
int gps_linha(FILE *fp, char *buf);
int arq_str(FILE *fp, char *buf, unsigned int limite);
void data_prn(char *vt);
void hora_prn(char *vt);

void main(void){
	int opc;
	
	//printf("\nProcessar arquivos dxxx$.txt da Caixa Preta.\n");
	
	opc=0;
	while(opc!=7){
		opc=menu();
		switch(opc){
			case 1: triagem();	break;
			case 2: cal_lig();	break;
			case 3: cal_fab();	break;
			case 4: printa();	break;
			case 5: inter();	break;
			case 6: lista();	break;
			case 7: 			break;
			default:	printf("opcao nao prevista\n");
		}
	}
	printf("\nFim do Programa Toca\n");
}

void cal_fab(void){printf("\nCalibracao de Fabrica.\n");}
void print(void){printf("\nVer arquivo.\n");}
void inter(void) {printf("\nInterpreta Arquivo.\n");}

// 1 - Separar todos os arquivos
void triagem(void){
	FILE *fpt,*fpa,*fpg,*fph,*fpn,*fpp,*fpv;
	char buf[256];
	int tam;
	unsigned int qtd;	//Qtd de linhas em cada arquivo
	unsigned int i,j;		//Contador
	int nr;
	fpt=NULL;
	printf("\nSeparar arquivos.\n");
	while(fpt==NULL){
		printf("\nQual o numero do arquivo? ");
		scanf("%d",&nr);
		//printf("\nEscolheu [%d].",nr);
		fpt=abre_arq(nr,'$');		//Tudo
	}
	//Abrir e criar os arquivos
	fpa=abre_arq(nr,'a');		//Acelerômetro
	fpg=abre_arq(nr,'g');		//Giroscópio
	fph=abre_arq(nr,'h');		//Magnetômetro
	fpn=abre_arq(nr,'n');		//GPS
	fpp=abre_arq(nr,'p');		//Latitude e Longitude
	fpv=abre_arq(nr,'v');		//Velocidade
	
	// #[m ==> MPU
	// Separar dados MPU
	i=0;
	buf[0]='\0';
	while( strcmp(buf,"#[m") != 0)	arq_str(fpt, buf, 120);
	printf("\nEncontrado #[m ==> Separar Acel - Giro - Mag. ");
	arq_str(fpt, buf, 120);	//Data
	arq_str(fpt, buf, 120);	//Hora
	arq_str(fpt, buf, 120);	//Endereço qdo SEL foi acionado
	arq_str(fpt, buf, 120);	//String qtd de linhas
	strcat(buf,"\n\0");
	fputs(buf,fpa); //qtd de linhas acel
	fputs(buf,fpg); //qtd de linhas giro
	fputs(buf,fph); //qtd de linhas mag
	qtd=atoi(buf);			//qtd de linhas
	for(i=0; i<qtd;i++){
		arq_str(fpt, buf, 120);	//Indexador
		arq_str(fpt, buf, 120);	//Endereço
		arq_str(fpt, buf, 120); strcat(buf," "); fputs(buf,fpa);  //ax
		arq_str(fpt, buf, 120); strcat(buf," "); fputs(buf,fpa);  //ay
		arq_str(fpt, buf, 120); strcat(buf,"\n"); fputs(buf,fpa); //az
		;
		arq_str(fpt, buf, 120); strcat(buf," "); fputs(buf,fpg);  //gx
		arq_str(fpt, buf, 120); strcat(buf," "); fputs(buf,fpg);  //gy
		arq_str(fpt, buf, 120); strcat(buf,"\n"); fputs(buf,fpg); //gz
		;
		arq_str(fpt, buf, 120); strcat(buf," "); fputs(buf,fph);  //hx
		arq_str(fpt, buf, 120); strcat(buf," "); fputs(buf,fph);  //hy
		arq_str(fpt, buf, 120); strcat(buf,"\n"); fputs(buf,fph); //hz
	}
	arq_str(fpt, buf, 120);
	if( strcmp(buf,"m]#") != 0)
		printf("Aviso: Não encontrado terminador m]#.\n");

	// #[g ==> GPS
	// Separar dados MPU
	i=0;
	buf[0]='\0';
	while( strcmp(buf,"#[g") != 0)	arq_str(fpt, buf, 120);
	printf("\nEncontrado #[g ==> Separar GPS. ");
	arq_str(fpt, buf, 120);	//Data qdo SEL foi acionado
	arq_str(fpt, buf, 120);	//Hora qdo SEL foi acionado
	arq_str(fpt, buf, 120);	//Endereço qdo SEL foi acionado
	arq_str(fpt, buf, 120);	//String qtd de linhas
	qtd=atoi(buf);			//qtd de linhas]
	
	/*
	//Contar quantidade de linhas válidas
	i=0;
	j=1;
	for (i=0; i<qtd;i++){			//Contar linhas válidas
		arq_str(fpt, buf, 120); 	//Pular contador
		arq_str(fpt, buf, 120); 	//Pular endereço
		gps_linha(fpt, buf);		//Ler uma linha do GPS
		printf("[i=%d] %s\n",i,buf);
		if (buf[0]=='A')	j++;
	}
	printf("Encontradas %d linhas validas de %d.\n",j,qtd);
	fprintf(fpn,"%d\n",j);		//Gravar nr de linhas válidas
	rewind(fpt);				//Voltar arq principal para o começo
	//#[g - Achar novamente o começo do GPS 
	i=0;
	buf[0]='\0';
	while( strcmp(buf,"#[g") != 0)	arq_str(fpt, buf, 120);
	*/

	//arq_str(fpt, buf, 120);	//Endereço qdo SEL foi acionado
	//arq_str(fpt, buf, 120);	//String qtd de linhas
	//qtd=atoi(buf);			//qtd de linhas
	fprintf(fpn,"%d\n",qtd);	//fpn receber qtd de linhas
	fprintf(fpp,"%d\n",qtd);	//fpp receber qtd de linhas
	fprintf(fpv,"%d\n",qtd);	//fpv receber qtd de linhas
	for (i=0; i<qtd;i++){			//Contar linhas válidas
		arq_str(fpt, buf, 120); 	//Pular contador
		arq_str(fpt, buf, 120); 	//Pular endereço
		gps_linha(fpt, buf);		//Ler uma linha do GPS
		if (buf[0]=='A'){
			fputs(buf,fpn);			//Gravar a linha
			j=0;
			while(buf[j] != ' ')	j++;		//pular A/V
			j++;
			while(buf[j] != ' ')	j++;		//data
			j++;
			while(buf[j] != ' ')	j++;		//pular hora
			j++;
			while(buf[j] != ' ')	fputc(buf[j++],fpp);	//Gravar LAT
			fputc(' ',fpp);
			j++;
			while(buf[j] != ' ')	fputc(buf[j++],fpp);	//Gravar LONG
			fputc('\n',fpp);
			j++;
			while(buf[j] != ' ')	fputc(buf[j++],fpv);	//Gravar Veloc Km/h
			fputc('\n',fpv);
		}
		else{
			fputs("V\n",fpn);			//V - Linhas GPS não válida
			fputs("0.0N 0.0E\n",fpp);	//0.0N e 0.0W - Posição não válida
			fputs("0.0K\n",fpv);			//0.0K - Velocidade não válida
		}
		
	}
	

	fclose(fpt);
	fclose(fpa);
	fclose(fpg);
	fclose(fph);
	fclose(fpn);
	fclose(fpp);
	fclose(fpv);


}

// 2 - Imprime Calibração ao ligar
void cal_lig(void){
	int nr;
	FILE *fp=NULL;
	while(TRUE){
		printf("\nImprime Calibracao ao Ligar.\n");
		printf("\nQual o numero do arquivo (0=fim)? ");
		scanf("%d",&nr);	getchar();
		if (nr == 0)		break;
		fp=abre_arq_rd(nr,'$');
		if (fp == NULL){
			printf("Falha ao abrir arquivo [d%03d%$.txt]:\n",nr);
		}
		else{
			printf("Dados de Calibra ao Ligar, arquivo [d%03d%$.txt]:\n",nr);
			cal_lig_prn(fp);
		}
	}
}

// Comenta os dados da Calibração ao Ligar, do arquivo dxxx$.txt
void cal_lig_prn(FILE *fp){
	char buf[256];
	int x,j;
	rewind(fp);

	buf[0]='\0';
	while( strcmp(buf,"#[l") != 0)	arq_str(fp, buf, 120);
	printf("\nEncontrado #[l ==> Iniciar.\n");
	
	arq_str(fp, buf, 120);	printf("[%s] ==> ",buf);	//Bateu?
	x=atoi(buf);
	if 		(x==COD_SIM)	printf("Bateu.\n");
	else if (x==COD_NAO)	printf("Nao Bateu.\n");
	else					printf("Nao sei se bateu.\n");
	
	arq_str(fp, buf, 120);	printf("[%s] ==> ",buf);	//Self Teste?
	x=atoi(buf);
	if 		(x==COD_SIM)	printf("Self Teste OK.\n");
	else if (x==COD_NAO)	printf("Self Teste NAO OK.\n");
	else					printf("Nao sei sobre Self Teste.\n");

	arq_str(fp, buf, 120);	printf("[%s] ==> ",buf);	//Calib de Fabrica?
	x=atoi(buf);
	if 		(x==COD_SIM)	printf("Tem Calibracao de Fabrica.\n");
	else if (x==COD_NAO)	printf("Nao Tem Calibracao de Fabrica.\n");
	else					printf("Nao sei sobre Calibracao de Fabrica.\n");

	arq_str(fp, buf, 120);	printf("[%s] ==> ",buf);	//Qtd de medidas para media?
	j=x=atoi(buf);
	printf("Feitas %s medidas para calcular a media.\n",buf);

	arq_str(fp, buf, 120);	printf("[%s] ==> ",buf);	//Acel - Escala para media
	x=atoi(buf);
	x=2<<x;
	printf("Escala de calibracao do acelerometro = +/- %d g.\n",x);

	arq_str(fp, buf, 120);	printf("[%s] ==> ",buf);	//Giro - Escala para media
	x=atoi(buf);
	x=250<<x;
	printf("Escala de calibracao do giroscopiio  = +/- %d graus/seg.\n",x);
	
	printf("Media das %d leituras.\n",j);
	printf("  ax     ay     az     tp     gx     gy     gz\n");
	for (j=0; j<7; j++){
		arq_str(fp, buf, 120);	printf("%s ",buf);		//Medias das medidas
	}
	putchar('\n');
	
	arq_str(fp, buf, 120);	printf("[%s] ==> ",buf);	//Qtd medidas Mag
	j=atoi(buf);
	printf("Quantidade de medidas do Magnetometro=%d.\n",j);
	
	arq_str(fp, buf, 120);	printf("[%s] ==> ",buf);	//Escala do Mag
	printf("Escala do Magnetometro=%s.\n",buf);
	
	printf("Media das %d leituras.\n",j);
	printf("  hx     hy     hz\n");
	for (j=0; j<3; j++){
		arq_str(fp, buf, 120);	printf("%s ",buf);		//Medias das medidas
	}
	putchar('\n');
	
	arq_str(fp, buf, 120);	printf("[%s] ==> ",buf);	//Freq de amostragem
	x=atoi(buf);
	x= 1000/(x+1);
	printf("Frequencia de amostragem do MPU = %d Hz.\n",x);
	
	arq_str(fp, buf, 120);	printf("[%s] ==> ",buf);	//Acel - Escala para operacao
	x=atoi(buf);
	x=2<<x;
	printf("Escala de operacao do Acelerometro = +/- %d g.\n",x);

	arq_str(fp, buf, 120);	printf("[%s] ==> ",buf);	//Giro - Escala para operacao
	x=atoi(buf);
	x=250<<x;
	printf("Escala de operacao do Giroscopio = +/- %d graus/seg.\n",x);

	arq_str(fp, buf, 120);	printf("[%s] ==> ",buf);	//Mag - Escala para operacao
	x=atoi(buf);
	x=250<<x;
	printf("Escala de operacao do Magnetometro = +/- %d ???.\n",x);

	printf("Limiares de disparo.\n",j);
	printf("  ax     ay     az    gx     gy     gz\n");
	for (j=0; j<6; j++){
		arq_str(fp, buf, 120); printf("%s ",buf);		//Limiares de disparo
	}
	putchar('\n');

	arq_str(fp, buf, 120);	printf("[%s] ==> ",buf);	//Ponteiro MPU no Disparo
	printf("Instante do disparo. Ponteiro MPU = %s.\n",buf);
	
	arq_str(fp, buf, 120);	printf("[%s] ==> ",buf);	//Ponteiro GPS no Disparo
	printf("Instante do disparo. Ponteiro GPS = %s.\n",buf);
	
	printf("Quem disparou.\n");										//Quem disparou
	printf(" ax  ay  az  gy  gx  gz\n");
	for (j=0; j<6; j++){
		arq_str(fp, buf, 120);
		x=atoi(buf);
		printf("[%d]",x);
		if 		(x==COD_SIM)		printf("S ");		//Sim
		else if (x==COD_NAO)		printf("N ");		//Nao
		else						printf("? ");		//Nao sei
	}
	putchar('\n');

	arq_str(fp, buf, 120);	printf("[%s] ==> Data: ",buf);	//Data do Disparo
	data_prn(buf);	putchar('\n');

	arq_str(fp, buf, 120);	printf("[%s] ==> Hora: ",buf);	//Hora do Disparo
	hora_prn(buf);	putchar('\n');

	while( strcmp(buf,"l]#") != 0)	arq_str(fp, buf, 120);	//Finalizar
	printf("\nEncontrado l]# ==> Finalizar.\n");
}

// Ler uma linha do GPS, termina com CR LF (0xd 0xA)
int gps_linha(FILE *fp, char *buf){
	char dt;
	unsigned int i=0;
	
	// Comer espaços, CR e LF
	dt=fgetc(fp);
	while (dt==' ' || dt==CR || dt==LF)
		dt=fgetc(fp);
	buf[i++]=dt;
	
	// Ler uma string
	while(dt != CR){
		dt=fgetc(fp);
		buf[i++]=dt;
	}
	buf[i]='\0';
	//printf("%s\n",buf);
	return i-1;

}

// 4 - Imprimir um arquivo
void printa(void){
	int nr;
	char tipo,c;
	FILE *fp=NULL;
	printf("\nImprimir um arquivo.");
	while(TRUE){
		printf("\nQual o numero do arquivo (0=fim)? ");
		scanf("%d",&nr);	getchar();
		if (nr == 0)		break;
		printf("Qual o tipo do arquivo ($, a, g, h, n, p, v)? ");
		scanf("%c",&tipo);	getchar();
		fp=abre_arq_rd(nr,tipo);
		if (fp == NULL){
			printf("Falha ao abrir arquivo [d%03d%c.txt]:\n",nr,tipo);
		}
		else{
			printf("Conteudo do arquivo [d%03d%c.txt]:\n",nr,tipo);
			while (TRUE){
				c=getc(fp);
				if (c==EOF)	break;
				putchar(c);
			}
		}
	}
}

// 6 - Listar arquivos do tipo dxxx$.txt
void lista(void){
	unsigned int nr=0;
	char msg[10];		//Receber string do número
	char nome[10]="dxxx$.txt";
	char buf[256];
	FILE *fp=NULL;

	printf("\nArquivos disponiveis:\n");
	while(nr<999){
		nr++;
		sprintf(msg,"%05d",nr);
		nome[1]=msg[2];
		nome[2]=msg[3];
		nome[3]=msg[4];
		fp=fopen(nome,"r");
		if (fp != NULL){
			printf("%s ==> ",nome);
			arq_str(fp, buf, 120);	//Data
			arq_str(fp, buf, 120);	//Data
			data_prn(buf);
			arq_str(fp, buf, 120);	//Data
			hora_prn(buf);
			putchar('\n');
		}
		fclose(fp);
	}
}

// Receber número e abre o arquivos 
FILE *abre_arq(int nr, char tipo){
	FILE *fp=NULL;
	char nome[15]="dxxxz.txt";
	char aux[10];
	char buf[256];
	
	sprintf(aux,"%05d",nr);
	nome[1]=aux[2];
	nome[2]=aux[3];
	nome[3]=aux[4];
	nome[4]=tipo;
	
	if (tipo == '$')	fp=fopen(nome,"r");
	else				fp=fopen(nome,"w+");
	if (fp == NULL){
		printf("Nao pode abrir [%s].\n",nome);
		printf("Programa interrompido.\n");
		exit(0);	//Interromper
	}
	printf("Abriu [%s].\n",nome);
	return fp;
}

// Abrir um determinado arquivo para leitura
FILE *abre_arq_rd(int nr, char tipo){
	FILE *fp=NULL;
	char nome[15]="dxxxz.txt";
	char aux[10];
	char buf[256];
	
	sprintf(aux,"%05d",nr);
	nome[1]=aux[2];
	nome[2]=aux[3];
	nome[3]=aux[4];
	nome[4]=tipo;
	
	fp=fopen(nome,"r");
	return fp;
}



int menu(void){
	int opc;
	printf("\n<<<-- Processar arquivos Caixa Preta, OPERA 8 -->>>\n");
	printf("1: Separar arquivo dxxx$.txt em diversos arquivos.\n");
	printf("2: Mostrar Calibracao ao Ligar.\n");
	printf("3: Mostrar Calibracao de Fabrica.\n");
	printf("4: Imprimir um arquivo.\n");
	printf("5: Interpretar os dados do arquivo principal.\n");
	printf("6: Lista de arquivos disponiveis\n");
	printf("7: Sair\n");
	opc=escolha(7);
	//printf("Escolheu %d",opc);
	return opc;
	
}

// Selecionar opção na faixa [1, 2, ..., lim]
int escolha(int lim){
	int i;
	int esc=0;
	while(esc<1 || esc>lim){
		printf("Escolha uma das opcoes (");
		for (i=1; i<lim; i++)	printf("%d, ",i);
		printf("%d):",i);

		/*
		esc=getchar();
		if (esc == LF)	esc=getchar();
		esc -= 0x30;
		getchar();
		*/
		
		scanf("%d",&esc);	
		if (esc<1 || esc>lim)
			printf("Opcao nao valida!\n");
	}
	return esc;
}


// Separar uma string da arquivo
// Pega tudo até encontrar espaço, CR ou LF ou '\0'
// Retorna tamanho da string sem contar o zero
int arq_str(FILE *fp, char *buf, unsigned int limite){
	char dt;
	unsigned int i=0;
	
	// Comer espaços, CR e LF
	dt=fgetc(fp);
	while (dt==' ' || dt==CR || dt==LF)
		dt=fgetc(fp);
	buf[i++]=dt;
	
	// Ler uma string
	while(TRUE){
		dt=fgetc(fp);
		if (dt==' ')			break;
		if (dt==CR || dt==LF)	break;
		if (dt=='\0')			break;
		buf[i++]=dt;
		if (i==limite-1)		break;
	}
	buf[i]='\0';
	//printf("Arq_str [%s].\n",buf);
	return i-1;
}


// Imprimir data dd/mm/aa
void data_prn(char *vt){
	putchar(vt[0]);	putchar(vt[1]);	putchar('/');
	putchar(vt[2]);	putchar(vt[3]);	putchar('/');
	putchar(vt[4]);	putchar(vt[5]);	putchar(' ');
}

// Imprimir data dd/mm/aa
void hora_prn(char *vt){
	putchar(vt[0]);	putchar(vt[1]);	putchar(':');
	putchar(vt[2]);	putchar(vt[3]);	putchar(':');
	putchar(vt[4]);	putchar(vt[5]);	putchar(' ');
}


