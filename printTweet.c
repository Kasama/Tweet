#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tweet.h"

char *printTweet(TWEET *tweet, char *fileName) {
	char *favCount, *reCount, *viewCount, *RRN;
	char *entireTweet;
	int i, entries;
	FILE *file;

	favCount = malloc(sizeof(char) * 11);
	reCount = malloc(sizeof(char) * 11);
	RRN = malloc(sizeof(char) * 11);
	viewCount = malloc(sizeof(char) * 20);

	sprintf(favCount, "%d", tweet->favoriteCount);
	sprintf(reCount, "%d", tweet->retweetCount);
	sprintf(viewCount, "%ld", tweet->viewsCount);

	entireTweet = malloc(sizeof(TWEET) + 11 + sizeof(char) * 11);

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

	file = fopen(fileName);
	entries = ftell(file);
	entries = entries / sizeof(TWEET);
	fclose(file)

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
