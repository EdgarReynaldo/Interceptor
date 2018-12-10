# Point collisions #

## Point vs Point##

This is a degenerate case of the circle vs circle collion where r1 and r2 are both zero.

See [[Circle Collision 2D]]

## Point vs Line Segment ##

If we have a line segment LS composed of points P1 and P2, and a third point P3, we can find when point P3 crosses the line segment.

If we make two lines, one from the line segment, and one from the third point along with its direction of relative travel, we can do a simple intersection check.

First we find consider the line to be stationary, and the point to be moving. To do this, we subtract the velocity of the line from the velocity of both the line and the point. This makes the velocity of the line segment 0, and the velocity of the point VP - VL.

See [[Line Collision 2D]] for details.

We get this for the first equation :
```
A = -dy = P1y - P2y
B = dx  = P2x - P1x
C = -dx*P1y + dy*P1x
```
```
D = -dy = -Vy
E = dx  = Vx
F = -Vx*P3y + VyP3x
```

We know the point of intersection from setting the two equations equal.
```x = (CE-BF)/(BD-AE)```
```y = (AF-CD)/(BD-AE)```

If BD-AE is zero, then the point is traveling parallel to the same line as the line segment. This means that it will hit end on with the line segment. 

The equation of the point is :
```P(x,y) = P3 + (VP - VL)*t```


The equation of the line segment is 

## Point vs Triangle ##
