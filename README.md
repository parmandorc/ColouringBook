# SecondGame


I was responsible for implementing the physics that would allow the colour stains to expand once they touch the floor. 

The implementation followed a simplified version of the Diffusion methods in fluid dynamics simulations, which are most commonly based on the Navier-Stokes equations. Named after Claude-Louise Navier and Gabriel Stokes, this series of equations describe the motion of viscous fluid by applying Newtons’ Second Law, and they supply a precise mathematical model for most fluids occurring in Nature.
In a famous paper presented at the Game Developers Conference in 2003, Joe Stamm of Alias Systems Corporation came up with a simplified version of those equations, that could be adapted to real time simulations in videogames.

Below are Joe Stamm’s versions of the Navier-Stokes equations: on top is the equation for the velocity in a compact vector location, while on the bottom is the equation for density moving through the velocity field.


![alt text](https://github.com/parmandorc/ColouringBook/blob/Fede_Physics/formula.png)







![alt text](https://github.com/parmandorc/ColouringBook/blob/Fede_Physics/spread_diffuse.png)
