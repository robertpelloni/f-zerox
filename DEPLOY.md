# Deployment & Build Instructions

## Prerequisites
*   **Compiler:** GCC (MinGW for Windows, standard GCC for Linux).
*   **Libraries:** SDL2 (Development Headers).
*   **Tools:** Python 3.10+ (for asset scripts, optional).

## Building
1.  **Clone:** `git clone --recursive ...`
2.  **Dependencies (Linux):** `sudo apt-get install libsdl2-dev`
3.  **Build:**
    ```bash
    make -f Makefile.pc
    ```
4.  **Run:**
    ```bash
    ./build/pc/fzerox_pc
    ```

## Modding
*   Place `.obj` files in `mods/` (e.g., `mods/blue_falcon.obj`) to replace the player machine.
*   Textures can be placed in `textures/` (Feature In-Progress).

## Troubleshooting
*   **Missing Audio:** Ensure SDL2 Audio is configured.
*   **Black Screen:** Press 'V' to toggle View/Camera modes (if implemented) or check console for GBI errors.
*   **Input:** Default keys are Arrow Keys (Steer), Z (Gas), X (Boost/Brake).

## Environment Setup
- To compile the PC port, ensure you have GCC, Python 3.10+, and SDL2 development headers installed (`sudo apt-get install libsdl2-dev` on Debian/Ubuntu).
- Note: The current codebase has known conflicts between the N64 SDK headers and standard PC headers.

## Environment Variables
- `FZX_API_KEY`: (Placeholder) Add any necessary API keys here in the future.
- `FZX_SERVER_PORT`: (Placeholder) Port for the Netplay server, default 7000.
