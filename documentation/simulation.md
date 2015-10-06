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

If you notice your robot getting lost, you may want to follow these steps to debug the issue:

1. Restart the simulator with the same seed, and skip to a frame just before your robot starts getting lost. You can do this by entering the frame number into the "Skip To" box in the World window, and then clicking the "Skip To" button. Note that if you restart the simulator it will automatically advance to the "Skip To" frame.
2. Pause the simulator and open the Log window. 
3. Select "Localization" from the dropdown in the Log window, advance the simulation a single frame, and then make sure you can see the log output from your robot for the current frame. Be sure you use logs of logging statements to help diagnose issues. You can narrow these down by using the log level ranges at the bottom right of the Log window. For example, if your logging statements look like this:

    ```cpp
    log(43, "Started processing at frame %i", frame_id);
    /* ... */
    log(45, "Position estimated at %2.2f,%2.2f", x, y);
    ```

  Then you can select the start and end log level numbers "43" and "45"
4. With the simulator still paused, continue advancing the simulation one frame at a time until you identify the frame that caused the localization error to occur. You can now make changes, recompile, and restart the simulation with this seed at this frame repeatedly until you solve the issue.

### Simulating without UI

You may want to run simulations without the UI loaded so that you can get a better sense of your algorithm's performance on a large number of random paths. You can do this by running the tool with the `-x` option and redirecting standard output to `/dev/null`:

    ```bash
    $NAO_HOME/build/tool/UTNaoTool -x 1>/dev/null
    ```
    
This will produce output similar to the following:

    ----------------------------------------------------------
    Default RMSE dist error: 7694.68, rot error: 115.33, steps: 2671
    Seed: 596516649, Avg dist: 9624.26, Avg rot: 106.95, Avg steps: 3322.44
    Sim time: 0.18 seconds
    ----------------------------------------------------------
    Default RMSE dist error: 10103.61, rot error: 114.34, steps: 3369
    Seed: 1189641421, Avg dist: 9672.19, Avg rot: 107.69, Avg steps: 3327.10
    Sim time: 0.18 seconds
    ----------------------------------------------------------
    Default RMSE dist error: 8891.12, rot error: 105.51, steps: 3143
    Seed: 1025202362, Avg dist: 9601.19, Avg rot: 107.49, Avg steps: 3310.36
    Sim time: 0.15 seconds
    ----------------------------------------------------------
    Default RMSE dist error: 9940.03, rot error: 111.29, steps: 3391
    Seed: 1350490027, Avg dist: 9629.42, Avg rot: 107.81, Avg steps: 3317.08
    Sim time: 0.16 seconds
    ----------------------------------------------------------
    Default RMSE dist error: 12170.36, rot error: 121.17, steps: 4168
    Seed: 783368690, Avg dist: 9824.88, Avg rot: 108.83, Avg steps: 3382.54
    Sim time: 0.19 seconds
    ----------------------------------------------------------

This will help you gauge the general effectiveness of your filter. If a particular path yields a large amount of error, you can inspect it by copying the seed into the Simulator window and running the localization simulator manually on the generated path.
