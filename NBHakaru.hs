{-# LANGUAGE DataKinds
           , FlexibleContexts
           , NegativeLiterals
           , ForeignFunctionInterface
           #-}

module Main where

import           Prelude                          hiding (product)
import           Language.Hakaru.Runtime.Prelude
import           Language.Hakaru.Types.Sing
import           System.CPUTime
import           Data.Time.Clock
import           System.Environment
import           System.IO.Unsafe
import qualified System.Random.MWC                as MWC
import qualified System.Random.MWC.Distributions  as MWCD
import           Control.Monad
import qualified Data.Vector                      as V
import qualified Data.Vector.Generic              as G
import qualified Data.Vector.Storable             as SV
import qualified Data.Set                         as S
import qualified Data.Map.Strict                  as M
import qualified Data.Text                        as T
import           Foreign
import           Foreign.C
import           Foreign.Ptr                      (Ptr,nullPtr)
import           Foreign.Storable
import           Text.Printf

foreign import ccall "gibbsC_shim"
    gibbsC :: ArrayStruct Double
           -> ArrayStruct Double
           -> ArrayStruct Int
           -> ArrayStruct Int
           -> ArrayStruct Int
           -> Int
           -> IO (ArrayStruct Double)
    
data ArrayType a = ArrayType Int (SV.Vector a)
  deriving (Show, Eq)

getV (ArrayType _ x) = x
 
type ArrayStruct a = Ptr (ArrayType a)

sizeCInt  = sizeOf    (undefined :: CInt)
sizePtr   = sizeOf    (undefined :: Ptr a)
alignPtr_ = alignment (undefined :: Ptr a)

withVector :: SV.Storable a
           => SV.Vector a
           -> (ArrayStruct a -> IO b)
           -> IO b
withVector v f = with (ArrayType size v) f
    where size = SV.length v
            
instance SV.Storable a => SV.Storable (ArrayType a) where
  sizeOf _ = sizeCInt
             `div` (- alignPtr_)
                 * (- alignPtr_)
           + sizePtr
  alignment _ = sizePtr
  peek ptr    = do
      n <- peekByteOff ptr 0
      xptr  <- peekByteOff ptr (sizeCInt                      
                                `div` (- alignPtr_)  
                                    * (- alignPtr_))
      xfptr <- newForeignPtr_ xptr
      let x = SV.unsafeFromForeignPtr0 xfptr n
      return (ArrayType n x)
  poke ptr (ArrayType n x) = do
      pokeByteOff ptr 0 n
      SV.unsafeWith x $ \xptr ->
          pokeByteOff ptr (sizeCInt
                           `div` (- alignPtr_)
                                * (- alignPtr_))
                      xptr

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

type Label    = Int
type Vocab    = M.Map T.Text Int
type Features = V.Vector Integer
type Dataset  = (Features, Label)

runner
    :: Int
    -> Int
    -> Int
    -> Int
    -> IO ()
runner numDocs k vocabSize trial = do
    g      <- MWC.createSystemRandom
    Just (z, w) <- unMeasure (generateDataset k vocabSize numDocs doc) g
    sample <- time "" $ do
      printf "C,%d,%d,%d,%d," numDocs k vocabSize trial
      vocabP <- vocabPrior vocabSize g
      labelP <- labelPrior k g
      withVector (G.convert vocabP) $ \vocabP' ->
       withVector (G.convert labelP) $ \labelP' ->
        withVector (G.convert z) $ \z' ->
         withVector (G.convert w) $ \w' ->
          withVector (G.convert doc) $ \doc' ->
           gibbsC vocabP' labelP' z' w' doc' 1
    sample <- time "" $ do
      printf "Haskell,%d,%d,%d,%d," numDocs k vocabSize trial
      vocabP <- vocabPrior vocabSize g
      labelP <- labelPrior k g
      unMeasure (gibbs (G.convert vocabP) (G.convert labelP) z w doc 1) g
    return ()
  where doc :: MayBoxVec Int Int
        doc = G.concat $ map (G.replicate numDocs) [0..5] -- 300
        -- numDocs   = 20   -- 20000
        -- k         = 3    -- 20
        -- vocabSize = 100  -- 40000

time :: String -> IO a -> IO a
time label m = do
  t1 <- now
  r  <- m
  t2 <- now
  putStrLn $ filter (/= 's') (diff t1 t2)
  return r

type Time = UTCTime
now :: IO Time
now = getCurrentTime
diff :: Time -> Time -> String
diff real1 real2
  = show (diffUTCTime real2 real1)

main = do
  args <- getArgs
  case map read args of
    [numDocs, k, vocabSize, trial] -> runner numDocs k vocabSize trial
    _ -> error "NBHakaru <num docs> <k> <vocabSize> <trial>"

generateDataset =
    let_ (lam $ \ as1 ->
        (plate (unsafeNat (nat2int (size as1) +
                           negate (nat2int (nat_ 1)))) $
               \ i3 ->
               beta (summate (i3 + nat_ 1) (size as1) (\ j4 -> as1 ! j4))
                    (as1 ! i3)) >>= \ xs2 ->
        dirac (array (size as1) $
                     \ i5 ->
                     let_ (product (nat_ 0) i5 (\ j7 -> xs2 ! j7)) $ \ x6 ->
                     x6 *
                     case_ (i5 + nat_ 1 == size as1)
                           [branch ptrue (nat2prob (nat_ 1)),
                            branch pfalse
                                   (unsafeProb (nat2real (nat_ 1) +
                                                negate (fromProb (xs2 ! i5))))])) $ \ dirichlet0 ->
  let_ (lam $ \k9 ->
        lam $ \v10 ->
        lam $ \z11 ->
        lam $ \doc12 ->
        let_ (array k9 $ \ k14 -> prob_ 1) $ \ topic_prior13 ->
        let_ (array v10 $ \ v16 -> prob_ 1) $ \ word_prior15 ->
        dirichlet0 `app` topic_prior13 >>= \ theta17 ->
        (plate k9 $ \ k19 -> dirichlet0 `app` word_prior15) >>= \ phi18 ->
        (plate z11 $ \ i21 -> categorical theta17) >>= \ z20 ->
        (plate (size doc12) $
               \ n23 -> categorical (phi18 ! (z20 ! (doc12 ! n23)))) >>= \ w22 ->
        dirac (ann_ (SData (STyApp (STyApp (STyCon (SingSymbol :: Sing "Pair"))
                                    (SArray SNat)) (SArray SNat))
                     (SPlus (SEt (SKonst (SArray SNat))
                             (SEt (SKonst (SArray SNat)) SDone)) SVoid))
               ((pair z20 w22)))) $ \ naive_bayes8 ->
  naive_bayes8

gibbs = 
  lam $ \ topic_prior0 ->
  lam $ \ word_prior1 ->
  lam $ \ z2 ->
  lam $ \ w3 ->
  lam $ \ doc4 ->
  lam $ \ docUpdate5 ->
  categorical (array (size topic_prior0) $
                     \ zNew丏6 ->
                     product (nat_ 0)
                             (size topic_prior0)
                             (\ i7 ->
                              product (nat_ 0)
                                      (size word_prior1)
                                      (\ i丣8 ->
                                       product (nat_ 0)
                                               (summate (nat_ 0)
                                                        (size w3)
                                                        (\ i丙10 ->
                                                         case_ (docUpdate5 == doc4 ! i丙10)
                                                               [branch ptrue
                                                                       (case_ (i7 == zNew丏6 &&
                                                                               i丣8 == w3 ! i丙10)
                                                                              [branch ptrue
                                                                                      (nat_ 1),
                                                                               branch pfalse
                                                                                      (nat_ 0)]),
                                                                branch pfalse (nat_ 0)]))
                                               (\ j9 ->
                                                (nat2prob (summate (nat_ 0)
                                                                   (size w3)
                                                                   (\ i丙11 ->
                                                                    case_ (doc4 ! i丙11
                                                                           == docUpdate5)
                                                                          [branch ptrue (nat_ 0),
                                                                           branch pfalse
                                                                                  (case_ (i7
                                                                                          == z2
                                                                                             ! (doc4
                                                                                                ! i丙11) &&
                                                                                          i丣8
                                                                                          == w3
                                                                                             ! i丙11)
                                                                                         [branch ptrue
                                                                                                 (nat_ 1),
                                                                                          branch pfalse
                                                                                                 (nat_ 0)])])) +
                                                 nat2prob j9 +
                                                 word_prior1 ! i丣8) *
                                                (nat2prob (summate (nat_ 0)
                                                                   (size z2)
                                                                   (\ i丙12 ->
                                                                    case_ (i丙12 == docUpdate5)
                                                                          [branch ptrue (nat_ 0),
                                                                           branch pfalse
                                                                                  (case_ (zNew丏6
                                                                                          == z2
                                                                                             ! i丙12)
                                                                                         [branch ptrue
                                                                                                 (nat_ 1),
                                                                                          branch pfalse
                                                                                                 (nat_ 0)])])) +
                                                 topic_prior0 ! zNew丏6) *
                                                recip (nat2prob (summate (nat_ 0)
                                                                         (size z2)
                                                                         (\ i丙13 ->
                                                                          case_ (i丙13 == docUpdate5)
                                                                                [branch ptrue
                                                                                        (nat_ 0),
                                                                                 branch pfalse
                                                                                        (case_ (z2
                                                                                                ! i丙13
                                                                                                < nat_ 0)
                                                                                               [branch ptrue
                                                                                                       (nat_ 0),
                                                                                                branch pfalse
                                                                                                       (nat_ 1)])])) +
                                                       summate (nat_ 0)
                                                               (size topic_prior0)
                                                               (\ i丙14 -> topic_prior0 ! i丙14)) *
                                                recip (product (nat_ 0)
                                                               (size topic_prior0)
                                                               (\ i15 ->
                                                                product (nat_ 0)
                                                                        (summate (nat_ 0)
                                                                                 (size w3)
                                                                                 (\ i丙17 ->
                                                                                  case_ (docUpdate5
                                                                                         == doc4
                                                                                            ! i丙17)
                                                                                        [branch ptrue
                                                                                                (case_ (i15
                                                                                                        == zNew丏6)
                                                                                                       [branch ptrue
                                                                                                               (nat_ 1),
                                                                                                        branch pfalse
                                                                                                               (nat_ 0)]),
                                                                                         branch pfalse
                                                                                                (nat_ 0)]))
                                                                        (\ i丣16 ->
                                                                         nat2prob (summate (nat_ 0)
                                                                                           (size w3)
                                                                                           (\ i丙18 ->
                                                                                            case_ (doc4
                                                                                                   ! i丙18
                                                                                                   == docUpdate5)
                                                                                                  [branch ptrue
                                                                                                          (nat_ 0),
                                                                                                   branch pfalse
                                                                                                          (case_ (i15
                                                                                                                  == z2
                                                                                                                     ! (doc4
                                                                                                                        ! i丙18))
                                                                                                                 [branch ptrue
                                                                                                                         (nat_ 1),
                                                                                                                  branch pfalse
                                                                                                                         (nat_ 0)])])) +
                                                                         nat2prob i丣16 +
                                                                         summate (nat_ 0)
                                                                                 (size word_prior1)
                                                                                 (\ i丙19 ->
                                                                                  word_prior1
                                                                                  ! i丙19))))))))

