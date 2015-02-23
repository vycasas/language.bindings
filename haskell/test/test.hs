import Data.Maybe

import qualified HaskellModule.Library as HMLibrary
import qualified HaskellModule.Address as HMAddress
import qualified HaskellModule.Person as HMPerson

main :: IO ()
main = do
    HMLibrary.initialize
    versionStr <- HMLibrary.getVersionString
    putStrLn $ "Library initialized... version " ++ versionStr

    putStrLn "Creating a new address..."
    (maybeAddress, createAddressResult) <- HMAddress.create 9898 "Corner St." "Gotham" "CA" "Antartica" "4132"
    if createAddressResult /= 0 || maybeAddress == Nothing
        then do
            putStrLn $ "An error has occurred. Code: " ++ (show createAddressResult)
        else do
            let address = fromJust maybeAddress :: HMAddress.Address
            putStrLn "New address created!"
            addressStr <- HMAddress.asString address
            putStrLn $ "Address:\n" ++ addressStr

            putStrLn "Creating a new person..."
            (maybePerson, createPersonResult) <- HMPerson.create "Wayne" "Bruce" 28 address
            if createPersonResult /= 0 || maybePerson == Nothing
                then do
                    putStrLn $ "An error has occurred. Code: " ++ (show createPersonResult)
                else do
                    let person = fromJust maybePerson :: HMPerson.Person
                    putStrLn "New person created!"
                    personStr <- HMPerson.asString person
                    putStrLn $ "Person:\n" ++ personStr
                    
                    HMPerson.destroy person
                    return () -- note: This only completes the statement and doesn't end the expression!

            HMAddress.destroy address
            return ()

    HMLibrary.terminate
    return ()
