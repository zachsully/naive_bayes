{-# LANGUAGE DataKinds, NegativeLiterals #-}
module GibbsOptBucket where

import           Data.Number.LogFloat hiding (product)
import           Prelude              hiding (product, exp, log, (**))

import           Language.Hakaru.Runtime.LogFloatPrelude
import           Language.Hakaru.Types.Sing
import qualified System.Random.MWC                as MWC
import           Control.Monad

prog = 
  lam $ \ topic_prior0 ->
  lam $ \ word_prior1 ->
  lam $ \ z2 ->
  lam $ \ w3 ->
  lam $ \ doc4 ->
  lam $ \ docUpdate5 ->
  case_ (docUpdate5 < size z2)
        [branch ptrue
                ((pose (product (nat_ 0)
                                (size topic_prior0)
                                (\ i6 ->
                                 product (nat_ 0)
                                         (size word_prior1)
                                         (\ i丣7 ->
                                          product (nat_ 0)
                                                  (let_ (bucket (nat_ 0)
                                                                (size w3)
                                                                ((r_split (\ (i丙10,()) ->
                                                                           docUpdate5
                                                                           == doc4 ! i丙10)
                                                                          r_nop
                                                                          (r_index (\ () ->
                                                                                    size topic_prior0)
                                                                                   (\ (i丙10,()) ->
                                                                                    z2
                                                                                    ! (doc4 ! i丙10))
                                                                                   (r_index (\ (i11,()) ->
                                                                                             size word_prior1)
                                                                                            (\ (i丙10,(i11,())) ->
                                                                                             w3
                                                                                             ! i丙10)
                                                                                            (r_add (\ (i丙10,(i丣12,(i11,()))) ->
                                                                                                    nat_ 1))))))) $ \ summary9 ->
                                                   case_ summary9
                                                         [branch (ppair PVar PVar)
                                                                 (\ y13 z14 -> z14)]
                                                   ! i6
                                                   ! i丣7)
                                                  (\ j8 -> nat2prob j8 + word_prior1 ! i丣7))) *
                        product (nat_ 0)
                                (size topic_prior0)
                                (\ i15 ->
                                 product (nat_ 0)
                                         (let_ (bucket (nat_ 0)
                                                       (size z2)
                                                       ((r_split (\ (i丙18,()) -> i丙18 == docUpdate5)
                                                                 r_nop
                                                                 (r_index (\ () ->
                                                                           size topic_prior0)
                                                                          (\ (i丙18,()) -> z2 ! i丙18)
                                                                          (r_add (\ (i丙18,(i19,())) ->
                                                                                  nat_ 1)))))) $ \ summary17 ->
                                          case_ summary17
                                                [branch (ppair PVar PVar) (\ y20 z21 -> z21)]
                                          ! i15)
                                         (\ j16 -> nat2prob j16 + topic_prior0 ! i15)) *
                        recip (product (nat_ 0)
                                       (summate (nat_ 0)
                                                (size z2)
                                                (\ i丙23 ->
                                                 case_ (i丙23 == docUpdate5)
                                                       [branch ptrue (nat_ 0),
                                                        branch pfalse
                                                               (case_ (z2 ! i丙23 < nat_ 0)
                                                                      [branch ptrue (nat_ 0),
                                                                       branch pfalse (nat_ 1)])]))
                                       (\ i22 ->
                                        nat2prob i22 +
                                        summate (nat_ 0)
                                                (size topic_prior0)
                                                (\ i丙24 -> topic_prior0 ! i丙24))) *
                        recip (product (nat_ 0)
                                       (size topic_prior0)
                                       (\ i25 ->
                                        product (nat_ 0)
                                                (let_ (bucket (nat_ 0)
                                                              (size w3)
                                                              ((r_split (\ (i丙28,()) ->
                                                                         w3 ! i丙28
                                                                         < nat_ 0)
                                                                        r_nop
                                                                        (r_split (\ (i丙28,()) ->
                                                                                  docUpdate5
                                                                                  == doc4 ! i丙28)
                                                                                 r_nop
                                                                                 (r_index (\ () ->
                                                                                           size topic_prior0)
                                                                                          (\ (i丙28,()) ->
                                                                                           z2
                                                                                           ! (doc4
                                                                                              ! i丙28))
                                                                                          (r_add (\ (i丙28,(i29,())) ->
                                                                                                  nat_ 1))))))) $ \ summary27 ->
                                                 case_ (case_ summary27
                                                              [branch (ppair PVar PVar)
                                                                      (\ y30 z31 -> z31)])
                                                       [branch (ppair PVar PVar) (\ y32 z33 -> z33)]
                                                 ! i25)
                                                (\ i丣26 ->
                                                 nat2prob i丣26 +
                                                 summate (nat_ 0)
                                                         (size word_prior1)
                                                         (\ i丙34 -> word_prior1 ! i丙34))))) $
                       (categorical (array (size topic_prior0) $
                                           \ zNew丏35 ->
                                           product (nat_ 0)
                                                   (size topic_prior0)
                                                   (\ i36 ->
                                                    product (nat_ 0)
                                                            (size word_prior1)
                                                            (\ i丣37 ->
                                                             product (nat_ 0)
                                                                     (let_ (bucket (nat_ 0)
                                                                                   (size w3)
                                                                                   ((r_fanout (r_index (\ () ->
                                                                                                        size z2)
                                                                                                       (\ (i丙40,()) ->
                                                                                                        doc4
                                                                                                        ! i丙40)
                                                                                                       (r_index (\ (docUpdate41,()) ->
                                                                                                                 size word_prior1)
                                                                                                                (\ (i丙40,(docUpdate41,())) ->
                                                                                                                 w3
                                                                                                                 ! i丙40)
                                                                                                                (r_add (\ (i丙40,(i丣42,(docUpdate41,()))) ->
                                                                                                                        nat_ 1))))
                                                                                              r_nop))) $ \ summary39 ->
                                                                      case_ (i36 == zNew丏35)
                                                                            [branch ptrue
                                                                                    (case_ summary39
                                                                                           [branch (ppair PVar
                                                                                                          PVar)
                                                                                                   (\ y43
                                                                                                      z44 ->
                                                                                                    y43)]
                                                                                     ! docUpdate5
                                                                                     ! i丣37),
                                                                             branch pfalse
                                                                                    (nat_ 0)])
                                                                     (\ j38 ->
                                                                      nat2prob (let_ (bucket (nat_ 0)
                                                                                             (size w3)
                                                                                             ((r_split (\ (i丙46,()) ->
                                                                                                        doc4
                                                                                                        ! i丙46
                                                                                                        == docUpdate5)
                                                                                                       r_nop
                                                                                                       (r_index (\ () ->
                                                                                                                 size topic_prior0)
                                                                                                                (\ (i丙46,()) ->
                                                                                                                 z2
                                                                                                                 ! (doc4
                                                                                                                    ! i丙46))
                                                                                                                (r_index (\ (i47,()) ->
                                                                                                                          size word_prior1)
                                                                                                                         (\ (i丙46,(i47,())) ->
                                                                                                                          w3
                                                                                                                          ! i丙46)
                                                                                                                         (r_add (\ (i丙46,(i丣48,(i47,()))) ->
                                                                                                                                 nat_ 1))))))) $ \ summary45 ->
                                                                                case_ summary45
                                                                                      [branch (ppair PVar
                                                                                                     PVar)
                                                                                              (\ y49
                                                                                                 z50 ->
                                                                                               z50)]
                                                                                ! i36
                                                                                ! i丣37) +
                                                                      nat2prob j38 +
                                                                      word_prior1 ! i丣37))) *
                                           ((let_ (bucket (nat_ 0)
                                                          (size z2)
                                                          ((r_split (\ (i丙52,()) ->
                                                                     i丙52
                                                                     == docUpdate5)
                                                                    r_nop
                                                                    (r_index (\ () ->
                                                                              size topic_prior0)
                                                                             (\ (i丙52,()) ->
                                                                              z2
                                                                              ! i丙52)
                                                                             (r_add (\ (i丙52,(zNew丏53,())) ->
                                                                                     nat_ 1)))))) $ \ summary51 ->
                                             nat2prob (case_ summary51
                                                             [branch (ppair PVar PVar)
                                                                     (\ y54 z55 -> z55)]
                                                       ! zNew丏35)) +
                                            topic_prior0 ! zNew丏35) *
                                           recip (nat2prob (summate (nat_ 0)
                                                                    (size z2)
                                                                    (\ i丙56 ->
                                                                     case_ (i丙56 == docUpdate5)
                                                                           [branch ptrue (nat_ 0),
                                                                            branch pfalse
                                                                                   (case_ (z2 ! i丙56
                                                                                           < nat_ 0)
                                                                                          [branch ptrue
                                                                                                  (nat_ 0),
                                                                                           branch pfalse
                                                                                                  (nat_ 1)])])) +
                                                  summate (nat_ 0)
                                                          (size topic_prior0)
                                                          (\ i丙57 -> topic_prior0 ! i丙57)) *
                                           recip (product (nat_ 0)
                                                          (size topic_prior0)
                                                          (\ i58 ->
                                                           product (nat_ 0)
                                                                   (let_ (bucket (nat_ 0)
                                                                                 (size w3)
                                                                                 ((r_fanout (r_index (\ () ->
                                                                                                      size z2)
                                                                                                     (\ (i丙61,()) ->
                                                                                                      doc4
                                                                                                      ! i丙61)
                                                                                                     (r_split (\ (i丙61,(docUpdate62,())) ->
                                                                                                               w3
                                                                                                               ! i丙61
                                                                                                               < nat_ 0)
                                                                                                              r_nop
                                                                                                              (r_add (\ (i丙61,(docUpdate62,())) ->
                                                                                                                      nat_ 1))))
                                                                                            r_nop))) $ \ summary60 ->
                                                                    case_ (i58 == zNew丏35)
                                                                          [branch ptrue
                                                                                  (case_ (case_ summary60
                                                                                                [branch (ppair PVar
                                                                                                               PVar)
                                                                                                        (\ y63
                                                                                                           z64 ->
                                                                                                         y63)]
                                                                                          ! docUpdate5)
                                                                                         [branch (ppair PVar
                                                                                                        PVar)
                                                                                                 (\ y65
                                                                                                    z66 ->
                                                                                                  z66)]),
                                                                           branch pfalse (nat_ 0)])
                                                                   (\ i丣59 ->
                                                                    nat2prob (let_ (bucket (nat_ 0)
                                                                                           (size w3)
                                                                                           ((r_split (\ (i丙68,()) ->
                                                                                                      w3
                                                                                                      ! i丙68
                                                                                                      < nat_ 0)
                                                                                                     r_nop
                                                                                                     (r_split (\ (i丙68,()) ->
                                                                                                               doc4
                                                                                                               ! i丙68
                                                                                                               == docUpdate5)
                                                                                                              r_nop
                                                                                                              (r_index (\ () ->
                                                                                                                        size topic_prior0)
                                                                                                                       (\ (i丙68,()) ->
                                                                                                                        z2
                                                                                                                        ! (doc4
                                                                                                                           ! i丙68))
                                                                                                                       (r_add (\ (i丙68,(i69,())) ->
                                                                                                                               nat_ 1))))))) $ \ summary67 ->
                                                                              case_ (case_ summary67
                                                                                           [branch (ppair PVar
                                                                                                          PVar)
                                                                                                   (\ y70
                                                                                                      z71 ->
                                                                                                    z71)])
                                                                                    [branch (ppair PVar
                                                                                                   PVar)
                                                                                            (\ y72
                                                                                               z73 ->
                                                                                             z73)]
                                                                              ! i58) +
                                                                    nat2prob i丣59 +
                                                                    summate (nat_ 0)
                                                                            (size word_prior1)
                                                                            (\ i丙74 ->
                                                                             word_prior1
                                                                             ! i丙74)))))))),
         branch pfalse (reject)]
