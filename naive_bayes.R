#!/usr/bin/env Rscript

suppressMessages(library('rjags'))
suppressMessages(library('coda'))
suppressMessages(library('assertthat'))
suppressMessages(library('MASS'))

ascending <- function (x) all(diff(x) >= 0)

scan.file <- function (f, suffix) {
    scan(paste(f, suffix, sep="."), quiet=TRUE) + 1
}

args = commandArgs(trailingOnly=TRUE)
if (length(args) != 2) {
  cat("naive_bayes.R <docsPerTopic> <trial>\n")
} else {

docsPerTopic <- as.numeric(args[1])
trial        <- as.numeric(args[2])

topics <- scan.file("topics", docsPerTopic)
words  <- scan.file("words",  docsPerTopic)
docs   <- scan.file("docs",   docsPerTopic)

invisible(assert_that(ascending(topics)))
invisible(assert_that(ascending(docs)))

docsSize  <- length(topics)
topicSize <- length(unique(topics))
vocabSize <- length(unique(words))

# We take a subset of the smaller dataset to use as
# a test set
trainTestSplit <- fractions(9/10)
testDocsPerTopic <- ceiling(docsPerTopic * (1 - trainTestSplit))
topicIndices <- c(sapply(0:(topicSize-1),
                         function(i)
                           (docsPerTopic*i+1):(docsPerTopic*i+testDocsPerTopic)))

zTrues <- topics[topicIndices]
topics[topicIndices] <- NA

jags <- jags.model('naive_bayes.jags',
                   data = list('Nwords'     = length(words),
                               'Ndocs'      = docsSize,
                               'Ntopics'    = topicSize,
                               'Nvocab'     = vocabSize,
                               'onesTopics' = rep(1,topicSize),
                               'onesVocab'  = rep(1,vocabSize),
                               'z'          = topics,
                               'w'          = words,
                               'doc'        = docs),
                   n.chains = 1,
                   n.adapt = 10,
                   quiet=TRUE)

start.time <- Sys.time()

update(jags, 1);

samples <- jags.samples(jags, c('z'), 1);
zPredicts <- samples$"z"[topicIndices]

end.time <- Sys.time()
duration <- difftime(end.time, start.time, units="sec")

accuracy <- length(zTrues[zPredicts == zTrues])/length(zTrues)

cat("JAGS",
    as.numeric(docsSize),
    format(trial),
    format(accuracy),
    as.numeric(duration),
    sep=",")
cat(",")

## print(zPredicts)
## print(zTrues)

}
