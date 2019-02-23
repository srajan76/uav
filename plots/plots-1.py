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

num_targets = [25, 50, 75, 100]
num_scenarios = [20, 40, 60, 80, 100]
p_nom = 0.5

f = open('results_nom.csv', 'r')
reader = csv.reader(f)
rows = []
for row in reader:
    rows.append(row)


lb_variation = {20 : [], 40 : [], 60 : [], 80 : [], 100 : [] }
ub_variation = {20 : [], 40 : [], 60 : [], 80 : [], 100 : [] }
lb = {20 : [], 40 : [], 60 : [], 80 : [], 100 : [] }
ub = {20 : [], 40 : [], 60 : [], 80 : [], 100 : [] }
rel_gap = {20 : [], 40 : [], 60 : [], 80 : [], 100 : [] }
lb_rel_gap = {20 : [], 40 : [], 60 : [], 80 : [], 100 : [] }
ub_rel_gap = {20 : [], 40 : [], 60 : [], 80 : [], 100 : [] }

for i in range(1, len(rows)):
    row = rows[i]
    lb[20].append(float(row[1]))
    lb_variation[20].append(float(row[2]))
    ub[20].append(float(row[3]))
    ub_variation[20].append(float(row[4]))
    gap = ub[20][-1] + ub_variation[20][-1] - lb[20][-1] + lb_variation[20][-1]
    if (gap > 0.0):
        rel_gap[20].append(gap/(lb[20][-1] - lb_variation[20][-1]) * 100)
    else:
        rel_gap[20].append(0.0)
    lb_rel_gap[20].append(float(row[26]))
    ub_rel_gap[20].append(float(row[27]))
    
    
    lb[40].append(float(row[6]))
    lb_variation[40].append(float(row[7]))
    ub[40].append(float(row[8]))
    ub_variation[40].append(float(row[9]))
    gap = ub[40][-1] + ub_variation[40][-1] - lb[40][-1] + lb_variation[40][-1]
    if (gap > 0.0):
        rel_gap[40].append(gap/(lb[40][-1] - lb_variation[40][-1]) * 100)
    else:
        rel_gap[40].append(0.0)
    lb_rel_gap[40].append(float(row[28]))
    ub_rel_gap[40].append(float(row[29]))
    
    lb[60].append(float(row[11]))
    lb_variation[60].append(float(row[12]))
    ub[60].append(float(row[13]))
    ub_variation[60].append(float(row[14]))
    gap = ub[60][-1] + ub_variation[60][-1] - lb[60][-1] + lb_variation[60][-1]
    if (gap > 0.0):
        rel_gap[60].append(gap/(lb[60][-1] - lb_variation[60][-1]) * 100)
    else:
        rel_gap[60].append(0.0)
    lb_rel_gap[60].append(float(row[30]))
    ub_rel_gap[60].append(float(row[31]))
    
    lb[80].append(float(row[16]))
    lb_variation[80].append(float(row[17]))
    ub[80].append(float(row[18]))
    ub_variation[80].append(float(row[19]))
    gap = ub[80][-1] + ub_variation[80][-1] - lb[80][-1] + lb_variation[80][-1]
    if (gap > 0.0):
        rel_gap[80].append(gap/(lb[80][-1] - lb_variation[80][-1]) * 100)
    else:
        rel_gap[80].append(0.0)
    lb_rel_gap[80].append(float(row[32]))
    ub_rel_gap[80].append(float(row[33]))
    
    lb[100].append(float(row[21]))
    lb_variation[100].append(float(row[22]))
    ub[100].append(float(row[23]))
    ub_variation[100].append(float(row[24]))
    gap = ub[100][-1] + ub_variation[100][-1] - lb[100][-1] + lb_variation[100][-1]
    if (gap > 0.0):
        rel_gap[100].append(gap/(lb[100][-1] - lb_variation[100][-1]) * 100)
    else:
        rel_gap[100].append(0.0)
    lb_rel_gap[100].append(float(row[34]))
    ub_rel_gap[100].append(float(row[35]))

plt.rcParams["font.family"] = "Times New Roman"
plt.rc('text', usetex=True)

