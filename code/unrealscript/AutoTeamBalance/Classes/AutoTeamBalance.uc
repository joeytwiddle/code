
/
/
=
=
 
AutoTeamBalance
 
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=

\n

\n
/
/
 
A
 
mutator
 
that
 
makes
 
fair
 
teams
 
at
 
the
 
beginning
 
of
 
each
 
teamgame,
 
by
 
recording
 
the
 
relative
 
strengths
 
of
 
players
 
on
 
the
 
server
 
(linked
 
to
 
their
 
nick
/
ip)
.

\n
/
/
 
It
 
also
 
attempts
 
to
 
put
 
a
 
player
 
joining
 
the
 
game
 
on
 
an
 
appropriate
 
team
.

\n
/
/
 
It
 
can
 
only
 
build
 
player
 
stats
 
for
 
regular
 
players
 
who
 
stay
 
until
 
the
 
end
 
of
 
the
 
game
.

\n
/
/
 
It
 
will
 
also
 
take
 
a
 
while
 
after
 
running
 
to
 
build
 
up
 
accurate
 
stats
 
of
 
the
 
players
.

\n
/
/
 
For
 
the
 
first
 
week
 
or
 
so
 
you
 
may
 
wish
 
to
 
collect
 
stats
 
but
 
not
 
attempt
 
team-balancing:
 
just
 
set
 
bAutoBalanceTeamsFor*
=
False
 
but
 
leave
 
bUpdatePlayerStatsFor*
=
True
.

\n
/
/
 
by
 
Daniel
 
Mastersourcerer
 
at
 
Kitana
'
s
 
Castle
 
and
 
F0X|nogginBasher
 
of
 
no
 
fixed
 
abode
.

\n
/
/
 
(c)opyleft
 
May
 
2007
 
under
 
GNU
 
Public
 
Licence
\n

\n
/
/
 
vim:
 
tabstop
=
2
 
shiftwidth
=
2
 
expandtab
\n

\n
/
/
 
The
 
field
 
delimeter
 
for
 
playerData
 
in
 
the
 
config
 
files
 
is
 
a
 
space
 
"
 
"
 
since
 
that
 
can
'
t
 
appear
 
in
 
UT
 
nicks
 
(always
 
_)
\n

\n
/
/
 
DONE:
 
don
'
t
 
do
 
any
 
evening
 
of
 
teams
 
if
 
bTournament
=
True
;
 
but
 
atm
 
it
 
will
 
still
 
do
 
end-game
 
player
 
stats
 
updates
 
in
 
tournament
 
mode
;
 
and
 
why
 
not?!
\n

\n
/
/
 
DONE:
 
catch
 
the
 
end-of-game
 
event
 
and
 
collect
 
scores
 
then
 
(check
 
Level
.
Game
.
bGameEnded)
\n

\n
/
/
 
HALF-DONE:
 
when
 
the
 
playerData
 
array
 
gets
 
full,
 
old
 
records
 
are
 
not
 
recycled
 
properly
 
(atm
 
the
 
last
 
is
 
just
 
overwritten
 
repeatedly
 
:|
 
)
 
 
 
-
 
ok
 
now
 
we
 
recycle
 
the
 
record
 
with
 
shortest
 
play
 
hours
.
 
 
TODO:
 
It
'
s
 
not
 
perfect
 
because
 
the
 
new
 
player
'
s
 
stats
 
might
 
get
 
overwritten
 
pretty
 
soon
.
 
 
We
 
either
 
need
 
a
 
little
 
randomnity,
 
or
 
we
 
could
 
store
 
date_last_played[]
\n

\n
/
/
 
TODO:
 
catch
 
a
 
player
 
saying
 
"
!teams
"
,
 
maybe
 
write
 
some
 
custom
 
code
 
to
 
balance
 
the
 
teams
 
then
 
(by
 
swapping
 
1
/
2
 
players
 
only,
 
maybe
 
slightly
 
randomised
 
so
 
it
 
can
 
be
 
repeated
 
if
 
unsatisfactory
;
 
noo
 
that
 
could
 
get
 
too
 
spammy
 
:E)
\n

\n
/
/
 
Done
 
now:
 
i
 
shouldn
'
t
 
be
 
taking
 
averages
 
over
 
time,
 
but
 
over
 
#polls
 
:S
 
 
Actually
 
either
 
is
 
fine,
 
but
 
I
 
was
 
doing
 
it
 
weirdly
 
before
.

\n

\n
/
/
 
CONSIDER:
 
in
 
cases
 
of
 
a
 
standoff
 
(e
.
g
.
 
none
 
of
 
the
 
players
 
are
 
found
 
in
 
the
 
DB
 
so
 
all
 
have
 
UnknownStrength),
 
choose
 
something
 
random!
 
 
What
 
we
 
are
 
given
 
may
 
not
 
be
 
random
 
enough
 
(like
 
bPlayersBalanceTeams)
.

\n

\n
/
/
 
TODO:
 
config
 
option
 
bRankBots
 
(might
 
be
 
interesting
 
to
 
see
 
how
 
Visse
 
compares
 
to
 
the
 
humans
 
^^
 
)
\n

\n
/
/
 
Half-Done:
 
could
 
also
 
analyze
 
TDM
 
(DeathMatchPlus)
 
scores,
 
but
 
without
 
the
 
CTF
 
bonuses,
 
these
 
will
 
be
 
much
 
lower
 
(store
 
in
 
separate
 
fields?
 
e
.
g
.
 
avg_TDM_score
 
TDM_hours_played)
 
 
What
 
about
 
a
 
method
 
to
 
separate
 
all
 
teamgames?
 
 
OR
 
Easier:
 
make
 
a
 
separate
 
player
 
with
 
nic
k+
"
 
"
+i
p+
"
 
"
+gameType
 
hash
\n

\n
/
/
 
Current
 
rankings
 
method:
\n
/
/
 
We
 
wait
 
until
 
the
 
end
 
of
 
the
 
game,
 
then
 
we
 
update
 
the
 
stats
 
for
 
each
 
player
.

\n
/
/
 
Hence
 
we
 
collect
 
each
 
player
'
s
 
average
 
endgame
 
score
.

\n

\n
/
/
 
Half-Done:
 
I
'
m
 
not
 
so
 
sure
 
about
 
averaging
 
end-scores
 
of
 
each
 
game
.
 
 
For
 
example,
 
a
 
game
 
might
 
be
 
very
 
short
 
with
 
low
 
scores,
 
doesn
'
t
 
mean
 
all
 
the
 
players
 
strengths
 
should
 
be
 
punished
 
for
 
that
.
 
 
So
 
I
 
guess
 
scores
 
should
 
be
 
proportional
 
to
 
time
.
 
 
So
 
maybe
 
we
 
should
 
use
 
weightScore
 
to
 
calculate
 
each
 
player
'
s
 
average
 
score-per-hour
 
(I
 
think
 
XOL
 
used
 
SPH
/
FPH)
.

\n

\n
/
/
 
OLD
 
rankings
 
method:
\n
/
/

\n
/
/
 
At
 
the
 
moment
 
we
 
are
 
polling
 
the
 
game
 
every
 
PollMinutes
 
minutes,
 
and
 
updating
 
the
 
players
 
according
 
to
 
their
 
current
 
in-game
 
score
.

\n
/
/
 
So
 
we
 
are
 
basically
 
measuring
 
their
 
average
 
score
 
during
 
the
 
whole
 
(any,all)
 
time
 
they
 
are
 
on
 
the
 
server
 
(playing
 
CTF
 
with
 
4+
 
players
;
 
and
 
even
 
teams?!)
.

\n
/
/

\n
/
/
 
We
 
are
 
currently
 
taking
 
player
 
snapshots
 
about
 
6
 
times
 
during
 
each
 
15
 
minute
 
game,
 
and
 
storing
 
the
 
average
 
score
 
(usually
 
SmartCTF
 
score,
 
not
 
default
 
frags)
.

\n
/
/
 
This
 
is
 
NOT
 
their
 
average
 
score
 
at
 
the
 
end
 
of
 
the
 
game,
 
but
 
their
 
average
 
score
 
at
 
"
random
"
 
intervals
 
during
 
the
 
game
.

\n
/
/
 
This
 
might
 
seem
 
unfair
 
to
 
players
 
who
 
join
 
the
 
server
 
only
 
for
 
a
 
few
 
minutes,
 
and
 
get
 
a
 
low
 
average
 
score
.
 
 
Screw
 
them,
 
they
 
might
 
leave
 
halfway
 
through
 
the
 
next
 
game
.
 
 
Regular
 
players
 
get
 
rightly
 
punished
 
(and
 
highly
 
ranked!)
 
for
 
holding
 
high
 
scores
 
during
 
long
 
games
.
 
 
:)
\n
/
/

\n
/
/
 
Thus
 
recorded
 
for
 
each
 
player
 
is
 
their
 
average
 
score
 
over
 
all
 
the
 
time
 
they
 
ever
 
spent
 
on
 
the
 
server
 
(until
 
they
 
change
 
nick
 
or
 
IP)
.

\n
/
/
 
Players
 
will
 
be
 
forever
 
punished
/
catching
 
up
 
if
 
they
 
started
 
playing
 
with
 
low
 
scores
 
on
 
the
 
server
.

\n
/
/
 
We
 
could
 
offer
 
a
 
MaxHoursPlayed
 
or
 
MaxPollsBeforeRecyclingStrength
 
(or
 
MaxGamesPlayed),
 
after
 
which
 
their
 
time_on_server
 
does
 
not
 
increase,
 
and
 
their
 
ranking
 
becomes
 
more
 
oriented
 
towards
 
the
 
players
 
most
 
recent
 
scores
 
(older
 
scores
 
get
 
phased
 
out)
.
 
 
E
.
g
.
 
with
 
MaxPollsBeforeRecyclingStrength
=
99,
 
new_score
 
=
 
((old
 
score
 
*
 
99)
 
+
 
current_score)
 
/
 
100
 
=
 
.
99*old_score
 
+
 
.
01*current_score
 
<
--
 
This
 
is
 
still
 
a
 
very
 
slow
 
way
 
of
 
forgetting
 
the
 
past
\n

\n
/
/
 
Post-thoughts:
\n
/
/
 
Scores
 
can
 
vary
 
greatly
 
on
 
different
 
maps
 
with
 
different
 
numbers
 
of
 
players
 
and
 
with
 
different
 
teams
.

\n
/
/
 
On
 
a
 
server
 
like
 
iNzane
 
or
 
XOL,
 
which
 
has
 
a
 
fairly
 
small
 
mapcycle,
 
and
 
which
 
has
 
regular
 
players,
 
maybe
 
this
 
is
 
not
 
a
 
problem
.

\n
/
/
 
But
 
on
 
my
 
server
 
I
 
have
 
CTF,
 
iCTF,
 
tCTF,
 
maps
 
for
 
2
 
players
 
and
 
maps
 
for
 
20
 
players,
 
scores
 
from
 
different
 
games
 
will
 
vary
 
wildly
.

\n
/
/
 
Recording
 
average-score-per-hour
 
on
 
such
 
a
 
server
 
might
 
run
 
into
 
problems
.

\n
/
/
 
For
 
example
 
maybe
 
a
 
lowskilled
 
player
 
asks
 
a
 
real
 
noob
 
player
 
for
 
a
 
1v1
 
on
 
CTF-PureAction
.

\n
/
/
 
In
 
10
 
minutes
 
the
 
stronger
 
player
 
might
 
have
 
made
 
15
 
flagkills,
 
10
 
caps,
 
and
 
a
 
whole
 
bunch
 
of
 
points,
 
which
 
when
 
converted
 
into
 
a
 
score-per-hour
 
might
 
make
 
that
 
player
 
look
 
very
 
powerful
.

\n
/
/
 
However
 
his
 
score
 
was
 
abnormally
 
boosted
 
by
 
the
 
low
 
skill
 
of
 
his
 
opponent
.

\n
/
/
 
He
 
might
 
have
 
increased
 
his
 
average
 
score-per-hour
 
to
 
something
 
comparable
 
with
 
those
 
of
 
highskill
 
players,
 
but
 
who
 
only
 
play
 
against
 
other
 
highskill
 
players,
 
on
 
more
 
serious
 
maps,
 
e
.
g
.
 
CTF-Terra
.

\n

\n
/
/
 
So
 
what
 
else
 
could
 
we
 
try?
\n
/
/
 
-
 
TODO
 
Team
 
Cap
 
Bonus:
 
regardless
 
of
 
frags,
 
the
 
team
 
which
 
played
 
the
 
best
 
CTF
 
will
 
get
 
the
 
most
 
caps
.
 
 
All
 
players
 
on
 
the
 
winning
 
team
 
could
 
be
 
rewarded
 
for
 
having
 
done
 
so,
 
even
 
if
 
their
 
actual
 
score
 
was
 
pretty
 
low
.

\n
/
/
 
-
 
The
 
actual
 
scores
 
might
 
not
 
be
 
useful,
 
but
 
the
 
distribution
 
of
 
those
 
scores
 
might
 
be
 
interesting
.

\n
/
/
 
 
 
For
 
example,
 
the
 
order
 
of
 
players
 
on
 
the
 
scoreboard
 
(imagine
 
if
 
both
 
teams
 
were
 
merged
 
into
 
one)
 
should
 
give
 
an
 
idea
 
of
 
the
 
relative
 
skills
 
of
 
certain
 
players
.
 
 
e
.
g
.
 
top
 
player
 
gets
 
100
 
points,
 
all
 
other
 
players
 
get
 
less,
 
bottom
 
player
 
gets
 
10
.

\n
/
/
 
 
 
TODO
 
Or
 
the
 
relative
 
scores
 
could
 
be
 
considered
.
 
 
E
.
g
.
 
the
 
scores
 
from
 
the
 
game
 
could
 
be
 
scaled
 
so
 
that
 
they
 
always
 
have
 
a
 
mean
 
of
 
say
 
50
 
points
.
 
 
So
 
we
 
can
 
still
 
use
 
the
 
game
 
scores,
 
but
 
the
 
scaling
 
will
 
"
normalise
"
 
those
 
scores
 
so
 
that
 
as
 
much
 
benefit
 
comes
 
from
 
doing
 
well
 
in
 
a
 
low-scoring
 
game
 
as
 
in
 
a
 
high-scoring
 
game
.

\n
/
/
 
 
 
 
 
 
 
 
Ofc
 
this
 
would
 
mean
 
the
 
best
 
noobs
 
get
 
similar
 
scores
 
to
 
the
 
best
 
el33ts,
 
if
 
they
 
the
 
noobs
 
and
 
leets
 
never
 
actually
 
play
 
at
 
the
 
same
 
time,
 
which
 
kinda
 
makes
 
sense
.
 
 
^^
\n

\n
/
/
 
TODO:
 
consider
 
adding
 
just
 
a
 
little
 
randomnity
.
 
 
If
 
we
 
have
 
the
 
same
 
8
 
players
 
on
 
the
 
server
 
for
 
4
 
games
 
in
 
a
 
row,
 
and
 
their
 
stats
 
don
'
t
 
change
 
enough
 
to
 
actually
 
switch
 
any
 
of
 
their
 
positions
 
in
 
the
 
ranking,
 
won
'
t
 
Daniel
'
s
 
initial
 
teambalance
 
create
 
identical
 
teams
 
for
 
every
 
game?
 
 
Can
 
we
 
find
 
a
 
way
 
to
 
avoid
 
that?
 
 
Mix
 
up
 
the
 
lower
 
skilled
 
players
 
a
 
bit,
 
since
 
that
 
will
 
have
 
least
 
impact?
\n
/
/
 
looks
 
like
 
FRand()
 
might
 
returns
 
a
 
number
 
between
 
0
 
and
 
1
.

\n

\n
/
/
 
TODO:
 
throughout
 
the
 
code
 
and
 
comments
 
i
 
have
 
referred
 
to
 
strength,avg_score,ranking,rating,stats
 
but
 
often
 
meant
 
the
 
same
 
thing
.
 
 
Daniel
 
stuck
 
to
 
"
Strength
"
 
so
 
maybe
 
I
 
should
 
consolidate
 
around
 
that
 
name
.

\n

\n
/
/
 
Note:
 
at
 
the
 
moment
 
no
 
attempt
 
is
 
made
 
to
 
update
 
the
 
strengths
 
of
 
players
 
who
 
leave
 
the
 
server
 
before
 
the
 
end
 
of
 
the
 
game
.

\n
/
/
 
Only
 
players
 
on
 
the
 
server
 
when
 
the
 
game
 
ends
 
will
 
get
 
their
 
strenghts
 
updated
.

\n
/
/
 
Maybe
 
that
'
s
 
undesirable
.
 
 
iDeFiX
'
s
 
teambalancer
 
updates
 
stats
 
mid-game
 
(but
 
i
 
think
 
it
 
caches
 
the
 
info
 
and
 
does
 
more
 
processesing
 
at
 
the
 
end
 
of
 
the
 
game)
.

\n

\n
/
/
 
CONSIDER:
 
If
 
players
 
manage
 
to
 
make
 
the
 
teams
 
unfair
 
anyway,
 
and
 
then
 
go
 
and
 
cap
 
10
 
flags
 
3v1,
 
then
 
the
 
scores
 
from
 
that
 
game
 
will
 
hardly
 
be
 
an
 
accurate
 
representation
 
of
 
what
 
they
 
should
 
have
 
