import dubins
import math 
import matplotlib.pyplot as plt

grid_size = (10, 10)
source = (1.0, 1.0, 0.0)
destination = (9.0, 9.0, 0.0)
q0 = (3.0, 4.0, 0.0)
q00 = (4.0, 4.0, math.pi/2)
q1 = (6.0, 8.0, 0.0)
q10 = (5.0, 8.0, math.pi/2)
turning_radius = 0.5
step_size = 0.1

target_x = [q0[0], q1[0]] 
target_y = [q0[1], q1[1]]
satellite_x = [q00[0], q10[0]]
satellite_y = [q00[1], q10[1]]

plt.rcParams["font.family"] = "Times New Roman"
plt.rc('text', usetex=True)

# plot the points
fig, ax = plt.subplots()

path = dubins.shortest_path(source, q0, turning_radius)
configurations, _ = path.sample_many(step_size)
path_x = [x for (x, y, theta) in configurations]
path_y = [y for (x, y, theta) in configurations]
path_x.append(q0[0])
path_y.append(q0[1])
ax.plot(path_x, path_y, c='black', linewidth=1.0, zorder=1)

path = dubins.shortest_path(q0, q1, turning_radius)
configurations, _ = path.sample_many(step_size)
path_x = [x for (x, y, theta) in configurations]
path_y = [y for (x, y, theta) in configurations]
path_x.append(q1[0])
path_y.append(q1[1])
ax.plot(path_x, path_y, c='black', linewidth=1.0, zorder=1)

path = dubins.shortest_path(q1, destination, turning_radius)
configurations, _ = path.sample_many(step_size)
path_x = [x for (x, y, theta) in configurations]
path_y = [y for (x, y, theta) in configurations]
path_x.append(destination[0])
path_y.append(destination[1])
ax.plot(path_x, path_y, c='black', linewidth=1.0, zorder=1)

ax.scatter(source[0], source[1], c='lightblue', edgecolor='k', linewidths=0.3, zorder=2)
ax.scatter(destination[0], destination[1], c='peachpuff', edgecolor='k', linewidths=0.3, zorder=2)
ax.scatter(target_x, target_y, c='lightgreen', edgecolor='k', linewidth=0.3, zorder=2)
plt.scatter(satellite_x, satellite_y, c='rosybrown', edgecolor='k', linewidth=0.3, zorder=2)

ax.text(source[0], source[1]+0.25, r'$s$')
ax.text(q0[0]-0.2, q0[1]+0.25, r'$t_1$')
ax.text(q00[0]+0.2, q00[1]+0.25, r'$\ell_1$')
ax.text(q1[0]-0.2, q1[1]+0.25, r'$t_2$')
ax.text(q10[0]-0.2, q10[1]+0.25, r'$\ell_2$')
ax.text(destination[0], destination[1]-0.5, r'$d$')

ax.grid(True, alpha=0.3, linewidth=0.2)
ax.set_xticklabels([])
ax.set_yticklabels([])

plt.figtext(0.6, 0.5, 'source', backgroundcolor='lightblue', color='black')
plt.figtext(0.6, 0.43, 'destination', backgroundcolor='peachpuff',color='black')
plt.figtext(0.6, 0.36, 'targets', backgroundcolor='lightgreen', color='black')
plt.figtext(0.6, 0.29, 'peripheral locations', backgroundcolor='rosybrown',color='black')
plt.savefig('illustration_1.pdf', format='pdf')


# plot the points
fig, ax = plt.subplots()

path = dubins.shortest_path(source, q0, turning_radius)
configurations, _ = path.sample_many(step_size)
path_x = [x for (x, y, theta) in configurations]
path_y = [y for (x, y, theta) in configurations]
path_x.append(q0[0])
path_y.append(q0[1])
ax.plot(path_x, path_y, c='black', linewidth=1.0, zorder=1)

path = dubins.shortest_path(q0, q00, turning_radius)
configurations, _ = path.sample_many(step_size)
path_x = [x for (x, y, theta) in configurations]
path_y = [y for (x, y, theta) in configurations]
path_x.append(q00[0])
path_y.append(q00[1])
ax.plot(path_x, path_y, c='black', linewidth=1.0, zorder=1)

path = dubins.shortest_path(q00, q1, turning_radius)
configurations, _ = path.sample_many(step_size)
path_x = [x for (x, y, theta) in configurations]
path_y = [y for (x, y, theta) in configurations]
path_x.append(q1[0])
path_y.append(q1[1])
ax.plot(path_x, path_y, c='black', linewidth=1.0, zorder=1)

path = dubins.shortest_path(q1, q10, turning_radius)
configurations, _ = path.sample_many(step_size)
path_x = [x for (x, y, theta) in configurations]
path_y = [y for (x, y, theta) in configurations]
path_x.append(q10[0])
path_y.append(q10[1])
ax.plot(path_x, path_y, c='black', linewidth=1.0, zorder=1)

path = dubins.shortest_path(q10, destination, turning_radius)
configurations, _ = path.sample_many(step_size)
path_x = [x for (x, y, theta) in configurations]
path_y = [y for (x, y, theta) in configurations]
path_x.append(destination[0])
path_y.append(destination[1])
ax.plot(path_x, path_y, c='black', linewidth=1.0, zorder=1)

plt.scatter(source[0], source[1], c='lightblue', edgecolor='k', linewidths=0.3, zorder=2)
plt.scatter(destination[0], destination[1], c='peachpuff', edgecolor='k', linewidths=0.3, zorder=2)
plt.scatter(target_x, target_y, c='lightgreen', edgecolor='k', linewidth=0.3, zorder=2)
plt.scatter(satellite_x, satellite_y, c='rosybrown', edgecolor='k', linewidth=0.3, zorder=2)

ax.text(source[0], source[1]+0.25, r'$s$')
ax.text(q0[0]-0.2, q0[1]+0.25, r'$t_1$')
ax.text(q00[0]+0.2, q00[1]+0.25, r'$\ell_1$')
ax.text(q1[0]-0.2, q1[1]+0.25, r'$t_2$')
ax.text(q10[0]-0.2, q10[1]+0.25, r'$\ell_2$')
ax.text(destination[0], destination[1]-0.5, r'$d$')

ax.grid(True, alpha=0.3, linewidth=0.2)
ax.set_xticklabels([])
ax.set_yticklabels([])

plt.figtext(0.6, 0.5, 'source', backgroundcolor='lightblue', color='black')
plt.figtext(0.6, 0.43, 'destination', backgroundcolor='peachpuff',color='black')
plt.figtext(0.6, 0.36, 'targets', backgroundcolor='lightgreen', color='black')
plt.figtext(0.6, 0.29, 'peripheral locations', backgroundcolor='rosybrown',color='black')
plt.savefig('illustration_2.pdf', format='pdf')