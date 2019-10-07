# Query-Scoring-Algorithm
A project for Unimelb COMP10002 Foundation of Algorithm subject semester 2

## Function
It takes queries as input in form of words. Then it will finds the words from the queries that match with the line 
in the text file and calculate the score for each line. Then it will display the top 5 line with 
highest score based on the scoring criteria sorted descendingly. 

## File Description
alice-eg.txt & pg11.txt is the test text <br />
test0-out.txt is the output from the query "ali lat long" for alice-eg.txt<br />
test1-out.txt is the output from the query "f dow long way" for pg11.txt<br />
test2-out.txt is the output from the query "ali lat long" for alice-eg.txt<br />
test3-out.txt is the output from the query "f dow long way" for pg11.txt<br />

## How To Use
Compile the ass1.c code using gcc. Then use it from terminal in this format:
./(name of compiled exec file) (list of query separated by space) < (the name of the text file to search the query from) 
