{-# LANGUAGE OverloadedStrings  #-}

module NaiveBayes where

-- Implements Gibbs sampling for a semi-supervised naive bayes
-- model. Includes functions to load text and classify.
--
-- This is the Dirichlet-Multinomial model from McCallum and Nigam 1998

import qualified Data.Map.Strict                 as M
import qualified Data.ByteString                 as BS
import qualified Data.Text.ICU.Convert           as ICU
import qualified Data.Text.IO                    as IO
import qualified Data.Text                       as T
import qualified Data.Bifunctor                  as BI
import           Data.Tuple
import qualified Data.Vector                     as V
import qualified Data.Vector.Mutable             as MV
import qualified Data.Set                        as S
import           Data.Char
import           Control.Monad

import           Data.Time.Clock

import           Text.Printf
import qualified System.Random.MWC               as MWC
import qualified System.Random.MWC.Distributions as MWCD
import           System.Directory   (listDirectory)
import           System.FilePath


iterateM
    :: Monad m
    => Int
    -> (Int -> a -> m a)
    -> a
    -> m a
iterateM 0 _ a = return a
iterateM n f a = f (n-1) a >>= iterateM (n-1) f

groupBy
    :: Ord k
    => (a -> k)
    -> [a]
    -> M.Map k [a]
groupBy f xs = foldr (\x m -> M.insertWith (++) (f x) [x] m)
                     M.empty
                     xs
                     
when'
    :: Applicative f
    => a
    -> Bool
    -> f a
    -> f a
when' _ True  m = m
when' d False _ = pure d

convReadFile :: ICU.Converter -> FilePath -> IO T.Text
convReadFile conv file = do
  bs   <- BS.readFile file
  return (ICU.toUnicode conv bs)

type Label    = Int
type Vocab    = M.Map T.Text Int
type Features = M.Map T.Text Int -- TODO: replace with IntMap Int?
type Dataset  = V.Vector (Features, Label)

loadDataset :: S.Set String -> FilePath -> IO Dataset
loadDataset labels s = do
  conv <- ICU.open "utf-8" Nothing
  dirs <- listDirectory s
  rows <- forM dirs $ \l ->
            when' mempty (S.member l labels) $ do
                  files <- listDirectory (s </> l)
                  forM (V.fromList files) $ \f -> do
                    contents <- convReadFile conv (s </> l </> f)
                    return (toFeatures contents, S.findIndex l labels)
  return (mconcat rows)

accuracy
    :: V.Vector Label
    -> V.Vector Label
    -> Double
accuracy ytrue ypred = fromIntegral trues / (fromIntegral $ V.length ytrue)
    where trues = V.length . V.filter id $ V.zipWith (==) ytrue ypred

normalizeWord :: T.Text -> T.Text
normalizeWord = T.toLower . T.filter isAlphaNum

getFeatures :: Dataset -> V.Vector Features
getFeatures = fst . V.unzip

getLabels :: Dataset -> V.Vector Label
getLabels = snd . V.unzip

bagOfWords :: T.Text -> Features
bagOfWords d = foldr (\x m -> M.insertWith (\_ old -> old + 1) x 1 m)
                M.empty
                (map normalizeWord (T.words d))

dropStopWords :: Features -> Features
dropStopWords wc = M.withoutKeys wc stopWords

toFeatures :: T.Text -> Features
toFeatures = dropRareWords . dropStopWords . bagOfWords

sumWordCounts :: V.Vector Features -> Features
sumWordCounts = M.unionsWith (+) . V.toList

addWordCount :: Features -> Features -> Features
addWordCount = M.unionWith (+)

subWordCount :: Features -> Features -> Features
subWordCount = M.unionWith (-)

dropRareWords :: Features -> Features
dropRareWords = M.filter (> 1)

dropUnknownWords
    :: Vocab
    -> Features
    -> Features
dropUnknownWords vocab wc =
    M.restrictKeys wc (M.keysSet vocab)

trainTestSplit
    :: Double
    -> Dataset
    -> (Dataset, Dataset)
trainTestSplit r d = V.splitAt (trainSize $ V.length d) d
  where trainSize s = floor (r * fromIntegral s)

labelAwareTrainTestSplit
    :: Double
    -> Dataset
    -> (Dataset, Dataset)
labelAwareTrainTestSplit r d =
    let m = groupBy snd (V.toList d) in
    BI.bimap mconcat mconcat
      (unzip . M.elems $ M.map (trainTestSplit r . V.fromList) m)

-- this is \gamma_{\pi} in the Resnik & Hardisty paper
labelHP :: Double
labelHP = 1.0

labelPrior :: Int -> MWC.GenIO -> IO (V.Vector Double)
labelPrior k g = MWCD.dirichlet (V.generate k (const labelHP)) g

-- this is \gamma_{\theta} in the Resnik & Hardisty paper
vocabHP :: Double
vocabHP = 1.0

vocabPrior :: Int -> MWC.GenIO -> IO (V.Vector Double)
vocabPrior v g = MWCD.dirichlet (V.generate v (const vocabHP)) g

initLabel
    :: Int
    -> MWC.GenIO
    -> IO Int
initLabel k g = do
  m <- labelPrior k g
  MWCD.categorical m g

initTest
    :: Int
    -> Vocab
    -> Dataset
    -> MWC.GenIO
    -> IO Dataset
initTest k vocab test g =
  V.forM test $ \(wc, _) -> do
       l <- initLabel k g
       return (dropUnknownWords vocab wc, l)

buildVocab
    :: Dataset
    -> Vocab
buildVocab = M.fromAscList
           . flip zip [0..]
           . M.keys
           . sumWordCounts
           . getFeatures

-- Calculates number of documents with a given category (C_x)
-- from list of current category labels
countDocumentCategories
    :: Int
    -> V.Vector Label
    -> V.Vector Int
countDocumentCategories k
    = mkLabelVec k
    . M.fromListWith (+)
    . map (\x -> (x,1))
    . V.toList

mkLabelVec k m = V.generate k $ \x -> M.findWithDefault 0 x m

-- Calculates frequency of a word in a given category (N_c)
countWordFreqCategories
    :: Int
    -> Dataset
    -> V.Vector Features
countWordFreqCategories k
    = mkWordFreq k
    . M.fromListWith addWordCount
    . V.toList
    . V.map swap

mkWordFreq k m = V.generate k $ \x -> M.findWithDefault M.empty x m

time :: String -> IO a -> IO a
time label m = do
  t1 <- now
  r  <- m
  t2 <- now
  putStrLn ("Time to " ++ label ++ ": " ++ diff t1 t2)
  return r

type Time = UTCTime

now :: IO Time
now = getCurrentTime

diff :: Time -> Time -> String
diff real1 real2 =
    "real " ++ show (diffUTCTime real2 real1)

sampleLabel
    :: Int                        -- ^ Total number of documents (train+test)
    -> Int                        -- ^ Total number of categories
    -> Vocab                      -- ^ vocab list
    -> V.Vector (V.Vector Double) -- ^ probability of category c for word i
    -> V.Vector Label             -- ^ current labels for documents
    -> Features                   -- ^ word counts for document j
    -> MWC.GenIO                  -- ^ random seed
    -> IO Label                   -- ^ new label to assign to document
sampleLabel n k vocab theta l wc g = do
    {-time "Baseline: " $ -}MWCD.categorical labelPosterior g
    where
      categoryCounts   = countDocumentCategories k l
      docLikelihood t  = M.foldrWithKey' (\word freq acc ->
                           acc + log (t V.! (vocab M.! word)) * fromIntegral freq)
                           0
                           wc
      labelPosterior   = V.generate k $ \x ->
        (fromIntegral (categoryCounts V.! x) + labelHP - 1)    /
        (fromIntegral n + 2 * labelHP - 1)  *
        (exp $ docLikelihood (theta V.! x))

