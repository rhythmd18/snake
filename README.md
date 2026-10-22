# Snake
Complete snake game logic built from scratch in C using the SDL3 library.

## Demo


https://github.com/user-attachments/assets/d9bbbd4e-2112-4b1f-827f-3edfcdeb4211



## Prerequisites
- A C/C++ compiler of your choice.
- [CMake](https://github.com/Kitware/CMake).
- That's it!

## Usage
- Clone the repo and `cd` into it
```bash
git clone https://github.com/rhythmd18/snake.git
cd snake
```

- Get the SDL3 library
```bash
git clone https://github.com/libsdl-org/SDL.git vendored/SDL
```

- Build
```bash
cmake -S . -B build
cmake --build build
```

- Run
  - For Windows
  ```
  cd build/Debug
  ./snake
  ```
  - For Linux/MacOS
  ```bash
  cd build
  ./snake
  ```

- Enjoy

---
PS since this isn't vibe coded and built from scratch, bugs might have crept in. Feel free to break it while playing. If so, put it up on the issues. Please 🙏
