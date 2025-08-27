% [mary, pizza, john, alice, chocolate] are atoms aka constant symbols
likes(mary, pizza).
% These are the facts, 
likes(john, mary).
likes(alice, chocolate).

% Rule :- <fact> {logical connectivities: and, or; not\+} <fact2>.
happy(X) :- likes(X, pizza); likes(X, chocolate).

% queries to automatically run as opposed to manually type in repl
run_queries :-
    % list of queries in prolog, TestQueries is var since it starts with upper case
    TestQueries = [happy(mary), happy(john), likes(john, Who), likes(alice, What)],
    % for loop, assigns member of TestQueries to Q and runs following commands
    % call(Q)->runs the Query, and choses true or the false statement (assigned to Result var)
    forall(member(Q, TestQueries),
           (
             (call(Q) -> Result = true ; Result = false),
    % format('') is printf, ~w is %s, ~n is \n, values are in list instead of open vars 
             (format('~w -> ~w~n', [Q, Result]))
            )
    ).

% Initialization ensures run_queries is executed automatically on load
:- initialization(run_queries).
