HOMEWORK 8: B+ TREES




NAME:  <Michael Levine>




COLLABORATORS AND OTHER RESOURCES:
List the names of everyone you talked to about this assignment
(classmates, TAs, ALAC tutors, upperclassmen, students/instructor via
LMS, etc.), and all of the resources (books, online reference
material, etc.) you consulted in completing this assignment.


<Dan McCrevan, Claudio Lobraico, Herta Calvo-Faugier, Mik Bukow>


Remember: Your implementation for this assignment must be done on your
own, as described in "Academic Integrity for Homework" handout.




ESTIMATE OF # OF HOURS SPENT ON THIS ASSIGNMENT:  30




TESTING & DEBUGGING STRATEGY:
Discuss your strategy for testing & debugging your program.
What tools did you use (gdb/lldb/Visual Studio debugger,
Valgrind/Dr. Memory, std::cout & print, etc.)?  How did you test the
"corner cases" of your implementation?
I used lldb to debug the program by running it through, stepping through the functions and displaying the values of the member variables after the functions to see how/if they worked. I also used drmemory mainly in cases where I had segmentation faults, or when I was trying to confirm that the dynamic memory I used did not cause any leaks.
To test corner cases, I made my own test called MikeTest() and included it in my local version of the hw8_test.cpp




MISC. COMMENTS TO GRADER:
(optional, please be concise!)




MISC. COMMENTS TO GRADER:  
(optional, please be concise!)

