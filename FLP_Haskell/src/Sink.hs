module Sink
( fullyDefine
, removeSink
)where

import Data.List ( (\\) )

import Types ( Automat(..) )

-- Takes a automat and adds sink state if not fully defined
fullyDefine :: Automat -> Automat
fullyDefine a = Automat {allStates=_allStates, albhabet=_albhabet, startState=_startState, endStates=_endStates, transitions=_transitions}
          -- add sink state to all states if condition is met
    where _allStates = allStates a ++ [sinkState | condition]
          _albhabet = albhabet a
          _startState = startState a
          _endStates = endStates a
          -- add new transition, also add transition from sink to sink if condition is met
          _transitions = transitions a ++ newTransitionsToSink ++ if condition then sinkToSink sinkState (albhabet a) else []
          newTransitionsToSink = [ (q,s,sinkState) | q <- allStates a, s <- _albhabet, noTransition q s (allStates a) (transitions a) ]
          -- sink state is identified as one hieger than last state
          sinkState = last (allStates a) + 1
          -- are there any new transitions?
          condition = not ( null newTransitionsToSink)

-- Creates transitions from sink to sink
sinkToSink :: Int -> String -> [(Int,Char,Int)]
sinkToSink _ [] = []
sinkToSink sink albha = [ (sink, symbol, sink) | symbol <- albha]

-- Param 1 -> a state
-- Param 2 -> a symbol
-- Param 3 -> allStates
-- Param 4 -> transitions
-- Return -> true if transition from Param 1 to any state over Param 2 does not exist
noTransition :: Int -> Char -> [Int] -> [(Int, Char, Int)] -> Bool
noTransition q s allS t = null [(q,s,c) | c <- allS, (q,s,c) `elem` t]

-- Removes sink states and affiliated transitions form automat
removeSink :: Automat -> Automat
removeSink a = Automat {allStates=_allStates, albhabet=_albhabet, startState=_startState, endStates=_endStates, transitions=_transitions}
    where _allStates = allStates a \\ sinkStates
          _albhabet = albhabet a
          _startState = startState a
          _endStates = endStates a
          _transitions = removeTransitionsWithSinkStates sinkStates (transitions a)
          unescapableStates = findUnescapableStates (allStates a) (transitions a)
          sinkStates = unescapableStates \\ _endStates

-- Param 1 -> all states in automat
-- Param 2 -> all transitions
-- Return -> unescapable states
findUnescapableStates :: [Int] -> [(Int, Char, Int)] -> [Int]
findUnescapableStates [] _ = []
findUnescapableStates (s:ss) t = isUnescapable s t ++ findUnescapableStates ss t

-- Param 1 -> state to evaluate
-- Param 2 -> all transitions
-- Return -> Param 1 if unescapable
-- description: find all transitions that lead from Param 1,
--    if such transition does not exist than Param 1 is unescapable
isUnescapable :: Int -> [(Int, Char, Int)] -> [Int]
isUnescapable s t
    | null escapeTransitions = [s]
    | otherwise = []
    where escapeTransitions = [ (a,b,c) | (a,b,c) <- t, a == s, a /= c ]

-- Param 1 -> all sink states
-- Param 2 -> all transitions
-- Return -> transitions without sink 
-- description: removes all transition (a,b,c) where c is sink state
removeTransitionsWithSinkStates :: [Int] -> [(Int, Char, Int)] -> [(Int, Char, Int)]
removeTransitionsWithSinkStates s t = [ (a,b,c) | (a,b,c) <- t, c `notElem` s]
