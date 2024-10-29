{-# OPTIONS_GHC -Wno-unrecognised-pragmas #-}
{-# HLINT ignore "Use head" #-}

-- project: flp21-fun
-- author: David Sladký
-- login: xsladk07
-- year 2022

module Main ( main ) where

import System.Environment ( getArgs )
import GHC.IO.Handle (hClose, hGetContents)

import MyUtils ( parseArgs, openInput, printAutomat, parseAutomat )
import Unreachable ( removeUnreachableStates )
import Sink ( fullyDefine, removeSink )
import Minimalize ( reduce ) 
import Canon ( toCanon )

main :: IO ()
main = do
    args <- getArgs
    let (i,t,file) = parseArgs args

    handle <- openInput file
    content <- hGetContents handle
    let automat = parseAutomat ( lines content )
    

    if i
        then do
            print automat
        else do
            return ()

    if t
        then do
            let a = removeUnreachableStates automat
            let b = fullyDefine a
            let c = reduce b
            let d = removeSink c
            let e = toCanon d
            printAutomat e

        else do
            return ()

    hClose handle