been
.
 
 
So
 
should
 
we
 
be
 
detecting
 
whether
 
teams
 
were
 
actually
 
fair
 
when
 
we
 
update
 
the
 
stats?
 
 
Well,
 
maybe
 
it
'
s
 
ok:
 
the
 
lamers
 
who
 
teamed
 
up
 
will
 
get
 
punished
 
by
 
unrealistically
 
high
 
stats,
 
so
 
next
 
time
 
they
 
play
 
they
 
will
 
get
 
balanced
 
with
 
weak
 
players
 
on
 
their
 
team
.
 
 
Mwuhahaha!
 
 
And
 
the
 
guy
 
who
 
got
 
bullied
 
will
 
obviously
 
need
 
a
 
break,
 
so
 
him
 
losing
 
some
 
strength
 
stats
 
will
 
just
 
mean
 
he
 
gets
 
some
 
stronger
 
team-mates
 
in
 
future
.
 
 
:)
 
 
So
 
I
 
guess
 
we
 
don
'
t
 
really
 
need
 
to
 
detect
 
whether
 
teams
 
were
 
fair
 
when
 
updating
 
stats!
\n

\n
/
/
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=
=

\n

\n
class
 
AutoTeamBalance
 
expands
 
Mutator
 
config(AutoTeamBalance)
;

\n

\n
var
 
string
 
HelloBroadcast
;
 
/
/
 
TODO
 
CONSIDER:
 
make
 
this
 
configurable,
 
and
 
make
 
it
 
say
 
nothing
 
if
 
"
"

\n

\n
var
 
config
 
bool
 
bAutoBalanceTeamsForCTF
;

\n
var
 
config
 
bool
 
bAutoBalanceTeamsForTDM
;

\n
var
 
config
 
bool
 
bAutoBalanceTeamsForAS
;

\n
var
 
config
 
bool
 
bAutoBalanceTeamsForOtherTeamGames
;

\n
/
/
 
var
 
config
 
string
 
BalanceTeamsForGameTypes
;
 
/
/
 
TESTING_List_desired_gametypes
\n
/
/
 
For
 
updating
 
player
 
strength
 
in-game:
\n
var
 
config
 
bool
 
bUpdatePlayerStatsForCTF
;

\n
var
 
config
 
bool
 
bUpdatePlayerStatsForTDM
;

\n
var
 
config
 
bool
 
bUpdatePlayerStatsForAS
;

\n
var
 
config
 
bool
 
bUpdatePlayerStatsForOtherTeamGames
;

\n
var
 
config
 
bool
 
bUpdatePlayerStatsForNonTeamGames
;

\n
/
/
 
var
 
config
 
string
 
UpdateStatsForGameTypes
;
 
/
/
 
TESTING_List_desired_gametypes
\n
/
/
 
var
 
config
 
bool
 
bUpdateStatsForCTFOnly
;
 
 
/
/
 
Stats
 
were
 
updating
 
during
 
other
 
gametypes,
 
which
 
yield
 
entirely
 
different
 
scores
.
 
 
(Maybe
 
stats
 
for
 
different
 
gametypes
 
should
 
be
 
handled
 
separately
.
)
 
 
If
 
your
 
server
 
runs
 
only
 
one
 
team
 
gametype,
 
or
 
gametypes
 
with
 
comparably
 
scores,
 
you
 
can
 
set
 
this
 
to
 
False
.

\n
/
/
/
/
 
These
 
didn
'
t
 
work
 
for
 
me
;
 
maybe
 
config
 
vars
 
can
'
t
 
be
 
complex
 
types
 
like
 
"
name
"

\n
/
/
 
var
 
config
 
name
 
OnlyBalanceTeamsIfGametypeIsA
;
 
/
/
 
Defaults
 
to
 
'
TeamGamePlus
'
 
so
 
it
 
will
 
try
 
to
 
balance
 
teams
 
for
 
all
 
team
 
games
.

\n
/
/
 
var
 
config
 
name
 
OnlyUpdateStatsIfGametypeIsA
;
 
 
/
/
 
Stats
 
were
 
updating
 
during
 
other
 
gametypes
 
than
 
CTF,
 
which
 
yield
 
entirely
 
different
 
scores
.
 
 
(Maybe
 
stats
 
for
 
different
 
gametypes
 
should
 
be
 
handled
 
separately
.
)
 
 
You
 
can
 
set
 
this
 
to
 
your
 
own
 
server
'
s
 
favourite
 
gametype,
 
or
 
to
 
'
TeamGamePlus
'
 
if
 
you
 
only
 
host
 
one
 
gametype,
 
or
 
player
 
scores
 
are
 
comparable
 
across
 
all
 
your
 
gametypes
.

\n
/
/
 
var
 
config
 
float
 
PollMinutes
;
 
 
 
 
/
/
 
e
.
g
.
 
every
 
2
.
4
 
minutes,
 
update
 
the
 
player
 
stats
 
from
 
the
 
current
 
game
\n
var
 
config
 
int
 
MaxPollsBeforeRecyclingStrength
;
 
 
 
 
/
/
 
after
 
this
 
many
 
polls,
 
player
'
s
 
older
 
scores
 
are
 
slowly
 
phased
 
out
.
 
 
This
 
feature
 
is
 
disabled
 
by
 
setting
 
MaxPollsBeforeRecyclingStrength
=
0
 
/
/
 
TODO:
 
refactor
 
this
 
to
 
MaxHoursOfOldStats,
 
more
 
tangible
 
unit
 
for
 
admin
 
to
 
edit
\n
var
 
config
 
int
 
MinHumansForStats
;
 
/
/
 
below
 
this
 
number
 
of
 
human
 
players,
 
stats
 
will
 
not
 
be
 
updated,
 
i
.
e
.
 
current
 
game
 
scores
 
will
 
be
 
ignored
\n
var
 
config
 
bool
 
bNormaliseScores
;

\n
/
/
 
deprecated:
 
var
 
config
 
bool
 
bDoWeightedUpdates
;

\n

\n
var
 
config
 
bool
 
bBroadcastStuff
;
 
 
 
/
/
 
Be
 
noisy
 
to
 
in-game
 
console
\n
var
 
config
 
bool
 
bExtraDebugLogging
;
 
 
 
 
 
/
/
 
logs
 
are
 
more
 
verbose
/
spammy
 
than
 
usual
;
 
recommended
 
only
 
for
 
developers
\n
var
 
config
 
bool
 
bBroadcastCookies
;
 
/
/
 
Silly
 
way
 
to
 
debug
;
 
each
 
players
 
strength
 
is
 
spammed
 
at
 
end
 
of
 
game
 
as
 
their
 
number
 
of
 
cookies
\n
/
/
 
TODO:
 
now
 
we
 
are
 
doing
 
p
.
ClientMessage()
 
sometimes,
 
we
 
don
'
t
 
really
 
need
 
to
 
BroadcastMessage
 
as
 
well
 
(I
 
only
 
want
 
it
 
as
 
a
 
developer
 
to
 
see
 
changes
 
during
 
the
 
game
.
)
\n
/
/
 
var
 
config
 
bool
 
bOnlyMoreCookies
;
 
 
/
/
 
only
 
broadcast
 
a
 
players
 
cookies
 
when
 
they
 
have
 
recently
 
increased
\n
var
 
config
 
bool
 
bBroadcastLostCookies
;
 
 
/
/
 
should
 
we
 
broadcast
 
when
 
someone
 
has
 
moved
 
down
 
the
 
ranking?
\n

\n
/
/
 
Defaults
 
(Daniel
'
s):
\n
var
 
config
 
int
 
UnknownStrength
;
 
 
 
 
/
/
 
Default
 
strength
 
for
 
unknown
 
players
\n
/
/
 
var
 
config
 
float
 
UnknownMinutes
;
 
 
 
/
/
 
Initial
 
virtual
 
time
 
spend
 
on
 
server
 
by
 
new
 
players
\n
var
 
config
 
int
 
BotStrength
;
 
 
 
 
 
 
 
 
/
/
 
Default
 
strength
 
for
 
bots
\n
var
 
config
 
int
 
FlagStrength
;
 
 
 
 
 
 
 
/
/
 
Strength
 
modifier
 
for
 
captured
 
flags
\n
var
 
config
 
bool
 
bClanWar
;
 
 
 
 
 
 
 
 
 
 
/
/
 
Make
 
teams
 
by
 
clan
 
tag
\n
var
 
config
 
string
 
clanTag
;
 
 
 
 
 
 
 
 
 
/
/
 
Clan
 
tag
 
of
 
red
 
team
 
(all
 
other
 
players
 
to
 
blue)
\n
/
/
 
var
 
config
 
String
 
RedTeam[16]
;
 
 
 
 
 
/
/
 
Players
 
on
 
red
 
team
 
(unreferenced)
\n
/
/
 
var
 
config
 
String
 
BlueTeam[16]
;
 
 
 
 
/
/
 
Players
 
on
 
blue
 
team
 
(unreferenced)
\n

\n
/
/
 
For
 
storing
 
player
 
strength
 
data:
\n
var
 
int
 
MaxPlayerData
;
 
/
/
 
The
 
value
 
4096
 
is
 
used
 
in
 
the
 
following
 
array
 
declarations
 
and
 
the
 
defaultproperties,
 
but
 
throughout
 
the
 
rest
 
of
 
the
 
code,
 
MaxPlayerData
 
can
 
be
 
used
 
to
 
save
 
duplication
 
lol
\n
var
 
config
 
String
 
playerData[4096]
;
 
/
/
 
String-format
 
of
 
the
 
player
 
data
 
stored
 
in
 
the
 
config
 
(ini-file),
 
including
 
ip
/
nick
/
avg_score
/
time_played
 
data
\n

\n
/
/
 
Internal
 
(parsed)
 
player
 
data:
\n
var
 
String
 
ip[4096]
;

\n
var
 
String
 
nick[4096]
;

\n
var
 
float
 
avg_score[4096]
;

\n
var
 
float
 
hours_played[4096]
;

\n
/
/
 
var
 
int
 
games_played[4096]
;

\n
/
/
 
TODO:
 
var
 
int
 
date_last_played[4096]
;
 
/
/
 
would
 
be
 
good
 
for
 
recycling
 
old
 
stats
;
 
otherwise
 
recycle
 
on
 
lowest
 
hours_played
 
I
 
guess,
 
although
 
if
 
the
 
server
/
playerData
 
lasts
 
1billion
 
years,
 
it
 
might
 
be
 
hard
 
for
 
the
 
current
 
generation
 
of
 
players
 
to
 
get
 
into
 
the
 
ranking
\n

\n
/
/
 
For
 
local
 
state
 
caching
 
(not
 
repeating
 
when
 
called
 
by
 
Tick
'
s
 
or
 
Timer
'
s):
\n
var
 
bool
 
initialized
;
 
 
 
 
 
 
 
 
 
 
 
 
 
 
/
/
 
Mutator
 
initialized
 
flag
\n
var
 
bool
 
gameStartDone
;
 
 
 
 
 
 
 
 
 
 
 
 
/
/
 
Teams
 
initialized
 
flag
 
(we
 
never
 
initialise
 
this
 
to
 
False,
 
but
 
I
 
guess
 
Unreal
 
does
 
that
 
for
 
us)
\n
var
 
bool
 
gameEndDone
;

\n

\n
var
 
int
 
timeGameStarted
;

\n

\n
defaultproperties
 
{
\n
 
 
HelloBroadcast
=
"
AutoTeamBalance
 
(beta)
 
is
 
attempting
 
to
 
balance
 
the
 
teams
"

\n
 
 
bAutoBalanceTeamsForCTF
=
True
\n
 
 
bAutoBalanceTeamsForTDM
=
True
\n
 
 
bAutoBalanceTeamsForAS
=
True
\n
 
 
bAutoBalanceTeamsForOtherTeamGames
=
True
\n
 
 
/
/
 
BalanceTeamsForGameTypes
=
"
CTFGame,TeamGamePlus,JailBreak,*
"

\n
 
 
bUpdatePlayerStatsForCTF
=
True
\n
 
 
bUpdatePlayerStatsForTDM
=
True
 
/
/
 
If
 
you
 
are
 
normalising
 
scores,
 
then
 
updating
 
stats
 
for
 
TDM
 
should
 
be
 
ok
.
 
 
But
 
if
 
you
 
are
 
not
 
normalising
 
scores,
 
then
 
the
 
different
 
bonuses
 
in
 
CTF
 
will
 
make
 
stats
 
from
 
the
 
different
 
gametypes
 
incompatible
.
 
 
(Basically
 
TDMers
 
will
 
get
 
lower
 
strengths
 
because
 
they
 
never
 
get
 
the
 
bonus
 
points
 
from
 
caps
/
covers
/
etc
.
)
 
 
So
 
in
 
this
 
case
 
you
 
are
 
recommended
 
only
 
to
 
build
 
stats
 
for
 
your
 
server
'
s
 
most
 
popular
 
gametype
.

\n
 
 
bUpdatePlayerStatsForAS
=
False
 
 
/
/
 
Probably
 
best
 
left
 
False
 
(unless
 
you
 
are
 
running
 
an
 
AS-only
 
server)
 
because
 
AS
 
scores
 
are
 
crazy
 
(one
 
guy
 
gets
 
100
 
for
 
the
 
last
 
objective,
 
even
 
though
 
it
 
was
 
a
 
team
 
effort)
\n
 
 
bUpdatePlayerStatsForOtherTeamGames
=
False
\n
 
 
bUpdatePlayerStatsForNonTeamGames
=
False
\n
 
 
/
/
 
UpdateStatsForGameTypes
=
"
CTFGame
"

\n
 
 
/
/
 
bUpdateStatsForCTFOnly
=
True
\n
 
 
/
/
 
OnlyUpdateStatsIfGametypeIsA
=
'
CTFGame
'
 
/
/
 
Would
 
have
 
been
 
nice
 
to
 
offer
 
it
 
this
 
way,
 
but
 
I
 
didn
'
t
 
get
 
it
 
working
.

\n
 
 
/
/
 
OnlyBalanceTeamsIfGametypeIsA
=
'
TeamGamePlus
'
 
/
/
 
TODO:
 
we
 
CAN
 
do
 
it
 
this
 
way,
 
e
.
g
.
 
using
 
String(gametype
.
Class)
 
=
=
 
"
Botpack
.
Assault
"

\n
 
 
/
/
 
PollMinutes
=
2
.
4
\n
 
 
MaxPollsBeforeRecyclingStrength
=
200
 
/
/
 
I
 
think
 
for
 
a
 
returning
 
player
 
with
 
a
 
previous
 
average
 
of
 
100(!),
 
and
 
a
 
new
 
skill
 
of
 
around
 
50,
 
and
 
with
 
24
 
polls
 
an
 
hour
 
and
 
MaxPollsBeforeRecyclingStrength
=
100,
 
after
 
100
 
more
 
polls
 
(4
 
more
 
hours),
 
the
 
player
'
s
 
new
 
average
 
will
 
look
 
like
 
60
.
5
.
 
 
That
 
seems
 
too
 
quick
 
for
 
me,
 
so
 
I
'
ve
 
gone
 
for
 
200
.
 
 
^^
 
 
btw
 
this
 
maths
 
is
 
wrong
 
:|
 
but
 
approx
 
i
 
guess
\n
 
 
MinHumansForStats
=
1
 
 
 
 
 
/
/
 
TODO:
 
for
 
release,
 
recommended
 
4
\n
 
 
bNormaliseScores
=
True
 
 
 
 
 
/
/
 
Normalise
 
scores
 
so
 
that
 
the
 
average
 
score
 
for
 
every
 
game
 
is
 
50
.
 
 
Recommended
 
for
 
servers
 
where
 
some
 
games
 
end
 
with
 
very
 
high
 
scores
 
and
 
some
 
not
 
(e
.
g
.
 
if
 
you
 
have
 
different
 
styles
 
of
 
map
 
and
 
game-modes,
 
like
 
mixing
 
normal
 
weapons
 
clanwar
 
maps
 
with
 
instagib
 
action
 
maps)
.
 
 
You
 
can
 
turn
 
this
 
off
 
if
 
your
 
server
 
has
 
a
 
fixed
 
mapcycle
 
and
 
always
 
the
 
same
 
game-mode
.
 
 
Normalising
 
results
 
in
 
a
 
*relative*
 
ranking
 
of
 
players
 
who
 
play
 
the
 
same
 
games
.
 
 
Not
 
normalising
 
would
 
be
 
better
 
for
 
separating
 
weak
 
and
 
strong
 
players
 
who
 
never
 
actually
 
played
 
together
.
 
 
If
 
you
 
have
 
10
 
strong
 
players
 
getting
 
high
 
scores
 
on
 
one
 
game,
 
and
 
10
 
noobs
 
getting
 
low
 
scores
 
during
 
a
 
different
 
game,
 
normalising
 
would
 
actually
 
put
 
the
 
strongest
 
noob
 
up
 
with
 
the
 
strongest
 
pwnzor
.
 
 
TODO
 
CONSIDER:
 
would
 
it
 
be
 
a
 
useful
 
compromise
 
to
 
"
half-normalise
"
?
 
 
And
 
how
 
would
 
we
 
do
 
that?
 
 
I
 
think
 
some
 
logarithmic
 
maths
 
might
 
be
 
required
.

\n
 
 
/
/
 
deprecated:
 
bDoWeightedUpdates
=
False
 
 
/
/
 
Untested
 
experimental
 
stats
 
updating
 
method
\n
 
 
bBroadcastStuff
=
True
\n
 
 
bExtraDebugLogging
=
True
 
 
 
 
 
 
/
/
 
TODO:
 
for
 
release,
 
recommended
 
False
 
(some
 
logging
 
is
 
ok
 
tho!)
\n
 
 
bBroadcastCookies
=
True
\n
 
 
/
/
 
bOnlyMoreCookies
=
False
\n
 
 
bBroadcastLostCookies
=
True
 
/
/
 
TODO:
 
for
 
release,
 
default
 
to
 
False
\n
 
 
UnknownStrength
=
30
 
 
 
 
 
 
/
/
 
New
 
player
 
records
 
start
 
with
 
an
 
initial
 
strength
 
of
 
avg_score
 
40
 
(I
 
changed
 
this
 
to
 
30
 
when
 
I
 
started
 
normalising
 
scores)
\n
 
 
/
/
 
UnknownMinutes
=
10
 
 
 
 
 
 
 
/
/
 
New
 
player
 
records
 
start
 
with
 
a
 
virtual
 
10
 
minutes
 
of
 
time
 
played
 
already
\n
 
 
BotStrength
=
20
\n
 
 
FlagStrength
=
50
 
 
 
 
 
 
 
 
 
/
/
 
If
 
it
'
s
 
3:0,
 
the
 
winning
 
team
 
will
 
get
 
punished
 
an
 
extra
 
150
 
points
;
 
used
 
when
 
new
 
players
 
join
 
the
 
game
 
and
 
number
 
of
 
players
 
on
 
each
 
team
 
are
 
even
\n
 
 
bClanWar
=
False
\n
 
 
MaxPlayerData
=
4096
\n
 
 
/
/
 
bHidden
=
True
 
/
/
 
what
 
is
 
this?
 
 
iDeFiX
 
says
 
it
'
s
 
only
 
needed
 
for
 
ServerActors
\n
}
\n

