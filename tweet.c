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

struct tweet{

	char text[TEXT_SIZE];
	char userName[USER_SIZE];
	char coords[COORDINATES_SIZE];
	char language[LANGUAGE_SIZE];
	int favoriteCount;
	int retweetCount;
	long viewsCount;

};

// insert a tweet into the database
// return SUCCESS if successful and ERROR if it wasn't possible to insert
int insertTweet( char *fileName, char *text, char *userName, char *coords, int favoriteCount, char *language, int retweetCount, long viewsCount){

	FILE *file = NULL;
	TWEET *tweet = NULL;
	int i, n;


	if(retweetCount < 0 || viewsCount < 0 || favoriteCount < 0)
		return ERROR;

	file = fopen(fileName, "r+");
	if(file == NULL)
		return ERROR;
	if(fseek(file, 0l, SEEK_END) == -1)
		goto insertFailed;
	n = ftell(file)/sizeof(TWEET);
	if(n < 0)
		goto insertFailed;
	if(fseek(file, 0l, SEEK_SET) == -1)
		goto insertFailed;
	tweet = (TWEET*) malloc(sizeof(TWEET));
	if (tweet == NULL)
		goto insertFailed;
	for(i = 0; i < n; i++){
		if(fread(tweet, sizeof(TWEET), 1, file) == 0)
			goto insertFailedAlloc;
		if(tweet->viewsCount < 0 && tweet->retweetCount < 0 && tweet->favoriteCount < 0){
			break;
		}
	}
	if(fseek(file, sizeof(TWEET) * i, SEEK_SET) == -1)
		goto insertFailed;
	tweet->favoriteCount = favoriteCount;
	tweet->retweetCount = retweetCount;
	tweet->viewsCount = viewsCount;
	strcpy(tweet->language, language);
	strcpy(tweet->coords, coords);
	strcpy(tweet->text, text);
	strcpy(tweet->userName, userName);
	if(fwrite(tweet, sizeof(TWEET), 1, file) == 0)
		goto insertFailedAlloc;

	free(tweet);
	fclose(file);
	return SUCCESS;

insertFailedAlloc:
	free(tweet);
insertFailed:
	fclose(file);
	return ERROR;
}

// return all tweets (array), NULL if there is none
// returns via pointer, the number of tweets found 0 if none
TWEET **requestAllTweets(char *fileName, int *ammount){

	FILE *file = NULL;
	TWEET *tweet = NULL;
	TWEET **tweets = NULL;
	int i, n;
	*ammount = 0; 

	file = fopen(fileName, "r+");
	if(file == NULL)
		return NULL;
	if(fseek(file, 0l, SEEK_END) == -1)
		goto requestAllFailed;
	n = ftell(file)/sizeof(TWEET);
	if(n < 0)
		goto requestAllFailed;
	fclose(file);

	for (i = 0; i < n; i++){
		tweet = requestTweet(fileName, i);
		if (tweet != NULL){
			(*ammount)++;
			tweets = (TWEET**) realloc(tweets, sizeof(TWEET*) * *ammount);
			tweets[(*ammount)-1] = tweet;
		}
	}

	return tweets;

requestAllFailed:
	*ammount = 0;
	fclose(file);
	return NULL;

}

/**
 * A function that returns a single tweet, identified by it's RRN in the file
 *
 * @param fileName - the name of te file to request from
 * @param RRN - the relative register number of the tweet to request
 *
 * @variable file - the file to request from
 * @variable tweet - the requested tweet
 *
 * @return tweet on success, NULL otherwise
 *
 */
TWEET *requestTweet(char *fileName, int RRN){

	FILE *file = NULL;
	TWEET *tweet = NULL;
	
	// open the data file
	file = fopen(fileName, "r+");
	// if the data file could not be opened, return NULL
	if(file == NULL)
		return NULL;
	// go to the requested tweet position, if it fails to do so, return NULL
	if(fseek(file, RRN*sizeof(TWEET), SEEK_SET) == -1)
		goto RequestFailed;
	// alloc the size of a TWEET to store the tweet to be read
	tweet = (TWEET*) malloc(sizeof(TWEET));
	// if it fails to allocate memmory, return NULL
	if(tweet == NULL)
		goto RequestFailed;
	// reads the requested tweet, if it fails to do so, return NULL
	if(fread(tweet, sizeof(TWEET), 1, file) == 0){
		goto RequestFailedAlloc;
	}
	// check to see whether the tweet exists or has been removed
	if(tweet->viewsCount < 0 || tweet->retweetCount < 0 || tweet->favoriteCount < 0)
		goto RequestFailedAlloc;
	fclose(file);
	return tweet;

RequestFailedAlloc:
	free(tweet);
RequestFailed:
	fclose(file);
	return NULL;

}

// return all tweets (array) from a user using sequencial search, NULL if none found
// returns via pointer, the number of tweets found 0 if none
TWEET **findTweetByUser(char *fileName, char *userName, int *ammount){
	FILE *file = NULL;
	TWEET *tweet = NULL;
	TWEET **tweets = NULL;
	int i, n;
	*ammount = 0; 

	file = fopen(fileName, "r+");
	if(file == NULL)
		return NULL;
	if(fseek(file, 0l, SEEK_END) == -1)
		goto requestAllFailed;
	n = ftell(file)/sizeof(TWEET);
	if(n < 0)
		goto requestAllFailed;
	fclose(file);

	for (i = 0; i < n; i++){
		tweet = requestTweet(fileName, i);
		if (tweet != NULL && strcmp(tweet->userName, userName) == 0){
			(*ammount)++;
			tweets = (TWEET**) realloc(tweets, sizeof(TWEET*) * *ammount);
			tweets[(*ammount)-1] = tweet;
		}
	}

	return tweets;

requestAllFailed:
	*ammount = 0;
	fclose(file);
	return NULL;
}

/**
 * A function that virtually removes a tweet by setting negative values to
 * retweetCount, viewsCount and favoriteCount
 *
 * @param fileName - the name of the file to remove from
 * @param RRN - the relative register number of the tweet to remove
 *
 * @variable file - the file from which to remove
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

// returns a string to be printed of a single tweet
// needs to be freed
char *printTweet(TWEET *tweet){
	return tweet->text;
}
