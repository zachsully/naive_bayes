#!/usr/bin/env python

import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

df = pd.read_csv("experiments.csv")
a = df['Time'].groupby([df['Alg'],df['Docs'],df['VocabSize']]).mean()
b = a.reset_index()
#b.plot.scatter(x='Docs',y='Time')
#plt.show()

df2 = pd.read_csv("experiments2.csv")
a2 = df2['Time'].groupby([df2['Alg'],df2['Docs'],df2['VocabSize']]).mean()
b2 = a2.reset_index()
b2.plot.scatter(x='VocabSize',y='Time')
plt.show()
