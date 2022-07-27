import numpy as np
import matplotlib.pyplot as plt
import matplotlib.lines as mlines
import matplotlib.transforms as mtransforms
import statistics

  
# x axis values
x = list(range(1, 21))
# corresponding y axis values
y1 = [4.7874e+06,2.51811e+06,1.65871e+06,1.25336e+06,1.0043e+06,828354,697936,749709,686250,729940,693674,666156,687221,685776,654989,638645,670819,630220,657510,659130]
y2 = [4.78471e+06,2.39721e+06,1.60096e+06,1.19768e+06,961271,800275,694573,672404,680903,636489,660505,723495,662480,658614,631599,643456,634397,611148,621505,654835]
y3 = [4.78363e+06,2.40209e+06,1.59694e+06,1.19615e+06,957009,803053,684859,687029,720491,749374,718747,643106,690072,660185,664807,643565,638734,615113,644956,615915]
y4 = [4.78241e+06,2.40578e+06,1.5979e+06,1.19663e+06,958445,808022,684446,599147,676118,722308,687052,618593,662029,647978,631462,653494,632653,643973,619814,652745]
y5 = [4.78439e+06,2.39806e+06,1.59476e+06,1.19898e+06,959585,798330,686981,681849,675760,723488,691680,653053,708879,666256,656671,628764,624490,650144,614687,620888]

avgY = [statistics.mean(k) for k in zip(y1, y2, y3, y4, y5)]

T1 = avgY[0]

speedUp = [T1 / x for x in avgY]


fig, ax = plt.subplots()
ax.scatter(x, avgY , c='black')
line = mlines.Line2D([3, 2], [0,2], color='red')
transform = ax.transAxes
line.set_transform(transform)
ax.add_line(line)
plt.show()
plt.xlabel("Number of Threads")
plt.ylabel("Time (Microseconds)")

# function to show the plot
plt.show()
plt.savefig('foo.png')


fig, ax = plt.subplots()
ax.scatter(x, speedUp , c='black')
line = mlines.Line2D([3, 2], [0,2], color='red')
transform = ax.transAxes
line.set_transform(transform)
ax.add_line(line)
plt.show()
plt.xlabel("Number of Threads")
plt.ylabel("Speed Up")

# function to show the plot
plt.show()
plt.savefig('foo1.png')