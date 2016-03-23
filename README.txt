HOMEWORK 7: WORD FREQUENCY MAPS


NAME:  Jacob Martin


COLLABORATORS:  
(You must do this assignment on your own, as described in "Academic
Integrity for Homework" handout.  If you did discuss the problem or
errors messages, etc. with anyone, please list their names here. )




ANALYSIS OF PERFORMANCE OF YOUR ALGORITHM:
(order notation & concise paragraph, < 200 words)

n = total number of words in the sample text file
m = number of unique words in the file
w = width of the sequencing window
p = average number of words observed to follow a particular word

How much memory will the map data structure require, in terms of n, m,
w, and p (order notation for memory use)?
m*p^(w-1)


What is the order notation for performance (running time) of each of
the commands?
Load   nlog(m) log is due to using the find member of map
Print p + log(p)   because my for loop goes through p words and outside of the loop it calls find in a map of p
Generate  Length*(log(p)*p+(w-1)) 




EXTRA CREDIT
Parsing & using punctuation data, implementation supports *any*
window size, new test cases (describe & summarize performance, but
don't include large datasets with submission).
Can you explain how you could write this to support any window size?


MISC. COMMENTS TO GRADER:  
In hindsight, I probably should have made a function for each command and each window of the command instead of allowing there to be such a large main file,
but I dont have much time left. I will definetly not let my main file get this messy in the future.


