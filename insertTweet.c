/*#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define ERROR 0
#define SUCCESS 1
#define TAM 30

typedef struct {
		char   fileName[TAM];       
		char   text[TAM];           
		char   userName[TAM];       
		char   coords[TAM];         
		int    favoriteCount;   
		char   language[TAM];      
		int    retweetCount;    
		long   viewsCount; 
}TWEET;
*/

int insertTweet(				
		char   *fileName,       
		char   *text,           
		char   *userName,       
		char   *coords,         
		int    favoriteCount,   
		char   *language,      
		int    retweetCount,    
		long   viewsCount       
){
	FILE *tweetFile = NULL;
	TWEET *tweet = NULL, *aux = NULL;
	int RRN = -1, flag = 0;//ler comentário abaixo...

	if( favoriteCount < 0 || retweetCount < 0 || viewsCount < 0)
		return ERROR;	

	tweetFile = fopen(fileName, "a+");
	if(tweetFile == NULL)
		return ERROR;

	tweet = (TWEET*) malloc(sizeof(TWEET));
	if(tweet == NULL)
		return ERROR;	

	//criando volume de escrita
	strcpy(tweet->text, text);
	strcpy(tweet->userName, userName);
	strcpy(tweet->coords, coords);
	tweet->favoriteCount = favoriteCount;
	strcpy(tweet->language, language);
	tweet->retweetCount = retweetCount;
	tweet->viewsCount = viewsCount;
	
	aux = (TWEET*) malloc(sizeof(TWEET));
	if(aux == NULL)
		return  ERROR;

	//determinando onde serão escritos os dados
	fseek(tweetFile, SEEK_SET, SEEK_SET);
	while(fread(aux, sizeof(TWEET), 1, tweetFile) > 0 ){
		if(aux->favoriteCount == -1 && aux->retweetCount == -1  && aux->viewsCount == -1){
			flag = 1;
			break;
		}
		RRN++;
	}

	//Mesmo que o arquivo esteja vazio, o pornteiro escreve no começo.A(RRN == 0)(flag = 0)
	//Se o arquivo não tiver nenhum espaço de remoção logica, o ponteiro
	//chega ao final do arquivo e a escrita sera feita ao final mesmo.(mesmo que append)(flag = 0)
	//Se o ponteiro não chegar no final do arquivo, a escrita sera no valor de RRN.(flag = 1)
	
	if(flag){//se o flag for 1, vou transformar o RRN em bytes para poder colocar o ponteiro no lugar certo do arquivo
		RRN *= sizeof(TWEET);
		if(fseek(tweetFile, RRN, SEEK_SET) == -1){
			free(tweet);
			fclose(tweetFile);
			return ERROR;
		}
	}
	//escrita no arquivo
	if(fwrite(tweet, sizeof(TWEET), 1, tweetFile) == 0){
		free(tweet);
		fclose(tweetFile);
		return ERROR;	
	}

	fclose(tweetFile);
	free(tweet);
	free(aux);
	return SUCCESS;
 }
/*
void  main(){
	char *fileName = NULL, *text = NULL, *userName = NULL, *coords = NULL, *language = NULL;
	int favoriteCount, retweetCount, viewsCount;

	fileName = (char*) malloc(sizeof(char)*30);
	text = (char*) malloc(sizeof(char)*30);
	userName = (char*) malloc(sizeof(char)*30);
	coords = (char*) malloc(sizeof(char)*30);
	language = (char*) malloc(sizeof(char)*30);
	favoriteCount = 1;
	retweetCount = 341;
	viewsCount = 23;
	
	strcpy(text, "fdksljfsdljf");
	strcpy(fileName, "bla.txt");
	strcpy(userName, "fdakf");
	strcpy(coords, "reowir");
	strcpy(language, "lkjsf");
	
	insertTweet(fileName, text, userName, coords, favoriteCount, language, retweetCount ,viewsCount);
}*/
