module Canon
( toCanon
)where

import Data.List ( (\\) )
import Data.Maybe (isNothing, fromJust)

import Types ( Automat(..) )
import MyUtils ( getThirds, getJustSeconds, nubSort )

toCanon :: Automat -> Automat
toCanon a = Automat {allStates=_allStates, albhabet=_albhabet, startState=_startState, endStates=_endStates, transitions=_transitions}
    where _allStates = nubSort(getJustSeconds statesWithNewId)
          _albhabet = nubSort(albhabet a)
          _startState = fromJust (snd(getPairWithKey (startState a) statesWithNewId))
          _endStates = nubSort (getJustSeconds ( getPairsWithKey (endStates a) statesWithNewId))
          _transitions = nubSort (transformTransitions statesWithNewId (transitions a))
          statesWithNewId = doCannon startingList (transitions a)
          zipped = zip (allStates a) (repeat Nothing)
          startingList = giveStatesNewNumber [startState  a] 0 zipped

-- same as above, but does for multiple states
-- Param 1 -> original states
-- Param 2 -> new number for state
-- Param 3 -> list of states and their new numbers
-- Return -> modified list
giveStatesNewNumber :: [Int] -> Int -> [(Int, Maybe Int)] -> [(Int, Maybe Int)]
giveStatesNewNumber _ _ [] = []
giveStatesNewNumber o n (l:ls)
    | fst l `elem` o && isNothing (snd l) = (fst l, Just n) : giveStatesNewNumber o (n+1) ls
    | otherwise = l : giveStatesNewNumber o n ls

-- Param 1 -> current lowest
-- Param 2 -> list of pairs
-- Return -> lowest sedond from pairs
getLowestValue :: Int -> [(Int,Maybe Int)] -> Int
getLowestValue c [] = c
getLowestValue c (p:ps) = getLowestValue newMin ps
    where newMin = maybe c (min c) (snd p)

-- Param 1 -> value
-- Param 2 -> all pairs
-- Return -> pair with value
getPairWithValue :: Int -> [(Int,Maybe Int)] -> (Int,Maybe Int)
getPairWithValue _ [] = error "No more pairs for compare!"
getPairWithValue l (p:ps)
    | l == fromJust(snd p) = p
    | otherwise = getPairWithValue l ps

-- Param 1 -> key
-- Param 2 -> all pairs
-- Return -> pair with value
getPairWithKey :: Int -> [(Int,Maybe Int)] -> (Int,Maybe Int)
getPairWithKey _ [] = error "No more pairs for compare!"
getPairWithKey l (p:ps)
    | l == fst p = p
    | otherwise = getPairWithKey l ps

-- Param 1 -> key
-- Param 2 -> all pairs
-- Return -> pair with value
getPairsWithKey :: [Int] -> [(Int,Maybe Int)] -> [(Int,Maybe Int)]
getPairsWithKey [] _ = error "No more pairs for compare!"
getPairsWithKey _ [] = error "No more pairs for compare!"
getPairsWithKey l p = [ (a,b) | (a,b) <- p, a `elem` l]

-- Param 1 -> current highest
-- Param 2 -> list of pairs
-- Return -> highest sedond from pairs
getHighestValue :: Int -> [(Int,Maybe Int)] -> Int
getHighestValue c [] = c
getHighestValue c (p:ps) = getLowestValue newMax ps
    where newMax = maybe c (max c) (snd p)

-- Param 1 -> init state
-- Param 2 -> all transitions
-- Return -> states with new Id
doCannon :: [(Int,Maybe Int)] -> [(Int,Char,Int)] -> [(Int,Maybe Int)]
doCannon [] _ = []
doCannon i t = lowestPair : doCannon nextIteration t
    where lowestValue = getLowestValue (maxBound :: Int) i
          highestValue = getHighestValue (minBound :: Int) i
          lowestPair = getPairWithValue lowestValue i
          withoutLowestPair = i \\ [lowestPair]
          nextStates = getNextStates (fst lowestPair) t
          nextIteration = giveStatesNewNumber nextStates (highestValue+1) withoutLowestPair

-- Param 1 -> state to expand
-- Param 2 -> transitions
-- Return -> expanded states
getNextStates :: Int -> [(Int,Char,Int)] -> [Int]
getNextStates a t = getThirds p
    where p = [ (m,n,l) | (m,n,l) <- t, m == a ]

-- Param 1 -> states with their new id
-- Param 2 -> original transitions
-- Return -> transformed transitions
transformTransitions :: [(Int,Maybe Int)] -> [(Int,Char,Int)] -> [(Int,Char,Int)]
transformTransitions _ [] = []
transformTransitions x ((q,w,e):ts) = (a,w,c) : transformTransitions x ts
    where a = fromJust(snd(getPairWithKey q x))
          c = fromJust(snd(getPairWithKey e x))