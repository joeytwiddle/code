
~ FixWeaponBalance (3rd generation)

  Allows admin to tweak the damage dealt by various weapons.  Now applies
  changes *before* armor absorption.  As a mutator, it should work for a
  variety of gametypes, provided they process damage in the normal way.

  Admins who are logged in can try "mutate help".  The mod can be toggled with
  "mutate fwb" and display live damage changes with "mutate fwbreport".
  A summary of the current configuration is available on "mutate status".

  There is a new feature bScaleSniperByDistance, which can reduce or increase
  sniper damage according to distance-to-target.  Within SniperPowerRange the
  sniper does full or above normal damage, but beyond that damage is reduced,
  halving in power every SniperHalflifeRange.  This is a very significant
  change to the weapon, but may offer some benefits for gameplay:

    - Stops rewarding long-distance campers.
    - Does not reward snipers who run away backwards from a fight.
    - DOES reward snipers who jump in close to make the kill!
    - It affectively turns the sniper into a close-range weapon, like most of
      the others!

  Note that adjusting the two range parameters mentioned will affect the
  maximum damage possible at point-blank-range.  Check "mutate status" to see
  how your changes affect the "point-blank-range damage scale", which is what
  the normal shot damage will be multiplied by if you are so close you are
  touching the other player.

  I have enabled 'bScaleSniperByDistance' by default, to encourage people to try
  it out.  Just set that config var to False if you don't want to use it.

  There are a number of identical mutators:

    FixWeaponBalance.FixWeaponBalance
    FixWeaponBalance.FixWeaponBalanceZP
    FixWeaponBalance.FixWeaponBalanceSiege

  which differ only in their default configuration.  So on a server with
  multiple game modes you can use different mutators to apply different
  settings.

  The defaults for FixWeaponBalance leave all weapons at 100% normal damage (so
  I hope you use PureStats for tickrate-independence!) except for the sniper
  which has reduced damage according to distance.

  The defaults for FixWeaponBalanceZP are intended for games with Zeroping and
  normal weapons.  They reduce pulse and minigun damage, to compensate for
  PureStats being unavailable.  There is also a fix for the crouching bug in
  ZeroPingPlus103.

  FixWeaponBalanceSiege contains defaults for zeroping non-hardcore Siege
  games.  Most projectile weapon damage is increased by 25% (semi-hardcore,
  compensate for strong hitscan weapons) whilst minugun and pulse are reduced
  (too powerful near superpros).  Enforcer, sniper and ripper primary are left
  unaffected.  Damage done to buildings is unaffected.

  If you think that's too much, try putting everything back to 100% but leaving
  in the sniper distance dropoff and crouch bug fix.  This will stop rewarding
  campers/retreaters and instead reward snipers who *move in for the kill*.

  Caveats:

    I do NOT recommend fixing SniperHeadshotMaxDamage or
    SniperBodyshotMaxDamage at e.g. 150 and 67, as this will also apply to
    *amped* shots, which is probably not what you want!

    Put the mutator early in the chain (before any other damage mutators), but
    *after* the ZeroPing mutator!

    There are two "ugly" bits of code.  The double-call to TakeDamage is needed
    to pick up the Instigator before applying damage, it's filthy but works!
    The bExtraCheckForPlayerSpawn is a workaround for a bug in Siege gametypes
    where ModifyPlayer() is not called.  This might be improved, so suggestions
    are welcome.

  NewNet/UTProMod is also attempting to fix various weapons with new codes:

    http://www.prounreal.org/forums/viewtopic.php?t=81140&postdays=0&postorder=asc&start=0&sid=831433b665e346683c2b24a6345777bd

  Alternatives?  Another idea to rebalance weapons might be to change their
  re-fire time.  That could be a separate mutator called WeaponsOverheat.

  Also see NerfAmmo...

  Here are the default configs:

[FixWeaponBalance.FixWeaponBalance]
bEnabled=True
bLogging=False
bAdvertise=True
bExtraCheckForPlayerSpawn=False
TriggerExtraSpawnCheckItem=enforcer
bUseMagicMethod=False
bAnyoneMayToggleReporting=False
bLiveReports=False
bReportDamageReceived=False
bReportUnchangedDamage=False
bKeepHeadshotsInAllModes=False
SniperHeadshotDamageScale=1.00
SniperBodyshotDamageScale=1.00
SniperHeadshotMaxDamage=9999
SniperBodyshotMaxDamage=9999
bScaleSniperByDistance=True
SniperHalflifeRange=1024.00
bCalcDamageScaleFromRange=True
SniperPowerRange=320.00
SniperMinDamage=12
bFixCrouchingBug=False
RocketDamageScale=1.00
RocketGrenadeDamageScale=1.00
BioDamageScale=1.00
EnforcerDamageScale=1.00
MinigunDamageScale=1.00
ShockPrimaryDamageScale=1.00
ShockBallDamageScale=1.00
FlakPrimaryDamageScale=1.00
FlakSecondaryDamageScale=1.00
PulseDamageScale=1.00
PulsebeamDamageScale=1.00
RipperPrimaryDamageScale=1.00
RipperSecondaryDamageScale=1.00
RipperHeadshotDamageScale=1.00
MeleeWeaponDamageScale=1.00
UnknownWeaponDamageScale=1.00
bAdjustBoost=True
bOnlyAdjustBoostDownwards=True

[FixWeaponBalance.FixWeaponBalanceZP]
bFixCrouchingBug=True
; Reduced power to compensate for zeroping
SniperPowerRange=256
MinigunDamageScale=0.78
PulsebeamDamageScale=0.78

[FixWeaponBalance.FixWeaponBalanceSiege]
bFixCrouchingBug=True
bExtraCheckForPlayerSpawn=True
; Raise sniper power (semi-hardcore)
SniperPowerRange=480
; Other weapons at 125% (semi-hardcore)
RocketDamageScale=1.25
RocketGrenadeDamageScale=1.25
BioDamageScale=1.25
EnforcerDamageScale=1.00
MinigunDamageScale=0.6
ShockPrimaryDamageScale=0.90
ShockBallDamageScale=1.15
FlakPrimaryDamageScale=1.25
FlakSecondaryDamageScale=1.25
PulseDamageScale=1.25
PulsebeamDamageScale=0.5
RipperPrimaryDamageScale=1.00
RipperSecondaryDamageScale=1.25
RipperHeadshotDamageScale=1.00
MeleeWeaponDamageScale=1.10

