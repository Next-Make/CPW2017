# Implementing Real Games

So you've done the prototyping and are ready to implement on the board.
We'll assume that you have the board and AVR coder on hand, so we'll walk
you through what you need to do on your computer. You will need:

* **avr-gcc**
* **avrdude**
* **avr-objcopy**
* **avr-libc** (AVR libraries)

These can be easily installed on Linux with the `apt` package manager.
You can similarly do so on a Mac with `Homebrew`. As usual, installing packages
like these is a struggle on Windows. However, you can find the packages on
`SourceForge` and the `Atmel` website.

Once you have all of these libraries and packages installed, you can get
started by running the following command:
```
./upload.sh example
```

To run your own game (we'll call it `your_game.c`), you can run it as follows:
```
./upload.sh your_game
```

The example code provides a lot of useful functions transliterated from
`example.py` for prototyping your own games. As with the prototyping code, if
you think a function you wrote could be of use for others, feel free to add it
to `example.c`!
