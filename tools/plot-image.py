#!/usr/bin/env python

from mpl_toolkits.mplot3d import Axes3D
import matplotlib as mpl
import matplotlib.pyplot as plt
import matplotlib.image as mpimg
import numpy as np
import sys

def clamp_rgb(v):
    def clamp(x):
        return np.maximum(0, np.minimum(1, x))

    return np.array([clamp(v[0]), clamp(v[1]), clamp(v[2])])

def normalized(x):
    return x / np.linalg.norm(x)

def power_method(a):
    b = normalized(np.array([1, 1, 1]))
    for i in range(0, 20):
        b = normalized(np.dot(a, b))
    return b

def projection(line, p):
    k, m = line
    return np.dot(p-m, k.T) / np.vdot(k, k)

def param(line, ts):
    k, m = line
    if isinstance(ts, float) or isinstance(ts, int):
        return k*ts + m
    else:
        return np.array([param(line, t) for t in ts])

def drop_alpha(image):
    width = image.shape[0]
    height = image.shape[1]
    return image[:,:,0:3].reshape(width*height, 3)

def colors_to_tuples(image):
    return [tuple(color) for color in image]

def linear_regression(points):
    x = points[:,0]
    y = points[:,1]
    z = points[:,2]

    n = x.size
    print("n =", n, "\n")

    assert x.shape == (n,)
    assert y.shape == (n,)
    assert z.shape == (n,)

    sum = np.array([np.sum(x), np.sum(y), np.sum(z)])
    print("sum =", sum, "\n")

    m = [sum[0]/n, sum[1]/n, sum[2]/n]
    print("m =", m, "\n")

    w = points - m
    print("w =", w, "\n")

    a = (1.0/n)*np.dot(w.T, w)
    print("a =", a, "\n")

    k = np.linalg.svd(a)[0][:,0]
    print("k =", k, "\n")

    return (k, m)

def bounding_box(points):
    x = points[:,0]
    y = points[:,1]
    z = points[:,2]

    a = np.array([min(x), min(y), min(z)])
    b = np.array([max(x), max(y), max(z)])
    print("a =", a, "\n")
    print("b =", b, "\n")

    m = a
    k = b-a
    return k, m

def rescale_line(line, points):
    ts = projection(line, points)

    p0 = param(line, min(ts))
    p1 = param(line, max(ts))

    print("p0 =", p0)
    print("p1 =", p1)
    print()

    new_k = p1 - p0
    new_m = p0

    return new_k, new_m

def plot_colors(ax, colors, label, marker):
    unique = np.array(sorted(set(colors)))
    ax.scatter(unique[:,0], unique[:,1], unique[:,2], color=unique, label=label, marker=marker)

def plot_line(ax, line, label):
    k, m = line

    ts = np.linspace(0, 1, 100)
    xyz = param(line, ts)
    x = xyz[:,0]
    y = xyz[:,1]
    z = xyz[:,2]
    ax.plot(x, y, z, label=label)

def main():
    image = drop_alpha(mpimg.imread(sys.stdin.buffer) / 255.0)

    print("colors =", image)
    print()

    line_linreg = rescale_line(linear_regression(image), image)
    line_bounding = bounding_box(image)

    proj_linreg = clamp_rgb(param(line_linreg, projection(line_linreg, image)))
    proj_bounding = clamp_rgb(param(line_bounding, projection(line_bounding, image)))

    fig = plt.figure()
    fig.suptitle("Endpoint Estimation")
    ax = fig.gca(projection='3d')
    plot_colors(ax, colors_to_tuples(image), "texels", "o")
    plot_colors(ax, colors_to_tuples(proj_linreg), "projected linear regression", "+")
    plot_colors(ax, colors_to_tuples(proj_bounding), "projected bounding box", "s")
    plot_line(ax, line_linreg, "linear regression")
    plot_line(ax, line_bounding, "bounding box")
    ax.set_xlabel('red')
    ax.set_ylabel('green')
    ax.set_zlabel('blue')
    ax.legend()
    plt.show()

np.set_printoptions(threshold=np.nan)
main()
