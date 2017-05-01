# Colouring Book - Physics - Federico Soncini


I was responsible for implementing the physics that would allow the colour stains to expand once they touch the floor. 

The implementation followed a simplified version of the Diffusion methods in fluid dynamics simulations, which are most commonly based on the Navier-Stokes equations. Named after Claude-Louise Navier and Gabriel Stokes, this series of equations describe the motion of viscous fluid by applying Newtons’ Second Law, and they supply a precise mathematical model for most fluids occurring in Nature.
In a famous paper presented at the Game Developers Conference in 2003, Joe Stamm of Alias Systems Corporation came up with a simplified version of those equations, that could be adapted to real time simulations in videogames.

Below are Joe Stamm’s versions of the Navier-Stokes equations: on top is the equation for the velocity in a compact vector location, while on the bottom is the equation for density moving through the velocity field.


![alt text](https://github.com/parmandorc/ColouringBook/blob/Fede_Physics/formula.png "Navier Stokes equations")


The state of a fluid at a given instant is mathematically modeled as a velocity vector field, as a function that assigns a velocity vector to every point in space. The motion of particles that produces the effect of movement is obtained by converting the velocities surrounding the object into body forces.

The functionality is included in the ColouringBookCanvas.cpp and ColouringBookCanvas.h files. The DiffuseInk() function checks the entire canvas for colours - in our program the canvas is defined as the white portion of the screen that can be coloured by shooting at the puppets. The canvas is checked for every single pixel at the positions i + 1, i -1, i + (j+1) and i + (j-1), where i represents pixels on the horizontal axis, and j on the vertical axis. If we call these adjacents as “next pixels”, we can check if one of those next pixels has been sprayed by colour (and we can also determine which colour by checking the playerID). If a pixel has colour, we can funnel that pixel into a an array of pixels to diffuse (IPixelsToDiffuse and JPixelsToDiffuse), which are going to be the pixels we are going to apply the diffusion methods on.
Once we have obtained those array of Pixels, we call for every pixel the function ColorPixel, which taxes as arguments the pixels numbers, the player ID, and the alpha value of the colour we are going to apply to the diffusion.
The ColorPixel function is a modified version of the previous PostInitializeComponents(), where we obtain the desired dissolving effect by tweakign the alpha value of each pixel.


![alt text](https://github.com/parmandorc/ColouringBook/blob/Fede_Physics/spread_diffuse.png "Screenshot from the Game") 
