import numpy as np
import matplotlib.pyplot as plt
### X, Y设置
X = np.linspace(-np.pi, np.pi, 256, endpoint=True)
C, S = np.cos(X), np.sin(X)
### 绘图区域、曲线颜色、粗细、格式指定
plt.figure(figsize=(10,6), dpi=100) #设置绘图区域大小、dpi
plt.plot(X, C, color="blue", linewidth=2.5, linestyle="-", label="cosine")
plt.plot(X, S, color="red",  linewidth=2.5, linestyle="-", label="sine")
### 设置坐标范围
plt.xlim(X.min()*1.1, X.max()*1.1) 
plt.ylim(C.min()*1.1, C.max()*1.1) 
### 设置坐标值
plt.xticks([-np.pi, -np.pi/2, 0, np.pi/2, np.pi],
       [r'$-\pi$', r'$-\pi/2$', r'$0$', r'$+\pi/2$', r'$+\pi$'])
plt.xticks()
plt.yticks([-1, 0, +1],
       [r'$-1$', r'$0$', r'$+1$'])
#plt.yticks(np.linspace(-1.2, 1.2, 5))
### 移动曲线位置
ax = plt.gca()
ax.spines['right'].set_color('none')
ax.spines['top'].set_color('none')
ax.xaxis.set_ticks_position('bottom')
ax.spines['bottom'].set_position(('data',0))
ax.yaxis.set_ticks_position('left')
ax.spines['left'].set_position(('data',0))
### 添加 legend
plt.legend(loc='upper left', frameon=False) #plt.plot()需指定label参数
### 细节问题，坐标值字号等
for label in ax.get_xticklabels() + ax.get_yticklabels():
    label.set_fontsize(16)
    label.set_bbox(dict(facecolor='white', edgecolor='None', alpha=0.65 ))

plt.show()