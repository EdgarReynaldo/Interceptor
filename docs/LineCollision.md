# Line collisions #

To utilize lines properly you need to forget about everything you know, including y = mx + b, and you need to use the general first degree line equation described below.

```Ax +By + C = 0```

This equation describes every line in 2D that exists, ever. Not just the nice ones that aren't vertical like they taught you in school.

## Getting the equation of a line ##



### From two points ###

If we know two points that are distinct, we can form a line.

Since we know Ax + By + C = 0, we can form two equations for the line from those two points. We know have :

```
Ax2 + By2 + C = 0
Ax1 + By1 + C = 0
-------------------
A(x2 - x1) + B(y2 - y1) + C - C = 0
```

If we call (x2 - x1) dx, and we call (y2 - y1) dy, we have :

```
A(dx) + B(dy) = 0
A(dx) = B(-dy)
```
If we assign A the value -dy and B the value dx, then both sides of the equation agree : 
```
A = -dy
B = dx
-dy(dx) + dx(dy) = 0
```

This means we can now solve for C using one of the points : 
```
C = -Ax - By
C = -(-dy)x1 - (dx)y1
C = (y2 - y1)x1 - (x2 - x1)y1
```

We now have an equation for A, B, and C given points x1,y1 and x2,y2 : 
```
A = -(y2 - y1)
B = (x2 - x1)
C = (y2 - y1)x1 - (x2 - x1)y1
```
### From a point and an angle ###

Now we have a single point on the line, x1,y1, and an angle of the line in radians, alpha.

From the unit circle, we know that the y difference is sin(alpha) and the x difference is cos(alpha). We can simply replace dx and dy with these directly to get the formula : 
```
dx = cos(a)
dy = sin(a)
A = -sin(a)
B = cos(a)
C = (sin(a))x1 - (cos(a)y1
```

## Line vs Line intercept ##

Given two lines in general first degree form, we can solve for their intersection point, if any exists.

Say we have line 1, Ax + By + C = 0, and line 2, Dx + Ey + F = 0. We can solve for x or y by multiplying one equation by a certain factor.
```
   D(Ax + By + C) = D(0)
  -A(Dx + Ey + F) = A(0)
------------------------
      0 + (BD - AE)y + CD - AF = 0

y = (AF-CD)/(BD-AE)
            
   E(Ax + By + C) = E(0)
  -B(Dx + Ey + F) = B(0)
------------------------
   (AE-BD)x + CE - BF = 0
   
x = (CE-BF)/(BD-AE)
```

You'll notice that the expression BD-AE is common to both x and y. If this value equals 0.0, the lines do not have a well defined intersection point. They are either parallel, or the same line. If BD-AE is non-zero, you will get a single value for x and y. This is the intersection point.


### Line segment overlap ###

Now that we know the intersection point of two lines, we can find whether or not two line segments overlap.

Find the equations of the lines and solve for x and y. If BD-AE is zero, they don't overlap. Otherwise, they intersect at x,y.

However, we have to find out whether or not x,y is on the line segment. We can do a simple rectangle vs point overlap check.

```
bool collision =  !(x > x2 || y > y2 || x < x1 || y < y1);
```
Where x and y are the point of intersection, and x1,y1 is the top left coordinate, and x2,y2 is the bottom right coordinate of the line segment.

### Line segment intercept ###

This makes it a little trickier, since we now have to factor in the velocity vector.


## Line vs Circle Intercept ##





