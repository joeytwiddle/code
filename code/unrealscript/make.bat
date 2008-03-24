@echo off

del System\SiegeAnywhere.u
del System\SiegeAS.u
del System\FastBuildXXL2dRC1.u
: : del System\Screen.u
del System\ActorEditor.u
: del System\ScreenExtensions.u
del System\AutoTeamBalance.u
: del System\PubliciseScore.u
: : del System\TeamScreen.u
del System\nBAutoSpectate.u
del System\Resize.u
del System\ScreenSaver.u
del System\SniperRealism.u
del System\DynamicPackageLoader.u
del System\UTAlive.u
del System\PostBox.u

System\ucc make ini=..\compiling.ini
