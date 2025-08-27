% Facts: parent(Parent, Child)
parent(john, mary).
parent(john, mike).
parent(mary, alice).
parent(mike, bob).
parent(alice, claire).

% Facts: gender
male(john). male(mike). male(bob).
female(mary). female(alice). female(claire).

% Rules
father(X, Y) :- parent(X, Y), male(X).
mother(X, Y) :- parent(X, Y), female(X).
grandparent(X, Y) :- parent(X, Z), parent(Z, Y).
sibling(X, Y) :- parent(Z, X), parent(Z, Y), X \= Y.

run_queries:-
    TestQueries = [
        father(john, mary),
        mother(mary, alice),
        grandparent(john, alice),
        sibling(mary, mike),
        sibling(alice, bob)
    ],

    forall(member(Q, TestQueries),
           (
             (call(Q) -> Result = true ; Result = false),
             (format('~w -> ~w~n', [Q, Result]))
            )
    ).

:- initialization(run_queries).
