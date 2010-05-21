//================================================================================
// WallJumpUT.
//================================================================================

class WallJumpUT extends Mutator config (kxWallJump);

// var config bool bRestrictFC;
// var config int MaxJumps;
// var config int JumpType;
// var config float JumpHeight;
// var config float RechargeRate;

function ModifyPlayer (Pawn Other)
{
  local Class<WallJumpBoots> InventoryClass;
  local WallJumpBoots DJ_Inv;

  Super.ModifyPlayer(Other); // 0x00000014 : 0x0000
  if ( Other.IsA('Spectator') || (!Other.IsA('PlayerPawn') && !Other.IsA('Bot')) ) // 0x00000019 : 0x000B
  {
    return; // 0x0000003A : 0x0037
  }
  // InventoryClass = Class<WallJumpBoots>(DynamicLoadObject("kxWallJump.WallJumpBoots",Class'Class')); // 0x0000003C : 0x0039
  // InventoryClass = Class<WallJumpBoots>(DynamicLoadObject(String(class'WallJumpBoots'),Class'Class')); // 0x0000003C : 0x0039
  InventoryClass = class'WallJumpBoots';
  DJ_Inv = Spawn(Class'WallJumpBoots',Other,,Other.Location); // 0x00000066 : 0x006E
  if ( DJ_Inv != None ) {
    DJ_Inv.RespawnTime = 0.0; // 0x00000082 : 0x009B
    DJ_Inv.GiveTo(Other); // 0x00000091 : 0x00AF
    // DJ_Inv.MaxJumps = MaxJumps; // 0x0000009C : 0x00C3
    // DJ_Inv.JumpType = JumpType; // 0x000000A7 : 0x00D7
    // DJ_Inv.JumpHeight = JumpHeight; // 0x000000B2 : 0x00EB
    // DJ_Inv.RechargeRate = RechargeRate; // 0x000000B2 : 0x00EB
    // TODO parts missing!
  }
}

function Mutate (string MutateString, PlayerPawn Sender)
{
  local string MyMutateString;
  local int tempMaxJumps;
  local int tempJumpType;
  local float tempJumpHeight;

  // TODO: SaveConfig()?  Nah, for that let admin do "admin set" :P

  if ( Sender.PlayerReplicationInfo.bAdmin || (Level.NetMode == 0) ) // 0x00000014 : 0x0000
  {
    MyMutateString = MutateString; // 0x0000003B : 0x0034
    if ( Left(MyMutateString,12) ~= "WallJumpUT" ) // 0x00000040 : 0x003F
    {
      MyMutateString = Mid(MyMutateString,13); // 0x00000059 : 0x005B
      if ( Left(MyMutateString,8) ~= "MaxJumps" ) // 0x00000062 : 0x006A
      {
        tempMaxJumps = int(Mid(MyMutateString,9)); // 0x00000077 : 0x0082
        if ( tempMaxJumps != 0 ) // 0x00000081 : 0x0092
        {
          class'WallJumpBoots'.default.MaxJumps = tempMaxJumps; // 0x00000089 : 0x009D
          Sender.ClientMessage("Maximum number of jumps is now: " $ string(class'WallJumpBoots'.default.MaxJumps)); // 0x0000008E : 0x00A8
        } else { // 0x000000BA : 0x00DD
          Sender.ClientMessage("MaxJumps should be 1 or higher."); // 0x000000BD : 0x00E0
        }
      } else { // 0x000000E3 : 0x010C
        if ( Left(MyMutateString,8) ~= "JumpType" ) // 0x000000E6 : 0x010F
        {
          tempJumpType = int(Mid(MyMutateString,9)); // 0x000000FB : 0x0127
          switch (tempJumpType) // 0x00000105 : 0x0137
          {
            case 0: // 0x00000109 : 0x013E
            class'WallJumpBoots'.default.JumpType = 0; // 0x0000010D : 0x0142
            Sender.ClientMessage("Jumptype is now \"At apex\""); // 0x00000111 : 0x0149
            break; // 0x00000131 : 0x016F
            case 1: // 0x00000134 : 0x0172
            class'WallJumpBoots'.default.JumpType = 1; // 0x00000138 : 0x0176
            Sender.ClientMessage("Jumptype is now \"Going up and apex\""); // 0x0000013C : 0x017D
            break; // 0x00000166 : 0x01AD
            case 2: // 0x00000169 : 0x01B0
            class'WallJumpBoots'.default.JumpType = 2; // 0x0000016E : 0x01B5
            Sender.ClientMessage("Jumptype is now \"Always\""); // 0x00000173 : 0x01BD
            break; // 0x00000192 : 0x01E2
            default: // 0x00000195 : 0x01E5
            Sender.ClientMessage("Jumptype should be 0, 1 or 2."); // 0x00000198 : 0x01E8
            break; // 0x000001BC : 0x0212
          }
        } else { // 0x000001BF : 0x0215
          if ( Left(MyMutateString,10) ~= "JumpHeight" ) // 0x000001C2 : 0x0218
          {
            tempJumpHeight = float(Mid(MyMutateString,11)); // 0x000001D9 : 0x0232
            if ( tempJumpHeight >= 100 ) // 0x000001E4 : 0x0242
            {
              class'WallJumpBoots'.default.JumpHeight = tempJumpHeight / 100; // 0x000001EF : 0x024F
              Sender.ClientMessage("JumpHeight is now "$ class'WallJumpBoots'.default.JumpHeight); // 0x00000198 : 0x01E8
            // TODO
            // File ended here!

            }
          }
        }
      }
    }
  }
  Super.Mutate(MutateString,Sender);
}

defaultproperties
{
}
