% A simple expert system copied from Bratko's book "Prolog Programming for AI".
% To use it:
%
%   % sicstus                       start sicstus
%   | ?- [expert].                  load program file expert.pl
%   | ?- expert.                    start program
%   Question please
%   |: peter isa tiger.             give overall goal with full stop
%   Is it true: peter has hair? y.  answer with y n or w (why) and a full stop
%   ...
%   | ?- halt.                      stop sicstus

%------------------------------------------------------------------------------
% SICSTUS SYSTEM DETAILS.  We have to tell sicstus which relations we are going
% to allow to be extended with assert, and we have to define some relations
% which are used in Bratko but are not standard in sicstus.

:- dynamic wastold/3, end_answers/1, lastindex/1.

:- op(900, fy, not).

not X :- \+ X.

instantiated( X) :-
 nonvar( X),
 X =.. [_ | Args],
 nonvarlist( Args).

nonvarlist( []).
nonvarlist( [A | AL]) :-
 nonvar( A),
 nonvarlist( AL).

member(X, [X|_]).
member(X, [_|L]) :- member(X, L).

%------------------------------------------------------------------------------
% THE INFERENCE ENGINE.  Operators are defined for representing rules as data
% structures.  Note that we use :: for defining rules because : has a standard
% meaning in sicstus; relation N::R means "there is a rule R called N".

:- op(900, xfx, ::).
:- op(800, xfx, was).
:- op(870, fx,  if).
:- op(880, xfx, then).
:- op(550, xfy, or).
:- op(540, xfy, and).
:- op(300, fx,  'derived by').
:- op(600, xfx, from).
:- op(600, xfx, by).

explore( Goal, _, Goal is true was 'found as a fact') :-
 fact :: Goal.

explore( Goal, Trace, Goal is TruthValue was 'derived by' Rule from Answer) :-
 Rule :: if Condition then Goal,
 explore( Condition, [Goal by Rule | Trace], Answer),
 truth( Answer, TruthValue).

explore( Goal1 and Goal2, Trace, Answer) :- !,
 explore( Goal1, Trace, Answer1),
 continue( Answer1, Goal1 and Goal2, Trace, Answer).

explore( Goal1 or Goal2, Trace, Answer) :-
 exploreyes( Goal1, Trace, Answer);
 exploreyes( Goal2, Trace, Answer).

explore( Goal1 or Goal2, Trace, Answer1 and Answer2) :- !,
 not exploreyes( Goal1, Trace, _),
 not exploreyes( Goal2, Trace, _),
 explore( Goal1, Trace, Answer1),
 explore( Goal2, Trace, Answer2).

explore( Goal, Trace, Goal is Answer was told) :-
 useranswer( Goal, Trace, Answer).


exploreyes( Goal, Trace, Answer) :-
 explore( Goal, Trace, Answer),
 positive( Answer).


continue( Answer1, _ and Goal2, Trace, Answer) :-
 positive(Answer1),
 explore( Goal2, Trace, Answer2),
 ( positive( Answer2), Answer = Answer1 and Answer2;
   negative( Answer2), Answer = Answer2).

continue( Answer1, _ and _, _, Answer1) :-
 negative( Answer1).


truth( _ is TruthValue was _, TruthValue) :- !.

truth( Answer1 and Answer2, TruthValue) :-
 truth( Answer1, true),
 truth( Answer2, true), !,
 TruthValue = true;
 TruthValue = false.

positive( Answer) :- truth( Answer, true).

negative( Answer) :- truth( Answer, false).

%------------------------------------------------------------------------------
% THE USER INTERFACE.

getreply( Reply) :-
 read( Answer),
 means( Answer, Meaning), !,
 Reply = Meaning;
 nl, write('Answer unknown, try again please'), nl,
 getreply( Reply).

means(yes,yes).
means(y,yes).
means(no,no).
means(n,no).
means(why,why).
means(w,why).

useranswer( Goal, Trace, Answer) :-
 askable( Goal, _),
 freshcopy( Goal, Copy),
 useranswer( Goal, Copy, Trace, Answer, 1).

useranswer( Goal, _, _, _, N) :-
 N > 1,
 instantiated( Goal), !,
 fail.

useranswer( Goal, Copy, _, Answer, _) :-
 wastold( Copy, Answer, _),
 instance_of( Copy, Goal), !.

useranswer( Goal, _, _, true, N) :-
 wastold( Goal, true, M),
 M >= N.

useranswer( Goal, Copy, _, _, _) :-
 end_answers( Copy),
 instance_of( Copy, Goal), !,
 fail.

useranswer( Goal, _, Trace, Answer, N) :-
 askuser( Goal, Trace, Answer, N).

askuser( Goal, Trace, Answer, N) :-
 askable( Goal, ExternFormat),
 format( Goal, ExternFormat, Question, [], Variables),
 ask( Goal, Question, Variables, Trace, Answer, N).
 
ask( Goal, Question, Variables, Trace, Answer, N) :-
 nl,
 ( Variables = [], !,
   write('Is it true: ');
   write('Any (more) solution to: ')),
 write( Question), write('? '),
 getreply( Reply), !,
 process( Reply, Goal, Question, Variables, Trace, Answer, N).


process( why, Goal, Question, Variables, Trace, Answer, N) :-
 showtrace( Trace),
 ask( Goal, Question, Variables, Trace, Answer, N).

process( yes, Goal, _, Variables, Trace, true, _) :-
 nextindex( Next),
 Next1 is Next + 1,
 ( askvars( Variables),
   assertz( wastold( Goal, true, Next) );
   freshcopy( Goal, Copy),
   useranswer( Goal, Copy, Trace, _, Next1) ).

