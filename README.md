# TrafficLights
Description: in this project, you will implement an intelligent
traffic light system where cars arrive at a "one-way only" tunnel.
Your program should enforce the requirement that "only at most N
cars in one direction can go through the tunnel at a time".
The goal of the project is to learn multi-threading with locks and
condition variables.

For implementation, you need to have variables for traffic lights
on both North and South directions. Lights can be RED or GREEN only.
If the light turns RED, no car should attempt to enter the tunnel.
Otherwise, N cars should be allowed to enter and leave the tunnel.
You may either have one thread per car, in this case, N threads need
to be woken up when the light turns GREEN. Or you may have one thread
per direction and then manage to make N cars leave the tunnel in a
sequence.

Cars should arrive at a random time with an average interval
of 60 seconds. To achieve this, you may do random*120, where
the random number is within (0,1). Or you can do rand()%121.
It should serve the same purpose.

Please take account of the time for cars to leave the tunnel,
e.g., 60 seconds, as well as the time for a car to start and
follow another car, e.g., 5 seconds. Such time can be implemented
through a sleep() function. Your program should calculate the total
time taken for M cars in both directions together to pass the tunnel.

Your program should display the following events on screen:

-Thread creation
-When light changes for one directions
-Periodic print out of time taken so far