# gap reduction instances plot
fig, ax = plt.subplots()
bplot_1 = ax.boxplot([lb_variation[i] for i in num_scenarios], notch=0, positions=[1,3,5,7,9], sym='+', widths=0.5,  vert=True, patch_artist=True)
set_box_color(bplot_1, 'lightgreen', '-')

ax.set_xlim(0, 10)
labels = [r'$m=20$', r'$m=40$', r'$m=60$', r'$m=80$', r'$m=100$']
ax.set_xticklabels(labels)
ax.set_ylabel(r'Deviation in the point estimate of the lower bound')
ax.set_xticks([1, 3, 5, 7, 9])
plt.savefig('lb_variation.pdf', format='pdf')

# average lb deviation and ub deviation
print("\\toprule")
print("$|T|$ & Avg. Rel. LB gap (\%) & Avg. Avg. Rel. UB gap (\%)")
print("\\midrule")
for i in range(0, len(num_targets)):
    t = num_targets[i]
    lb_gap = lb_rel_gap[20][i*10:(i+1)*10]
    ub_gap = ub_rel_gap[20][i*10:(i+1)*10]
    lb_gap += lb_rel_gap[40][i*10:(i+1)*10]
    ub_gap += ub_rel_gap[40][i*10:(i+1)*10]
    lb_gap += lb_rel_gap[60][i*10:(i+1)*10]
    ub_gap += ub_rel_gap[60][i*10:(i+1)*10]
    lb_gap += lb_rel_gap[80][i*10:(i+1)*10]
    ub_gap += ub_rel_gap[80][i*10:(i+1)*10]
    lb_gap += lb_rel_gap[100][i*10:(i+1)*10]
    ub_gap += ub_rel_gap[100][i*10:(i+1)*10]
    mean_lb_gap = sum(lb_gap)/len(lb_gap)
    mean_ub_gap = sum(ub_gap)/len(ub_gap)
    print("{:d} & {:.2f} & {:.2f}\\\\".format(t, mean_lb_gap, mean_ub_gap))
print("\\bottomrule")

# gap reduction plot
fig, ax = plt.subplots()
bplot_1 = ax.boxplot([rel_gap[i][0:10] for i in num_scenarios], notch=0, positions=[1,6,11,16,21], sym='+', widths=0.5,  vert=True, patch_artist=True)
set_box_color(bplot_1, 'lightgreen', '-')

bplot_2 = ax.boxplot([rel_gap[i][10:20] for i in num_scenarios], notch=0, positions=[2,7,12,17,22], sym='+', widths=0.5,  vert=True, patch_artist=True)
set_box_color(bplot_2, 'lightblue', '-')

bplot_3 = ax.boxplot([rel_gap[i][20:30] for i in num_scenarios], notch=0, positions=[3,8,13,18,23], sym='+', widths=0.5,  vert=True, patch_artist=True)
set_box_color(bplot_3, 'peachpuff', '-')

bplot_4 = ax.boxplot([rel_gap[i][30:40] for i in num_scenarios], notch=0, positions=[4,9,14,19,24], sym='+', widths=0.5,  vert=True, patch_artist=True)
set_box_color(bplot_4, 'lightgray', '-')

ax.set_xlim(0, 25)
labels = [r'$m=20$', r'$m=40$', r'$m=60$', r'$m=80$', r'$m=100$']
ax.set_xticklabels(labels)
ax.set_ylabel(r'Relative Gap Estimate (\%)')
ax.set_xticks([2.5, 7.5, 12.5, 17.5, 22.5])
plt.figtext(0.65, 0.79, r'$|T|=25$', backgroundcolor='lightgreen', color='black')
plt.figtext(0.65, 0.72, r'$|T|=50$', backgroundcolor='lightblue',color='black')
plt.figtext(0.65, 0.65, r'$|T|=75$', backgroundcolor='peachpuff', color='black')
plt.figtext(0.65, 0.58, r'$|T|=100$', backgroundcolor='lightgray',color='black')
plt.savefig('opt_gap.pdf', format='pdf')
