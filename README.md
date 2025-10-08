# What's this?
This is a simple "flappy bird" clone running on an ATmega32U4 and rendered to a common 16x2 LCD 
module. It only requires one button to play the game which controls the player character to move 
up or down on the LCD. Random pipes/blocks are generated and scroll in from the right to left and 
it's up to the player to dodge these obstacles.


## How to run?
To run you need the following:
* A commong 16x2 LCD module
* A single button
* An ATmega32U4 microprocessor (will probably run on others but haven't tested)  

I tested on an Olimexino32U4 so maybe get that?

With the above, just clone and setup with the following commands
```sh
git clone https://github.com/willofdaedalus/flappy.git
cd flappy
make flash
```

The above commands will clone and flash the game to your device. 
