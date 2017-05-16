module Main where

import News (getNews)
import Text.Printf (printf)
import GibbsOptBucket (prog)
import Control.Monad (forever, replicateM, forM_)
import qualified Data.ByteString.Char8 as B
import Data.List (sort)
import Data.Maybe
import Data.Monoid
import Data.Number.LogFloat
import Data.Vector.Unboxed ((!))
import qualified Data.Vector.Unboxed as V
import System.Environment
import System.IO
import qualified System.Random.MWC as MWC

writeVec :: String -> V.Vector Int -> IO ()
writeVec file v = withFile file WriteMode $ \h -> do
  V.forM_ v $ \x -> hPrint h x

main = do
  args  <- getArgs
  case length args == 1 of
    False -> putStrLn "./write-news <docsPerTopic≻"
    True  -> do
      let suffix = "." <> head args
      let docsPerTopic = fmap read (listToMaybe args)
      (words, docs, topics) <- getNews docsPerTopic [0..]
      writeVec ("words"  <> suffix) words
      writeVec ("docs"   <> suffix) docs
      writeVec ("topics" <> suffix) topics
