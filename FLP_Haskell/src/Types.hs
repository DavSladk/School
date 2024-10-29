module Types where

data Automat = Automat { allStates :: [Int]
                       , albhabet :: String
                       , startState :: Int
                       , endStates :: [Int]
                       , transitions :: [(Int,Char,Int)]
                       } deriving (Show)
