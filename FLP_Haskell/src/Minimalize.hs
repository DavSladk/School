module Minimalize
( reduce
) where

import Data.List ( intersect )

import Types ( Automat(..) )
import MyUtils ( nubSort )

reduce :: Automat -> Automat
reduce x = Automat {allStates=_allStates, albhabet=_albhabet, startState=_startState, endStates=_endStates, transitions=_transitions}
    where _allStates = [0..length sortedEqClasses-1]
          _albhabet = albhabet x
          _startState = startEqClasses (startState x) sortedEqClasses 0
          _endStates = endEqClasses (endStates x) sortedEqClasses 0
          _transitions = nubSort (transformTransitions sortedEqClasses (transitions x))
          eqClasses = createEqClasses x
          sortedEqClasses = sortIntoEqClasses eqClasses

---- createEqClasses - START ----

-- Param 1 -> automat to reduce
-- Return -> states in equv relation
-- description: sets up starting state and runs loop
createEqClasses :: Automat -> [(Int, Int)]
createEqClasses x = nextEqClasses x _startingEqClasses
    where _startingEqClasses = startingEqClasses x

-- Param 1 -> automat
-- Param 2 -> equivivalet relation of states
-- Return -> equivivalet relation of states in minimal form
-- description: called with init state in Param 2. Recursion with next iteration.
nextEqClasses :: Automat -> [(Int, Int)] -> [(Int, Int)]
nextEqClasses a original
    | original == new = original
    | otherwise = nextEqClasses a new
    where new = [ (q,p) | (q,p) <- original, areInEqClass (forEverySymbol (q,p) (albhabet a) (transitions a) (allStates a)) original ]

-- creates init state for createEqClasses
startingEqClasses :: Automat -> [(Int, Int)]
startingEqClasses x = startEqClassA x ++ startEqClassB x
    where   startEqClassA :: Automat -> [(Int, Int)]
            startEqClassA y = [(p, q) |
                p <- allStates y,
                p `elem` endStates y,
                q <- allStates y,
                q `elem` endStates y]

            startEqClassB :: Automat -> [(Int, Int)]
            startEqClassB y = [(p, q) |
                p <- allStates y,
                p `notElem` endStates y,
                q <- allStates y,
                q `notElem` endStates y]

-- Param 1 -> states in a relation
-- Param 2 -> albhabet
-- Param 3 -> transitions
-- Param 4 -> all states
-- Return -> list of pair of state that can be reached from Param 1
forEverySymbol :: (Int, Int) -> String -> [(Int,Char,Int)] -> [Int] -> [(Int, Int)]
-- end if all symbols where used
forEverySymbol _ [] _ _= []
-- dive into loop with next next symbol
forEverySymbol (a,b) (x:xs) t as = r ++ forEverySymbol (a,b) xs t as
    -- pair (q, p)
    --      q <- all states
    --      p <- all states
    --      (Param 1, symbol, q) is in transitions
    --      (Param 1, symbol, p) is in transitions
    where r = [(q, p) | q <- as, (a, x, q) `elem` t, p <- as, (b, x, p) `elem` t]

-- searches if all pairs are in list
areInEqClass :: [(Int, Int)] -> [(Int, Int)] -> Bool
areInEqClass [] _ = False
areInEqClass [q] x = isInEqClass q x
areInEqClass (q:qs) x = isInEqClass q x && areInEqClass qs x

-- searches if pair is in list
isInEqClass :: (Int, Int) -> [(Int, Int)] -> Bool
isInEqClass q x = q `elem` x

---- createEqClasses - END ----

-- Param 1 -> list of all pairs that are in eq relation
-- Return -> list of list that are classes but only with states
-- description: start condition
sortIntoEqClasses :: [(Int,Int)] -> [[Int]]
sortIntoEqClasses x = nubSort ( findEqClassesForEach t x )
    where t = [ (q,p) | (q,p) <- x, q==p ]

-- Param 1 -> pairs (a,b) where a == b
-- Param 2 -> list of all pairs
-- Return -> list of list that are classes but only with states
-- description: iterate for each pair
findEqClassesForEach :: [(Int,Int)] -> [(Int,Int)] -> [[Int]]
findEqClassesForEach [] _ = []
findEqClassesForEach ((a,_):pairs) x = getAllStateInEqTo [a] x : findEqClassesForEach pairs x

-- Param 1 -> list of states to look for
-- Param 2 -> all pairs
-- 
getAllStateInEqTo :: [Int] -> [(Int,Int)] -> [Int]
getAllStateInEqTo x p
    | x == e = x
    | otherwise = getAllStateInEqTo e p
    where e = nubSort ( [ m | m <- map fst d] ++ [ n | n <- map snd d] )
          d = [ (a,b) | (a,b) <- p , a `elem` x || b `elem` x]

-- Param 1 -> original start state
-- Param 2 -> original states sorted into eq classes
-- Param 3 -> counter
-- Return -> In what eq class is starting state
startEqClasses :: Int -> [[Int]] -> Int -> Int
startEqClasses _ [] _ = error "Starting Eq class not found!"
startEqClasses a (b:bs) i
    | a `elem` b = i
    | otherwise = startEqClasses a bs (i+1)

-- Param 1 -> original end states
-- Param 2 -> original states sorted into eq classes
-- Param 3 -> counter
-- Returns -> index of all eq classes that has original end state in it
-- creates new end states for minimal automat
endEqClasses :: [Int] -> [[Int]] -> Int -> [Int]
endEqClasses _ [] _ = []
endEqClasses a (b:bs) i
    | a `intersect` b /= [] = i : endEqClasses a bs (i+1)
    | otherwise = endEqClasses a bs (i+1)

-- Param 1 -> list of eq classes
-- Param 2 -> list of original transitions
-- Return -> list of transformed transitions
transformTransitions :: [[Int]] -> [(Int, Char, Int)] -> [(Int, Char, Int)]
transformTransitions _ [] = []
transformTransitions x (t:ts) = transformClinic x t : transformTransitions x ts

-- Param 1 -> List of eq classes
-- Param 2 -> transition to transform
-- Return -> transformed transition
transformClinic :: [[Int]] -> (Int, Char, Int) -> (Int, Char, Int)
transformClinic [] _ = error "Transofrm clinic run out of doctors!"
transformClinic x (a,b,c) = (a',b,c')
    where a' = findInWhatEqClassIsThisState a x 0
          c' = findInWhatEqClassIsThisState c x 0

-- Param 1 -> a state to loo for
-- Param 2 -> eq classes
-- Param 3 -> counter
-- Return -> in what eq class is Param 1
findInWhatEqClassIsThisState :: Int -> [[Int]] -> Int -> Int
findInWhatEqClassIsThisState _ [] _ = error "State not in any eq class!"
findInWhatEqClassIsThisState s (e:eq) i
    | s `elem` e = i
    | otherwise = findInWhatEqClassIsThisState s eq (i+1)
