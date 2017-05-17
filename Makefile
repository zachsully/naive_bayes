SANDBOX_PATH = $(shell find .cabal-sandbox -name "*-packages.conf.d")
SANDBOX_OPTIONS = -no-user-package-db -package-db $(SANDBOX_PATH)

all: data-file summary compile nb_simp.c nb_simp_bucket.c
	gcc -O3 -c gibbs.c gibbsBucket.c
	ghc -O2 gibbs.o gibbsBucket.o --make NBHakaru.hs # Haskell and C
	ghc Baseline.hs -O2 -o nb # Baseline
	ghc -O2 GetNews.hs
	ghc -O2 NaiveBayesMain.hs -o NaiveBayes

all-sandbox: data-file summary nb_simp.c
	#gcc -O3 -g -c gibbs.c
	ghc -O2 gibbs.o $(SANDBOX_OPTIONS) NBHakaru.hs # Haskell and C
	ghc Baseline.hs -O2 $(SANDBOX_OPTIONS) -o nb # Baseline
	ghc -O2 GetNews.hs $(SANDBOX_OPTIONS)
	ghc -O2 NaiveBayesMain.hs $(SANDBOX_OPTIONS) -o NaiveBayes

data-file:
	bash ./download-data.sh

summary: nb_simp.hk
	summary nb_simp.hk -o GibbsOptBucket.hs -M GibbsOptBucket --logfloat-prelude

compile: nb_simp.hk
	compile nb_simp.hk -o Gibbs.hs -M Gibbs --logfloat-prelude

nb_simp.hk:
	simplify naive_bayes_gibbs.hk > nb_simp.hk
	perl unsample.pl nb_simp.hk > nb_simp2.hk
	mv nb_simp2.hk nb_simp.hk

nb_simp.c: nb_simp.hk
	hkc -F gibbsC nb_simp.hk -o nb_simp.c

nb_simp_bucket.c: nb_simp.hk
	hkc -SF gibbsCBucket nb_simp.hk -o nb_simp_bucket.c; perl -i -pe "s/logSumExp/logsumexp/g;" nb_simp_bucket.c

clean:
	rm NBHakaru nb *.o *.hi nb_simp.hk nb_simp.c nb_simp_bucket.c *.core NaiveBayes GetNews words.* docs.* topics.* plots/*.pdf

distclean: clean
	rm -rf 20news-19997.tar.gz 20_newsgroups

run:
	sh run.sh

plot:
	Rscript plot.R

%.hs : %.hk
	compile $<

%.core : %.hs
	ghc -O2 -ddump-simpl -dverbose-core2core -dppr-cols200 $< nb_simp.o > $@
