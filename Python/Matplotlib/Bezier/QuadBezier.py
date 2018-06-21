import os
import numpy as np
from geomdl import BSpline
from geomdl import utilities
import matplotlib.pyplot as plt
from matplotlib.lines import Line2D

p = [[0.0, 0.0],[0.2, 1.0], [0.5, 1.0] ,[0.8, 1.0] ,[1.0, 2.0]] #p包含Bezier曲线的控制点
p_array = np.array(p)

### 
def getQuadBezier(P0, P1, P2, Degree=2, nSample =50):
    """
    返回一个QuadBezier对象, P0 P1 P2为控制点，Degree大于2可能报错，nSample为采样数目
    """
    curve = BSpline.Curve()
    curve.ctrlpts = [P0, P1, P2]
    curve.degree = Degree
    curve.knotvector = utilities.generate_knot_vector(curve.degree, len(curve.ctrlpts))
    curve.sample_size = nSample
    return curve

### 返回Bezier曲线上的离散点
def getCurveXY(curve):
    """
    返回Bezier曲线上的离散点
    :return curve_x, curve_y
    """
    curve_x = np.array(curve._curve_points)[:,0]
    curve_y = np.array(curve._curve_points)[:,1]
    return curve_x, curve_y

curve1 = getQuadBezier(p[0], p[1], p[2])
curve1.evaluate() #通过evaluate生成曲线

curve2 = getQuadBezier(p[2], p[3], p[4])
curve2.evaluate()

curve1_X, curve1_Y = getCurveXY(curve1)
curve2_X, curve2_Y = getCurveXY(curve2)

fig = plt.figure(figsize=(8,6), dpi=100)
ax = fig.add_subplot(111)

ax.plot(curve1_X, curve1_Y, '-', lw=2, color='red')
ax.plot(curve2_X, curve2_Y, '-', lw=2, color='blue')

for i in range(len(p)):
    ax.plot(p_array[i,0], p_array[i,1], 'x--', lw=2, color='black', ms=10)
for i in range(len(p)-1):
    ax.add_line(Line2D((p_array[i,0],p_array[i+1,0]), (p_array[i,1],p_array[i+1,1]), linewidth=1, linestyle="--",color='black'))

ax.text(-0.05, -0.05, 'P0')
ax.text(0.15, 1.05, 'P1')
ax.text(0.5, 0.85, 'P2')
ax.text(0.85, 1.0, 'P3')
ax.text(0.95, 2.0, 'P4')

plt.show()