\n

\n

\n
/
/
 
=
=
=
=
 
Daniel
'
s
 
HOOKS
 
/
 
OVERRIDES
 
=
=
=
=
 
/
/

\n

\n
/
/
 
Initialize
 
the
 
system
\n
function
 
PostBeginPlay()
 
{
\n
 
 
if
 
(initialized)
 
return
;

\n

\n
 
 
Super
.
PostBeginPlay()
;

\n

\n
 
 
/
/
 
register
 
as
 
mutator
\n
 
 
/
/
 
I
 
was
 
getting
 
an
 
infinite
 
recursion
 
error,
 
so
 
I
 
removed
 
this
.

\n
 
 
/
/
 
Maybe
 
because
 
I
 
had
 
already
 
added
 
the
 
mutator
\n
 
 
/
/
 
Maybe
 
this
 
was
 
designed
 
for
 
ServerActor
\n
 
 
/
/
 
Before
 
uncommenting,
 
consider
 
moving
 
the
 
initialized
=
true
;
 
to
 
the
 
line
 
before
.

\n
 
 
/
/
 
Level
.
Game
.
BaseMutator
.
AddMutator(Self)
;

\n
 
 
initialized
 
=
 
true
;

\n
 
 
gameEndDone
 
=
 
false
;

\n

\n
 
 
timeGameStarted
 
=
 
Level
.
TimeSeconds
;

\n

\n
 
 
/
/
 
Call
 
Timer()
 
every
 
PollMinutes
.

\n
 
 
/
/
 
SetTimer(PollMinutes*60,True)
;

\n
 
 
/
/
 
Log(
"
AutoTeamBalance
.
PostBeginPlay():
 
Set
 
Timer()
 
for
 
"
$(PollMinutes*60)$
"
 
seconds
.
"
)
;

\n
 
 
/
/
 
SetTimer(10,True)
;
 
/
/
 
Now
 
checking
 
once
 
a
 
minute
 
to
 
see
 
if
 
game
 
has
 
ended
;
 
changed
 
to
 
10
 
seconds
 
since
 
we
 
lost
 
our
 
alternative
 
MessageMutator
 
hook
\n
 
 
/
/
 
if
 
(bExtraDebugLogging)
 
{
 
Log(
"
AutoTeamBalance
.
PostBeginPlay():
 
Set
 
Timer()
 
for
 
10
 
seconds
.
"
)
;
 
}
\n
 
 
/
/
 
This
 
has
 
been
 
moved
 
to
 
HandleEndGame,
 
so
 
no
 
checks
 
need
 
to
 
be
 
made
 
during
 
game-play
 
(ok
 
well
 
maybe
 
it
'
s
 
checked
 
during
 
overtime)!
 
 
:)
\n

\n
 
 
/
/
 
Level
.
Game
.
RegisterMessageMutator(
 
Self
 
)
;
 
/
/
 
TESTING
 
Matt
'
s
 
MutatorBroadcastMessage
 
hook
 
below
\n
 
 
/
/
 
deprecated
 
because
 
it
 
was
 
hiding
 
server
 
broadcasts
 
(like
 
adwvaad
 
used
 
to
;
 
might
 
be
 
fixed
 
by
 
making
 
it
 
a
 
serveractor,
 
although
 
may
 
or
 
may
 
not
 
be
 
possible
 
for
 
what
 
we
 
are
 
doing
 
with
 
this
 
mutator)
\n

\n
}
\n

\n
/
/
 
Check
 
if
 
game
 
is
 
about
 
to
 
start,
 
and
 
if
 
so,
 
balance
 
the
 
teams
 
before
 
it
 
does
.
 
 
(We
 
want
 
to
 
do
 
this
 
as
 
late
 
as
 
possible
 
before
 
the
 
game
 
starts,
 
to
 
give
 
players
 
time
 
to
 
join
 
the
 
server
.
)
\n
/
/
 
DONE:
 
Determine
 
whether
 
this
 
check
 
every
 
Tickrate
 
is
 
more
 
or
 
less
 
efficient
 
than
 
using
 
SetTimer()
 
and
 
Timer(),
 
then
 
merge
 
the
 
two
 
CheckGameStart()
 
and
 
UpdateStatsAtEndOfGame()
 
into
 
the
 
more
 
efficient
 
method
.

\n
/
/
 
 
 
 
 
 
 
We
 
may
 
not
 
need
 
to
 
check
 
more
 
often
 
than
 
once
 
every
 
-2
 
+
 
270
 
seconds
.

\n
/
/
 
 
 
 
 
 
 
If
 
TickRate
 
20
 
means
 
20
 
calls
 
to
 
Tick
 
per
 
second,
 
presumably
 
Timer()
 
is
 
(an
 
Engine
 
(non
 
UScript)
 
event?
 
and)
 
more
 
efficient
.

\n
/
/
 
 
 
 
 
 
 
Ahhh
 
but
 
maybe
 
Daniel
 
used
 
Tick()
 
because
 
it
'
s
 
before
 
game
 
start,
 
too
 
early
 
to
 
use
 
a
 
timer
.
 
 
To
 
look
 
into
.
.
.

\n
/
/
 
 
 
 
 
 
 
Yes
 
that
'
s
 
the
 
reason,
 
can
'
t
 
use
 
Timer()
.
 
 
But
 
what
 
about
 
PreBeginPlay()?
\n
/
/
 
 
 
 
 
 
 
Anyway,
 
I
 
inlined
 
the
 
bool
 
check
 
into
 
Tick()
 
so
 
no
 
extra
 
function
 
call
 
is
 
needed
.
 
 
And
 
UpdateStatsAtEndOfGame()
 
is
 
dealt
 
with
 
by
 
Timer()
.

\n
/
/
 
 
 
 
 
 
 
TODO:
 
well
 
this
 
bool
 
check
 
must
 
be
 
pretty
 
fast,
 
but
 
consider
 
overriding
 
PreBeginPlay()
 
and
 
see
 
if
 
that
 
can
 
help
 
us
.

\n
event
 
Tick(float
 
DeltaTime)
 
{
\n
 
 
if
 
(!gameStartDone)
 
CheckGameStart()
;

\n
}
\n

\n
/
/
 
TODO:
 
just
 
a
 
TEST,
 
remove
 
it
 
if
 
it
 
can
'
t
 
help
 
us
.

\n
function
 
PreBeginPlay()
 
{
\n
 
 
if
 
(bExtraDebugLogging)
 
{
 
Log(
"
AutoTeamBalance
.
PreBeginPlay()
 
WAS
 
CALLED
 
NOW
"
)
;
 
}
\n
}
\n

\n
function
 
bool
 
ShouldBalance(GameInfo
 
game)
 
{
\n

\n
 
 
/
/
 
TODO:
 
AS
 
is
 
a
 
teamgame,
 
so
 
ok
 
to
 
balance,
 
BUT
 
ONLY
 
on
 
the
 
first
 
half-game
.

\n
 
 
/
/
 
 
 
 
 
 
 
Teams
 
should
 
remain
 
the
 
same
 
for
 
the
 
second
 
half-game
.
 
 
(I
 
fear
 
if
 
it
 
does
 
the
 
balancing
 
for
 
both
 
half-games,
 
the
 
same
 
ppl
 
will
 
be
 
defending
 
twice!)
\n
 
 
/
/
 
Also
 
TODO:
 
make
 
AS
 
a
 
separate
 
option
 
in
 
the
 
booleans
 
above
;
 
atm
 
it
'
s
 
classified
 
under
 
Other
.

\n

\n
 
 
/
/
/
/
 
TESTING_List_desired_gametypes
\n
 
 
/
/
 
local
 
string[20]
 
gametypes
;

\n
 
 
/
/
 
local
 
int
 
n,i
;

\n

\n
 
 
/
/
 
if
 
(bExtraDebugLogging)
 
{
 
Log(
"
AutoTeamBalance
.
ShouldBalance(
"
$game$
"
)
 
Game
.
Name
=
"
$Game
.
Name$
"
 
Game
.
Class
=
"
$Game
.
Class$
"
"
)
;
 
}
\n
 
 
/
/
 
Never
 
balance
 
in
 
tournament
 
mode
\n
 
 
if
 
(DeathMatchPlus(Level
.
Game)
.
bTournament)
\n
 
 
 
 
return
 
False
;

\n
 
 
/
/
 
We
 
can
'
t
 
balance
 
if
 
it
'
s
 
not
 
a
 
teamgame
\n
 
 
if
 
(!Level
.
Game
.
GameReplicationInfo
.
bTeamGame)
\n
 
 
 
 
return
 
False
;

\n

\n
 
 
/
/
 
We
 
only
 
balance
 
CTF
 
games
 
if
 
asked
\n
 
 
/
/
 
if
 
(Level
.
Game
.
Name
 
=
=
 
'
CTFGame
'
)
\n
 
 
if
 
(String(Level
.
Game
.
Class)
 
=
=
 
"
Botpack
.
CTFGame
"
)
\n
 
 
 
 
return
 
bAutoBalanceTeamsForCTF
;

\n
 
 
/
/
 
We
 
only
 
balance
 
TDM
 
games
 
if
 
asked
 
(NOTE:
 
we
 
don
'
t
 
use
 
IsA
 
here,
 
because
 
other
 
teamgames
 
might
 
be
 
a
 
subclass
 
of
 
TeamGamePlus)
\n
 
 
if
 
(String(Level
.
Game
.
Class)
 
=
=
 
"
Botpack
.
TeamGamePlus
"
)
\n
 
 
 
 
return
 
bAutoBalanceTeamsForTDM
;

\n
 
 
if
 
(String(Level
.
Game
.
Class)
 
=
=
 
"
Botpack
.
Assault
"
)
\n
 
 
 
 
return
 
bAutoBalanceTeamsForAS
;

\n

\n
 
 
/
/
/
/
 
TESTING_List_desired_gametypes
\n
 
 
/
/
 
n
 
=
 
Split(BalanceTeamsForGameTypes,
"
,
"
,gametypes)
;

\n
 
 
/
/
 
for
 
(i
=
0
;
i
<
n
;
i+
+)
 
{
\n
 
 
 
 
/
/
 
if
 
(gametypes[i]
 
=
=
 
"
*
"
)
\n
 
 
 
 
 
 
/
/
 
return
 
True
;

\n
 
 
 
 
/
/
 
if
 
(gametypes[i]
 
=
=
 
String(Level
.
Game
.
Name))
\n
 
 
 
 
 
 
/
/
 
return
 
True
;

\n
 
 
/
/
 
}
\n
 
 
/
/
 
if
 
(bExtraDebugLogging)
 
{
 
Log(
"
AutoTeamBalance
.
ShouldBalance(
"
$game$
"
):
 
Did
 
not
 
match
 
any
 
of
 
the
 
specified
 
gametypes:
 
"
$BalanceTeamsForGameTypes)
;
 
}
\n

\n
 
 
/
/
 
OK
 
so
 
it
'
s
 
an
 
unknown
 
teamgame
\n
 
 
if
 
(bExtraDebugLogging)
 
{
 
Log(
"
AutoTeamBalance
.
ShouldBalance(
"
$game$
"
)
 
Game
.
Name
=
"
$Game
.
Name$
"
 
Game
.
Class
=
"
$Game
.
Class$
"
 
returning
 
"
$bAutoBalanceTeamsForOtherTeamGames)
;
 
}
\n
 
 
return
 
bAutoBalanceTeamsForOtherTeamGames
;

\n
}
\n

\n
function
 
bool
 
ShouldUpdateStats(GameInfo
 
game)
 
{
\n
 
 
if
 
(bExtraDebugLogging)
 
{
 
Log(
"
AutoTeamBalance
.
ShouldUpdateStats(
"
$game$
"
)
 
Game
.
Name
=
"
$Game
.
Name$
"
 
Game
.
Class
=
"
$Game
.
Class$
"
"
)
;
 
}
\n
 
 
/
/
 
We
 
only
 
build
 
stats
 
for
 
CTF
 
games
 
if
 
asked
\n
 
 
if
 
(String(Level
.
Game
.
Class)
 
=
=
 
"
Botpack
.
CTFGame
"
)
\n
 
 
 
 
return
 
bUpdatePlayerStatsForCTF
;

\n
 
 
/
/
 
We
 
only
 
build
 
stats
 
for
 
TDM
 
games
 
if
 
asked
 
(NOTE:
 
we
 
don
'
t
 
use
 
IsA
 
here,
 
because
 
other
 
teamgames
 
might
 
be
 
a
 
subclass
 
of
 
TeamGamePlus)
\n
 
 
if
 
(String(Level
.
Game
.
Class)
 
=
=
 
"
Botpack
.
TeamGamePlus
"
)
\n
 
 
 
 
return
 
bUpdatePlayerStatsForTDM
;

\n
 
 
if
 
(String(Level
.
Game
.
Class)
 
=
=
 
"
Botpack
.
Assault
"
)
\n
 
 
 
 
return
 
bUpdatePlayerStatsForAS
;

\n
 
 
/
/
 
OK
 
so
 
it
'
s
 
not
 
CTF
 
or
 
TDM
 
or
 
AS,
 
but
 
is
 
it
 
another
 
type
 
of
 
team
 
game?
\n
 
 
if
 
(Level
.
Game
.
GameReplicationInfo
.
bTeamGame)
\n
 
 
 
 
return
 
bUpdatePlayerStatsForOtherTeamGames
;

\n
 
 
/
/
 
It
'
s
 
not
 
a
 
team
 
game
.
 
 
Build
 
stats
 
because
 
it
'
s
 
a
 
non
 
teamgame?
 
 
(For
 
admins
 
more
 
interested
 
in
 
player
 
stats
 
than
 
balancing
 
teams
.
)
\n
 
 
return
 
bUpdatePlayerStatsForNonTeamGames
;

\n
}
\n

\n
/
/
 
If
 
a
 
new
 
player
 
joins
 
a
 
game
 
which
 
has
 
already
 
started,
 
this
 
will
 
send
 
him
 
to
 
the
 
most
 
appropriate
 
(
"
weaker
"
)
 
team
 
(based
 
on
 
summed
 
strength
 
of
 
each
 
team,
 
plus
 
capbonuses)
.

\n
function
 
ModifyLogin(out
 
class
<
playerpawn
>
 
SpawnClass,
 
out
 
string
 
Portal,
 
out
 
string
 
Options)
 
