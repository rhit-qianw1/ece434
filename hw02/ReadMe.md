# Buttons and LEDs
Done

# Measuring a gpio pin on an Oscilloscope
## Bash
1. Max voltage: 2.45 V, Min voltage: 0 V
2. Period: 236 ms, Frequency: 4.23 Hz
3. It is not close to 100 ms.
4. It may because there are multiple processes running in background. The system needs to swap between processes. Also, it takes time to call system calls.
5. I am using 3.2 processes
6. 
| option | period | process usage |
| ---- | ------ | ------------- |
| 0.1 | 236ms | 3.2% |
| 0.01 | 56ms | 17.1% |
| 0.001 | 36ms | 21.3% |
7. Not very stable
8. Still not stable
9. The period is a little bit shorter.
10. The period is a little bit shorter.
11. 34ms is the shortest 

## Python
1. Shortest period: 152us, frequency: 6579Hz
2. I am using 1.1 cpu processes
3. 
| time | bash period | python period | bash process usage | python process usage usage |
| ---- | ------ | ------------- | --- | --- |
| 0.1 | 236ms | 203ms | 3.2% | 1.1%
| 0.01 | 56ms | 22ms | 17.1% | 5.7%
| 0.001 | 36ms | 2.3ms| 21.3% | 31.1%

## C
1. Shortest period: 7us, frequency: 143KHz
2. I am using 0.3 cpu processes
3. 
| time | bash | python | C | C lseek | bash | python | C | C lseek
| ---- | ------ | ------------- | --- | --- | --- | --- | --- | --- |
| 0.1 | 236ms | 203ms | 201ms | 200.6ms |  3.2% | 1.1% | 0.3% | 0.3%
| 0.01 | 56ms | 22ms | 21ms | 20.4ms | 17.1% | 5.7% | 2.9% | 2.9%
| 0.001 | 36ms | 2.3ms | 2.1ms | 2.1ms | 21.3% | 31.1% | 13.2% | 12.1%

It is faster than the one does not use lseek

# GPIOD

|    | toggle1.c | toggle1.py | toggle2.c | toggle2.py |
| --- | --- | --- | --- | --- |
| lowest period | 3.5us | 19.8us | 3.8us | 21.3us |  
| process usage | 100% | 100% | 100% | 100% 

# Security
1. Change port from 22 to 2022: Done
3. Use fail2ban to reject ssh connections for 15 seconds after 2 failed attempts: Done

# Etch-a-sketch
Done


# What the code does
It runs an infinity loop.  
Inside the loop, the code first gets the user input and renders the canvas.  

# How to use it
The game was written in C.  
To compile it, run  

```bash
make game
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

(Note: If the canvas size is too big, the window may not fit in all the content. So, if you encounter some weired output, try a smaller canvas size, or enlarge your window.) 

Inside the game, you can:
Use w, a, s, d to move.  
Use w + a to switch the pen between up and down.(Yellow X for up, and green X for down)  
Use w + s to clean the screen.  
Use w + d to quit the game.  
(w is the first button from left to right. a is the second button from left to right. s is the third button from left to right. d is the fourth button from left to right.)

# Troubleshooting
If there is no `make` in your system, you can run this to install:
```bash
sudo apt-get install make
```

If there is no `g++` in your system, you can run this to install:
```bash
sudo apt-get install g++
```

If you do not have the permission to run the game, you can try this:
```bash
chmod +x game
```
