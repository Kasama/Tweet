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
	TWEET *tweet = NULL, aux = NULL;
	int RRN = 0, flag = 0;//ler comentário abaixo...

	if(favoriteCount < 0 || retweetCount < 0 || viewsCount < 0)
		return ERROR;	

	tweetFile = fopen(filename, "a+");
	tweet = (TWEET*) malloc(sizeof(TWEET));

	if(tweetFile == NULL || tweet == NULL)
		return ERROR;	

	//criando volume de escrita
	strcpy(tweet->text, text);
	strcpy(tweet->userName, userName);
	strcpy(tweet->coords, coords);
	tweet->favoriteCount = favoriteCount;
	strcpy(tweet->language, language);
	tweet->viewsCount = viewsCount;
	
	//determinando onde serão escritos os dados
	while(fread(aux, sizeof(TWEET), 1, tweetFile) > 0 ){
		if(aux->viewsCount == -1){
			flag = 1;
			break;
		}
		RRR++;
	}

	//Mesmo que o arquivo esteja vazio, o pornteiro escreve no começo.A(RRN == 0)(flag = 0)
	//Se o arquivo não tiver nenhum espaço de remoção logica, o ponteiro
	//chega ao final do arquivo e a escrita sera feita ao final mesmo.(mesmo que append)(flag = 0)
	//Se o ponteiro não chegar no final do arquivo, a escrita sera no valor de RRN.(flag = 1)
	
	if(flag){//se o flag for 1, vou transformar o RRN em bytes para poder colocar o ponteiro no lugar certo do arquivo
		RRN *= sizeof(TWEET);
		fsetpos(tweetFile, &RRN);
	}
	//escrita no arquivo
	fwrite(tweet, sizeof(TWEET), 1, tweetFile);
	fclose(tweetFile);

	return SUCCESS;
 }