{
\n
 
 
local
 
int
 
selectedTeam
;

\n
 
 
local
 
int
 
teamSize[2]
;

\n
 
 
local
 
int
 
teamSizeWithBots[2]
;

\n
 
 
local
 
int
 
teamStr[2]
;
 
/
/
 
each
 
team
'
s
 
strength,
 
only
 
used
 
if
 
the
 
#players
 
on
 
each
 
team
 
is
 
equal
\n
 
 
local
 
int
 
teamnr
;

\n
 
 
local
 
String
 
plname
;

\n
 
 
local
 
Pawn
 
p
;

\n
 
 
local
 
TournamentGameReplicationInfo
 
GRI
;

\n

\n
 
 
/
/
 
(nogginBasher)
 
as
 
far
 
as
 
i
 
can
 
tell
 
we
 
don
'
t
 
actually
 
have
 
the
 
pawn
 
of
 
the
 
player
 
we
 
are
 
moving
\n
 
 
/
/
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
we
 
need
 
his
 
nic
k+
ip
 
to
 
get
 
his
 
own
 
strength
.
 
 
Since
 
we
 
can
'
t
 
at
 
the
 
moment
 
we
 
just
 
assume
 
his
 
strength
 
is
 
>
0
 
and
 
put
 
him
 
on
 
the
 
weaker
 
team
.

\n

\n
 
 
if
 
(NextMutator!
=
 
None)
 
NextMutator
.
ModifyLogin(SpawnClass,
 
Portal,
 
Options)
;

\n

\n
 
 
/
/
 
check
 
if
 
this
 
is
 
a
 
team
 
game
 
and
 
if
 
InitTeams
 
has
 
been
 
passed
\n
 
 
/
/
 
Done:
 
don
'
t
 
we
 
want
 
to
 
put
 
this
 
new
 
player
 
on
 
the
 
right
 
team
 
even
 
if
 
InitTeams
 
has
 
been
 
passed?
 
 
so
 
should
 
be
 
ignore
 
gameStartDone?
 
 
nooo,
 
this
 
check
 
is
 
that
 
the
 
game
 
*has*
 
started,
 
because
 
we
 
don
'
t
 
need
 
to
 
switch
 
the
 
players
 
when
 
joining
 
a
 
new
 
map,
 
because
 
InitTeams
 
will
 
do
 
that
.

\n
 
 
if
 
(!gameStartDone)
 
return
;

\n

\n
 
 
if
 
(!ShouldBalance(Level
.
Game))
 
return
;

\n

\n
 
 
if
 
(bExtraDebugLogging)
 
{
 
Log(
"
AutoTeamBalance
.
ModifyLogin()
"
)
;
 
}
\n

\n
 
 
/
/
 
read
 
this
 
player
'
s
 
selected
 
team
\n
 
 
selectedTeam
=
Level
.
Game
.
GetIntOption(Options,
"
Team
"
,255)
;

\n

\n
 
 
/
/
 
get
 
team
 
scores
\n
 
 
GRI
=
TournamentGameReplicationInfo(Level
.
Game
.
GameReplicationInfo)
;

\n
 
 
teamStr[0]
=
GRI
.
Teams[0]
.
Score*FlagStrength
;

\n
 
 
teamStr[1]
=
GRI
.
Teams[1]
.
Score*FlagStrength
;

\n
 
 
teamSize[0]
=
0
;

\n
 
 
teamSize[1]
=
0
;

\n
 
 
teamSizeWithBots[0]
=
0
;

\n
 
 
teamSizeWithBots[1]
=
0
;

\n

\n
 
 
/
/
 
Check
 
team
 
balance
 
of
 
current
 
players
 
in
 
game
\n
 
 
/
/
 
Calculate
 
sum
 
of
 
player
 
strengths
 
for
 
each
 
team
 
(as
 
well
 
as
 
the
 
flagbonus
 
above)
\n
 
 
for
 
(p
=
Level
.
PawnList
;
 
p!
=
None
;
 
p
=
p
.
NextPawn)
\n
 
 
{
\n
 
 
 
 
/
/
 
ignore
 
non-player
 
pawns
\n
 
 
 
 
if
 
(p
.
bIsPlayer
 
&&
 
!p
.
IsA(
'
Spectator
'
))
\n
 
 
 
 
{
\n
 
 
 
 
 
 
teamnr
=
p
.
PlayerReplicationInfo
.
Team
;

\n
 
 
 
 
 
 
if
 
(teamnr
<
2)
\n
 
 
 
 
 
 
{
\n
 
 
 
 
 
 
 
 
if
 
(!p
.
IsA(
'
Bot
'
))
 
teamSize[teamnr]+
=
1
;

\n
 
 
 
 
 
 
 
 
teamSizeWithBots[teamnr]+
=
1
;

\n
 
 
 
 
 
 
 
 
teamStr[teamnr]+
=
GetPawnStrength(p)
;

\n
 
 
 
 
 
 
}
\n
 
 
 
 
}
\n
 
 
}
\n

\n
 
 
if
 
(bClanWar)
\n
 
 
{
\n

\n
 
 
 
 
/
/
 
send
 
player
 
to
 
his
 
clan
'
s
 
team
\n
 
 
 
 
teamnr
=
0
;

\n
 
 
 
 
plname
=
Level
.
Game
.
ParseOption(Options,
"
Name
"
)
;

\n
 
 
 
 
if
 
(Instr(Caps(plname),Caps(clanTag))
=
=
-1)
 
teamnr
=
1
;

\n

\n
 
 
}
 
else
 
{
\n

\n
 
 
 
 
/
/
 
if
 
both
 
teams
 
have
 
the
 
same
 
number
 
of
 
players
 
send
 
the
 
new
 
player
 
to
 
the
 
weaker
 
team
\n
 
 
 
 
if
 
(teamSize[0]
=
=
teamSize[1])
\n
 
 
 
 
{
\n
 
 
 
 
 
 
teamnr
=
0
;
 
if
 
(teamStr[0]
>
teamStr[1])
 
teamnr
=
1
;

\n
 
 
 
 
 
 
Log(
"
AutoTeamBalance
.
ModifyLogin():
 
"
$teamSize[0]$
"
v
"
$teamSize[1]$
"
 
so
 
sending
 
new
 
player
 
to
 
WEAKER
 
team
 
"
$teamnr$
"
.
"
)
;

\n
 
 
 
 
}
 
else
 
{
\n
 
 
 
 
 
 
/
/
 
send
 
player
 
to
 
the
 
team
 
with
 
fewer
 
players
\n
 
 
 
 
 
 
teamnr
=
0
;
 
if
 
(teamSize[0]
>
teamSize[1])
 
teamnr
=
1
;

\n
 
 
 
 
 
 
Log(
"
AutoTeamBalance
.
ModifyLogin():
 
"
$teamSize[0]$
"
v
"
$teamSize[1]$
"
 
so
 
sending
 
new
 
player
 
to
 
SMALLER
 
team
 
"
$teamnr$
"
.
"
)
;

\n
 
 
 
 
}
\n

\n
 
 
}
\n

\n
 
 
/
/
 
if
 
selected
 
team
 
does
 
not
 
equal
 
forced
 
team
 
then
 
modify
 
login
\n
 
 
if
 
(teamnr!
=
selectedTeam)
 
Options
=
"
?Team
=
"
 
$
 
teamnr
 
$
 
Options
;

\n

\n
 
 
/
/
 
fix
 
teamsize
 
bug
 
in
 
Botpack
.
TeamGamePlus
\n
 
 
if
 
(GRI
.
Teams[0]
.
Size!
=
teamSizeWithBots[0]
 
||
 
GRI
.
Teams[1]
.
Size!
=
teamSizeWithBots[1])
\n
 
 
{
\n
 
 
 
 
Log(
"
AutoTeamBalance
.
ModifyLogin():
 
Fixing
 
team
 
size
 
(
"
 
$
 
GRI
.
Teams[0]
.
Size
 
$
 
"
,
"
 
$
 
GRI
.
Teams[1]
.
Size
 
$
 
"
)
 
should
 
be
 
(
"
 
$
 
teamSizeWithBots[0]
 
$
 
"
,
"
 
$
 
teamSizeWithBots[1]
 
$
 
"
)
"
)
;

\n
 
 
 
 
GRI
.
Teams[0]
.
Size
=
teamSizeWithBots[0]
;

\n
 
 
 
 
GRI
.
Teams[1]
.
Size
=
teamSizeWithBots[1]
;

\n
 
 
}
\n
 
 
/
/
 
nogginBasher:
 
I
 
don
'
t
 
understand
 
this
 
bit,
 
but
 
maybe
 
it
'
s
 
because
 
we
 
modified
 
the
 
player
'
s
 
login
\n
 
 
/
/
 
I
 
was
 
thinking
 
of
 
stripping
 
teamSizeWithBots
 
because
 
i
 
have
 
adwvaad
 
on
 
my
 
server,
\n
 
 
/
/
 
but
 
since
 
it
 
isn
'
t
 
used
 
to
 
make
 
the
 
decision
 
of
 
which
 
team
 
to
 
join,
 
it
'
s
 
fine
 
to
 
leave
 
it
 
in
 
for
 
this
 
bugfix
.

\n
 
 
/
/
 
Mmm
 
when
 
someone
 
joins
 
the
 
server
 
mid-game,
 
the
 
bot
 
scores
 
are
 
used
 
to
 
balance
 
the
 
teams
\n
 
 
/
/
 
With
 
advwaad,
 
if
 
there
 
is
 
a
 
bot,
 
he
'
ll
 
be
 
on
 
the
 
smaller
 
team,
 
so
 
i
 
guess
 
it
'
s
 
fine
 
the
 
player
 
will
 
take
 
the
 
bots
 
place
\n
 
 
/
/
 
If
 
there
 
isn
'
t
 
a
 
bot,
 
the
 
new
 
player
 
will
 
go
 
on
 
the
 
weaker
 
team,
 
and
 
adwvaad
 
will
 
add
 
a
 
new
 
bot
 
to
 
the
 
stronger
 
team
\n
}
\n

\n
/
/
 
nogginBasher
 
TESTING
 
hook:
 
HandleEndGame()
 
is
 
a
 
Mutator
 
function
 
called
 
by
 
GameInfo
.
EndGame()
.

\n
/
/
 
OK
 
well
 
it
 
did
 
get
 
called!
 
:)
\n
/
/
 
BUT
 
later
 
mutators
 
in
 
the
 
list
 
have
 
the
 
right
 
to
 
force
 
the
 
game
 
into
 
overtime
 
(we
 
should
 
pass
 
the
 
call
 
onto
 
them
 
here
 
I
 
think),
 
so
 
it
 
may
 
not
 
actually
 
BE
 
the
 
end
 
of
 
the
 
game!
\n
/
/
 
Maybe
 
better
 
just
 
to
 
wait
 
with
 
a
 
timer
 
until
 
bGameEnded
 
=
=
 
True
.

\n
/
/
 
Or
 
start
 
that
 
timer
 
here?
 
^^
 
(so
 
it
 
doesn
'
t
 
need
 
to
 
check
 
during
 
the
 
game)
\n
/
/
 
Hmm
 
from
 
tests
 
I
 
found
 
this
 
function
 
gets
 
called
 
twice
 
for
 
overtime
 
games,
 
but
 
both
 
times
 
bGameEnded
=
False
\n
function
 
bool
 
HandleEndGame()
 
{
\n
 
 
local
 
bool
 
b
;

\n
 
 
if
 
(bExtraDebugLogging)
 
{
 
Log(
"
AutoTeamBalance
.
HandleEndGame()
 
bOverTime
=
"
$Level
.
Game
.
bOvertime$
"
 
bGameEnded
=
"
$Level
.
Game
.
bGameEnded)
;
 
}
\n

\n
 
 
SetTimer(10,True)
;
 
/
/
 
Now
 
checking
 
once
 
a
 
minute
 
to
 
see
 
if
 
game
 
has
 
ended
;
 
changed
 
to
 
10
 
seconds
 
since
 
we
 
lost
 
our
 
alternative
 
MessageMutator
 
hook
\n
 
 
if
 
(bExtraDebugLogging)
 
{
 
Log(
"
AutoTeamBalance
.
HandleEndGame():
 
Set
 
Timer()
 
for
 
10
 
seconds
.
"
)
;
 
}
\n

\n
 
 
if
 
(
 
NextMutator
 
!
=
 
None
 
)
 
{
\n
 
 
 
 
b
 
=
 
NextMutator
.
HandleEndGame()
;

\n
 
 
 
 
if
 
(bExtraDebugLogging)
 
{
 
Log(
"
AutoTeamBalance
.
HandleEndGame()
 
NextMutator
 
returned
 
"
$b$
"
 
 
bOverTime
=
"
$Level
.
Game
.
bOvertime$
"
 
bGameEnded
=
"
$Level
.
Game
.
bGameEnded)
;
 
}
\n
 
 
 
 
return
 
b
;

\n
 
 
}
\n
 
 
return
 
false
;

\n
}
\n

\n
/
/
 
TESTING
 
Alternative,
 
there
 
is:
 
event
 
GameEnding()
 
{
 
.
.
.
 
}
 
implemented
 
in
 
GameInfo
.
 
 
Can
 
we
 
drop
 
our
 
own
 
event
 
catcher
 
in
 
here,
 
without
 
overriding
 
the
 
other?
\n
/
/
 
or
 
ofc
 
we
 
can
 
use
 
a
 
timer
 
and
 
check
 
Level
.
Game
.
bGameEnded
 
but
 
the
 
timer
 
mustn
'
t
 
do
 
this
 
twice
 
at
 
the
 
end
 
of
 
one
 
game
.
 
:P
\n
/
/
 
From
 
testing
 
this:
 
it
 
never
 
gets
 
called!
 
 
(Maybe
 
we
 
must
 
somehow
 
register
 
to
 
receive
 
events
.
)
\n
/
/
 
event
 
GameEnding()
 
{
\n
 
 
/
/
 
if
 
(bExtraDebugLogging)
 
{
 
Log(
"
AutoTeamBalance
.
GameEnding()
 
even
 
was
 
CALLED!
 
 
bOverTime
=
"
$Level
.
Game
.
bOvertime$
"
 
bGameEnded
=
"
$Level
.
Game
.
bGameEnded)
;
 
}
\n
/
/
 
}
\n

\n

\n

\n
/
/
 
=
=
=
=
 
Daniel
'
s
 
other
 
functions:
 
=
=
=
=
 
/
/

\n

\n
/
/
 
Checks
 
if
 
the
 
game
 
has
 
begun
 
(CONSIDER
/
TEST:
 
optimise
 
this
 
and
 
move
/
inline
 
the
 
check
 
right
 
into
 
Tick()
 
above
.
 
 
ATM
 
it
 
is
 
called
 
every
 
tick,
 
although
 
exits
 
fast
.
)
\n
function
 
CheckGameStart()
 
