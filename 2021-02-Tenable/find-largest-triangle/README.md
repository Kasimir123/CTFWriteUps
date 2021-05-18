# Tenable Find Largest Triangle  Write Up

## Details:
Points: 125

Jeopardy style CTF

Category: Code

Comment:

Below is some starter code that reads in 3D points (x,y,z) from stdin and calls FindLargestTriangleArea. You just need to impliment the "FindLargestTriangleArea" function. This function should find the largest triangle area (2D) you can make using any three points. The function simply takes a list of 3D points and returns the area of the largest triangle found. The code then prints that result rounded to the nearest whole number.

``` python
# points is a list of 3D points
# ie: [[2, 9, -15], [0, 33, -20], ...]
def FindLargestTriangleArea(points):
  # return largest area

# Reading space delimited points from stdin
# and building list of 3D points
points_data = raw_input()
points = []
for point in points_data.split(' '):
  point_xyz = point.split(',')
  points.append([int(point_xyz[0]), int(point_xyz[1]), int(point_xyz[2])])

# Compute Largest Triangle and Print Area rounded to nearest whole number
area = FindLargestTriangleArea(points)
print int(round(area))
```

stdin example:

```
-21,59,-93 -4,91,-2 1,61,2, 0,44,1
```

stdout example:

```
2241
```

You can ignore the default code below and instead use code above. Once function is implimented, simply copy/paste into box and run. Ensure not to print anything else during your code execution!

## Write up:

This challenge was made using python 2 so I created the following python script, comments here are better than an explanation after so thats what I did:

``` python
import math

# Gets the area based on 3 3D points using the following formula:
# A=0.5*|(B-A)X(C-A)|
def area(points):
  A = points[0]
  B = points[1]
  C = points[2]

  BA = [B[0]-A[0], B[1]-A[1], B[2]-A[2]]
  CA = [C[0]-A[0], C[1]-A[1], C[2]-A[2]]

  cross = [BA[1]*CA[2] - BA[2]*CA[1], BA[2]*CA[0] - BA[0]*CA[2], BA[0]*CA[1] - BA[1]*CA[0]]

  return (abs(math.sqrt((cross[0]**2)+(cross[1]**2)+(cross[2]**2)))*0.5)

# points is a list of 3D points
# ie: [[2, 9, -15], [0, 33, -20], ...]
def FindLargestTriangleArea(points):
  # return largest area

  res = 0

  # Gets length of all points
  N = len(points)

  # loop through all combination of points
  for i in range(N-2):
    for j in range(i+1, N-1):
      for k in range(i+2, N):
        x = []
        x.append(points[i])
        x.append(points[j])
        x.append(points[k])
        res = max(res, area(x))

  return res

# Reading space delimited points from stdin
# and building list of 3D points
points_data = raw_input()
points = []
for point in points_data.split(' '):
  point_xyz = point.split(',')
  points.append([int(point_xyz[0]), int(point_xyz[1]), int(point_xyz[2])])

# Compute Largest Triangle and Print Area rounded to nearest whole number
area = FindLargestTriangleArea(points)
print(int(round(area)))
```