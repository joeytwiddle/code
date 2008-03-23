class FastBuildGI extends SiegeGI;

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
}

