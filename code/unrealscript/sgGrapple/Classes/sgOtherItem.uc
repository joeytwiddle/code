class sgOtherItem expands sgaItem;

// Right now we use this class to import some sounds, because we can't do this
// in .jpp files!
#exec AUDIO IMPORT FILE="..\Sounds\Pull.wav"   NAME="Pull"
#exec AUDIO IMPORT FILE="..\Sounds\greset.wav" NAME="Slurp"
#exec AUDIO IMPORT FILE="..\Sounds\hit1g.wav"  NAME="hit1g"

// But in the future, this class could be a template for 10 "spare" admin
// string-configurable custom items to add to the build options!

