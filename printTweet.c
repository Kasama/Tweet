#include <stdio.h>
#include "tweet.h"

char *printTweet(TWEET *tweet) {

	printf("Tweet : %s\n", tweet->text);
	printf("User : @%s\n", tweet->userName);
	printf("Coordenates : %s\n", tweet->coords);
	printf("Langueage : %s\n", tweet->language);
	printf("Favortites #%d\n", tweet->favoriteCount);
	printf("Retweets #%d\n", tweet->retweetCount);
	printf("Views #%ld\n", tweet->viewsCount);

	return tweet->text;
}
