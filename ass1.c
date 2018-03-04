#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

// Maximum character in one line
#define MAX_LINE 1000
// Maximum character in one query and word
#define MAX 1000
// Maximum number of rank available
#define MAX_RANK 5
// Zero for score if it's lower than 0.00001 as in %.3lf format 0.00001 is 0
#define LOWER_BOUND_ZERO 0.00001
int mygetchar();
void stage1(int argc, char *argv[]);
double stage3(  int count_line_word, 
                char line_word[MAX_LINE][MAX], 
                int argc, 
                char *argv[], 
                int line);
void stage4_organizer(  char one_line[], 
                        int num_line_keep[], 
                        double score_keep[],
                        char line_keep[MAX_RANK][MAX_LINE], 
                        double score, 
                        int line);
void int_swap(int *number1, int *number2);
void double_swap(double *number1, double *number2);

int main(int argc, char *argv[]){
    int i;
    stage1(argc,argv);

    // variable for Stage 2
    int bytes = 0;
    int line = 0;
    int word = 0;
    int temp_char;
    int done = 0;
    int last_is_char;

    // variable for stage 3
    char line_word[MAX_LINE][MAX];
    int character = 0;

    // variable for stage 4
    char one_line[MAX_LINE];
    double score_keep[MAX_RANK];
    char line_keep[MAX_RANK][MAX_LINE];
    int num_line_keep[MAX_RANK];
    double score;

    
    while ((temp_char = mygetchar()) != EOF){
        // condition to print the divider
        if (done != 1){
            printf("---\n");
            done = 1;
        }

        /* if it's not end of the line, put the char to one_line 
           which will be a string of the one_line */
        if (temp_char != '\n'){
            printf("%c",temp_char);
            one_line[bytes] = temp_char;
            bytes++;
        }

        // algorithm to determine words in the line, add the word to line_word
        if (isalnum(temp_char)){
            last_is_char = 1;
            line_word[word][character] = temp_char;
            character++;
        } else if (!isalnum(temp_char) && last_is_char == 1){
            last_is_char = 0;
            line_word[word][character] = '\0';
            word++;
            character = 0;
        }

        if (temp_char == '\n'){
            line++;
            one_line[bytes] = '\0';
            // if there's nothing in the line, continue to the next iteration
            if(bytes == 0){
                continue;
            }
            printf("\n");
            printf("S2: line = %d, bytes = %d, words = %d\n", line,bytes,word);
            score = stage3(word, line_word, argc, argv, line);
            stage4_organizer(   one_line, 
                                num_line_keep, 
                                score_keep, 
                                line_keep, 
                                score, 
                                line);
            word = 0;
            bytes = 0;
            character = 0;
            last_is_char = 0;
            done = 0;
        }
    }
    printf("------------------------------------------------\n");

    // loop for printing the top 5 for stage 4
    for (i = 0; i < MAX_RANK; i++){
        if (score_keep[i] < LOWER_BOUND_ZERO){
            break;
        }
        printf("S4: line = %d, score = %.3lf\n",num_line_keep[i],score_keep[i]);
        printf("%s", line_keep[i]);
        printf("\n");
        printf("---\n");
    }

	return 0;	
}

// function that takes every char from the stdin and ignore 'cr' character
int mygetchar() {
    int c;
    while ((c=getchar())=='\r') {
    }
    return c;
}

// function that filter whether the input are valid or not for stage 1
void stage1(int argc, char *argv[]){
    int i,j,ch;
    int invalid = 0;
    // if there is no argument in command line
    if (argc == 1){
        printf("S1: No query specified, must provide at least one word\n");
        exit(EXIT_FAILURE);
    }
    // printing all query
    printf("S1: query =");
    for (i = 1; i < argc; i++){
        printf(" %s", argv[i]);
    }
    printf("\n");
    // checking invalid character in query
    for (i = 1; i < argc; i++){
        if (strlen(argv[i]) == 0){
            printf("S1: there is string with length 0 in the query\n");
            invalid = 1;
            continue;
        }
        for (j = 0; argv[i][j] != 0; j++){
            ch = argv[i][j];
            if (!((ch <= 'z' && ch >= 'a')||(ch >= 0 && ch <= 9))){
                printf("S1: %s: invalid character(s) in query\n", argv[i]);
                invalid = 1;
                break;
            }
        }
    }
    // if there is invalid character in query exit the program
    if(invalid == 1){
        exit(EXIT_FAILURE);
    }
}

/* function that takes the number of the word in the line as count_line_word, 
   and each word in the line as line_word and takes the query which is the
   input from stdin and the line number as line then it calculates and 
   print the S3 for the program */
double stage3(  int count_line_word, 
                char line_word[MAX_LINE][MAX], 
                int argc, 
                char *argv[], 
                int line){

    int i,j;
    int query_len;
    int wrong;
    double score;
    double freq = 0;
    double number_of_word = (double) count_line_word;

    // compare every query and every word in the line then calculate the score 
    for (j = 1; j < argc; j++){
        freq = 0;
        query_len = strlen(argv[j]);
        for (i = 0; i < count_line_word; i++){
            wrong = 0;
            if (strncasecmp(argv[j],line_word[i],query_len) != 0){
                wrong = 1;
            }
            if (wrong != 1){
                freq++;
            }
        }
        score = score + (log(1.0 + freq)/log(2.0));
    }
    score = score/(log(8.5 + number_of_word)/log(2.0));
    printf("S3: line = %d, score = %.3lf\n", line, score);
    return score;
}

/* function that takes a string of the line in one_line and the array that store 
   the top 5 line number in num_line_keep and top 5 score in score_keep and also 
   the top 5 line in line_keep, it also takes the score for the line it's in for 
   argument. It will then update the top 5 line, score, and line after called */
void stage4_organizer(  char one_line[], 
                        int num_line_keep[], 
                        double score_keep[],
                        char line_keep[MAX_RANK][MAX_LINE], 
                        double score, 
                        int line){
    int i;
    int count;
    char temp[MAX_LINE];
    for (i = 0; i < MAX_RANK; i++){
        if (score > score_keep[i]){
            // sort the top 5 score, line, line_number and update it
            for (count = MAX_RANK-1; count != i; count--){
                int_swap(&num_line_keep[count-1],&num_line_keep[count]);
                double_swap(&score_keep[count-1],&score_keep[count]);
                strcpy(temp,line_keep[count-1]);
                strcpy(line_keep[count-1],line_keep[count]);
                strcpy(line_keep[count],temp);
            }
            num_line_keep[i] = line;
            score_keep[i] = score;
            strcpy(line_keep[i],one_line);
            break;
        }
    }
}


void int_swap(int *number1, int *number2){
    int temp = 0;
    temp = *number1;
    *number1 = *number2;
    *number2 = temp;
}

void double_swap(double *number1, double *number2){
    double temp = 0;
    temp = *number1;
    *number1 = *number2;
    *number2 = temp;
}

// algorithm is fun