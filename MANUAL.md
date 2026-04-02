# F-Zero X PC Port Manual

## Introduction
Welcome to the F-Zero X PC Port. This project aims to recreate the high-speed anti-gravity racing experience of the original N64 title, enhanced with modern PC features, 30-player multiplayer, and a track editor.

## Controls
### Keyboard
*   **Arrow Keys**: Steering (Left/Right) and Pitch (Up/Down)
*   **Z**: Accelerate
*   **X**: Boost (Requires Energy)
*   **A / S**: Strafe Left / Right (Slide Turn)
*   **Space**: Drift / Slide
*   **Esc**: Menu / Pause

### Gamepad (SDL2)
*   **Left Stick**: Steering
*   **A Button**: Accelerate
*   **B Button**: Boost
*   **L / R Triggers**: Strafe / Drift

## Gameplay Mechanics
### Energy & Boost
Your energy meter serves as both your shield and your boost fuel.
*   **Boosting**: Press Boost to gain a burst of speed. This consumes energy.
*   **Recovery**: Drive over pink recharge strips (Pit Areas) to restore energy.
*   **Combat**: Colliding with walls or enemies drains energy. If energy reaches zero, you explode (Retire).

### Advanced Tech
*   **Side Attack**: Double tap Strafe (L/R) to ram enemies. (Planned)
*   **Spin Attack**: Hold L+R and steer to spin. (Planned)
*   **MTS (Momentum Turbo Slide)**: Strafe in one direction while steering in the opposite to build boost reserves. (GX Mechanic - Experimental)

## Track Editor
The built-in Track Editor allows you to create custom circuits.
*   **Ribbons**: Create standard roads with variable width and banking.
*   **Tubes**: Create 360-degree tunnels. (WIP)
*   **Loops**: Vertical loops are supported.

## Network Play
The game supports 30-player LAN multiplayer via UDP Broadcast.
1.  Navigate to the **Netplay** tab.
2.  Enter your Pilot Name.
3.  Click **Start/Join** to begin broadcasting your position.
4.  Other players on the same network will automatically appear in your race.

## Modding
*   **Custom Machines**: Place `.obj` files in `mods/machines/` to replace the default Blue Falcon.
*   **Textures**: Textures can be hot-loaded from `mods/textures/`.

## Debugging
*   Press **F1** or access the **Debug** tab to view physics visualization and performance metrics.
