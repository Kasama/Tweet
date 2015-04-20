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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CMD_LENGTH 30

#define CMD_INSERT "insert"
#define CMD_EXIT "exit"
#define CMD_REMOVE "remove"
#define CMD_REQUEST "request"
#define CMD_REQUEST_ALL "all"
#define CMD_REQUEST_RRN "rrn"
#define CMD_REQUEST_USER "user"
#define CMD_CREDITS "credits"
#define CMD_LS "ls"
#define CMD_CHANGE_DATAFILE "cd"
#define CMD_CLEAR "clear"
#define CMD_HELP "help"

#define SPEC_PDF "http://wiki.icmc.usp.br/images/6/63/SCC0215012015projeto01TurmaBa.pdf"
#define DOC_PDF "INSERT NAME HERE"
#define TEACHER_NAME "Cristina Ciferre"


#ifdef _WIN32
#define CLEAR "cls"
#define CD "cd"
#define LS "dir"
#define SEP "\\"
#else
#define CLEAR "clear"
#define CD "cd"
#define LS "ls"
#define SEP "/"
#endif

enum {

	PROGNAME,	//0
	FILENAME	//1

};

void cmdInsert(char *fileName){
	char user[USER_SIZE];
	char lang[LANGUAGE_SIZE];
	char coords[COORDINATES_SIZE];
	char text[TEXT_SIZE];
	int fav, retweet, ret;
	long view;
	printf("Please type a user name:\n");
	scanf("\n");
	fgets(user, sizeof user, stdin);
	user[strlen(user)-2] = 0;
	printf("Type your geographic coordinates:\n");
	fgets(coords, sizeof coords, stdin);
	coords[strlen(coords)-2] = 0;
	printf("Specify the language of your text:\n");
	fgets(lang, sizeof lang, stdin);
	lang[strlen(lang)-2] = 0;
	printf("Type your text:\n");
	fgets(text, sizeof text, stdin);
	text[strlen(text)-2] = 0;
	printf("Type how many favorites your tweet has:\n");
	scanf("%d", &fav);
	printf("Type how many views your tweet has:\n");
	scanf("%ld", &view);
	printf("Type how many retweets your tweet has:\n");
	scanf("%d", &retweet);

	ret = insertTweet(fileName, text, user, coords, fav, lang, retweet, view);

	if(ret == SUCCESS){
		printf("Tweet inserted successfully!\n");
	}else{
		printf("Could not insert the tweet. Is the filesystem read only?\n");
	}
}

void cmdRemove(char *fileName){
	int RRN, ret;

	printf("Type the ID (RRN) of the Tweet to be removed: ");
	scanf("%d", &RRN);

	ret = removeTweet(fileName, RRN);
	if(ret == SUCCESS){
		printf("The Tweet %d was successfully removed\n", RRN);
	}else{
		printf("The Tweet %d could not be removed. Maybe it does not exist\n", RRN);
	}
}

void cmdRequest(char *fileName){
	TWEET **tweets;
	TWEET *tweet;
	char cmd[CMD_LENGTH], buf[10*CMD_LENGTH];
	int RRN, ammount, i;

	scanf("%s", cmd); 
	if(strcmp(cmd, CMD_REQUEST_ALL) == 0){
		tweets = requestAllTweets(fileName, &ammount);
	}else if(strcmp(cmd, CMD_REQUEST_USER) == 0){
		scanf("%s", cmd);
		tweets = findTweetByUser(fileName, cmd, &ammount);
	}else if(strcmp(cmd, CMD_REQUEST_RRN) == 0){
		scanf("%d", &RRN);
		tweet = requestTweet(fileName, RRN);
	}else{
		printf("Invalid command: %s%s, try typing '%s' for help\n", cmd, fgets(buf, sizeof buf, stdin), CMD_HELP);
	}

	if(tweets == NULL && tweet == NULL){
		printf("Could not find requested Tweet(s). Maybe it does not exist\n");
	}else{
		if(tweet == NULL){
			for(i = 0; i < ammount; i++){
				printf(printTweet(tweets[i]));
				free(tweets[i]);
			}
			free(tweets);
		}else{
			printf(printTweet(tweet));
			free(tweet);
		}
		printf("---------------------------------------------------\n");
	}
}