{
\n
 
 
local
 
int
 
c,n,e
;

\n

\n
 
 
/
/
 
if
 
(gameStartDone)
 
return
;
 
/
/
 
This
 
was
 
moved
 
up
 
to
 
Tick()
\n

\n
 
 
/
/
 
this
 
mod
 
works
 
on
 
team
 
games
 
only
\n
 
 
/
/
 
only
 
TeamGamePlus
 
has
 
the
 
"
countdown
"
 
variable
 
and
 
of
 
course
 
teams
\n
 
 
/
/
 
we
 
now
 
check
 
this
 
in
 
ShouldBalance
\n
 
 
/
/
 
if
 
(!bAutoBalanceTeams
 
||
 
!Level
.
Game
.
IsA(OnlyBalanceTeamsIfGametypeIsA)
 
||
 
!Level
.
Game
.
GameReplicationInfo
.
bTeamGame
 
||
 
DeathMatchPlus(Level
.
Game)
.
bTournament)
 
{
\n
 
 
if
 
(!ShouldBalance(Level
.
Game))
 
{
 
/
/
 
We
 
do
 
this
 
early,
 
to
 
check
 
at
 
the
 
very
 
least
 
that
 
this
 
is
 
a
 
teamgame
\n
 
 
 
 
gameStartDone
=
True
;

\n
 
 
 
 
return
;

\n
 
 
}
\n

\n
 
 
/
/
 
read
 
starting
 
countdown
\n
 
 
c
 
=
 
TeamGamePlus(Level
.
Game)
.
countdown
;

\n
 
 
n
 
=
 
TeamGamePlus(Level
.
Game)
.
NetWait
;

\n
 
 
e
 
=
 
TeamGamePlus(Level
.
Game)
.
ElapsedTime
;

\n
 
 
c
 
=
 
Min(c,n-e)
;

\n

\n
 
 
/
/
 
initialize
 
teams
 
1
 
second
 
before
 
game
 
is
 
starting
\n
 
 
if
 
(c
<
2)
 
{
\n
 
 
 
 
if
 
(bBroadcastStuff)
 
{
 
BroadcastMessageAndLog(HelloBroadcast)
;
 
}
\n
 
 
 
 
InitTeams()
;

\n
 
 
 
 
gameStartDone
=
True
;

\n
 
 
}
\n

\n
 
 
/
/
 
Log(
"
AutoTeamBalance
.
CheckGameStart():
 
Done
.
"
)
;
 
 
/
/
 
Too
 
noisy
;
 
gets
 
called
 
many
 
times
 
before
 
the
 
balancin
g+
start
\n

\n
}
\n

\n
function
 
CheckGameEnd()
 
{
\n
 
 
if
 
(gameEndDone)
 
return
;

\n
 
 
if
 
(Level
.
Game
.
bGameEnded)
 
{
\n
 
 
 
 
gameEndDone
 
=
 
true
;

\n
 
 
 
 
if
 
(ShouldUpdateStats(Level
.
Game))
 
{
\n
 
 
 
 
 
 
UpdateStatsAtEndOfGame()
;

\n
 
 
 
 
}
\n
 
 
}
\n
}
\n

\n
/
/
 
Balance
 
the
 
teams
 
just
 
before
 
the
 
start
 
of
 
a
 
new
 
game
.
 
 
No
 
need
 
for
 
FlagStrength
 
here
.

\n
function
 
InitTeams()
 
{
\n
 
 
local
 
Pawn
 
p
;

\n
 
 
local
 
int
 
st
;

\n
 
 
local
 
int
 
pid
;

\n
 
 
local
 
Pawn
 
pl[64]
;
 
/
/
 
list
 
of
 
pawns,
 
with
 
i
 
=
 
a
 
hash
 
of
 
PlayerID
\n
 
 
local
 
int
 
ps[64]
;
 
/
/
 
their
 
strengths
\n
 
 
/
/
 
local
 
int
 
tg[64]
;
 
/
/
 
their
 
strengths,
 
but
 
they
 
get
 
zeroed
 
during
 
the
 
player
 
sorting
/
ranking
\n
 
 
/
/
 
local
 
bool
 
moved[64]
;
 
/
/
 
now
 
i
'
ve
 
decided
 
to
 
use
 
a
 
bool
 
to
 
say
 
whether
 
a
 
player
 
has
 
been
 
moved
 
(because
 
some
 
players
 
might
 
actually
 
have
 
strength
 
zero!)
\n
 
 
local
 
int
 
moved[64]
;
 
/
/
 
dammit
 
bool
 
arrays
 
are
 
not
 
allowed!
 
 
so
 
0
=
false
 
1
=
true
 
:P
\n
 
 
local
 
int
 
plorder[32]
;

\n
 
 
local
 
int
 
i
;

\n
 
 
local
 
int
 
n
;

\n
 
 
local
 
int
 
mx
;

\n
 
 
local
 
int
 
teamnr
;

\n
 
 
local
 
int
 
teamstr[2]
;

\n
 
 
local
 
TeamGamePlus
 
g
;
 
/
/
 
my
 
linux
 
ucc
 
make
 
had
 
trouble
 
with
 
TeamGamePlus
 
:|
\n
 
 
local
 
int
 
oldMaxTeamSize
;

\n
 
 
local
 
bool
 
oldbPlayersBalanceTeams,
 
oldbNoTeamChanges
;

\n

\n
 
 
/
/
 
LoadConfig()
;
 
/
/
 
TODO
 
CONSIDER:
 
If
 
possible,
 
we
 
could
 
also
 
LoadConfig()
 
here
.
 
 
Then
 
this
 
mutator
 
would
 
be
 
the
 
only
 
one
 
I
 
know
 
that
 
lets
 
you
 
edit
 
the
 
ini
 
file
 
without
 
needing
 
to
 
restart
 
the
 
server!
 
 
OK
 
well
 
apparently
 
LoadConfig()
 
doesn
'
t
 
exist
.
 
 
:P
\n
 
 
CopyConfigIntoArrays()
;
 
 
/
/
 
First
 
time
 
the
 
data
 
is
 
needed,
 
we
 
must
 
convert
 
it
.

\n

\n
 
 
if
 
(bExtraDebugLogging)
 
{
 
Log(
"
AutoTeamBalance
.
InitTeams():
 
Running
.
.
.
"
)
;
 
}
\n

\n
 
 
/
/
 
rate
 
all
 
players
\n
 
 
for
 
(p
=
Level
.
PawnList
;
 
p!
=
None
;
 
p
=
p
.
NextPawn)
\n
 
 
{
\n
 
 
 
 
/
/
 
ignore
 
non-player
 
pawns
\n
 
 
 
 
if
 
(p
.
bIsPlayer
 
&&
 
!p
.
IsA(
'
Spectator
'
))
\n
 
 
 
 
{
\n
 
 
 
 
 
 
st
=
GetPawnStrength(p)
;

\n
 
 
 
 
 
 
pid
=
p
.
PlayerReplicationInfo
.
PlayerID
 
%
 
64
;

\n
 
 
 
 
 
 
/
/
 
Why
 
does
 
Sourceror
 
store
 
the
 
players
 
by
 
this
 
pid
 
hash
 
(which
 
might
 
possibly
 
collide)?
 
 
Why
 
not
 
just
 
add
 
the
 
players
 
to
 
a
 
list?
\n
 
 
 
 
 
 
/
/
 
Worth
 
noting,
 
from
 
GameInfo
.
uc:
\n
 
 
 
 
 
 
/
/
 
/
/
 
Set
 
the
 
player
'
s
 
ID
.

\n
 
 
 
 
 
 
/
/
 
NewPlayer
.
PlayerReplicationInfo
.
PlayerID
 
=
 
CurrentI
D+
+
;

\n
 
 
 
 
 
 
/
/
 
So
 
I
 
guess
 
hashing
 
with
 
modulus
 
64
 
might
 
not
 
be
 
so
 
dangerous
.
 
 
:)
\n
 
 
 
 
 
 
pl[pid]
=
p
;

\n
 
 
 
 
 
 
ps[pid]
=
st
;

\n
 
 
 
 
 
 
/
/
 
tg[pid]
=
st
;

\n
 
 
 
 
 
 
moved[pid]
 
=
 
0
;

\n
 
 
 
 
 
 
/
/
 
p
.
PlayerReplicationInfo
.
PlayerName
\n
 
 
 
 
 
 
Log(
"
AutoTeamBalance
.
InitTeams():
 
Player
 
"
 
$
 
p
.
getHumanName()
 
$
 
"
 
on
 
team
 
"
 
$
 
p
.
PlayerReplicationInfo
.
Team
 
$
 
"
 
has
 
i
p+
port
 
"
 
$
 
PlayerPawn(p)
.
GetPlayerNetworkAddress()
 
$
 
"
 
and
 
score
 
"
 
$
 
p
.
PlayerReplicationInfo
.
Score
 
$
 
"
.
"
)
;

\n
 
 
 
 
 
 
/
/
 
if
 
(bBroadcastCookies
 
&&
 
!bOnlyMoreCookies)
 
{
 
BroadcastMessageAndLog(
"
"
 
$
 
p
.
getHumanName()
 
$
 
"
 
has
 
"
 
$st$
 
"
 
cookies
.
"
)
;
 
}
\n
 
 
 
 
}
\n
 
 
}
\n

\n
 
 
/
/
 
sort
 
players
 
by
 
strength
\n
 
 
n
=
0
;

\n
 
 
do
\n
 
 
{
\n
 
 
 
 
pid
=
-1
;

\n
 
 
 
 
mx
=
0
;

\n
 
 
 
 
/
/
 
find
 
pid
=
i
 
with
 
max
 
tg[i]
\n
 
 
 
 
for
 
(i
=
0
;
 
i
<
64
;
 
i+
+)
\n
 
 
 
 
{
\n
 
 
 
 
 
 
/
/
 
if
 
(tg[i]
>
mx)
 
/
/
 
BUG
 
FIXED:
 
if
 
a
 
player
 
has
 
strength
 
<
=
 
0,
 
they
 
will
 
never
 
get
 
into
 
the
 
plorder
 
list,
 
which
 
might
 
really
 
confuse
 
the
 
teams,
 
since
 
they
 
will
 
be
 
sitting
 
on
 
a
 
team
 
but
 
not
 
handled
 
by
 
teambalance
.

\n
 
 
 
 
 
 
/
/
 
if
 
(
 
(pid
 
=
=
 
-1
 
&&
 
pl[i]
 
!
=
 
None
 
&&
 
tg[pid]
 
=
=
 
ps[pid])
 
||
 
(tg[i]
>
mx)
 
)
 
/
/
 
OK
 
added
 
if
 
we
 
haven
'
t
 
yet
 
found
 
a
 
player,
 
and
 
we
 
have
 
one
 
right
 
here,
 
use
 
him
 
regardless
 
of
 
his
 
strength
.
 
 
NO
 
this
 
will
 
confuse
 
below,
 
we
 
use
 
td[pid]
=
0
 
to
 
"
clear
"
 
this
 
record
 
when
 
he
'
s
 
moved
 
to
 
ranking
.
 
 
OK
 
so
 
added
 
test
 
that
 
tg[pid]
 
has
 
not
 
been
 
reset
.
 
 
But,
 
what
 
if
 
it
 
was
 
0
 
to
 
begin
 
with?!
\n
 
 
 
 
 
 
/
/
 
OK
 
so
 
now:
 
is
 
it
 
real
 
player?
 
 
not
 
yet
 
moved?
 
 
first
 
we
 
encountered
 
this
 
scan
 
(grab
 
him!)
 
or
 
stronger
 
than
 
last
 
encountered?
\n
 
 
 
 
 
 
if
 
(
 
pl[i]
 
!
=
 
None
 
&&
 
moved[i]
=
=
0
 
&&
 
(pid
 
=
=
 
-1
 
||
 
ps[i]
>
mx)
 
)
\n
 
 
 
 
 
 
{
\n
 
 
 
 
 
 
 
 
pid
=
i
;

\n
 
 
 
 
 
 
 
 
mx
=
ps[i]
;

\n
 
 
 
 
 
 
}
\n
 
 
 
 
}
\n
 
 
 
 
/
/
 
If
 
we
 
found
 
one,
 
add
 
him
 
as
 
the
 
next
 
player
 
in
 
the
 
list
\n
 
 
 
 
if
 
(pid
 
!
=
 
-1)
\n
 
 
 
 
{
\n
 
 
 
 
 
 
plorder[n]
=
pid
;

\n
 
 
 
 
 
 
/
/
 
ps[pid]
=
0
;

\n
 
 
 
 
 
 
moved[pid]
 
=
 
1
;

\n
 
 
 
 
 
 
n+
+
;

\n
 
 
 
 
 
 
Log(
"
AutoTeamBalance
.
InitTeams():
 
[Ranking]
 
"
$ps[pid]$
"
 
"
$
 
pl[pid]
.
getHumanName()
 
$
"
"
)
;

\n
 
 
 
 
 
 
if
 
(bBroadcastCookies)
 
{
 
BroadcastMessageAndLog(
"
"
$
 
pl[pid]
.
getHumanName()
 
$
"
 
has
 
"
 
$ps[pid]$
 
"
 
cookies
.
"
)
;
 
}
\n
 
 
 
 
 
 
if
 
(bBroadcastCookies)
 
{
 
pl[pid]
.
ClientMessage(
"
You
 
have
 
"
 
$ps[pid]$
 
"
 
cookies
.
"
,
 
'
CriticalEvent
'
,
 
True)
;
 
}
\n
 
 
 
 
}
\n
 
 
}
 
until
 
(pid
=
=
-1)
;

\n

\n
 
 
/
/
 
save
 
team
 
changing
 
rules
 
to
 
override
 
them
\n
 
 
g
=
TeamGamePlus(Level
.
Game)
;

\n
 
 
oldMaxTeamSize
=
g
.
MaxTeamSize
;

\n
 
 
oldbPlayersBalanceTeams
=
g
.
bPlayersBalanceTeams
;

\n
 
 
oldbNoTeamChanges
=
g
.
bNoTeamChanges
;

\n

\n
 
 
/
/
 
deactivate
 
team
 
changing
 
rules
\n
 
 
g
.
MaxTeamSize
=
32
;

\n
 
 
g
.
bPlayersBalanceTeams
=
False
;

\n
 
 
g
.
bNoTeamChanges
=
False
;

\n

\n
 
 
if
 
(bClanWar)
\n
 
 
{
\n

\n
 
 
 
 
/
/
 
rebuild
 
teams
 
by
 
clan
 
tags
\n
 
 
 
 
teamstr[0]
=
0
;

\n
 
 
 
 
teamstr[1]
=
0
;

\n
 
 
 
 
for
 
(i
=
0
;
 
i
<
n
;
 
i+
+)
\n
 
 
 
 
{
\n
 
 
 
 
 
 
pid
=
plorder[i]
;

\n
 
 
 
 
 
 
teamnr
=
0
;

\n
 
 
 
 
 
 
/
/
 
NOTE:
 
here
 
we
 
are
 
using
 
Playername,
 
in
 
other
 
places
 
we
'
ve
 
used
 
getHumanName
.

\n
 
 
 
 
 
 
if
 
(Instr(Caps(pl[pid]
.
PlayerReplicationInfo
.
Playername),Caps(clanTag))
=
=
-1)
 
teamnr
=
1
;

\n
 
 
 
 
 
 
Level
.
Game
.
ChangeTeam(pl[pid],teamnr)
;

\n
 
 
 
 
 
 
teamstr[teamnr]+
=
ps[pid]
;

\n
 
 
 
 
}
\n

\n
 
 
}
 
else
 
{
\n

\n
 
 
 
 
/
/
 
Rebuild
 
teams
 
from
 
strength
 
order
 
1-2-2-1-1-2-2-1
 
.
.
.

\n
 
 
 
 
/
/
 
(On
 
the
 
way
 
we
 
also
 
calculate
 
total
 
team
 
strengths)
\n
 
 
 
 
teamstr[0]
=
0
;

\n
 
 
 
 
teamstr[1]
=
0
;

\n
 
 
 
 
for
 
(i
=
0
;
 
i
<
(n&254)
;
 
i+
+)
\n
 
 
 
 
{
\n
 
 
 
 
 
 
pid
=
plorder[i]
;

\n
 
 
 
 
 
 
teamnr
=
0
;

\n
 
 
 
 
 
 
if
 
((i&3)
=
=
1
 
||
 
(i&3)
=
=
2)
 
teamnr
=
1
;

\n
 
 
 
 
 
 
Log(
"
AutoTeamBalance
.
InitTeams():
 
i
=
"
$i$
"
 
Putting
 
pid
=
"
$pid$
"
 
pl
=
"
$pl[pid]
.
getHumanName()$
"
 
into
 
team
 
"
$teamnr$
"
.
"
)
;

\n
 
 
 
 
 
 
Level
.
Game
.
ChangeTeam(pl[pid],teamnr)
;

\n
 
 
 
 
 
 
teamstr[teamnr]+
=
ps[pid]
;

\n
 
 
 
 
}
\n

\n
 
 
 
 
/
/
 
if
 
there
 
is
 
an
 
odd
 
number
 
of
 
players
 
put
 
the
 
last
 
player
 
in
 
the
 
weaker
 
team
\n
 
 
 
 
if
 
((n&1)
=
=
1)
\n
 
 
 
 
{
\n
 
 
 
 
 
 
pid
=
plorder[i]
;

\n
 
 
 
 
 
 
teamnr
=
0
;
 
if
 
(teamstr[0]
>
teamstr[1])
 
teamnr
=
1
;

\n
 
 
 
 
 
 
Log(
"
AutoTeamBalance
.
InitTeams():
 
"
$n$
"
 
is
 
odd
 
so
 
sending
 
last
 
player
 
to
 
WEAKER
 
team
 
"
$teamnr$
"
.
"
)
;

\n
 
 
 
 
 
 
Level
.
Game
.
ChangeTeam(pl[pid],teamnr)
;

\n
 
 
 
 
 
 
teamstr[teamnr]+
=
ps[pid]
;

\n
 
 
 
 
}
\n

\n
 
 
}
\n

\n
 
 
/
/
 
restore
 
team
 
changing
 
rules
\n
 
 
g
.
MaxTeamSize
=
oldMaxTeamSize
;

\n
 
 
g
.
bPlayersBalanceTeams
=
oldbPlayersBalanceTeams
;

\n
 
 
g
.
bNoTeamChanges
=
oldbNoTeamChanges
;

\n

\n
 
 
/
/
 
Show
 
team
 
strengths
 
to
 
all
 
players
\n
 
 
Log(
"
AutoTeamBalance
.
InitTeams():
 
Red
 
team
 
strength
 
is
 
"
 
$
 
teamstr[0]
 
$
 
"
.
 
 
Blue
 
team
 
strength
 
is
 
"
 
$
 
teamstr[1]
 
$
 
"
.
"
)
;

\n
 
 
if
 
(bBroadcastStuff)
 
{
 
BroadcastMessage(
"
Red
 
team
 
strength
 
is
 
"
 
$
 
teamstr[0]
 
$
 
"
.
 
 
Blue
 
team
 
strength
 
is
 
"
 
$
 
teamstr[1]
 
$
 
"
.
"
)
;
 
}
\n

\n
 
 
/
/
 
Little
 
point
 
doing
 
this
 
here
;
 
wait
 
until
 
we
 
update
 
the
 
player
 
strengths
.

\n
 
 
/
/
 
CopyArraysIntoConfig()
;

\n
 
 
/
/
 
SaveConfig()
;

\n

\n
}
\n

\n
/
/
 
Returns
 
the
 
strength
 
of
 
a
 
player
 
or
 
a
 
bot
\n
function
 
int
 
GetPawnStrength(Pawn
 
p)
 
{
\n
 
 
local
 
int
 
st
;

\n

\n
 
 
if
 
(p
.
IsA(
'
PlayerPawn
'
)
 
&&
 
p
.
bIsHuman)
\n
 
 
{
\n
 
 
 
 
/
/
 
a
 
human
 
player
 
-
 
get
 
his
 
strength
\n
 
 
 
 
st
=
GetPlayerStrength(PlayerPawn(p))
;

\n
 
 
}
 
else
 
{
\n
 
 
 
 
/
/
 
a
 
bot
 
-
 
use
 
default
 
strength
\n
 
 
 
 
st
=
BotStrength
;

\n
 
 
}
\n

\n
 
 
if
 
(bExtraDebugLogging)
 
{
 
Log(
"
AutoTeamBalance
.
GetPawnStrength(
"
 
$
 
p
 
$
 
"
):
 
"
 
$
 
st
 
$
 
"
"
)
;
 
}
\n

\n
 
 
return
 
st
;

\n
}
\n

\n
/
/
 
Returns
 
the
 
strength
 
of
 
a
 
player
\n
function
 
int
 
GetPlayerStrength(PlayerPawn
 
p)
 
