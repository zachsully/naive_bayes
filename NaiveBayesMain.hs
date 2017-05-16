module Main where

import           Control.Monad              (forever, replicateM, forM_)
import qualified Data.ByteString.Char8 as B
import           Data.List                  (sort)
import           Data.Number.LogFloat
import           Data.Time.Clock
import qualified Data.Vector.Unboxed   as V
import           Data.Vector.Unboxed        (Vector, (!))
import           GibbsOptBucket             (prog)
import           Language.Hakaru.Runtime.LogFloatPrelude
                                            (unMeasure, prob_, categorical)
import           News                       (getNews)
import           System.Environment         (getArgs)
import qualified System.Random.MWC as MWC
import           Text.Printf                (printf)

diff :: UTCTime -> UTCTime -> String
diff t1 t2 = filter (/= 's') $ show (diffUTCTime t2 t1)

onesFrom :: Vector Int -> Vector LogFloat
onesFrom v = V.replicate (V.maximum v + 1) 1

sample g m = do
  maybeX <- unMeasure m g
  case maybeX of
    Nothing -> fail "Sample rejected"
    Just x  -> return x

accuracy
    :: V.Vector Int
    -> V.Vector Int
    -> Double
accuracy x y = V.sum z / (fromIntegral $ V.length x)
    where z = V.zipWith (\a b -> if a == b then 1 else 0) x y

initializeZs
    :: Int
    -> Int
    -> MWC.GenIO
    -> IO (Vector Int)
initializeZs testSize numTopics g =
    V.replicateM testSize $
      sample g (categorical $ V.replicate numTopics (prob_ 1))

main = do
  args  <- getArgs
  case length args == 2 of
    False -> putStrLn "./naive-bayes <docsPerTopic≻ <trial>"
    True  -> do
         let [docsPerTopic, trial] = map read args :: [Int]
         (words, docs, topics) <- getNews (Just docsPerTopic) [0..]
         g <- MWC.createSystemRandom
         let numTopics        = 20
             trainTestSplit   = 9/10
             testDocsPerTopic = ceiling (fromIntegral docsPerTopic *
                                         (1 - trainTestSplit))
             topicIndices     = getTopicIndices
                                  testDocsPerTopic
                                  docsPerTopic
                                  numTopics

             zTrues  = V.map (topics !) topicIndices
         zInits  <- initializeZs (V.length topicIndices) numTopics g
         let topics' = V.update_ topics topicIndices zInits
             zPrior  = onesFrom topics
             wPrior  = onesFrom words
             -- predict = prog zPrior wPrior topics' words docs
         t1 <- getCurrentTime
         topicsE <- V.foldM (\v i -> do
                               let predict = prog zPrior wPrior v words docs
                               z' <- sample g (predict i)
                               return (v V.// [(i, z')])) topics' topicIndices
         -- zPreds <- V.forM topicIndices $ \i -> sample g (predict i)
         let zPreds = V.map (topicsE !) topicIndices
         t2 <- getCurrentTime

         -- We don't print a newline as this will be called from a larger shell script
         -- that needs to add another field
         printf "Hakaru,%d,%d,%.6f,%s,"
                    (V.length topics)
                    trial
                    (accuracy zTrues zPreds)
                    (diff t1 t2)

getTopicIndices testDocsPerTopic docsPerTopic numTopics =
    V.concatMap
         (\i -> V.generate testDocsPerTopic (+ i))
         seq
    where seq = V.generate numTopics (* docsPerTopic)