process( no, Goal, _, _, _, false, _) :-
 freshcopy( Goal, Copy),
   wastold( Copy, true, _), !,
   assertz( end_answers( Goal)),
   fail;
 nextindex( Next),
   assertz( wastold( Goal, false, Next) ).

format( Var, Name, Name, Vars, [Var/Name | Vars]) :-
 var( Var), !.

format( Atom, Name, Atom, Vars, Vars) :-
 atomic( Atom), !,
 atomic( Name).

format( Goal, Form, Question, Vars0, Vars) :-
 Goal =.. [Functor | Args1],
 Form =.. [Functor | Forms],
 formatall( Args1, Forms, Args2, Vars0, Vars),
 Question =.. [Functor | Args2].

formatall([], [], [], Vars, Vars).

formatall([X|XL], [F|FL], [Q|QL], Vars0, Vars) :-
 formatall( XL, FL, QL, Vars0, Vars1),
 format( X, F, Q, Vars1, Vars).

askvars( []).

askvars( [Variable/Name | Variables]) :-
 nl, write( Name), write(' = '),
 read( Variable),
 askvars( Variables).

showtrace( []) :-
 nl, write( 'This was your question'), nl.

showtrace( [Goal by Rule | Trace]) :-
 nl, write('To investigate, by '),
 write( Rule), write( ', '),
 write( Goal),
 showtrace(Trace).


instance_of( Term, Term1) :-
 freshcopy( Term1, Term2),
 numbervars( Term2, 0, _), !,
 Term = Term2.

freshcopy( Term, FreshTerm) :-
 asserta( copy( Term)),
 retract( copy( FreshTerm)), !.

lastindex( 0).

nextindex( Next) :-
 retract( lastindex( Last)), !,
 Next is Last + 1,
 assert( lastindex( Next)).






present( Answer) :-
 nl, showconclusion( Answer),
 nl, write('Would you like to see how? '),
 getreply( Reply),
 ( Reply = yes, !, show(Answer); true).

showconclusion( Answer1 and Answer2) :- !,
 showconclusion( Answer1), write(' and '),
 showconclusion( Answer2).

showconclusion( Conclusion was _) :-
 write( Conclusion).

show( Solution) :-
 nl, show( Solution,0), !.

show( Answer1 and Answer2, H) :- !,
 show( Answer1, H),
 tab( H), write( and), nl,
 show( Answer2, H).

show( Answer was Found, H) :-
 tab( H), writeans( Answer),
 nl, tab( H),
 write('  was '),
 show1(Found,H).

show1( Derived from Answer, H) :- !,
 write( Derived), write(' from'),
 nl, H1 is H + 4,
 show( Answer, H1).

show1( Found, _) :-
 write( Found), nl.

writeans( Goal is true) :- !,
 write( Goal).

writeans( Answer) :-
 write( Answer).




expert :-
 getquestion( Question),
 ( answeryes( Question);
   answerno( Question) ).

answeryes( Question) :-
 markstatus( negative),
 explore( Question, [], Answer),
 positive( Answer),
 markstatus( positive),
 present( Answer), nl,
 write( 'More solutions? '),
 getreply( Reply),
 Reply = no.

answerno( Question) :-
 retract( no_positive_answer_yet), !,
 explore( Question, [], Answer),
 negative( Answer),
 present( Answer), nl,
 write( 'More negative solutions? '),
 getreply( Reply),
 Reply = no.

markstatus( negative) :-
 assert( no_positive_answer_yet).

markstatus( positive) :-
 retract( no_positive_answer_yet), !; true.

getquestion( Question) :-
 nl, write( 'Question please'), nl,
 read( Question).
 
%------------------------------------------------------------------------------
% AN EXAMPLE DATABASE.  Information about animals.

:- op(100, xfx, [has, gives, 'does not', eats, lays, isa]).
:- op(100, xf, [swims, flies]).

fact :: X isa animal :-
 member( X, [cheetah, tiger, penguin, albatross]).

rule1 :: if   Animal has hair or
              Animal gives milk
        then  Animal isa mammal.

rule2 :: if   Animal has feathers or
              Animal flies and
              Animal lays eggs
        then  Animal isa bird.

rule3 :: if   Animal isa mammal and
              ( Animal eats meat or
                Animal has 'pointed teeth' and
                Animal has claws and
                Animal has 'forward pointing eyes')
        then  Animal isa carnivore.

rule4 :: if   Animal isa carnivore and
              Animal has 'tawny colour' and
              Animal has 'dark spots'
        then  Animal isa cheetah.

rule5 :: if   Animal isa carnivore and
              Animal has 'tawny colour' and
              Animal has 'black stripes'
        then  Animal isa tiger.

rule6 :: if   Animal isa bird and
              Animal 'does not' fly and
              Animal swims
        then  Animal isa penguin.

rule7 :: if   Animal isa bird and
              Animal isa 'good flier'
        then  Animal isa albatross.

askable(_ gives _, 'Animal' gives 'What').
askable(_ flies, 'Animal' flies).
askable(_ lays eggs, 'Animal' lays eggs).
askable(_ eats _, 'Animal' eats 'What').
askable(_ has _, 'Animal' has 'Something').
askable(_ 'does not' _, 'Animal' 'does not' 'DoSomething').
askable(_ swims, 'Animal' swims).
askable(_ isa 'good flier', 'Animal' isa 'good flier').