void cmdCredits(){
	printf("---------------------------------------------------\n");
	printf("This program was made by:\n\
			* Frederico de Azevedo Marques - N USP: 8936926\n\
			* Henrique Martins Loschiavo   - N USP: 8936972\n\
			* Lucas Kassouf Crocomo        - N USP: 8937420\n\
			* Roberto Pommella Alegro      - N USP: 8936756\n\
			\n\
			Using the specification from the PDF file available at:\n\
			%s\n\
			and the knowledge obtained from the classes from teacher %s\n\
			---------------------------------------------------\n", SPEC_PDF, TEACHER_NAME);
}

void cmdHelp(char *progname){
	printf("---------------------------------------------------\n");
	printf("Usage: %s fileName.dat\n", progname);
	printf("\n"); 
	printf("%s - print this help\n", CMD_HELP);
	printf("%s - print the credits\n", CMD_CREDITS);
	printf("%s - insert a tweet into the database (You will be prompted asking for the information to store)\n", CMD_INSERT);
	printf("%s <RRN>- remove a tweet from the database using it's ID(RRN)\n", CMD_REMOVE);
	printf("%s %s - print all tweets in the database\n", CMD_REQUEST, CMD_REQUEST_ALL);
	printf("%s %s <UserName> - print all tweets of the user <UserName>\n", CMD_REQUEST, CMD_REQUEST_USER);
	printf("%s %s <RRN>- print a single tweet using its ID (RRN)\n", CMD_REQUEST, CMD_REQUEST_RRN);
	printf("%s <fileName> - change the data file to the specified one\n", CMD_CHANGE_DATAFILE);
	printf("%s - list all files in current directory\n", CMD_LS);
	printf("%s - clear the screen \n", CMD_CLEAR);
	printf("Further documentation can be found by reading the PDF available at: \n");
	printf("%s\n", DOC_PDF);
	printf("---------------------------------------------------\n");

}
void cmdCD(char *fileName){
	char fileN[FILENAME_MAX];
	FILE *file;
	printf("Currently open file: %s\n", fileName); 
	printf("Type a new filename to open or press enter to keep current one (it will be created if it does not exist):\n");
	scanf("%s", fileN);
	file = fopen(fileName, "a+");
	if (file == NULL){
		fprintf(stderr, "A problem was encountered when trying to open the file, will keep current one. Maybe check your permissions\n");
	}else{
		fclose(file);
		if (strcmp(fileN, "") != 0){
			strcpy(fileName, fileN);
		}
	}
}
void cmdLs(){
	system(LS);
}
void cmdClear(){
	system(CLEAR);
}

int main(int argc, char *argv[]){

	char cmd[CMD_LENGTH], buf[CMD_LENGTH * 10];
	char fileName[FILENAME_MAX];
	FILE *file;

	if (argc != 2){
		printf("please input the name of a file to work with. it will be created if it does not exist:\n");
		scanf("%s", fileName);
	}else{
		strcpy(fileName, argv[FILENAME]);
	}
	file = fopen(fileName, "a+");
	if (file == NULL){
		fprintf(stderr, "A problem was encountered when trying to open a file, maybe check your permissions\n");
		exit(1);
	}
	fclose(file);

	printf("~~> ");

	while(scanf("%s", cmd) != EOF){
		if(strcmp(cmd, CMD_INSERT) == 0){
			cmdInsert(fileName);
		}else if(strcmp(cmd, CMD_REMOVE) == 0){
			cmdRemove(fileName);
		}else if(strcmp(cmd, CMD_REQUEST) == 0){
			cmdRequest(fileName);
		}else if(strcmp(cmd, CMD_CREDITS) == 0){
			cmdCredits();
		}else if(strcmp(cmd, CMD_CHANGE_DATAFILE) == 0){
			cmdCD(fileName);
		}else if(strcmp(cmd, CMD_CLEAR) == 0){
			cmdClear();
		}else if(strcmp(cmd, CMD_LS) == 0){
			cmdLs();
		}else if(strcmp(cmd, CMD_HELP) == 0){
			cmdHelp(argv[PROGNAME]);
		}else if(strcmp(cmd, CMD_EXIT) == 0){
			break;
		}else{
			printf("Invalid command: %s%s, try typing '%s' for help\n", cmd, fgets(buf, sizeof buf, stdin), CMD_HELP);
		}
		printf("~~> ");
	}

	return 0;
}
