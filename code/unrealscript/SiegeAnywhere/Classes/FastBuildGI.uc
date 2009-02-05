class FastBuildGI extends SiegeGI config (SiegeAnywhere);

// TODO: It may be that the mutator is in the chain anyway, but we don't want to create 2.  Does the mutator itself prevent that?  If not, it or we should!

event InitGame(string options, out string error) {
	local Mutator m;
	m = Spawn(class'FastBuildMut');
	Log("FastBuildGI.InitGame(): Adding mutator "$m);
	Level.Game.BaseMutator.AddMutator(m);
	Super.InitGame(options,error);
}

function StartMatch() {
	local FastBuildMut m;
	Log("FastBuildGI.StartMatch()");
	Super.StartMatch();
	foreach AllActors(class'FastBuildMut',m) {
		if (!m.bGameStarted) {
			m.DoGameStart();
		}
	}
	Super.StartMatch();
}

