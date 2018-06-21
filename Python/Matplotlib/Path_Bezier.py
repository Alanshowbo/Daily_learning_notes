import matplotlib.pyplot as plt
from matplotlib.path import Path
import matplotlib.patches as patches

verts = [
    (0., 0.),  # P0
    (0.2, 1.), # P1
    (1.0, 1.0), # P2
    (1.5, 1.0), # P3
    (1.7, 2.0) # P4
    ]

codes = [Path.MOVETO,
         Path.CURVE3,
         Path.CURVE3,
         Path.CURVE3,
         Path.CURVE3,
         ]

path = Path(verts, codes)
print (path.get_extents())
fig = plt.figure()
ax = fig.add_subplot(111)
patch = patches.PathPatch(path, facecolor='none', lw=2)
ax.add_patch(patch)

xs, ys = zip(*verts)
print(*verts)
ax.plot(xs, ys, 'x--', lw=2, color='black', ms=10)

ax.text(-0.05, -0.05, 'P0')
ax.text(0.15, 1.05, 'P1')
ax.text(1.05, 0.85, 'P2')
ax.text(1.55, 1.05, 'P3')
ax.text(1.75, 1.05, 'P4')

ax.set_xlim(-0.1, 2.)
ax.set_ylim(-0.1, 2.5)
plt.show()