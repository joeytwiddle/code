type Property = (Type,String)

data Type = BaseType String
					| Obj String [Type] [Property]
					deriving Eq

eq (Obj s h fs) (Obj s2 h2 fs2) = True
eq (BaseType s) (BaseType s2) = True



baseTypes = [ string, int, float, date ]

string = BaseType "java.lang.String"
int = BaseType "int"
float = BaseType "float"
date = BaseType "java.util.Date"



userTypes = [ email, threadbit, address, eperson, gperson ]

email = Obj "email.Email" [] [ (address,"from"), (address,"to"), (date,"date"), (string,"subject"), (string,"body") ]

threadbit = Obj "email.ThreadBit" [] [ (email,"referredTo"), (email,"refersTo") ]

address = Obj "email.Address" [] [ (eperson,"person"), (string,"address") ]

eperson = Obj "email.Person" [gperson] [ (string,"nickName") ]

gperson = Obj "general.Person" [] [ (string,"firstName"), (string,"lastName"), (string,"middleName") ]



go = showAllData userTypes

noop :: IO ()
noop = return ()

showAllData [] = noop
showAllData (h:t) = (showData h) >> (showAllData t)

showData t = do
								putStrLn ""
								putStrLn ""
								putStrLn ""
								putStrLn name
								putStrLn ""
								putStr "Propertys: "
								putStrLn ( displayList (map displayProperty (allPropertysIn t)) )
								putStrLn ""
								putStr "Friends: "
								putStrLn ( displayList (map displayFriend (objsContaining t)) )
  where (Obj name inherits propertys) = t



displayList = foldl inbet ""

inbet "" b = b
inbet a b = a ++ ", " ++ b

displayProperty (Obj name i fs,fn) = name ++ " " ++ fn
displayProperty (BaseType name,fn) = name ++ " " ++ fn

displayFriend (userTypeName,propertyName) = "[" ++ userTypeName ++ "] " ++ propertyName ++ "()"

allPropertysIn (Obj name inherits propertys) = propertys ++ otherpropertys
	where otherpropertys = foldl (++) [] (map allPropertysIn inherits)

objsContaining userType = checkObjs userType userTypes

checkObjs :: Type -> [Type] -> [(String,String)]
checkObjs _ [] = []
checkObjs seekType (Obj name i fs : rest) =
	(checkPropertys seekType name fs) ++ (checkObjs seekType rest)

checkPropertys _ _ [] = []
checkPropertys seekType objName (f:fs) =
	if (propertyType == seekType) then
		(objName,propertyName) : rest
	else
		rest
  where (propertyType,propertyName) = f
        rest = checkPropertys seekType objName fs
