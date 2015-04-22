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


TWEET **requestAllTweets(char *fileName, int *ammount) {
    FILE *file = NULL;
    TWEET *tweet = NULL;
    TWEET **tweets = NULL;
    
    int ammountAux = 0;
    ammount = &ammountAux;
    
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

    fseek(file, RRN*sizeof(TWEET), SEEK_SET);
    if(ferror(file)) {
        fclose(file);
        return NULL;
    }
    else {
        tweet = (TWEET *) malloc(sizeof(TWEET));
        if(tweet == NULL) {
            return NULL;
        }
        
        fread(tweet, sizeof(TWEET), 1, file);
        if(ferror(file)) {
            free(tweet);
            fclose(file);
            return NULL;
        }
        fclose(file);
        if(tweet->viewsCount < 0 && tweet->retweetCount < 0 && tweet->favoriteCount < 0){
            free(tweet);
            return NULL;
        }
    } 
    
    return tweet;
}


TWEET **findTweetByUser(char *fileName, char *userName, int *ammount) {
    FILE *file = NULL;
    TWEET *tweet = NULL;
    TWEET **tweets = NULL;
    
    int ammountAux = 0;
    ammount = &ammountAux;
    
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
    
    fclose(file);
    return tweets;
}
