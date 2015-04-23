/**
 * Attempt to insert an tweet on tweet's file by searching for logical removed places on file before appending. Also, checks for inappropriate input values
 *
 * @param fileName		- receives file's name to be opened and written to.
 * @param text			- receives the tweet's text
 * @param userName		- receives the user's name
 * @param coords		- receives the geographic coordinates
 * @param favoriteCount - receives the amount of time the tweet has been favorited
 * @param language		- receives the tweet text's language 
 * @param etweetCount	- receives the amount of time the tweet has been retweeted
 * @param viewsCount    - receives the amount of time the tweet has been seen
 *
 *
 * @variable tweetFile  - pointer to tweet's file to be opened inside program
 * @variable tweet      - pointer that receives the function's input and store on TWEET structure to be written on the file
 * @variable aux		- just a variable to help gather data from file and check for logical removed tweets
 * @variable RRN        - stores the Relative Register Number where the insert will occur 
 * @variable flag		- indicates that the write will replace an logical removed tweet
 *
 * @return SUCCESS if tweet insertion was successful, ERROR otherwise
 **/
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
	int RRN = -1, flag = 0;//read comments below
	
	//checking for inappropriate values
	if( favoriteCount < 0 || retweetCount < 0 || viewsCount < 0)
		return ERROR;	
	
	//initiating pointers
	tweetFile = fopen(fileName, "r+");
	if(tweetFile == NULL)
		return ERROR;

	tweet = (TWEET*) malloc(sizeof(TWEET));
	if(tweet == NULL)
		return ERROR;	

	//creating data storage volume 
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

	//seeks for a place to insert data
	fseek(tweetFile, SEEK_SET, SEEK_SET);
	while(fread(aux, sizeof(TWEET), 1, tweetFile) > 0 ){
		if(aux->favoriteCount == -1 && aux->retweetCount == -1  && aux->viewsCount == -1){
			flag = 1;
			break;
		}
		RRN++;
	}

	//Even on an empty file, the file's pointer writes on file's begin. (RRN = 0)(flag = 0)
	//If there is no logical removed tweet on file, the write will be just an appending (writing at  eof)(RRN = x)(flag = 0)
	//If an logical removed tweet was found, then it will be replace by the new tweet from functions input stored on tweet pointer(RRN = x)(flag = 1)

	if(flag){//if flag equals 1, then RRN should be transform to the bytes number equivalent to the place where the tweet will be written on file.
		RRN *= sizeof(TWEET);
		if(fseek(tweetFile, RRN, SEEK_SET) == -1){
			free(tweet);
			fclose(tweetFile);
			return ERROR;
		}
	}
	//writing on file 
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

