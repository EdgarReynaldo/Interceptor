# Collision Detection #

Collision detection can be a very complex issue. But by reducing it to component parts, we can make calculating collisions much easier. Simple overlap checks can miss state, but can often be 'good enough' to get the job done, given small delta time between collision checks. Otherwise objects can easily pass through one another. However, they are simple, and effective for the general case. An improvement is intercept based collision detection, where an actual intercept time is calculated. These intercept times can be stored in a table, and then the only time you need to update the table is when time passes or an object changes direction. This means the more collisions there are, the more calculation will be needed. But the end result is that objects behave correctly under all circumstances, and you can optimize the checking routine to only run when an object changes direction.

## Two dimensional collision detection ##
[[Collision Detection 2D]]

## Three dimensional collision detection ##
[[Collision Detection 3D]]

