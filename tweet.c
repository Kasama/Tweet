/*
 * @Authors:
 *
 * Frederico de Azevedo Marques - 8936926
 * Henrique Martins Loschiavo   - 8936972
 * Lucas Kassouf Crocomo        - 8937420
 * Roberto Pommella Alegro      - 8936756
 *
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tweet.h"

// tweet structure
struct tweet{

	char   text[TEXT_SIZE];
	char   userName[USER_SIZE];
	char   coords[COORDINATES_SIZE];
	char   language[LANGUAGE_SIZE];
	int    favoriteCount;
	int    retweetCount;
	long   viewsCount;

};


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
 */
int insertTweet(\
		char   *fileName,		\
		char   *text,           \
		char   *userName,       \
		char   *coords,         \
		int    favoriteCount,   \
		char   *language,       \
		int    retweetCount,    \
		long   viewsCount       \
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
	if(tweet == NULL){
		fclose(tweetFile);
		return ERROR;	
	}

	//creating data storage volume 
	strcpy(tweet->text, text);
	strcpy(tweet->userName, userName);
	strcpy(tweet->coords, coords);
	tweet->favoriteCount = favoriteCount;
	strcpy(tweet->language, language);
	tweet->retweetCount = retweetCount;
	tweet->viewsCount = viewsCount;
	
	aux = (TWEET*) malloc(sizeof(TWEET));
	if(aux == NULL){
		free(tweet);
		fclose(tweetFile);
		return ERROR;
	}

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

/**
 * A function that returns all tweets from a user
 *
 * @param fileName - the name of the file to request from
 * @param amount   - the amount of returned tweets
 *
 * @variable file      - the file to request from
 * @variable tweet     - a pointer used to allocate space for every tweet in the tweets array
 * @variable tweets    - all tweets
 * @variable amountAux - a counter for the amount of tweets added to the array
 *
 * @return the amount of tweets in the array
 * @return tweets on success, NULL otherwise
 *
 */
TWEET **requestAllTweets(char *fileName, int *amount) {
    FILE *file = NULL;
    TWEET *tweet = NULL;
    TWEET **tweets = NULL;
    
    int amountAux = 0;
    
	// tries to open the file and check if it was successful
    file = fopen(fileName, "r");
    if(file == NULL) {
        return NULL;
    }
    
    tweet = (TWEET *) malloc(sizeof(TWEET));
	if(tweet == NULL) { // check if malloc was successful
		fclose(file);
        return NULL;
    }
    
	// reads every tweet from the data file
    while(fread(tweet, sizeof(TWEET), 1, file) != 0) {
		// checks if the read tweet is a valid one
        if(tweet != NULL && tweet->viewsCount >= 0 && tweet->retweetCount >= 0 && tweet->favoriteCount >= 0) {
			// adds the tweet to the array if it's valid. Reads next otherwise 
            amountAux++;
			// allocates the array of tweets to return
            tweets = (TWEET **) realloc(tweets, sizeof(TWEET*)*amountAux);
            tweets[amountAux-1] = tweet;
            tweet = (TWEET *) malloc(sizeof(TWEET));
            if(tweet == NULL) {
				for(amountAux--; amountAux >= 0; amountAux--){
					free(tweets[amountAux]);
				}
				free(tweets);
				return NULL;
            }
        } 
    }
	// set the secondary return value
	*amount = amountAux;
    
    fclose(file);
    return tweets;
}

/**
 * A function that returns a single tweet, identified by it's RRN in the file
 *
 * @param fileName - the name of the file to request from
 * @param RRN      - the relative register number of the tweet to request
 *
 * @variable file  - the file to request from
 * @variable tweet - the requested tweet
 *
 * @return tweet on success, NULL otherwise
 *
 */
TWEET *requestTweet(char *fileName, int RRN) {
    FILE *file = NULL;
    TWEET *tweet = NULL;

	// tries to open the file and check if it was successful
    file = fopen(fileName, "r");
    if(file == NULL) {
       return NULL;
    }

	// tries to position the file's pointer to the tweet specified by RRN
    if(fseek(file, RRN*sizeof(TWEET), SEEK_SET) == -1) {
		// if it is unsuccessful closes the file and return 
        fclose(file);
		return NULL;
	}
	tweet = (TWEET *) malloc(sizeof(TWEET));
	if(tweet == NULL) { // check if malloc was successful
		fclose(file);
		return NULL;
	}

	// tries to read a tweet from the file
	if(fread(tweet, sizeof(TWEET), 1, file) == 0) {
		// if it is unsuccessful closes the file, frees the pointer and return
		fclose(file);
		free(tweet);
		return NULL;
	}
	// closes the file
	fclose(file);
	// check if the read tweet is valid (was not logically removed)
	if(tweet->viewsCount < 0 && tweet->retweetCount < 0 && tweet->favoriteCount < 0){
		// if it is a removed tweet, do not return it. Free the pointer instead
		free(tweet);
		return NULL;
	}

	return tweet;
}


/**
 * A function that returns all tweets from a user
 *
 * @param fileName - the name of the file to request from
 * @param userName - the name of the user
 * @param amount   - the amount of returned tweets
 *
 * @variable file      - the file to request from
 * @variable tweet     - a pointer used to allocate space for every tweet in the tweets array
 * @variable tweets    - all tweets from the user
 * @variable amountAux - a counter for the amount of tweets added to the array
 *
 * @return the amount of tweets in the array
 * @return tweets on success, NULL otherwise
 *
 */
TWEET **findTweetByUser(char *fileName, char *userName, int *amount) {

	FILE *file = NULL;
	TWEET *tweet = NULL;
	TWEET **tweets = NULL;
	int amountAux = 0;

	// tries to open the file and check if it was successful
	file = fopen(fileName, "r");
	if(file == NULL) {
		return NULL;
	}

	tweet = (TWEET *) malloc(sizeof(TWEET));
	if(tweet == NULL) { // check if malloc was successful
		return NULL;
	}

	// reads every tweet from the data file
	while(fread(tweet, sizeof(TWEET), 1, file) != 0) {
		// checks if the read tweet is a valid one and if it is from user
		if(tweet != NULL && !strcmp(userName, tweet->userName) && tweet->viewsCount >= 0 && tweet->retweetCount >= 0 && tweet->favoriteCount >= 0) {
			// adds the tweet to the array if it's valid. Reads next otherwise 
			amountAux++;
			// allocates the array of tweets to return
			tweets = (TWEET **) realloc(tweets, sizeof(TWEET*)*amountAux);
			tweets[amountAux-1] = tweet;
			tweet = (TWEET *) malloc(sizeof(TWEET));
			if(tweet == NULL) {
				for(amountAux--; amountAux >= 0; amountAux--){
					free(tweets[amountAux]);
				}
				free(tweets);
				return NULL;
			}
		} 
	}
	// set the secondary return value
	*amount = amountAux;

	fclose(file);
	return tweets;
}

/**
 * A function that logically removes a tweet by setting negative values to
 * retweetCount, viewsCount and favoriteCount
 *
 * @param fileName - the name of the file to remove from
 * @param RRN      - the relative register number of the tweet to remove
 *
 * @variable file  - the file from which to remove
 * @variable tweet - the tweet that will be removed
 *
 * @return SUCCESS if the removal was successfull, ERROR otherwise
 *
 */
int removeTweet(char *fileName, int RRN){

	FILE *file;
	TWEET *tweet;

	// request the tweet to be removed
	tweet = requestTweet(fileName, RRN);
	if(tweet == NULL)
		return ERROR; // if the tweet does not exist, return ERROR
	// set negative values to "remove" the tweet
	tweet->retweetCount = -1;
	tweet->viewsCount = -1;
	tweet->favoriteCount = -1;

	//open the data file
	file = fopen(fileName, "r+");
	if(file == NULL)
		goto RemoveFailedNoFile; // if it fails to open the file, return
	// seeks to the original tweet position, if it fails, return
	if(fseek(file, RRN*sizeof(TWEET), SEEK_SET) == -1) 
		goto RemoveFailed;
	// overrides the original tweet with the modified one, if it fails, return
	if(fwrite(tweet, sizeof(TWEET), 1, file) == 0)
		goto RemoveFailed;
	
	// close the file, free the tweet and return
	fclose(file);
	free(tweet);
	return SUCCESS;

	// close the file, free the tweet and return
RemoveFailed:
	fclose(file);
RemoveFailedNoFile:
	free(tweet);
	return ERROR;

}

/**
 * A function that checks if a tweet is equal to another one
 *
 * @param t1 - the first tweet to compare
 * @param t2 - the second tweet to compare
 *
 * @return SUCCESS if both are equal, ERROR otherwise
 *
 */
int equalTweet(TWEET* t1, TWEET* t2){

	// if either of them is NULL, they are by definition not equal
	if(t1 == NULL || t2 == NULL)
		return ERROR;
	// if all fields of both tweets are equal, they are equal
	if(
		strcmp(t1->coords, t2->coords) == 0 &&
		strcmp(t1->userName, t2->userName) == 0 &&
		strcmp(t1->text, t2->text) == 0 &&
		strcmp(t1->language, t2->language) == 0 &&
		t1->retweetCount == t2->retweetCount &&
		t1->viewsCount == t2->viewsCount &&
		t1->favoriteCount == t2->favoriteCount
	) return SUCCESS;
	return ERROR;

}
/**
 * a function that returns a string to be printed externally
 *
 * @param tweet    - the tweet to be printed
 * @param fileName - the name of the data file
 *
 * @variable favCount    - temporary text for the favoriteCount field in the tweet struct
 * @variable reCount     - temporary text for the retweetCount field in the tweet struct
 * @variable viewsCount  - temporary text for the viewCount field in the tweet struct
 * @variable RRN         - temporary text for the RRN of a tweet
 * @variable entireTweet - the return string
 * @variable recTweet    - a temporary tweet to compare with the one to be printed
 * @variable i           - a counter
 * @variable entries     - maximum number of tweets in the file
 * @variable file        - the data file
 *
 * @return a string to be printed
 *
 */
char *printTweet(TWEET *tweet, char *fileName) {

	// defines
	#define TEXT_TEXT "Text: "
	#define TEXT_USER "\nUser: "
	#define TEXT_COORDS "\nCoordinates: "
	#define TEXT_LANG "\nLanguage: "
	#define TEXT_VIEWS "\nViews: "
	#define TEXT_FAV "\nFavorites: "
	#define TEXT_RETWEETS "\nRetweets: "
	#define TEXT_RRN "\nRRN: "
	#define TEXT_INT 11
	#define TEXT_LONG 20

	// maximum size that a string representation of a tweet can be
	#define SIZEOF_ENTIRETWEET \
   	sizeof(TWEET) - (sizeof(int)*2) - sizeof(long) + sizeof(TEXT_TEXT) + sizeof(TEXT_USER) + sizeof(TEXT_COORDS) +\
   	sizeof(TEXT_LANG) + sizeof(TEXT_VIEWS) + sizeof(TEXT_FAV) + sizeof(TEXT_RETWEETS) + sizeof(TEXT_RRN) + TEXT_INT*2 + TEXT_LONG

	char *favCount, *reCount, *viewCount, *RRN;
	char *entireTweet;
	TWEET *recTweet;
	int i, entries;
	FILE *file;

	// if the tweet is null, return a line break
	if(tweet == NULL) return "\n";

	// temporary text for the numbers
	favCount = malloc(sizeof(char) * TEXT_INT);
	reCount = malloc(sizeof(char) * TEXT_INT);
	RRN = malloc(sizeof(char) * TEXT_INT);
	viewCount = malloc(sizeof(char) * TEXT_LONG);

	// print the numbers into the string
	sprintf(favCount, "%d", tweet->favoriteCount);
	sprintf(reCount, "%d", tweet->retweetCount);
	sprintf(viewCount, "%ld", tweet->viewsCount);

	// allocate the return string
	entireTweet = malloc(SIZEOF_ENTIRETWEET);
	entireTweet[0] = 0;

	// concatenate all parts of the return string together
	strcat(entireTweet, TEXT_TEXT);
	strcat(entireTweet, tweet->text);
	strcat(entireTweet, TEXT_USER);
	strcat(entireTweet, tweet->userName);
	strcat(entireTweet, TEXT_COORDS);
	strcat(entireTweet, tweet->coords);
	strcat(entireTweet, TEXT_LANG);
	strcat(entireTweet, tweet->language);
	strcat(entireTweet, TEXT_VIEWS);
	strcat(entireTweet, viewCount);
	strcat(entireTweet, TEXT_FAV);
	strcat(entireTweet, favCount);
	strcat(entireTweet, TEXT_RETWEETS);
	strcat(entireTweet, reCount);
	strcat(entireTweet, TEXT_RRN);

	// get the max number of tweets in a file
	file = fopen(fileName, "a");
	entries = ftell(file);
	entries = entries / sizeof(TWEET);
	fclose(file);

	// look for a tweet that is equal to the one we want to print to find its RRN
	for(i = 0; i < entries; i++){
		recTweet = requestTweet(fileName, i);
		if(equalTweet(recTweet, tweet) == SUCCESS){
			free(recTweet);
			break;
		}
		free(recTweet);
	}

	// finish concatenating the RRN to the string
	sprintf(RRN, "%d", i);
	strcat(entireTweet, RRN);
	strcat(entireTweet, "\n");

	// free the temporary pointers
	free(favCount);
	free(reCount);
	free(viewCount);
	free(RRN);

	// return the string
	return entireTweet;
}
