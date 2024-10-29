module MyUtils
( parseArgs
, openInput
, printAutomat
, parseAutomat
, getThirds
, third
, nubSort
, getJustSeconds

) where

import System.IO ( openFile, IOMode(ReadMode), stdin, Handle )
import Types ( Automat(..) )
import Data.List ( nub, sort )
import Data.Char ( isLower )
import Data.Maybe (fromJust)

-- parses arguments into triplet
parseArgs :: [String] -> (Bool, Bool, String)
parseArgs xs =
    let i = findArg "-i" xs
        t = findArg "-t" xs
        file = findFileName xs
    in (i, t, file)

-- return True if string is found in array of strings (i.e. -i or -t)
findArg :: String -> [String] -> Bool
findArg _ [] = False
findArg a (x:xs) =
    a == x || findArg a xs

-- finds argument that is not -i or -t and returns it. Returns empty array if nothing is found
findFileName :: [String] -> String
findFileName [] = ""
findFileName (x:xs) =
    if x == "-i" || x == "-t"
        then findFileName xs
    else x

-- reads from stdin or from file
openInput :: [Char] -> IO Handle
openInput file =
    if file == ""
        then
            return stdin
    else do
        openFile file ReadMode

-- ! -- functions to parse automat -- ! --
parseAutomat :: [String] -> Automat
parseAutomat x = Automat {allStates=_allStates, albhabet=_albhabet, startState=_startState, endStates=_endStates, transitions=_transitions}
    where _allStates = nubSort (parseStates ( splitOnComma (head x) ))
          _albhabet = nubSort (parseAlbhabet (x!!1))
          _startState = parseStartState (x!!2)
          _endStates = nubSort (parseEndStates ( splitOnComma (x!!3) ))
          _transitions = nubSort (parseTrans ( drop 4 x ))

-- Takes string and cuts it into more string at comma
splitOnComma :: String -> [String]
splitOnComma x = words (replaceComma x)
    where
        replaceComma :: String -> String
        replaceComma [] = []
        replaceComma (y:ys)
            | y == ',' = ' ' : replaceComma ys
            | otherwise = y : replaceComma ys

-- Takes list of string of numbers, turns them into ints and create list of states
parseStates :: [String] -> [Int]
parseStates = map (\ x -> (read x :: Int))

-- Takes string of all symbols in alphabet and checks if they are in [a..z]
parseAlbhabet :: String -> String
parseAlbhabet [] = []
parseAlbhabet (x:xs)
    | x `elem` ['a'..'z'] = x : parseAlbhabet xs
    | otherwise = error "Invalid character in alphabet!"

-- Read Int from String as starting state
parseStartState :: String -> Int
parseStartState x = read x :: Int

-- alias for parseStates
parseEndStates :: [String] -> [Int]
parseEndStates = parseStates

-- parse input into tranition function representation
parseTrans :: [String] -> [(Int, Char, Int)]
parseTrans [] = []
parseTrans (x:xs) = lineToTriplet x : parseTrans xs
    where
        lineToTriplet y = (a,b,c)
            where a = read (head (splitOnComma y)) :: Int
                  b = if isLower ( head (splitOnComma y !! 1) ) then head (splitOnComma y !! 1) else error "Must be lower case!"
                  c = read (splitOnComma y !! 2) :: Int
---------------- ! --------------------


-- ! -- Function to printout automat -- ! --
printAutomat :: Automat -> IO ()
printAutomat a = do
    let allStatesString = intListToString "," (allStates a)
    let finalStateString = intListToString "," (endStates a)
    let transitionsString = transitionsListToString (transitions a)
    putStrLn allStatesString
    putStrLn (sort (albhabet a))
    print (startState a)
    putStrLn finalStateString
    putStr transitionsString

-- Param 1 -> delimetr
-- Param 2 -> list of ints
-- Return -> String of ints from Param 2 seperated with delimetr from Param 1
intListToString :: String -> [Int] -> String
intListToString _ [] = []
intListToString _ [x] = show x
intListToString d (x:xs) = show x ++ d ++ intListToString d xs

-- Param 1 -> List of transitions
-- Return -> Transitions transformet to printable string
transitionsListToString :: [(Int, Char, Int)] -> String
transitionsListToString [] = []
transitionsListToString ((a,b,c):xs) = show a ++ "," ++ [b] ++ "," ++ show c ++ "\n" ++ transitionsListToString xs
------------------ ! ---------------------

getJustSeconds :: [(a, Maybe b)] -> [b]
getJustSeconds = map (fromJust . snd)

getThirds :: [(a, b1, b2)] -> [b2]
getThirds = map third

third :: (a, b, c) -> c
third (_,_,x) = x

nubSort :: Ord a => [a] -> [a]
nubSort a = nub ( sort a)
