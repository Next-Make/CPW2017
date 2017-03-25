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

# Implementaton Details

Some of the code has been written specifically to interface properly with the
chip we are using, so unlike in Python prototyping, there are some parts of the
code that you should not touch or attempt to re-implement. To wit:

* The only method you should be modifying is `buttonTest()`. Do not attempt to
  modify other methods. Doing so could lead to unexpected results on the board.
* Alternatively, you can implement your method (with a different name instead of
  `buttonTest`). However, make sure that you do not call any methods like `write_LEDs`,
  `getButtons`, or `_delay_ms`.

  If you need to implement some kind of delay in your method, you can create variables
  that act as counters in which you exit immediately out of your function if your counter
  doesn't reach a certain number:

  ~~~c
  int COUNTER = 0;

  void your_function() {
      COUNTER++;

      if (COUNTER < 1000) {
          // Or some other cutoff value
          return;
      }
 
      COUNTER = 0;  // reset and continue...
      <your code here continues>
  }
  ~~~
