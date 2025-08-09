# Space Invaders

A **Space Invaders** clone developed in **C** using **SDL2** and **SDL2_ttf**.

## How to set up and run

This guide provides a way to have everything properly set up and ready to use. 
Make sure you have a C compiler installed (GCC).

---

### 1. Compile the game

Make sure you are in the project’s root folder, where all `.c` and `.h` files are located.

#### Windows (MSYS2/MinGW)
```
gcc main.c intro.c menu.c game.c scores.c render.c -I SDL2-2.30.10/include -L SDL2-2.30.10/lib/x64 -lSDL2 -lSDL2_ttf -o main.exe
```

### 2. Run the game

#### Windows
```
./main.exe
```

### 3. Controls

- **← / →** : Move the ship

- **SPACE** : Shoot

### 4. Notes

- **cour.ttf** must be in the same folder as the executable for text rendering.

- **scores.txt** is created automatically and updated with your best scores.

- On Windows, ensure **SDL2.dll** and **SDL2_ttf.dll** are available (same folder as the .exe or in your PATH).