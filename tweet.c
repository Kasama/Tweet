/*
 * @Authors:
 *
 * Frederico de Azevedo Marques - 8936926
 * Henrique Martins Loschiavo   - 8936972
 * Lucas Kassouf Crocomo        - 8937420
 * Roberto Pommella Alegro      - 8936756
 *
 */

#include "tweet.h" 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct tweet{

	char text[TEXT_SIZE];
	char userName[USER_SIZE];
	char coords[COORDINATES_SIZE];
	char language[LANGUAGE_SIZE];
	int favoriteCount;
	int retweetCount;
	long viewsCount;

};

int insertTweet(				\
		char   *fileName,       \
		char   *text,           \
		char   *userName,       \
		char   *coords,         \
		int    favoriteCount,   \
		char   *language,       \
		int    retweetCount,    \
		long   viewsCount       \
){
	return ERROR;
}

int removeTweet(char *fileName, int RRN){
	return ERROR;
}

// returns a string to be printed of a single tweet
// needs to be freed
char *printTweet(TWEET *tweet){
	return tweet->text;
}

TWEET **requestAllTweets(char *fileName, int *ammount) {
    FILE *file = NULL;
    TWEET *tweet = NULL;
    TWEET **tweets = NULL;
    
    int ammountAux = 0;
    
    file = fopen(fileName, "r");
    if(file == NULL) {
        return NULL;
    }
    
    tweet = (TWEET *) malloc(sizeof(TWEET));
    if(tweet == NULL) {
		fclose(file);
        return NULL;
    }
    rewind(file);
    
    while(fread(tweet, sizeof(TWEET), 1, file) != 0) {
        if(tweet != NULL && tweet->viewsCount >= 0 && tweet->retweetCount >= 0 && tweet->favoriteCount >= 0) {

            ammountAux++;
            tweets = (TWEET **) realloc(tweets, sizeof(TWEET*)*ammountAux);
            tweets[ammountAux-1] = tweet;
            tweet = (TWEET *) malloc(sizeof(TWEET));
            if(tweet == NULL) {
                return NULL;
            }
        } 
    }
	*ammount = ammountAux;
    
    fclose(file);
    return tweets;
}


TWEET *requestTweet(char *fileName, int RRN) {
    FILE *file = NULL;
    TWEET *tweet = NULL;

    file = fopen(fileName, "r");
    if(file == NULL) {
       return NULL;
    }

    if(fseek(file, RRN*sizeof(TWEET), SEEK_SET) == -1) {
        fclose(file);
		return NULL;
	}
	tweet = (TWEET *) malloc(sizeof(TWEET));
	if(tweet == NULL) {
		fclose(file);
		return NULL;
	}

	if(fread(tweet, sizeof(TWEET), 1, file) == 0) {
		free(tweet);
		fclose(file);
		return NULL;
	}
	fclose(file);
	if(tweet->viewsCount < 0 && tweet->retweetCount < 0 && tweet->favoriteCount < 0){
		free(tweet);
		return NULL;
	}

	return tweet;
}


TWEET **findTweetByUser(char *fileName, char *userName, int *ammount) {
	FILE *file = NULL;
	TWEET *tweet = NULL;
	TWEET **tweets = NULL;

	int ammountAux = 0;

	file = fopen(fileName, "r");
	if(file == NULL) {
		return NULL;
	}

	tweet = (TWEET *) malloc(sizeof(TWEET));
	if(tweet == NULL) {
		return NULL;
	}
	rewind(file);

	while(fread(tweet, sizeof(TWEET), 1, file) != 0) {
		if(tweet != NULL && !strcmp(userName, tweet->userName) && tweet->viewsCount >= 0 && tweet->retweetCount >= 0 && tweet->favoriteCount >= 0) {
			ammountAux++;
			tweets = (TWEET **) realloc(tweets, sizeof(TWEET*)*ammountAux);
			tweets[ammountAux-1] = tweet;
			tweet = (TWEET *) malloc(sizeof(TWEET));
			if(tweet == NULL) {
				return NULL;
			}
		} 
	}
	*ammount = ammountAux;

	fclose(file);
	return tweets;
}
