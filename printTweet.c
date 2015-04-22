#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tweet.h"

char *printTweet(TWEET *tweet, char *fileName) {
	char *favCount, *reCount, *viewCount, *RRN;
	char *entireTweet;
	int i, entries;

	favCount = malloc(sizeof(int));
	reCount = malloc(sizeof(int));
	RRN = malloc(sizeof(int));
	viewCount = malloc(sizeof(long));

	sprintf(favCount, "%d", tweet->favoriteCount);
	sprintf(reCount, "%d", tweet->retweetCount);
	sprintf(viewCount, "%ld", tweet->viewsCount);

	entireTweet = malloc(sizeof(TWEET) + 11 + sizeof(int));

	strcat(entireTweet, tweet->text);
	strcat(entireTweet, "\n");
	strcat(entireTweet, tweet->userName);
	strcat(entireTweet, "\n");
	strcat(entireTweet, tweet->coords);
	strcat(entireTweet, "\n");
	strcat(entireTweet, tweet->language);
	strcat(entireTweet, "\n");
	strcat(entireTweet, favCount);
	strcat(entireTweet, "\n");
	strcat(entireTweet, reCount);
	strcat(entireTweet, "\n");
	strcat(entireTweet, viewCount);
	strcat(entireTweet, "\n");

	entries = ftell(fileName);
	entries = entries / sizeof(TWEET);

	for(i = 0; i < entries; i++){
		if(requestTweet(fileName, i) == tweet)
			break;
		
	}

	sprintf(RRN, "%d", i);
	strcat(entireTweet, RRN);
	strcat(entireTweet, "\n");

	free(favCount);
	free(reCount);
	free(viewCount);
	free(RRN);

	return entireTweet;
}
