# STDISCM_PS2

# The PS2 folder contains the following files:
- config.txt
- dispatcher.h
- dispatcher.cpp
- dungeon.h
- dungeon.cpp
- main.cpp

## Example config.txt:
n=200
t=200
h=200
d=1000
t1=3
t2=10

- n --> number of dungeon instances 
- t --> tanks
- h --> healers
- d --> dps
- t1 --> minimum completion time for a dungeon
- t2 --> maximum completion time for a dungeon

### Note: Keep the config.txt in the same folder as the .exe

## Build/Compilation Instructions
1. Open the command prompt
2. Navigate to the variant folder.
3. Compile all .cpp files in one command.

g++ main.cpp dispatcher.cpp dungeon.cpp -o lfg.exe -std=c++17 -Wall -pthread

4. Run the program, e.g., lfg.exe