{
\n
 
 
local
 
int
 
found
;

\n
 
 
found
 
=
 
FindPlayerRecord(p)
;

\n
 
 
if
 
(found
 
=
=
 
-1)
 
{
\n
 
 
 
 
return
 
UnknownStrength
;
 
/
/
 
unknown
 
player
 
or
 
player
 
is
 
too
 
weak
 
for
 
list
\n
 
 
}
 
else
 
{
\n
 
 
 
 
return
 
avg_score[found]
;
 
/
/
 
player
'
s
 
average
 
score
 
(or
 
best
 
estimate
 
of
 
player)
\n
 
 
}
\n
 
 
/
/
 
return
 
UnknownStrength
;

\n
}
\n

\n

\n

\n
/
*
\n
/
/
 
Old
 
Timer
 
which
 
was
 
updating
 
stats
 
during
 
the
 
game
\n
event
 
Timer()
 
{
 
/
/
 
this
 
may
 
be
 
a
 
reasonably
 
hard
 
work
 
process
;
 
i
 
hope
 
it
'
s
 
been
 
given
 
it
'
s
 
own
 
thread!
\n
 
 
/
/
 
TESTING
 
these
 
counters
;
 
really
 
i
 
want
 
to
 
know
 
how
 
far
 
after
 
the
 
end
 
of
 
the
 
game
 
we
 
are
\n
 
 
local
 
int
 
c,n,e,l,t,s
;

\n
 
 
if
 
(bExtraDebugLogging)
 
{
\n
 
 
 
 
c
 
=
 
TeamGamePlus(Level
.
Game)
.
countdown
;

\n
 
 
 
 
n
 
=
 
TeamGamePlus(Level
.
Game)
.
NetWait
;

\n
 
 
 
 
e
 
=
 
TeamGamePlus(Level
.
Game)
.
ElapsedTime
;

\n
 
 
 
 
l
 
=
 
TeamGamePlus(Level
.
Game)
.
TimeLimit
;

\n
 
 
 
 
t
 
=
 
Level
.
TimeSeconds
;

\n
 
 
 
 
s
 
=
 
Level
.
Game
.
StartTime
;

\n
 
 
 
 
Log(
"
AutoTeamBalance
.
Timer()
 
DEBUG
 
c
=
"
$c$
"
 
b
=
"
$n$
"
 
e
=
"
$e$
"
 
l
=
"
$l$
"
 
t
=
"
$t$
"
 
s
=
"
$s$
"
 
bGameEnded
=
"
$Level
.
Game
.
bGameEnded)
;

\n
 
 
}
\n
 
 
/
/
 
if
 
(bUpdatePlayerStats)
 
{
\n
 
 
 
 
/
/
 
Stats
 
were
 
updating
 
during
 
a
 
game
 
of
 
DM
 
ffa,
 
3
 
players,
 
low
 
scores
.
 
 
This
 
gives
 
very
 
different
 
scores
 
than
 
CTF
 
games
.

\n
 
 
 
 
/
/
 
Presumably
 
we
 
have
 
not
 
checked
 
that
 
this
 
is
 
*really*
 
a
 
team-game
 
we
 
are
 
getting
 
stats
 
from
.

\n
 
 
 
 
/
/
 
For
 
now,
 
have
 
optionally
 
limited
 
stats
 
to
 
CTF
 
games
 
only:
\n
 
 
 
 
/
/
 
if
 
(Level
.
Game
.
IsA(
'
CTFGame
'
)
 
||
 
!bUpdateStatsForCTFOnly)
 
{
\n
 
 
 
 
/
/
 
if
 
(Level
.
Game
.
IsA(OnlyUpdateStatsIfGametypeIsA))
 
{
\n
 
 
 
 
if
 
(ShouldUpdateStats(Level
.
Game))
 
{
\n
 
 
 
 
 
 
UpdateStatsAtEndOfGame()
;

\n
 
 
 
 
}
 
else
 
{
\n
 
 
 
 
 
 
/
/
 
Log(
"
AutoTeamBalance
.
Timer():
 
not
 
running
 
UpdateStatsAtEndOfGame()
 
since
 
Level
.
Game
 
=
 
"
$Level
.
Game$
"
 
!
=
 
"
$OnlyUpdateStatsIfGametypeIsA$
"
.
"
)
;

\n
 
 
 
 
 
 
Log(
"
AutoTeamBalance
.
Timer():
 
Refusing
 
to
 
run
 
UpdateStatsAtEndOfGame()
.
"
)
;

\n
 
 
 
 
}
\n
 
 
/
/
 
}
\n
}
\n
*
/

\n

\n
/
/
 
New
 
Timer
 
which
 
is
 
just
 
looking
 
for
 
the
 
end
 
of
 
the
 
game
\n
event
 
Timer()
 
{
\n
 
 
/
*
\n
 
 
local
 
int
 
c,n,e,l,t,s
;

\n
 
 
if
 
(bExtraDebugLogging)
 
{
\n
 
 
 
 
c
 
=
 
TeamGamePlus(Level
.
Game)
.
countdown
;

\n
 
 
 
 
n
 
=
 
TeamGamePlus(Level
.
Game)
.
NetWait
;

\n
 
 
 
 
e
 
=
 
TeamGamePlus(Level
.
Game)
.
ElapsedTime
;

\n
 
 
 
 
l
 
=
 
TeamGamePlus(Level
.
Game)
.
TimeLimit
;

\n
 
 
 
 
t
 
=
 
Level
.
TimeSeconds
;

\n
 
 
 
 
s
 
=
 
Level
.
Game
.
StartTime
;

\n
 
 
 
 
Log(
"
AutoTeamBalance
.
Timer()
 
DEBUG
 
c
=
"
$c$
"
 
b
=
"
$n$
"
 
e
=
"
$e$
"
 
l
=
"
$l$
"
 
t
=
"
$t$
"
 
s
=
"
$s$
"
 
bGameEnded
=
"
$Level
.
Game
.
bGameEnded)
;

\n
 
 
}
\n
 
 
*
/

\n
 
 
CheckGameEnd()
;

\n
}
\n

\n

\n

\n
/
/
 
=
=
=
=
 
nogginBasher
'
s
 
player
 
records
 
back-end:
 
=
=
=
=
 
/
/

\n

\n
/
/
 
From
 
playerData[]
 
to
 
ip[],nick[],avg_score[],
.
.
.
 
(should
 
be
 
done
 
at
 
the
 
start)
\n
function
 
CopyConfigIntoArrays()
 
{
\n
 
 
local
 
int
 
field
;

\n
 
 
local
 
int
 
i
;

\n
 
 
local
 
String
 
data
;

\n
 
 
Log(
"
AutoTeamBalance
.
CopyConfigIntoArrays()
 
running
"
)
;

\n
 
 
for
 
(i
=
0
;
 
i
<
MaxPlayerData
;
 
i+
+)
 
{
\n
 
 
 
 
data
 
=
 
playerData[i]
;

\n
 
 
 
 
if
 
(
 
data
 
=
=
 
"
"
 
)
\n
 
 
 
 
 
 
continue
;

\n
 
 
 
 
/
/
 
I
 
didn
'
t
 
try
 
to
 
work
 
out
 
the
 
Divide()
 
and
 
Split()
 
fns
 
offered
 
on
 
http:
/
/
udn
.
epicgames
.
com
/
Two
/
UnrealScriptReference
.
html
\n
 
 
 
 
field
 
=
 
InStr(data,
"
 
"
)
;

\n
 
 
 
 
 
 
ip[i]
 
=
 
Left(data,field)
;

\n
 
 
 
 
data
 
=
 
Mid(data,fiel
d+
1)
;

\n
 
 
 
 
field
 
=
 
InStr(data,
"
 
"
)
;

\n
 
 
 
 
 
 
nick[i]
 
=
 
Left(data,field)
;

\n
 
 
 
 
data
 
=
 
Mid(data,fiel
d+
1)
;

\n
 
 
 
 
field
 
=
 
InStr(data,
"
 
"
)
;

\n
 
 
 
 
 
 
avg_score[i]
 
=
 
Float(Left(data,field))
;

\n
 
 
 
 
data
 
=
 
Mid(data,fiel
d+
1)
;

\n
 
 
 
 
 
 
hours_played[i]
 
=
 
Float(data)
;

\n
 
 
}
\n
 
 
Log(
"
AutoTeamBalance
.
CopyConfigIntoArrays()
 
done
"
)
;

\n
}
\n

\n
/
/
 
From
 
ip[],nick[],avg_score[],
.
.
.
 
to
 
playerData[]
 
(should
 
be
 
done
 
before
 
SaveConfig())
\n
function
 
CopyArraysIntoConfig()
 
{
\n
 
 
local
 
int
 
i
;

\n
 
 
Log(
"
AutoTeamBalance
.
CopyArraysIntoConfig()
 
running
"
)
;

\n
 
 
for
 
(i
=
0
;
 
i
<
MaxPlayerData
;
 
i+
+)
 
{
\n
 
 
 
 
 
if
 
(
 
ip[i]
 
!
=
 
"
"
 
)
 
{
\n
 
 
 
 
 
 
 
playerData[i]
 
=
 
ip[i]
 
$
 
"
 
"
 
$
 
nick[i]
 
$
 
"
 
"
 
$
 
avg_score[i]
 
$
 
"
 
"
 
$
 
hours_played[i]
;

\n
 
 
 
 
}
\n
 
 
}
\n
 
 
Log(
"
AutoTeamBalance
.
CopyArraysIntoConfig()
 
done
"
)
;

\n
}
\n

\n
/
/
 
guaranteed
 
to
 
return
 
index
 
i
 
into
 
playerData[]
 
and
 
ip[]
/
nick[]
/
.
.
.
 
arrays,
 
but
 
not
 
always
 
an
 
exact
 
player
 
match!
\n
function
 
int
 
FindPlayerRecord(PlayerPawn
 
p)
 
{
\n
 
 
local
 
int
 
found
;

\n
 
 
local
 
int
 
i
;

\n
 
 
found
 
=
 
-1
;

\n
 
 
for
 
(i
=
0
;
i
<
MaxPlayerData
;
i+
+)
 
{
\n
 
 
 
 
/
/
 
Exact
 
match!
 
return
 
the
 
index
 
immediately
\n
 
 
 
 
if
 
(p
.
getHumanName()
 
=
=
 
nick[i]
 
&&
 
stripPort(p
.
GetPlayerNetworkAddress())
 
=
=
 
ip[i])
 
{
\n
 
 
 
 
 
 
found
 
=
 
i
;

\n
 
 
 
 
 
 
Log(
"
AutoTeamBalance
.
FindPlayerRecord(p)
 
Exact
 
match
 
for
 
"
 
$nick[i]$
 
"
,
"
$ip[i]$
"
:
 
[
"
$found$
"
]
 
(
"
$avg_score[i]$
"
)
"
)
;

\n
 
 
 
 
 
 
return
 
found
;

\n
 
 
 
 
}
 
else
 
{
\n
 
 
 
 
 
 
/
/
 
Backups
 
if
 
we
 
don
'
t
 
find
 
the
 
exact
 
i
p+
nick
\n
 
 
 
 
 
 
if
 
(stripPort(p
.
GetPlayerNetworkAddress())
 
=
=
 
ip[i])
 
{
\n
 
 
 
 
 
 
 
 
found
 
=
 
i
;
 
/
/
 
matching
 
ip
\n
 
 
 
 
 
 
 
 
Log(
"
AutoTeamBalance
.
FindPlayerRecord(p)
 
IP
 
match
 
for
 
"
 
$p
.
getHumanName()$
 
"
,
"
$stripPort(p
.
GetPlayerNetworkAddress())$
"
:
 
[
"
$found$
"
]
 
"
$nick[i]$
"
 
(
"
$avg_score[i]$
"
)
"
)
;

\n
 
 
 
 
 
 
}
\n
 
 
 
 
 
 
if
 
(p
.
getHumanName()
 
=
=
 
nick[i]
 
&&
 
found
 
=
=
 
-1)
 
{
\n
 
 
 
 
 
 
 
 
Log(
"
AutoTeamBalance
.
FindPlayerRecord(p)
 
nick
 
match
 
for
 
"
 
$nick[i]$
 
"
,
"
$p
.
GetPlayerNetworkAddress()$
"
:
 
[
"
$found$
"
]
 
"
$ip[i]$
"
 
(
"
$avg_score[i]$
"
)
"
)
;

\n
 
 
 
 
 
 
 
 
found
 
=
 
i
;
 
/
/
 
if
 
not
 
yet
 
matching
 
an
 
ip,
 
match
 
the
 
same
 
nick
 
on
 
a
 
different
 
ip
\n
 
 
 
 
 
 
}
\n
 
 
 
 
 
 
/
/
 
TODO:
 
if
 
an
 
uneven
 
match,
 
choose
 
a
 
match
 
with
 
more
 
experience
 
(hours_played)
\n
 
 
 
 
 
 
/
/
 
TODO:
 
if
 
we
 
have
 
little
 
experience
 
(
<
10mins)
 
of
 
a
 
player,
 
assume
 
default
 
score?
\n
 
 
 
 
}
\n
 
 
}
\n
 
 
if
 
(found
 
=
=
 
-1)
 
{
\n
 
 
 
 
found
 
=
 
CreateNewPlayerRecord(p)
;

\n
 
 
}
\n
 
 
return
 
found
;

\n
}
\n

\n
/
/
 
Currently
 
new
 
players
 
override
 
the
 
one
 
at
 
the
 
bottom
 
of
 
the
 
list
.
 
 
At
 
the
 
moment
 
you
 
are
 
guaranteed
 
a
 
valid
 
record
 
after
 
calling
 
FindPlayerRecord()
 
or
 
CreateNewPlayerRecord()
.

\n
function
 
int
 
CreateNewPlayerRecord(PlayerPawn
 
p)
 
{
\n
 
 
local
 
int
 
pos
;

\n
 
 
/
/
 
Find
 
an
 
empty
 
slot:
\n
 
 
for
 
(pos
=
0
;
pos
<
MaxPlayerData
;
po
s+
+)
 
{
\n
 
 
 
 
if
 
(ip[pos]
 
=
=
 
"
"
 
&&
 
nick[pos]
 
=
=
 
"
"
)
 
{
\n
 
 
 
 
 
 
break
;

\n
 
 
 
 
}
\n
 
 
}
\n
 
 
/
/
 
TODO:
 
If
 
all
 
full,
 
could
 
add
 
somewhere
 
randomly
 
in
 
the
 
last
 
100
 
(ordered)
 
spots
 
(rather
 
than
 
just
 
1
 
spot
 
which
 
keeps
 
getting
 
re-used)
\n
 
 
/
/
 
DONE:
 
find
 
the
 
record
 
with
 
lowest
 
hours_played
 
and
 
replace
 
that
 
one
\n
 
 
/
/
 
better:
 
or,
 
find
 
the
 
oldest
 
record
 
and
 
replace
 
it
\n
 
 
if
 
(pos
 
=
=
 
MaxPlayerData)
 
{
 
/
/
 
all
 
records
 
were
 
full
\n
 
 
 
 
pos
 
=
 
FindShortestPlayerRecord()
;

\n
 
 
}
\n
 
 
ip[pos]
 
=
 
stripPort(p
.
GetPlayerNetworkAddress())
;

\n
 
 
nick[pos]
 
=
 
p
.
getHumanName()
;

\n
 
 
/
/
 
initialise
 
each
 
player
 
as
 
having
 
played
 
for
 
UnknownMinutes
 
(e
.
g
.
 
10
 
or
 
0
.
1)
 
minutes
 
already,
 
and
 
already
 
earned
 
an
 
average
 
UnknownStrength
 
(e
.
g
.
 
40)
 
frags
\n
 
 
avg_score[pos]
 
=
 
UnknownStrength
;
 
/
/
 
DO
 
NOT
 
set
 
this
 
to
 
0
;
 
it
 
will
 
screw
 
with
 
InitTeams()!
\n
 
 
hours_played[pos]
 
=
 
0
;
 
/
/
 
UnknownMinutes
/
60
;

\n
 
 
Log(
"
AutoTeamBalance
.
CreateNewPlayerRecord(
"
$p$
"
)
 
[
"
$pos$
"
]
 
"
$
 
nick[pos]
 
$
"
 
"
$
 
ip[pos]
 
$
"
 
"
$
 
avg_score[pos]
 
$
"
 
"
$
 
hours_played[pos]
 
$
"
.
"
)
;

\n
 
 
/
/
 
if
 
(bBroadcastCookies)
 
{
 
BroadcastMessageAndLog(
"
Welcome
 
"
$
 
nick[pos]
 
$
"
!
 
 
You
 
have
 
"
$
 
avg_score[pos]
 
$
"
 
cookies
.
"
)
;
 
}
\n
 
 
/
/
 
if
 
(bBroadcastCookies)
 
{
 
BroadcastMessageAndLog(
"
Welcome
 
to
 
the
 
server
 
"
$
 
nick[pos]
 
$
"
!
 
 
Have
 
a
 
cookie
.
 
 
:)
"
)
;
 
}
\n
 
 
if
 
(bBroadcastCookies)
 
{
 
p
.
ClientMessage(
"
Welcome
 
to
 
the
 
server
 
"
$
 
nick[pos]
 
$
"
!
 
 
Have
 
a
 
cookie
.
 
 
:)
"
,
 
'
CriticalEvent
'
,
 
True)
;
 
}
\n
 
 
/
/
 
SaveConfig()
;

\n
 
 
return
 
pos
;

\n
}
\n

\n
/
/
 
