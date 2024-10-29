module Unreachable
( removeUnreachableStates
) where

import Types ( Automat(..) )
import MyUtils ( getThirds, nubSort )
import Data.List ( intersect )

-- Creates automat without unreachable states
removeUnreachableStates :: Automat -> Automat
removeUnreachableStates x = Automat {allStates=_allStates, albhabet=_albhabet, startState=_startState, endStates=_endStates, transitions=_transitions}
    where _allStates = doRemove [_startState] (allStates x) _albhabet (transitions x)
          _albhabet = albhabet x
          _startState = startState x
          _endStates = _allStates `intersect` endStates x
          _transitions = removeExtraTransitions _allStates _albhabet (nubSort (transitions x))

-----------------------------------------
-- Param 1 -> States in current iteration
-- Param 2 -> All states in original automat
-- Param 3 -> Albphabet
-- Param 4 -> Transitions in original automat
-- Return -> List of reachable states
doRemove :: [Int] -> [Int] -> String -> [(Int,Char,Int)] -> [Int]
doRemove _newStates _allStates _albhabet _transitions
    -- compare current and next iteration, if same, then end
    | nubSort _newStates == nubSort ( _newStates ++ add ) = nubSort _newStates
    -- otherwise iterate
    | otherwise = doRemove ( nubSort _newStates ++ nubSort add ) _allStates _albhabet _transitions
    where add = getThirds (getPossibleTransitions _newStates _allStates _albhabet _transitions)

-----------------------------------------
-- Param 1 -> States in current iteration
-- Param 2 -> All states in original automat
-- Param 3 -> Albhabet
-- Param 4 -> Transitions in original automat
-- Return -> List of all posible transitions from states in current iteration
getPossibleTransitions :: [Int] -> [Int] -> String -> [(Int,Char,Int)] -> [(Int,Char,Int)]
getPossibleTransitions _currentStates _allStates _albhabet t
    -- all triplets (a,b,c) where
    --      a - all states in current iteration
    --      b - albhabet
    --      c - all state in original automat
    --      this triplet is in transition function of original automat
    = [ (a,b,c) | a <- _currentStates, b <- _albhabet, c <- _allStates, (a,b,c) `elem` t]

--------------------------------------
-- Param 1 -> List of reachable states
-- Param 2 -> Albhabet
-- Param 3 -> List of transition form original automat
-- Return -> List of transition without transition from/to unreachable states
removeExtraTransitions :: [Int] -> String -> [(Int, Char, Int)] -> [(Int, Char, Int)]
removeExtraTransitions _newStates _albhabet _transitions =
    -- all triplets (a,b,c) where
    --      a - all reachable states
    --      b - albhabet
    --      c - all reachable states
    --      this triplet is in original transitions
    [ (a,b,c) | a <- _newStates, b <- _albhabet, c <- _newStates, (a,b,c) `elem` _transitions]