sampleTheta
    :: Int
    -> Vocab
    -> Dataset
    -> MWC.GenIO
    -> IO (V.Vector (V.Vector Double))
sampleTheta k vocab d g = do
    V.generateM k (flip MWCD.dirichlet g . t)
    where n             = countWordFreqCategories k d
          vocab'        = V.fromList (M.keys vocab)
          findCount v c = M.findWithDefault 0 v (n V.! c)
          t c           = flip V.map vocab' $ \v ->
                              fromIntegral (findCount v c) + vocabHP

sampleIter
    :: Int
    -> V.Vector (V.Vector Double)
    -> Vocab
    -> Dataset
    -> Dataset
    -> MWC.GenIO
    -> IO (Dataset, V.Vector (V.Vector Double))
sampleIter k theta vocab train test g = do
    test'  <- iterateM (V.length test) go test
    theta' <- sampleTheta k vocab (train V.++ test') g
    return (test', theta')
    where
      n    = V.length train + V.length test
      go j test' =
          let (testPre, wcAndtestPost) = V.splitAt j test'
              (wc, _)  = V.head wcAndtestPost
              testPost = V.tail wcAndtestPost
              labels   = getLabels testPre V.++ getLabels testPost
          in do l <- sampleLabel n k vocab theta labels wc g
                return $ V.modify (\mv ->  MV.write mv j (wc, l)) test'

