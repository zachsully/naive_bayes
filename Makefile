SANDBOX_PATH = $(shell find .cabal-sandbox -name "*-packages.conf.d")
SANDBOX_OPTIONS = -no-user-package-db -package-db $(SANDBOX_PATH)

all: data-file nb_simp.hk nb_simp.c nb_simp_bucket.c
	gcc -O3 -c gibbs.c
	gcc -O3 -c gibbsBucket.c
	summary nb_simp.hk -o GibbsOptBucket.hs -M GibbsOptBucket --logfloat-prelude
	ghc -O2 gibbs.o gibbsBucket.o --make NBHakaru.hs # Haskell and C
	ghc Baseline.hs -O2 -o nb # Baseline

all-sandbox: data-file nb_simp.hk nb_simp.c nb_simp_bucket.c
	gcc -O3 -g -c gibbs.c
	gcc -O3 -c gibbsBucket.c
	summary nb_simp.hk -o GibbsOptBucket.hs -M GibbsOptBucket --logfloat-prelude
	ghc -O2 gibbs.o gibbsBucket.o $(SANDBOX_OPTIONS) NBHakaru.hs # Haskell and C
	ghc Baseline.hs -O2 $(SANDBOX_OPTIONS) -o nb # Baseline

data-file:
	bash ./download-data.sh

nb_simp.hk:
	simplify naive_bayes_gibbs.hk > nb_simp.hk

nb_simp.c: nb_simp.hk
	hkc -F gibbsC nb_simp.hk -o nb_simp.c

nb_simp_bucket.c: nb_simp.hk
	hkc -SF gibbsCBucket nb_simp.hk -o nb_simp_bucket.c

clean:
	rm NBHakaru nb *.o *.hi nb_simp.hk nb_simp.c *.core

run:
	sh run.sh
	Rscript naive_bayes.R

%.hs : %.hk
	compile $<

%.core : %.hs
	ghc -O2 -ddump-simpl -dverbose-core2core -dppr-cols200 $< nb_simp.o > $@
