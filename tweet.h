/*
 * @Authors:
 *
 * Frederico de Azevedo Marques - 8936926
 * Henrique Martins Loschiavo   - 8936972
 * Lucas Kassouf Crocomo        - 8937420
 * Roberto Pommella Alegro      - 8936756
 *
 */

#ifndef TWEET_H
#define TWEET_H 

#ifndef SUCCESS
#define SUCCESS 1
#endif // SUCCESS

#ifndef ERROR
#define ERROR 0
#endif // ERROR

#define TEXT_SIZE          140
#define USER_SIZE          70
#define COORDINATES_SIZE   40
#define LANGUAGE_SIZE      30

/*
 * Internal Structure of a Tweet:
 *
 * +------------------------+----------------+
 * |         Type           |   Field Name   |
 * +------------------------+----------------+
 * | char[TEXT_SIZE]        | text		     |
 * | char[USER_SIZE]        | userName		 |
 * | char[COORDINATES_SIZE] | coords		 |
 * | int                    | favoriteCount	 |
 * | char[LANGUAGE_SIZE]    | language		 |
 * | int                    | retweetCount	 |
 * | long                   | viewsCount	 |
 * +------------------------+----------------+
 * 
 */
typedef struct tweet TWEET;

// insert a tweet into the database
// return SUCCESS if successful and ERROR if it wasn't possible to insert
int insertTweet(				\
		char   *fileName,       \
		char   *text,           \
		char   *userName,       \
		char   *coords,         \
		int    favoriteCount,   \
		char   *language,       \
		int    retweetCount,    \
		long   viewsCount       \
);

// return all tweets (array), NULL if there is none
// returns via pointer, the number of tweets found 0 if none
TWEET **requestAllTweets(char *fileName, int *ammount);

// return a single tweet, using the ID number (RRN), NULL if RRN is invalid
TWEET *requestTweet(char *fileName, int RRN);

// return all tweets (array) from a user using sequencial search, NULL if none found
// returns via pointer, the number of tweets found 0 if none
TWEET **findTweetByUser(char *fileName, char *userName, int *ammount);

// remove a tweet, using it's ID number
// return SUCCESS if successful and ERROR if the register was not found
int removeTweet(char *fileName, int RRN);

// returns a string to be printed of a single tweet
// needs to be freed
char *printTweet(TWEET *tweet, char *fileName);

#endif // TWEET_H
