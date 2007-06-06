class FRemoteUTBotConfigClient extends RemoteUTBotConfigClient;

// replaces UMenuBotConfigClientWindow's version
function LoadCurrentValues()
{
	local int i;

	BotConfig = Class'FriendlyBotAPI.FriendlyChallengeBotInfo';
	for(i=0;i<8;i++)
		Skills[i] = BotConfig.default.Skills[i];

	BaseCombo.SetSelectedIndex(Min(BotConfig.default.Difficulty, 7));

	TauntLabel.SetText(SkillTaunts[BaseCombo.GetSelectedIndex()]);

	AutoAdjustCheck.bChecked = BotConfig.Default.bAdjustSkill;
	RandomCheck.bChecked = BotConfig.Default.bRandomOrder;

	NumBotsEdit.SetValue(string(class<DeathMatchPlus>(BotmatchParent.GameClass).Default.MinPlayers));

	if(BalanceTeamsCheck != None)
		BalanceTeamsCheck.bChecked = class'TeamGamePlus'.Default.bBalanceTeams;

	if(DumbDownCheck != None)
		DumbDownCheck.bChecked = !class'Domination'.Default.bDumbDown;
}

