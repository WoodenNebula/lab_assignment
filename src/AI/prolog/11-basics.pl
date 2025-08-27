% [mary, pizza, john, alice, chocolate] are atoms aka constant symbols
likes(mary, pizza).
% These are the facts, 
likes(john, mary).
likes(alice, chocolate).

% Rule :- <fact> {logical connectivities: and, or; not\+} <fact2>.
happy(X) :- likes(X, pizza); likes(X, chocolate).

% Query examples
?- happy(mary).
?- happy(alice).
?- likes(john, Who).
?- likes(chocolate, What).
