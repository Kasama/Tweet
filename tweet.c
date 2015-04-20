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
#include "tweet.h"

typedef struct tweet TWEET;

// insert a tweet into the database
// return SUCCESS if successful and ERROR if it wasn't possible to insert
int insertTweet( char *fileName, char *text, char *userName, char *coords, int favoriteCount, char *language, int retweetCount, long viewsCount){
	return 0;
}

// return all tweets (array), NULL if there is none
// returns via pointer, the number of tweets found 0 if none
TWEET **requestAllTweets(char *fileName, int *ammount){
	return NULL;
}

// return a single tweet, using the ID number (RRN), NULL if RRN is invalid
TWEET *requestTweet(char *fileName, int RRN){
	return NULL;
}

// return all tweets (array) from a user using sequencial search, NULL if none found
// returns via pointer, the number of tweets found 0 if none
TWEET **findTweetByUser(char *fileName, char *userName, int *ammount){
	return NULL;
}

// remove a tweet, using it's ID number
// return SUCCESS if successful and ERROR if the register was not found
int removeTweet(char *fileName, int RRN){
	return 0;
}

// returns a string to be printed of a single tweet
// needs to be freed
char *printTweet(TWEET *tweet){
	return "";
}
