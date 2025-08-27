% Objects and their types
is_a(mat1, mat).
is_a(cat1, cat).
is_a(tom, cat).
is_a(bird1, bird).
% Taxonomy / Hierarchy
is_a(cat, mammal).
is_a(bird, animal).
is_a(mammal, animal).

% Relations
sat_on(cat1, mat1).
sat_on(cat, mat1). % general cat sat on mat
caught(tom, bird1).
owned_by(tom, john).
color(tom, ginger).
likes(cat, cream).
has(mammal, fur).


run_queries :-
    TestQueries = [
        is_a(mat1, mat),
        is_a(cat1, cat),
        is_a(tom, cat),
        is_a(bird1, bird),
        sat_on(cat1, mat1),
        caught(tom, bird1),
        owned_by(tom, john),
        color(tom, ginger),
        likes(cat, cream),
        sat_on(cat, mat1),
        is_a(cat, mammal),
        is_a(bird, animal),
        is_a(mammal, animal),
        has(mammal, fur)
    ],
    forall(member(Q, TestQueries),
           (
             (call(Q) -> Result = true ; Result = false),
             (format('~w -> ~w~n', [Q, Result]))
           )
    ).

:- initialization(run_queries).
