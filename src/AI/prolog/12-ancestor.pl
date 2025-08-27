/*
    Prolog is a logic programming language often used in AI for 
    knowledge representation and reasoning. 
    This program demonstrates an "Ancestor" relationship using 
    facts and rules. The system can infer if one person is an 
    ancestor of another.

    Key concepts used:
    - Facts: direct parent relationships
    - Rules: infer ancestor recursively
    - Queries: ask the system questions
*/


% Facts: parent(Parent, Child)
parent(john, mary).
parent(mary, alice).
parent(john, mike).
parent(mike, bob).

% Rule: X is ancestor of Y if X is parent of Y
ancestor(X, Y) :- parent(X, Y).

% Rule: X is ancestor of Y if X is parent of Z and Z is ancestor of Y
ancestor(X, Y) :- 
    parent(X, Z), 
    ancestor(Z, Y).

% queries 
run_queries :-
    % list of queries in prolog, TestQueries is var since it starts with upper case
    TestQueries = [ancestor(john, alice), ancestor(mary, bob), ancestor(john, bob)],
    % for loop, assigns member of TestQueries to Q and runs following commands
    % call(Q)->runs the Query, and choses true or the false statement (assigned to Result var)
    forall(member(Q, TestQueries),
           (
             (call(Q) -> Result = true ; Result = false),
    % format('') is printf, ~w is %s, ~n is \n, values are in list instead of open vars 
             (format('~w -> ~w~n', [Q, Result]))
            )
    ).
% Initialization ensures run_tests is executed automatically on load
:- initialization(run_queries).
