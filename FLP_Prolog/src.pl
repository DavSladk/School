% Project: Kostra grafu (spanning tree)
% Author: Bc. David Sladký
% Login: xsladk07
main :-
    %set_prolog_flag(stack_limit, 2_147_483_648),
    read_lines(Ls),
    loadData(Ls,Es,_),
    sort(Es, Ess),
    bagof(X,subset(Ess,X),Sub),
    processBones(Sub).

:- dynamic(edge/2).
:- dynamic(state/1).

% Checks if graph is spanning tree and if it is, then print it
processBones([]).
processBones([H|T]) :-
    isBone(H,[],[]) -> (
        printBone(H),
        processBones(T)
    );
        processBones(T).

%-----------------------
% isBone(X,B,V) - checks if graph is spanning tree
%     X - graph to check
%     B - buffer of edges
%     V - visited states
%-----------------------
% If all edges were successfully parsed, make last
% check if all possible states were visited.
isBone([],[],V) :-
    sort(V,SV),
    bagof(O,state(O),Os),
    length(SV, SVl),
    length(Os, Osl),
    SVl == Osl.
% If both states of edge are same -> fail.
isBone([[S1,S2]|_],_,_) :-
    S1 == S2,
    !,
    false.
% Starting term - put both states of edge into visited states
isBone([[S1,S2]|T],[],[]) :-
    isBone(T,[],[S1,S2]).
% If exactly one state of a edge was visited, put its states
% into visted and return buffer into edges to evaluate
isBone([[S1,S2]|T],B,V) :-
    (memberchk(S1,V),\+memberchk(S2,V);\+memberchk(S1,V),memberchk(S2,V)),
    append(T,B,C),
    isBone(C,[],[S1,S2|V]).
% If neither state in edge was visited, then put edge into buffer.
isBone([[S1,S2]|T],B,V) :-
    \+ memberchk(S1,V),
    \+ memberchk(S2,V),
    isBone(T,[[S1,S2]|B],V).

% Print graph in desired format.
printBone([H]) :-
    format('~w-~w~n',H).
printBone([H,HH|T]) :-
    format('~w-~w ',H),
    printBone([HH|T]).

%-------loadData----------
% loadData(X,Y,Z)
%     X - data to load
%     Y - loaded edges
%     Z - loaded states
%  also puts every edge and state into database
loadData([],E,S) :-
    E = [],
    S = [].
loadData([[S1,W,S2|[]]|TT], Es, Ss) :-
    char_code(S1, S1c),
    char_code(S2, S2c),
    char_code(W, Wc),
    S1c >= 65,
    S1c =< 90,
    S2c >= 65,
    S2c =< 90,
    Wc == 32 -> (
        assertEdge(S1, S2),
        assertState(S1),
        assertState(S2),
        loadData(TT, Ess, Sss),
        sort([S1,S2],X),
        Es = [X|Ess],
        Ss = [S1,S2|Sss]
    )
    ; loadData(TT, Ess, Sss),
      Es = Ess,
      Ss = Sss.

% wrappers for assert to prevent duplicates
assertState(S) :- state(S).
assertState(S) :- assertz(state(S)).

assertEdge(S1,S2) :- edge(S1,S2).
assertEdge(S1,S2) :- assertz(edge(S1,S2)).
%-------loadData----------

%-------input-reading----------
% source: input2.pl
/** cte radky ze standardniho vstupu, konci na LF nebo EOF */
read_line(L,C) :-
    get_char(C),
    (isEOFEOL(C), L = [], !;
        read_line(LL,_),% atom_codes(C,[Cd]),
        [C|LL] = L).


/** testuje znak na EOF nebo LF */
isEOFEOL(C) :-
    C == end_of_file;
    (char_code(C,Code), Code==10).


read_lines(Ls) :-
    read_line(L,C),
    ( C == end_of_file, Ls = [] ;
      read_lines(LLs), Ls = [L|LLs]
    ).
%-------input-reading----------

% subset(X,Y)
%     X - input set
%     Y - output set of all subsets
% source: https://stackoverflow.com/a/4917016
% license: https://creativecommons.org/licenses/by-sa/3.0/
subset([], []).
subset([E|Tail], [E|NTail]):-
    subset(Tail, NTail).
subset([_|Tail], NTail):-
    subset(Tail, NTail).
%-------subset-----------------
