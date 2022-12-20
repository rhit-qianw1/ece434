# What the code does

## read_temperature.bash
It reads the temperature using i2c from TMP101 using bash 

## read_temperature.py
It reads the temperature using i2c from TMP101 using python 

## read_with_temperature_limits.py
It reads the temperature and set the alarm limit using i2c from TMP101 using python. If the temperature is in the limit range, it will print the temperature.

## Main.cpp
The Etch-a-sketch game main source file.

# How to use it
The game was written in C.  
To compile it, run  

```bash
make game
```

Then a file called `game` will be generated.  

You can run the game like this:  
```bash
./game
```

Inside the game, you can:  
Use buttons in GPIO 31, 5, 30, 115 to move.  
Use button in GPIO 111 to make the pen up or down.  
Use button in GPIO 110 to clean the screen.  

# Troubleshooting
If there is no `make` in your system, you can run this to install:
```bash
sudo apt-get install make
```

If there is no `gcc` in your system, you can run this to install:
```bash
sudo apt-get install g++
```

If you do not have the permission to run the game, you can try this:
```bash
chmod +x game
```