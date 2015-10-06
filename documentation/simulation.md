# Running simulations

* * *

The UTNaoTool provides a number of simulation options for debugging various aspects of the codebase. In particular, the simulator is a good choice for testing and debugging high level behaviors or localization.

* * *

### Localization Simulator

This section describes the purpose and basic features of the localization simulation. To run a simulation, follow these steps:

1. Run the UTNaoTool and open the World window.
2. Select `LocalizationSim` from the dropdown.
3. (Optional) select a seed - this will be used to generate a pseudo-random path for your robot to follow.
4. Click the play button.

The robot will now move from point to point through the environment. It will pass simulated odometry readings and vision observations with random noise added through the memory blocks to the core localization module. The text area will show RMSE error in terms of distance from your robot's belief to the ground truth, as well as in terms of rotational error.

In the OpenGL window you will see a white robot moving around - this is the ground truth robot. Similarly, the orange ball represents the ground truth ball. All solid, static field objects are ground truth as well.

Your robot's belief is represented with a blue robot, a blue ball, and translucent field objects representing your robot's projected observations.
