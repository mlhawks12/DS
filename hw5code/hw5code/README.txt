HOMEWORK 5: DSRADIO SONG GROUPS


NAME:  < Michael Levine >


COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.

< Herta Calvo-Faugier, Professor Thompson, Kyle Hackett, Daniel McCrevan >

Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.


ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  < 25 hours >



ORDER NOTATION:
For each of the functions below, write the order notation O().
Write each answer in terms of s = the number of songs in the library,
g = the number of song groups, a = the average number of songs by the same 
artist, and l = the average length of song groups.

AddSongToLibrary O(1)

GroupExists O(1)

AddToGroup O(g)

MakeGroup O(1)

PrintGroupRewind O(l)

PrintLongestGroupRewind O(g^2) 

RemoveGroup O(g)

DeleteAllSongGroups O(g^2)

RemoveFromGroup O(g)

PrintGroupMarathon (a)



TESTING & DEBUGGING STRATEGY: 
Discuss your strategy for testing & debugging your program.  
What tools did you use (gdb/lldb/Visual Studio debugger,
Valgrind/Dr. Memory, std::cout & print, etc.)?  How did you test the
"corner cases" of your implementation?
I used lldb, std::cout, and drmemory to debug. I tested corner cases by writing my own custom inputs and trying to break each function individually. I also drew out a lot of diagrams.


MISC. COMMENTS TO GRADER:  
(optional, please be concise!)

