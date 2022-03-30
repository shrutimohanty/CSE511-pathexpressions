# p1_path_expressions
There are two parts to the programming assignment. It is recommended to progress in the same order, and to go through the topic on Path Expressions in Bloom's thesisPreview the document prior to attempting the project. Your task will involve creating mechanisms to implement path expressions and subsequently use it to solve a synchronization problem.

**Part 1 - Mechanisms for Path Expressions**
**Description**
In the first part of the project, you will be provided with solutions (implementations) to two well known synchronization problems, viz. Readers-Writers problem and the Bounded buffer (one slot) problem. It will consist of several threads calling the routines (read/write or append/remove) implementing the problems. Additionally, it will also specify the paths which will govern the synchronization of the threads calling these routines. You should write routines which will take in path specifications given by these implementations, which perform the requisite synchronizations specified by these paths.

**Part 2 - Child Care problem**
**Description**
In the second part of the project, you will have to solve the Child-Care problem as described below and validate the solution using the mechanisms supported by your Path Expression synchronizer developed in the first part. The child care problem is described as follows.


Imagine you are running a child-care centre. Care-givers can come into the centre at any time, and there could be multiple care-givers and children in the centre at any time. However, State law stipulates that every child-care centre must be manned by at least one care-giver, before children can be dropped off. Children can be picked up by their parents at any time. Care-givers can also leave at any time PROVIDED there is at least 1 other care-giver in the center who can take care of the children present. The last care giver can leave only when there are no children in the centre.
