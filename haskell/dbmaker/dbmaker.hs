type Field = (Type,String)

data Type = BaseType String
					| UserType String [Type] [Field]
					deriving Eq

eq (UserType s h fs) (UserType s2 h2 fs2) = True
eq (BaseType s) (BaseType s2) = True



baseTypes = [ string, int, float, date ]

string = BaseType "java.lang.String"
int = BaseType "int"
float = BaseType "float"
date = BaseType "java.util.Date"



userTypes = [ email, threadbit, address, eperson, gperson ]

email = UserType "email.Email" [] [ (address,"from"), (address,"to"), (date,"date"), (string,"subject"), (string,"body") ]

threadbit = UserType "email.ThreadBit" [] [ (email,"referredTo"), (email,"refersTo") ]

address = UserType "email.Address" [] [ (eperson,"person"), (string,"address") ]

eperson = UserType "email.Person" [gperson] [ (string,"nickName") ]

gperson = UserType "general.Person" [] [ (string,"firstName"), (string,"lastName"), (string,"middleName") ]



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
								putStr "Fields: "
								putStrLn ( displayList (map displayField (allFieldsIn t)) )
								putStrLn ""
								putStr "Friends: "
								putStrLn ( displayList (map displayFriend (objsContaining t)) )
  where (UserType name inherits fields) = t



displayList = foldl inbet ""

inbet "" b = b
inbet a b = a ++ ", " ++ b

displayField (UserType name i fs,fn) = name ++ " " ++ fn
displayField (BaseType name,fn) = name ++ " " ++ fn

displayFriend (userTypeName,fieldName) = "[" ++ userTypeName ++ "] " ++ fieldName ++ "()"

allFieldsIn (UserType name inherits fields) = fields ++ otherfields
	where otherfields = foldl (++) [] (map allFieldsIn inherits)

objsContaining userType = checkObjs userType userTypes

checkObjs :: Type -> [Type] -> [(String,String)]
checkObjs _ [] = []
checkObjs seekType (UserType name i fs : rest) =
	(checkFields seekType name fs) ++ (checkObjs seekType rest)

checkFields _ _ [] = []
checkFields seekType objName (f:fs) =
	if (fieldType == seekType) then
		(objName,fieldName) : rest
	else
		rest
  where (fieldType,fieldName) = f
        rest = checkFields seekType objName fs
