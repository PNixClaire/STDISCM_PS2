# STDISCM_PS2

## The PS2 folder contains the following files:
- config.txt
- dispatcher.h
- dispatcher.cpp
- dungeon.h
- dungeon.cpp
- main.cpp

## Example config.txt:
n=200 <br>
t=200 <br>
h=200 <br>
d=1000 <br>
t1=3 <br>
t2=10 <br>

- n --> number of dungeon instances 
- t --> tanks
- h --> healers
- d --> dps
- t1 --> minimum completion time for a dungeon
- t2 --> maximum completion time for a dungeon

### Note: Keep the config.txt in the same folder as the .exe

## Build/Compilation Instructions
1. Download and extract then navigate to the PS2 folder.
2. Open the command prompt
3. Navigate to the PS2 folder.
4. Compile all .cpp files in one command.

g++ main.cpp dispatcher.cpp dungeon.cpp -o lfg.exe -std=c++17 -Wall -pthread

4. Run the program, e.g., lfg.exe
