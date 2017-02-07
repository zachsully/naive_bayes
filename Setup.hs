import Distribution.Simple
import Distribution.Simple.LocalBuildInfo
import Distribution.Simple.PreProcess
import Distribution.Simple.Program.Types
import Distribution.PackageDescription
import System.Exit
import System.FilePath
import System.Process

main :: IO ()
main = defaultMainWithHooks
  $ simpleUserHooks { hookedPreProcessors = [("hk",ppHakaru)] }

ppHakaru :: BuildInfo -> LocalBuildInfo -> PreProcessor
ppHakaru _ lbi = PreProcessor
  { platformIndependent = True
  , runPreProcessor = mkSimplePreProcessor $ \inFile outFile verbosity ->
      do putStrLn (unwords ["Compiling Hakaru:","(",inFile,",",outFile,")"])
         callPPHakaru inFile outFile
  }
  where callPPHakaru :: String -> String -> IO ()
        callPPHakaru inFile outFile =
          do exitCode <- rawSystem "compile" [inFile,"-o",outFile,"-M",takeBaseName outFile]
             case exitCode of
               ExitSuccess       -> return ()
               f@(ExitFailure _) -> fail $ "compile: " ++ show f