-- Sample function to use with handwritten implementation
sample
    :: Int
    -> Int
    -> Dataset
    -> Dataset
    -> MWC.GenIO
    -> (Dataset -> IO a)
    -> IO Dataset
sample iters k train test g post = do
    theta <- V.replicateM k (vocabPrior (M.size vocab) g)
    cleanedTest <- V.forM test $ \(wc, _) -> do
        l <- initLabel k g
        return (dropUnknownWords vocab wc, l)
    go iters theta cleanedTest
  where
  vocab = buildVocab train

  go 0 theta' test' = return test'
  go i theta' test' = do
      printf "Iteration: %3d" (iters - i + 1)
      (test'', theta'') <- sampleIter k theta' vocab train test' g
      post test''
      go (i-1) theta'' test''

stopWords :: S.Set T.Text
stopWords = S.fromList
     [ "i"
     , "me"
     , "my"
     , "myself"
     , "we"
     , "our"
     , "ours"
     , "ourselves"
     , "you"
     , "your"
     , "yours"
     , "yourself"
     , "yourselves"
     , "he"
     , "him"
     , "his"
     , "himself"
     , "she"
     , "her"
     , "hers"
     , "herself"
     , "it"
     , "its"
     , "itself"
     , "they"
     , "them"
     , "their"
     , "theirs"
     , "themselves"
     , "what"
     , "which"
     , "who"
     , "whom"
     , "this"
     , "that"
     , "these"
     , "those"
     , "am"
     , "is"
     , "are"
     , "was"
     , "were"
     , "be"
     , "been"
     , "being"
     , "have"
     , "has"
     , "had"
     , "having"
     , "do"
     , "does"
     , "did"
     , "doing"
     , "a"
     , "an"
     , "the"
     , "and"
     , "but"
     , "if"
     , "or"
     , "because"
     , "as"
     , "until"
     , "while"
     , "of"
     , "at"
     , "by"
     , "for"
     , "with"
     , "about"
     , "against"
     , "between"
     , "into"
     , "through"
     , "during"
     , "before"
     , "after"
     , "above"
     , "below"
     , "to"
     , "from"
     , "up"
     , "down"
     , "in"
     , "out"
     , "on"
     , "off"
     , "over"
     , "under"
     , "again"
     , "further"
     , "then"
     , "once"
     , "here"
     , "there"
     , "when"
     , "where"
     , "why"
     , "how"
     , "all"
     , "any"
     , "both"
     , "each"
     , "few"
     , "more"
     , "most"
     , "other"
     , "some"
     , "such"
     , "no"
     , "nor"
     , "not"
     , "only"
     , "own"
     , "same"
     , "so"
     , "than"
     , "too"
     , "very"
     , "s"
     , "t"
     , "can"
     , "will"
     , "just"
     , "don"
     , "should"
     , "now"
     ]
