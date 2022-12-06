# What the code does
It runs an infinity loop.  
Inside the loop, the code first gets the user input and renders the canvas.  

# How to use it
The game was written in C.  
To compile it, run  

```bash
make install
```

Then a file called `game` will be generated.  
The `game` takes two parameters, which are `[width]` and `[height]`. They are the width and height of the canvas of the game.  
You can run the game like this:  
```bash
./game [width] [height] 
```
For example, you can run the game with a canvas has a width of 20 and a height of 10 like this:
```bash
./game 20 10
```

Inside the game, you can:  
Use lower case w, a, s, d to move.  
Use lower case p to switch the pen between up and down.(Yellow X for up, and green X for down)  
Use c to clean the screen.  
Use q to quit the game.

# Troubleshooting
If there is no `make` in your system, you can run this to install:
```bash
sudo apt-get install make
```

If there is no `gcc` in your system, you can run this to install:
```bash
sudo apt-get install gcc
```

If you do not have the permission to run the game, you can try this:
```bash
chmod +x game
```