Finds
 
an
 
old
 
player
 
record
 
which
 
we
 
can
 
replace
.
 
 
Actually
 
since
 
we
 
don
'
t
 
have
 
a
 
last_seen
 
field,
 
we
'
ll
 
just
 
have
 
to
 
remove
 
the
 
"
shortest
"
 
record
.
 
 
(Player
 
didn
'
t
 
spend
 
long
 
on
 
server
;
 
their
 
stats
 
don
'
t
 
mean
 
a
 
lot)
\n
/
/
 
Only
 
problem,
 
if
 
the
 
database
 
really
 
is
 
saturated
 
(but
 
I
 
think
 
that
'
s
 
unlikely),
 
this
 
new
 
player
 
will
 
probably
 
be
 
the
 
next
 
record
 
to
 
be
 
replaced!
 
 
To
 
keep
 
his
 
record
 
in
 
the
 
database,
 
the
 
new
 
player
 
just
 
has
 
to
 
play
 
for
 
longer
 
than
 
the
 
now
 
"
shortest
"
 
record
 
before
 
another
 
new
 
player
 
joins
.

\n
/
/
 
Actually
 
one
 
nice
 
side-effect
 
of
 
the
 
particular
 
algorithm
 
we
'
re
 
using
 
below
 
(
<
lowest
 
instead
 
of
 
<
=
lowest):
 
if
 
a
 
few
 
records
 
share
 
the
 
"
shortest
 
record
"
 
time
 
(actually
 
this
 
was
 
more
 
likely
 
when
 
our
 
hours_played
 
were
 
incremented
 
in
 
fixed-size
 
steps),
 
it
 
will
 
be
 
the
 
first
 
of
 
them
 
that
 
gets
 
replaced
 
first
.
 
 
:)
 
 
Down-side:
 
the
 
new
 
player
 
now
 
in
 
that
 
early
 
position
 
in
 
the
 
stats-table
 
was
 
not
 
an
 
early
 
player
 
on
 
the
 
server,
 
so
 
he
 
breaks
 
this
 
very
 
pattern
.

\n
function
 
int
 
FindShortestPlayerRecord()
 
{
\n
 
 
local
 
int
 
i,found
;

\n
 
 
found
 
=
 
0
;

\n
 
 
for
 
(i
=
1
;
i
<
MaxPlayerData
;
i+
+)
 
{
\n
 
 
 
 
if
 
(hours_played[i]
 
<
 
hours_played[found])
 
{
\n
 
 
 
 
 
 
found
 
=
 
i
;

\n
 
 
 
 
}
\n
 
 
}
\n
 
 
return
 
found
;

\n
}
\n

\n
function
 
UpdateStatsAtEndOfGame()
 
{
\n
 
 
local
 
int
 
countHumanPlayers
;

\n
 
 
local
 
Pawn
 
p
;

\n

\n
 
 
/
/
 
Do
 
not
 
update
 
stats
 
for
 
games
 
with
 
<
MinHumansForStats
 
human
 
players
.

\n
 
 
countHumanPlayers
 
=
 
0
;

\n
 
 
for
 
(p
=
Level
.
PawnList
;
 
p!
=
None
;
 
p
=
p
.
NextPawn)
 
{
\n
 
 
 
 
if
 
(p
.
bIsPlayer
 
&&
 
!p
.
IsA(
'
Spectator
'
)
 
&&
 
!p
.
IsA(
'
Bot
'
)
 
&&
 
p
.
IsA(
'
PlayerPawn
'
)
 
&&
 
p
.
bIsHuman)
 
{
 
/
/
 
maybe
 
the
 
last
 
2
 
are
 
not
 
needed
\n
 
 
 
 
 
 
countHumanPlayer
s+
+
;

\n
 
 
 
 
}
\n
 
 
}
\n
 
 
if
 
(countHumanPlayers
<
MinHumansForStats)
 
{
\n
 
 
 
 
Log(
"
AutoTeamBalance
.
UpdateStatsAtEndOfGame():
 
not
 
updating
 
stats
 
since
 
countHumanPlayers
 
"
$countHumanPlayers$
"
 
<
 
"
$MinHumansForStats$
"
.
"
)
;

\n
 
 
 
 
return
;

\n
 
 
}
\n

\n
 
 
/
/
 
Update
 
stats
 
for
 
all
 
players
 
in
 
game
\n
 
 
Log(
"
AutoTeamBalance
.
UpdateStatsAtEndOfGame():
 
Updating
 
player
 
stats
.
"
)
;

\n
 
 
if
 
(bBroadcastStuff)
 
{
 
BroadcastMessageAndLog(
"
AutoTeamBalance
 
is
 
updating
 
player
 
stats
.
"
)
;
 
}
\n
 
 
/
/
 
TEST
 
considered
 
when
 
stats
 
were
 
being
 
updated
 
mid-game:
 
make
 
lag
 
here
 
on
 
purpose
 
and
 
see
 
how
 
bad
 
we
 
can
 
get
 
it
 
/
 
how
 
we
 
can
 
fix
 
it
.

\n
 
 
for
 
(p
=
Level
.
PawnList
;
 
p!
=
None
;
 
p
=
p
.
NextPawn)
 
{
\n
 
 
 
 
if
 
(p
.
bIsPlayer
 
&&
 
!p
.
IsA(
'
Spectator
'
)
 
&&
 
!p
.
IsA(
'
Bot
'
)
 
&&
 
p
.
IsA(
'
PlayerPawn
'
)
 
&&
 
p
.
bIsHuman)
 
{
 
/
/
 
lol
\n
 
 
 
 
 
 
UpdateStatsForPlayer(PlayerPawn(p))
;

\n
 
 
 
 
}
\n
 
 
}
\n

\n
 
 
/
/
 
Save
 
the
 
new
 
stats
 
in
 
the
 
config
/
ini
 
file:
\n
 
 
Log(
"
AutoTeamBalance
.
UpdateStatsAtEndOfGame():
 
saving
 
stats
 
to
 
file
"
)
;

\n
 
 
CopyArraysIntoConfig()
;

\n
 
 
SaveConfig()
;
 
/
/
 
This
 
is
 
the
 
only
 
place
 
this
 
gets
 
done
 
atm!
\n

\n
 
 
Log(
"
AutoTeamBalance
.
UpdateStatsAtEndOfGame():
 
done
"
)
;

\n

\n
}
\n

\n
function
 
UpdateStatsForPlayer(PlayerPawn
 
p)
 
{
\n
 
 
local
 
int
 
i,j
;

\n
 
 
local
 
float
 
current_score
;

\n
 
 
local
 
float
 
new_hours_played
;

\n
 
 
local
 
int
 
previousPolls
;

\n
 
 
local
 
int
 
gameDuration
;

\n
 
 
local
 
int
 
timeInGame
;

\n
 
 
local
 
float
 
weightScore
;

\n
 
 
local
 
float
 
previous_average
;

\n

\n
 
 
i
 
=
 
FindPlayerRecord(p)
;

\n

\n
 
 
if
 
(i
 
=
=
 
-1
 
||
 
ip[i]
 
!
=
 
stripPort(p
.
GetPlayerNetworkAddress())
 
||
 
nick[i]
 
!
=
 
p
.
getHumanName())
 
{
\n
 
 
 
 
/
/
 
This
 
is
 
not
 
an
 
exact
 
player
 
match,
 
so
 
we
 
should
 
not
 
update
 
its
 
stats
\n
 
 
 
 
/
/
 
since
 
we
 
didn
'
t
 
find
 
this
 
actual
 
i
p+
nick,
 
we
 
create
 
a
 
new
 
entry
\n
 
 
 
 
/
/
 
DONE:
 
should
 
we
 
base
 
the
 
new
 
players
 
stats
 
on
 
the
 
default
 
(below),
 
or
 
copy
 
over
 
the
 
estimated
 
player
'
s
 
stats?
 
 
(But
 
maybe
 
reset
 
their
 
time_on_server
.
)
\n
 
 
 
 
j
 
=
 
CreateNewPlayerRecord(p)
;
 
/
/
 
OLD
 
BUG
 
FIXED:
 
is
 
it
 
inefficient
 
to
 
repeatedly
 
create
 
a
 
PlayerPawn
 
from
 
the
 
same
 
Pawn?
\n
 
 
 
 
/
/
 
Copy
 
over
 
strength
 
from
 
the
 
partial-match
 
player,
 
but
 
only
 
make
 
that
 
strength
 
last
 
for
 
2
 
hours
.

\n
 
 
 
 
/
/
 
SO:
 
changing
 
nick
 
will
 
Not
 
reset
 
your
 
avg_score
 
immediately,
 
but
 
eventually
\n
 
 
 
 
avg_score[j]
 
=
 
avg_score[i]
;
 
/
/
 
Copy
 
score
 
from
 
partial
 
record
 
max
\n
 
 
 
 
hours_played[j]
 
=
 
Min(2
.
0,hours_played[i])
;
 
/
/
 
but
 
in
 
case
 
this
 
is
 
a
 
different
 
player
 
(or
 
maybe
 
the
 
same
 
player
 
but
 
in
 
a
 
different
 
environment),
 
give
 
the
 
new
 
record
 
max
 
2
 
hours
\n
 
 
 
 
i
 
=
 
j
;

\n
 
 
}
\n

\n
 
 
current_score
 
=
 
p
.
PlayerReplicationInfo
.
Score
;

\n
 
 
if
 
(bNormaliseScores)
 
{
\n
 
 
 
 
current_score
 
=
 
NormaliseScore(current_score)
;

\n
 
 
}
\n
 
 
/
/
 
Ideally
 
we
 
would
 
like
 
to
 
check
 
how
 
long
 
this
 
player
 
has
 
been
 
on
 
the
 
server
 
DONE
 
i
 
don
'
t
 
know
 
how
 
to
 
get
 
that
 
yet
 
^^
 
I
'
m
 
hoping
 
it
'
s
 
somewhere
 
in
 
the
 
code
 
otherwise
 
I
 
have
 
to
 
remember
 
the
 
times
 
that
 
players
 
joined
 
 
 
 
Ahh
 
got
 
it
 
from
 
iDeFiX
'
s
 
code,
 
ofc
 
it
'
s
 
in
 
PlayerReplicationInfo,
 
like
 
everything
 
else
 
I
 
can
'
t
 
find
 
in
 
PlayerPawn
 
:
>

\n
 
 
/
/
 
For
 
the
 
moment,
 
assume
 
all
 
players
 
were
 
on
 
server
 
the
 
whole
 
game:
\n
 
 
gameDuration
 
=
 
Level
.
TimeSeconds
 
-
 
timeGameStarted
;

\n
 
 
timeInGame
 
=
 
Level
.
TimeSeconds
 
-
 
p
.
PlayerReplicationInfo
.
StartTime
;

\n
 
 
/
/
 
I
 
don
'
t
 
know
 
if
 
this
 
will
 
ever
 
happen,
 
but
 
I
 
was
 
thinking
 
in
 
tournament
 
mode,
 
we
 
don
'
t
 
want
 
to
 
calculate
 
a
 
player
 
as
 
having
 
played
 
for
 
24
 
minutes
 
if
 
they
 
were
 
waiting
 
4
 
minutes
 
for
 
the
 
game
 
to
 
start
 
:P
\n
 
 
if
 
(timeInGame
>
gameDuration)
\n
 
 
 
 
timeInGame
 
=
 
gameDuration
;

\n
 
 
if
 
(bExtraDebugLogging)
 
{
 
Log(
"
AutoTeamBalance
.
UpdateStatsForPlayer(p)
 
timeInGame
=
"
$timeInGame$
"
 
gameDuration
=
"
$gameDuration$
"
 
Level
.
Game
.
StartTime
=
"
$Level
.
Game
.
StartTime$
"
 
Level
.
TimeSeconds
=
"
$Level
.
TimeSeconds$
"
"
)
;
 
}
\n
 
 
/
/
 
Well
 
if
 
this
 
player
 
was
 
only
 
in
 
the
 
server
 
for
 
5
 
minutes,
 
we
 
could
 
multiply
 
his
 
score
 
up
 
so
 
that
 
he
 
gets
 
a
 
score
 
proportional
 
to
 
the
 
other
 
players
.
 
 
(Ofc
 
if
 
he
 
was
 
lucky
 
or
 
unlucky,
 
that
 
luck
 
will
 
be
 
magnified
.
)
\n
 
 
if
 
(timeInGame
 
<
 
60)
 
{
 
/
/
 
The
 
player
 
has
 
been
 
in
 
the
 
game
 
for
 
less
 
than
 
1
 
minute
.

\n
 
 
 
 
Log(
"
AutoTeamBalance
.
UpdateStatsForPlayer(
"
$p$
"
)
 
Not
 
updating
 
this
 
player
 
since
 
his
 
timeInGame
 
"
$timeInGame$
"
 
<
 
60s
.
"
)
;

\n
 
 
 
 
return
;

\n
 
 
}
\n
 
 
new_hours_played
 
=
 
hours_played[i]
 
+
 
(Float(timeInGame)
 
/
 
60
 
/
 
60)
;

\n

\n
 
 
previous_average
 
=
 
avg_score[i]
;

\n

\n
 
 
/
*
 
I
 
never
 
tried
 
this,
 
but
 
the
 
method
 
below
 
is
 
superior
 
because
 
it
'
s
 
simpler!
\n
 
 
if
 
(bDoWeightedUpdates)
 
{
\n

\n
 
 
 
 
weightScore
 
=
 
Float(gameDuration)
 
/
 
Float(timeInGame)
;

\n
 
 
 
 
/
/
 
Let
'
s
 
weight
 
the
 
scores
 
more,
 
so
 
that
 
instead
 
of
 
becoming
 
score-per-endgame
 
it
 
becomes
 
score-per-hour
 
(in
 
case
 
this
 
was
 
a
 
short
 
game
 
with
 
low
 
frags,
 
or
 
overtime
 
with
 
many
 
frags)
.

\n
 
 
 
 
weightScore
 
=
 
weightScore
 
*
 
60
 
*
 
60
 
/
 
Float(gameDuration)
 
/
 
4
;
 
/
/
 
I
'
m
 
dividing
 
by
 
4
 
here
 
to
 
make
 
it
 
score-per-quarter-hour,
 
which
 
should
 
be
 
close
 
to
 
actual
 
end-game
 
scores,
 
at
 
least
 
on
 
my
 
15minute
 
game
 
server
.

\n
 
 
 
 
previousPolls
 
=
 
hours_played[i]
 
/
 
4
;
 
 
/
/
 
This
 
is
 
approx
 
#times
 
we
'
ve
 
updated
 
this
 
player
'
s
 
stats
 
before
 
(since
 
my
 
server
 
usually
 
has
 
15
 
minute
 
games)
.
 
 
But
 
it
'
s
 
not
 
too
 
bad
 
if
 
your
 
server
 
is
 
different
.
 
 
It
'
s
 
just
 
used
 
to
 
measure
 
the
 
significance
 
of
 
their
 
current
 
score
 
relative
 
to
 
the
 
number
 
of
 
scores
 
we
'
ve
 
seen
 
before
 
from
 
this
 
player
.
 
 
Servers
 
with
 
longer
 
game-times
 
will
 
make
 
new
 
scores
 
slightly
 
less
 
significant
.

\n
 
 
 
 
if
 
(MaxPollsBeforeRecyclingStrength
>
0
 
&&
 
previousPolls
 
>
 
MaxPollsBeforeRecyclingStrength)
 
{
\n
 
 
 
 
 
 
previousPolls
 
=
 
MaxPollsBeforeRecyclingStrength
 
-
 
1
;

\n
 
 
 
 
}
\n
 
 
 
 
Log(
"
AutoTeamBalance
.
UpdateStatsForPlayer(p)
 
[
"
$i$
"
]
 
"
$p
.
getHumanName()$
"
 
avg_score
 
=
 
(
 
(
"
$avg_score[i]$
"
 
*
 
"
$previousPolls$
"
)
 
+
 
"
$current_score$
"
*
"
$weightScore$
"
)
 
/
 
"
$(previousPoll
s+
1))
;

\n
 
 
 
 
/
/
 
avg_score[i]
 
=
 
(
 
(avg_score[i]
 
*
 
previousPolls)
 
+
 
current_score*weightScore)
 
/
 
(previousPoll
s+
1)
;

\n

\n
 
 
}
 
else
 
{
\n
 
 
*
/

\n

\n
 
 
/
/
 
TODO
 
CONSIDER:
 
should
 
we
 
give
 
bonus
 
points
 
for
 
being
 
on
 
the
 
winning
 
team
\n
 
 
/
/
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
otherwise
 
we
 
might
 
get
 
a
 
group
 
of
 
good
 
team
 
players
 
who
 
often
 
have
 
low
 
scores
 
but
 
their
 
team
 
always
 
wins
.

\n
 
 
/
/
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
 
well
 
that
'
s
 
unlikely,
 
due
 
to
 
SmartCTFStats
 
bonuses
\n

\n
 
 
/
/
 
Mmm
 
we
 
can
 
forget
 
all
 
the
 
weird
 
weighting
 
and
 
just
 
update
 
the
 
player
'
s
 
average_score_per_hour:
\n
 
 
Log(
"
AutoTeamBalance
.
UpdateStatsForPlayer(p)
 
[
"
$i$
"
]
 
"
$p
.
getHumanName()$
"
 
avg_score
 
=
 
(
 
(
"
$avg_score[i]$
"
 
*
 
"
$hours_played[i]$
"
)
 
+
 
"
$current_score$
"
/
4
.
0)
 
/
 
"
$(new_hours_played))
;

\n
 
 
avg_score[i]
 
=
 
(
 
(avg_score[i]
 
*
 
hours_played[i])
 
+
 
current_score
/
4
.
0)
 
/
 
new_hours_played
;
 
/
/
 
I
'
m
 
dividing
 
every
 
score
 
here
 
by
 
4
 
so
 
that
 
the
 
actual
 
averages
 
stored
 
in
 
the
 
config
 
will
 
be
 
score-per-quarter-hour,
 
which
 
should
 
be
 
close
 
to
 
actual
 
end-game
 
scores,
 
at
 
least
 
on
 
my
 
15minute
 
game
 
server
.
 
 
Just
 
makes
 
it
 
easier
 
to
 
read
.

\n
 
 
/
/
 
We
 
don
'
t
 
need
 
to
 
worry
 
about
 
how
 
long
 
he
 
spent
 
on
 
the
 
server
 
wrt
 
other
 
players,
 
or
 
how
 
long
 
the
 
game
 
was
.

\n

\n
 
 
/
/
 
}
\n

\n
 
 
hours_played[i]
 
=
 
new_hours_played
;

\n

\n
 
 
if
 
(bBroadcastCookies)
 
{
\n
 
 
 
 
if
 
(avg_score[i]
>
previous_averag
e+
1)
 
{
\n
 
 
 
 
 
 
BroadcastMessageAndLog(
"
"
$
 
p
.
getHumanName()
 
$
"
 
has
 
earned
 
"
$
 
Int(avg_score[i]-previous_average)
 
$
"
 
cookies!
"
)
;

\n
 
 
 
 
 
 
p
.
ClientMessage(
"
You
 
earned
 
"
$
 
Int(avg_score[i]-previous_average)
 
$
"
 
this
 
game
.
"
,
 
'
CriticalEvent
'
,
 
True)
;

\n
 
 
 
 
}
\n
 
 
 
 
else
 
if
 
(previous_average
>
avg_score[i]+1
 
&&
 
bBroadcastLostCookies)
 
{
\n
 
 
 
 
 
 
BroadcastMessageAndLog(
"
"
$
 
p
.
getHumanName()
 
$
"
 
has
 
lost
 
"
$
 
Int(previous_average-avg_score[i])
 
$
"
 
cookies
.
"
)
;

\n
 
 
 
 
 
 
p
.
ClientMessage(
"
You
 
lost
 
"
$
 
Int(previous_average-avg_score[i])
 
$
"
 
cookies
 
this
 
game
.
"
,
 
'
CriticalEvent
'
,
 
True)
;

\n
 
 
 
 
}
\n
 
 
}
\n
}
\n

