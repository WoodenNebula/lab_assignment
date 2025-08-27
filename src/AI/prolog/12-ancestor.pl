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

run_queries :-
    TestQueries = [ancestor(john, alice), ancestor(mary, bob), ancestor(john, bob)],
    forall(member(Q, TestQueries),
           (
             (call(Q) -> Result = true ; Result = false),
             (format('~w -> ~w~n', [Q, Result]))
            )
    ).

:- initialization(run_queries).
