## Circle vs Circle collision detection routines ##

Collision detection can be a very complex issue. But by reducing it to component parts, we can make calculating collisions much easier. Simple overlap checks can miss state, but can often be 'good enough' to get the job done, given small delta time between collision checks. Otherwise objects can easily pass through one another. However, they are simple, and effective for the general case. An improvement is intercept based collision detection, where an actual intercept time is calculated. These intercept times can be stored in a table, and then the only time you need to update the table is when time passes or an object changes direction. This means the more collisions there are, the more calculation will be needed. But the end result is that objects behave correctly under all circumstances, and you can optimize the checking routine to only run when an object changes direction.


## Overlap check ##

Circle overlap collision detection is by far the easiest and most straight forward collision detection method there is besides AABB (axis aligned bounding box) collision detection.

The basic algorithm is : 
```if (c1->FutureCircle(dt).overlaps(c2->FutureCircle(dt)) {collision();}```

The calculations are straight forward. We find the position of each circle in the future after time dt has passed.
```
Circle fc1 = c1;
Circle fc2 = c2;
fc1.Update(dt);
fc2.Update(dt);
double dy = fc2.y - fc1.y;
double dx = fc2.x - fc1.x;
double dsq = dx*dx + dy*dy;
double rsq = (r1 + r2);
rsq *= rsq;
dsq -= rsq;
if (dsq <= 0.0) {
   collision();
}
```

However, there are problems with this method. It is only finding if there is a collision at a certain time in the future. It doesn't take into account any time before or after this time. If the dt (delta time) is too large, collisions can be skipped. If dt is too small, it may be a waste of cpu or even a bottleneck in your collision code.



## Intercept check ##

We can use this method to find the exact time when the two circles would collide, using basic math.

First we find the relative velocity and position of ball c2 to ball c1. You just subtract the velocity and position of ball c1 from both balls. This leaves ball c1 at (0,0) with a relative velocity of (0,0), and ball c2 at (dx,dy) with relative velocity (dvx , dvy).

```
double dx = c2.x - c1.x;
double dy = c2.y - c1.y;
double dvx = c2.vx - c1.vx;
double dvy = c2.vy - c1.vy;
```

Now that we know the component velocity and position of ball c2 relative to ball c1, we can compute their relative position at any time t.

The equations for ball c2 are as follows :

```C(x) = dx + dvx*t```

```C(y) = dy + dvy*t```

The distance is determined by the square root of the sum of C(x)^2 + C(y)^2. We can square both sides to make it easier.

```D^2(x,y) = C^2(x) + C^2(y) = (R1 + R2)^2```

```C^2(x) = dx^2 + 2*dx*dvx*t + dvx^2*t^2```

```C^2(y) = dy^2 + 2*dy*dvy*t + dvx^2*t^2```

Adding them together and separating out terms of powers of t gives you :
```
(dvx^2 + dvy^2)t^2 + (2*(dx*dvx + dy*dvy))*t^1 + (dx^2 + dy^2)t^0 = (r1 + r2)^2
(dvx^2 + dvy^2)t^2 + (2*(dx*dvx + dy*dvy))*t^1 + (dx^2 + dy^2 - r1^2 - 2*r1*r2 - r2^2)t^0 = 0
```
And we now have the form of a quadratic equation in t.

The solution for 

```A*t^2 + B*t^1 + C*t^0 = 0 ```

is :

```t = (-B +/- sqrt(B^2 - 4*A*C))/2*A```

So now A,B, and C are :

```A = dvx^2 + dvy^2```

```B = 2*(dx*dvx + dy*dvy)```

```C = dx^2 + dy^2 - r1^2 - 2*r1*r2 - r2^2```

When you solve for t you get 0, 1, or 2 values for time.

The discriminant of the equation is B^2 - 4*A*C. If this value is less than zero, then there are no real roots to the equation, and you can be happy that a collision will never occur between these two circles given their current position and velocity.

If the discriminant equals zero, then you have a single point of contact. Evaluate t and if it is a negative value, then this collision happenened in the past, and you can safely ignore it.

If the discriminant is positive and non zero, then you have two possible collisions, one at time (-B + sqrt(DISCRIMINANT)) and one at time (-B - sqrt(DISCRIMINANT)). The first positive collision time gives you the earliest possible time of intercept, and that is the value you use for your collision detection. If both are negative, you can safely say that this collision happened in the past, and you can discard it.

## Collision intercept time table ##

Now, in order to make use of our newly found collision time, we need to store all of our possible collisions in a table. There are exactly (N^2 + N)/2 possible collision pairs between N circles, given that A collide B is the same as B collide A and we don't need to test for A collide A.

