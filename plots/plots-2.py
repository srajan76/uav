import matplotlib.pyplot as plt
import numpy as np
import csv
import math

def set_box_color(bp, color, ls):
    plt.setp(bp['boxes'], color='black', ls=ls, lw=1)
    plt.setp(bp['whiskers'], color='black', lw=1)
    plt.setp(bp['caps'], color='black', lw=1)
    plt.setp(bp['medians'], lw=1)
    for patch, c in zip(bp['boxes'], color):
        patch.set_facecolor(color)

p = [0.2, 0.5, 0.8]
num_targets = [25, 50, 75, 100]

f = open('detvsstoch.csv', 'r')
reader = csv.reader(f)
rows = []
for row in reader:
    rows.append(row)

det = {0.2 : [], 0.5 : [], 0.8 : []}
stoch = {0.2 : [], 0.5 : [], 0.8 : []}
improvement = {0.2 : [], 0.5 : [], 0.8 : []}

for i in range(1, len(rows)):
    row = rows[i]
    det[0.2].append(float(row[4]))
    det[0.5].append(float(row[5]))
    det[0.8].append(float(row[6]))
    stoch[0.2].append(float(row[1]))
    stoch[0.5].append(float(row[2]))
    stoch[0.8].append(float(row[3]))
    improvement[0.2].append(float(row[7]))
    improvement[0.5].append(float(row[8]))
    improvement[0.8].append(float(row[9]))


plt.rcParams["font.family"] = "Times New Roman"
plt.rc('text', usetex=True)

fig, ax = plt.subplots()
bplot_1 = ax.boxplot([improvement[i][0:10] for i in p], notch=0, positions=[1,6,11], sym='+', widths=0.5,  vert=True, patch_artist=True)
set_box_color(bplot_1, 'lightgreen', '-')

bplot_2 = ax.boxplot([improvement[i][10:20] for i in p], notch=0, positions=[2,7,12], sym='+', widths=0.5,  vert=True, patch_artist=True)
set_box_color(bplot_2, 'lightblue', '-')

bplot_3 = ax.boxplot([improvement[i][20:30] for i in p], notch=0, positions=[3,8,13], sym='+', widths=0.5,  vert=True, patch_artist=True)
set_box_color(bplot_3, 'peachpuff', '-')

bplot_4 = ax.boxplot([improvement[i][30:40] for i in p], notch=0, positions=[4,9,14], sym='+', widths=0.5,  vert=True, patch_artist=True)
set_box_color(bplot_4, 'lightgray', '-')

ax.set_xlim(0, 15)
labels = [r'$p=0.2$', r'$p=0.5$', r'$p=0.8$']
ax.set_xticklabels(labels)
ax.set_ylabel(r'Relative improvement in objective (\%)')
ax.set_xticks([2.5, 7.5, 12.5])
plt.figtext(0.3, 0.79, r'$|T|=25$', backgroundcolor='lightgreen', color='black')
plt.figtext(0.3, 0.72, r'$|T|=50$', backgroundcolor='lightblue',color='black')
plt.figtext(0.3, 0.65, r'$|T|=75$', backgroundcolor='peachpuff', color='black')
plt.figtext(0.3, 0.58, r'$|T|=100$', backgroundcolor='lightgray',color='black')
plt.savefig('vss.pdf', format='pdf')
