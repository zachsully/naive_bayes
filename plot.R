library(ggplot2)
library(tidyr)
library(dplyr)
library(extrafont)

suppressMessages(loadfonts())
dataFile <- "nbtimes.csv"
data <- read.csv(dataFile, header=T)

pd <- position_dodge(0.1)

ci_calc <- function(sd, n, conf.lev = .95) {
    sd/sqrt(n) * qt(conf.lev/2 + .5, n - 1)
}

dataT <- data %>%
    gather(Time.var, Time, Init.time, Update.time) %>%
    group_by(System,DocSize,Time.var) %>%
    summarise_each(funs(mean,
                        sd,
                        se=sd(.)/sqrt(n()),
                        ci=ci_calc(sd(.),n())),
                   Time) %>%
    rename(Time=mean)

dataT$System <- paste(dataT$System, dataT$Time.var, sep=".")
dataT$Time.var <- NULL

data <- data %>%
    group_by(System,DocSize) %>%
    summarise_each(funs(mean,
                        sd,
                        se=sd(.)/sqrt(n()),
                        ci=ci_calc(sd(.),n())),
                   Acc) %>%
    rename(Acc=mean)

theming <- theme_bw() +
        #ggtitle("Run times for Gaussian Mixture Model") +
        #expand_limits(x=0,y=0) +                        # Expand y range
        #scale_y_continuous(expand = c(0, 0), limits= c(0,4)) +
        ##scale_y_continuous(breaks=0:20*4) +         # Set tick every 4
    
        theme(panel.grid.major = element_line(colour = "black", size=0.15)) +
        theme(panel.grid.minor = element_blank()) +
        theme(panel.grid.major.x = element_blank()) +
        theme(panel.border = element_rect(colour = "black", fill=NA, size=1)) +
        theme(text = element_text(family="Times")) +
        theme(plot.title = element_text(size = rel(2))) +
        #theme(plot.margin = margin(0.5, 1, 0.5, 1, "cm")) +
        theme(axis.title.y = element_text(size = rel(1.5), angle = 90)) +
        theme(axis.title.x = element_text(size = rel(1.5))) +
        theme(axis.text.y = element_text(size = rel(1.5), angle = 90)) +
        theme(axis.text.x = element_text(size = rel(1.5), hjust = 1)) +
        theme(legend.title = element_text(size = rel(1.5))) +
        theme(legend.text = element_text(size = rel(1.3))) +
        theme(legend.background = element_rect(fill = "transparent")) +
    
        theme(legend.justification=c(0.02,1.0),
              legend.position=c(0.02,1.0))               # Position legend in bottom right

pAcc <- ggplot(data, aes(x=DocSize, y=Acc, colour=System, group=System)) +
        geom_errorbar(aes(ymin=Acc-se, ymax=Acc+se),
                      colour="black", width=.1, position=pd) +
        geom_line(position=pd) +
        #geom_point(position=pd, size=3, shape=21, fill="white") + # 21 is filled circle
        xlab("Data size (documents)") +
        ylab("Accuracy (%)") +
        geom_point(aes(shape=System), size=3) +
        scale_shape(name="") +
        scale_color_hue(name="", l=40) +
        scale_x_continuous(expand = c(0, 0)) +
        scale_y_continuous(expand = c(0, 0),
                           limits = c(0, 0.9)) +
        theming   

ggsave("nb_plot.pdf", pAcc) # width=4, height=3.5)
embed_fonts("nb_plot.pdf", outfile="plots/nbplotacc.pdf")

timing.fields = c("JAGS.Init.time", "JAGS.Update.time",
                  "Hakaru.Init.time", "Hakaru.Update.time")
timing.labels = c("JAGS + initialization",
                  "JAGS",
                  "Hakaru + initialization",
                  "Hakaru")

pT <-   ggplot(dataT, aes(x=DocSize, y=Time, colour=System, group=System)) +
        geom_errorbar(aes(ymin=Time-se, ymax=Time+se),
                    colour="black", width=.1, position=pd) +
        geom_line(position=pd) +
        #geom_point(position=pd, size=3, shape=21, fill="white") + # 21 is filled circle
        xlab("Data size (documents)") +
        ylab("Run time (seconds)") +
        geom_point(aes(shape=System), size=3) +
        scale_shape(name="",    # Legend label, use darker colors
                    breaks=timing.fields,
                    labels=timing.labels) +
        scale_color_hue(name="",    # Legend label, use darker colors
                        breaks=timing.fields,
                        labels=timing.labels,
                        l=40) +
        scale_x_continuous(expand = c(0, 0)) +
        scale_y_continuous(expand = c(0, 0),
                           limits = c(0, 10000)) +
        theming   


ggsave("nb_plot.pdf", pT) # width=4, height=3.5)
embed_fonts("nb_plot.pdf", outfile="plots/nbplottimes.pdf")
