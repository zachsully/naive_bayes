#!/usr/bin/env python3

import pandas as pd

data = pd.read_csv("gibbstimes.csv")
g = data.groupby(['Lang', 'Docs', 'Categories', 'Vocab'])

print("Mean results:")
print(g.mean())

print("Std-dev results:")
print(g.std())