\n
/
/
 
Normalises
 
a
 
player
'
s
 
score
 
so
 
that
 
average
 
of
 
all
 
scores
 
on
 
server
 
will
 
be
 
50
.

\n
/
/
 
This
 
is
 
to
 
fix
 
the
 
problem
 
that
 
some
 
games
 
(e
.
g
.
 
2v2
 
w00t
 
or
 
PureAction
 
or
 
iG)
 
have
 
much
 
higher
 
scores
 
than
 
others,
 
which
 
will
 
confuse
 
the
 
stats
.

\n
/
/
 
Now
 
your
 
stats
 
are
 
changed
 
by
 
about
 
the
 
same
 
amount
 
for
 
each
 
map,
 
regardless
 
of
 
your
 
actual
 
frags,
 
but
 
relative
 
to
 
the
 
other
 
players
 
on
 
the
 
server
.

\n
function
 
float
 
NormaliseScore(float
 
score)
 
{
\n
 
 
local
 
Pawn
 
p
;

\n
 
 
local
 
int
 
playerCount
;

\n
 
 
local
 
float
 
averageGameScore
;

\n

\n
 
 
/
/
 
Kinda
 
inefficient
 
to
 
calculate
 
the
 
average
 
once
 
for
 
every
 
player,
 
but
 
who
 
cares?
 
:P
\n
 
 
/
/
 
Mmm
 
well
 
maybe
 
it
 
could
 
cause
 
some
 
confusions
 
if
 
a
 
player
 
leaves
 
during
 
the
 
updating,
 
but
 
that
'
s
 
unlikely
.

\n
 
 
averageGameScore
 
=
 
0
.
0
;

\n
 
 
for
 
(p
=
Level
.
PawnList
;
 
p!
=
None
;
 
p
=
p
.
NextPawn)
 
{
\n
 
 
 
 
if
 
(p
.
bIsPlayer
 
&&
 
!p
.
IsA(
'
Spectator
'
)
 
&&
 
!p
.
IsA(
'
Bot
'
)
 
&&
 
p
.
IsA(
'
PlayerPawn
'
)
 
&&
 
p
.
bIsHuman)
 
{
 
/
/
 
lol
\n
 
 
 
 
 
 
averageGameScore
 
+
=
 
PlayerPawn(p)
.
PlayerReplicationInfo
.
Score
;

\n
 
 
 
 
 
 
playerCoun
t+
+
;

\n
 
 
 
 
}
\n
 
 
}
\n
 
 
averageGameScore
 
=
 
averageGameScore
 
/
 
Float(playerCount)
;

\n

\n
 
 
/
/
 
Avoid
 
division-by-zero
 
error
 
here
.
 
 
You
 
guys
 
got
 
average
 
<
2
 
frags?
 
 
Screw
 
you
 
I
'
m
 
not
 
scaling
 
that
 
up
 
to
 
50!
\n
 
 
if
 
(averageGameScore
 
<
 
2
.
0)
 
{
\n
 
 
 
 
averageGameScore
 
=
 
50
;

\n
 
 
}
\n

\n
 
 
if
 
(bExtraDebugLogging)
 
{
 
Log(
"
Normalising
 
from
 
average
 
"
$averageGameScore$
"
:
 
"
$score$
"
"
)
;
 
}
\n

\n
 
 
return
 
score
 
*
 
50
 
/
 
averageGameScore
;
 
/
/
 
TODO:
 
i
 
think
 
this
 
50
 
should
 
be
 
scaled
 
if
 
the
 
game
 
was
 
longer
/
shorter
 
than
 
usual
 
 
fewer
 
points
 
for
 
shorter
 
games?
 
 
remember
 
their
 
relative
 
score
 
will
 
be
 
scaled
 
up
 
by
 
their
 
time,
 
so
 
is
 
the
 
scale
 
really
 
needed?
 
 
:o
 
 
Mmm
 
I
 
conclude
 
we
 
don
'
t
 
need
 
to
 
scale
 
the
 
50
.

\n

\n
}
\n

\n
/
/
 
Takes
 
everything
 
before
 
the
 
first
 
"
:
"
 
-
 
you
 
should
 
almost
 
always
 
use
 
this
 
when
 
getting
 
PlayerPawn
.
GetPlayerNetworkAddress()
;
 
at
 
least
 
in
 
my
 
experience
 
the
 
client
'
s
 
port
 
number
 
changed
 
frequently
.

\n
function
 
string
 
stripPort(string
 
ip_and_port)
 
{
\n
 
 
return
 
Left(ip_and_port,InStr(ip_and_port,
"
:
"
))
;

\n
}
\n

\n
/
/
 
Code
 
snippet
 
from
 
advwaad,
 
taken
 
for
 
interest:
\n
/
/
 
for(P
=
level
.
pawnlist
;
 
P!
=
none
 
;
 
P
=
P
.
nextpawn)
\n
/
/
 
if(
 
P
.
IsA(
'
TournamentPlayer
'
)
 
||
 
P
.
IsA(
'
Bot
'
)
 
)
\n
/
/
 
if(
 
P
.
PlayerReplicationInfo
.
Team
 
=
=
 
0
 
)
\n
/
/
 
Re
d+
+
;

\n
/
/
 
else
 
if(
 
P
.
PlayerReplicationInfo
.
Team
 
=
=
 
1
 
)
\n
/
/
 
Blu
e+
+
;

\n

\n
/
/
 
which
 
team
 
is
 
bigger?
\n
/
/
 
local
 
TournamentGameReplicationInfo
 
TGRI
;

\n
/
/
 
TGRI
 
=
 
TournamentGameReplicationInfo(Level
.
Game
.
GameReplicationInfo)
;

\n
/
/
 
if
 
(
 
TGRI
 
!
=
 
none)
\n
/
/
 
{
\n
/
/
 
if
 
(TGRI
.
Teams[0]
 
>
 
TGRI
.
Teams[1])
\n
/
/
 
/
/
Do
 
something
.
.
.
.
.
.
.
Red
 
Team
 
>
 
Blue
 
Team
\n
/
/
 
if
 
(TGRI
.
Teams[0]
 
<
 
TGRI
.
Teams[1])
\n
/
/
 
/
/
Do
 
something
.
.
.
.
.
.
.
Blue
 
Team
 
>
 
Red
 
Team
\n
/
/
 
}
\n

\n
/
/
 
MORE
 
TESTING:
\n

\n
/
/
 
Matt
'
s
 
method
 
of
 
catch
 
player
 
joined
 
/
 
left
 
game
 
events:
\n
/
/
 
Mutator
 
broadcast
 
message
 
is
 
called
 
when
 
the
 
server
 
broadcasts
 
out
\n
/
/
 
UT
 
will
 
look
 
for
 
this
 
function,
 
and
 
then
 
you
 
can
 
do
 
whatever
 
you
 
want
\n
/
/
 
After
 
your
 
stuff
 
is
 
done,
 
then
 
it
 
has
 
to
 
pass
 
on
 
the
 
message
 
to
\n
/
/
 
the
 
next
 
mutator
 
in
 
line,
 
so
 
that
 
it
 
can
 
then
 
do
 
it
'
s
 
stuff
 
too
\n
/
/
 
nogginBasher:
 
this
 
seemed
 
to
 
be
 
suppressing
 
the
 
broadcasts
 
on
 
the
 
client
'
s
 
screens
 
(same
 
as
 
adwvaad),
 
so
 
at
 
the
 
end
 
I
'
m
 
calling
 
BroadcastMessage
 
to
 
send
 
them
 
out
 
again
.
 
 
Infinite
 
loop?
 
 
We
'
ll
 
see
.
.
.
 
^^
\n
/
/
 
Well
 
that
 
didn
'
t
 
work
 
either
.
 
 
:(
\n
/
/
 
So
 
I
'
m
 
gonna
 
take
 
it
 
out
 
entirely
\n
/
*
\n
function
 
bool
 
MutatorBroadcastMessage(
 
Actor
 
Sender,
 
Pawn
 
Receiver,
 
out
 
coerce
 
string
 
Msg,
 
optional
 
bool
 
bBeep,
 
out
 
optional
 
name
 
Type
 
)
 
{
\n

\n
 
 
CheckGameEnd()
;
 
/
/
 
Does
 
no
 
harm
 
to
 
do
 
this
 
twice
.
 
 
The
 
broadcast
 
from
 
mapvote
 
might
 
make
 
the
 
stats
 
parsing
 
come
 
sooner
 
than
 
waiting
 
for
 
the
 
timer
.

\n

\n
 
 
if
 
(bExtraDebugLogging)
 
{
 
/
/
 
TESTING
 
I
 
want
 
to
 
see
 
if
 
we
 
can
 
detect
 
a
 
player
 
saying
 
"
!teams
"
 
this
 
way
.
.
.
 
Answer:
 
no!
 
For
 
that
 
we
 
need
 
a
 
MessagingSpectator
\n
 
 
 
 
Log(
"
AutoTeamBalance
.
MutatorBroadcastMessage(
\
\
"
"
$Msg$
"
\
\
"
)
 
was
 
called
.
"
)
;

\n
 
 
}
\n

\n
 
 
/
/
 
Failed
 
Test:
 
Nope
 
that
 
was
 
never
 
broadcast
 
:P
\n
 
 
/
/
 
if
 
(
 
InStr(Msg,
"
game
 
has
 
ended
.
"
)
>
=
0
 
)
 
{
\n
 
 
 
 
/
/
 
Log(
"
AutoTeamBalance
.
MutatorBroadcastMessage(
\
\
"
"
$Msg$
"
\
\
"
)
 
detected
 
\
\
"
game
 
has
 
ended
.
\
\
"
 
-
 
DONE
 
elsewhere:
 
run
 
UpdateStatsAtEndOfGame()
 
here
.
"
)
;

\n
 
 
/
/
 
}
\n

\n
 
 
/
/
 
if
 
(Level
.
Game
.
bGameEnded)
 
{
\n
 
 
 
 
/
/
 
Log(
"
AutoTeamBalance
.
MutatorBroadcastMessage(
\
\
"
"
$Msg$
"
\
\
"
)
 
detected
 
Level
.
Game
.
bEnded
 
=
 
True
 
-
 
could
 
run
 
UpdateStatsAtEndOfGame()
 
here
.
"
)
;

\n
 
 
/
/
 
}
\n

\n
 
 
/
/
 
if
 
(
 
InStr(Msg,
"
 
entered
 
the
 
game
.
"
)
>
=
0
 
)
 
{
 
/
/
 
Can
 
we
 
find
 
the
 
new
 
player
 
pawn,
 
and
 
report
 
his
 
#cookies
 
?
\n
 
 
 
 
/
/
 
PlayerJoinedShowInfo(Msg)
;

\n
 
 
 
 
/
/
 
OK
 
well
 
that
 
didn
'
t
 
work,
 
apparently
 
the
 
player
 
didn
'
t
 
have
 
an
 
IP
 
address
 
when
 
he
 
first
 
joined
 
the
 
server
.
 
 
(Maybe
 
he
 
gets
 
assigned
 
one
 
once
 
this
 
stack
 
is
 
returned
.
)
\n
 
 
/
/
 
}
\n
 
 
/
/
 
if
 
(
 
InStr(Msg,
"
left
 
the
 
game
.
"
)
>
=
0
 
)
 
{
\n

\n
 
 
if
 
(
 
NextMessageMutator
 
!
=
 
None
 
)
 
{
\n
 
 
 
 
return
 
NextMessageMutator
.
MutatorBroadcastMessage(
 
Sender,
 
Receiver,
 
Msg,
 
bBeep,
 
Type
 
)
;

\n
 
 
}
 
else
 
{
\n
 
 
 
 
return
 
false
;

\n
 
 
}
\n

\n
 
 
BroadcastMessage(Msg)
;

\n

\n
}
\n
*
/

\n

\n
/
*
\n
function
 
PlayerJoinedShowInfo(string
 
Msg)
 
{
\n
 
 
local
 
int
 
i
;

\n
 
 
local
 
String
 
nick
;

\n
 
 
local
 
Pawn
 
p
;

\n
 
 
/
/
 
Extract
 
player
'
s
 
nick
 
from
 
the
 
game
 
broadcast
 
message,
 
e
.
g
.
:
 
"
nogginBasher
 
entered
 
the
 
game
.
"

\n
 
 
i
 
=
 
Instr(Msg,
"
 
entered
 
the
 
game
.
"
)
;

\n
 
 
nick
 
=
 
Left(Msg,i)
;

\n
 
 
/
/
 
Find
 
player
 
in
 
current
 
game
 
list
\n
 
 
for
 
(p
=
Level
.
PawnList
;
 
p!
=
None
;
 
p
=
p
.
NextPawn)
 
{
\n
 
 
 
 
if
 
(p
.
bIsPlayer
 
&&
 
!p
.
IsA(
'
Spectator
'
)
 
&&
 
!p
.
IsA(
'
Bot
'
)
 
&&
 
p
.
IsA(
'
PlayerPawn
'
)
 
&&
 
p
.
bIsHuman)
 
{
\n
 
 
 
 
 
 
if
 
(p
.
getHumanName()
 
=
=
 
nick)
 
{
\n
 
 
 
 
 
 
 
 
break
;
 
/
/
 
Found
 
him!
\n
 
 
 
 
 
 
}
\n
 
 
 
 
}
\n
 
 
}
\n
 
 
if
 
(p
 
=
=
 
None)
 
{
\n
 
 
 
 
Log(
"
AutoTeamBalance
.
PlayerJoinedShowInfo():
 
could
 
not
 
find
 
"
$nick$
"
 
in
 
current
 
game
.
"
)
;

\n
 
 
 
 
return
;

\n
 
 
}
\n
 
 
i
 
=
 
FindPlayerRecord(PlayerPawn(p))
;

\n
 
 
if
 
(bBroadcastCookies)
 
{
 
BroadcastMessageAndLog(nick$
"
 
has
 
"
$Int(avg_score[i])$
"
 
cookies
 
after
 
"
$hours_played[i]$
"
 
hours
 
on
 
the
 
server
.
"
)
;
 
}
\n
}
\n
*
/

\n

\n
/
/
 
I
 
want
 
to
 
Log
 
all
 
calls
 
to
 
BroadcastMessage()
 
so
 
that
 
I
 
can
 
see
 
without
 
playing
 
how
 
much
 
the
 
players
 
are
 
getting
 
spammed
 
by
 
broadcasts
.

\n
function
 
BroadcastMessageAndLog(string
 
Msg)
 
{
\n
 
 
Log(
"
AutoTeamBalance
 
Broadcasting:
 
"
$Msg)
;

\n
 
 
BroadcastMessage(Msg)
;

\n
}
\n

\n
