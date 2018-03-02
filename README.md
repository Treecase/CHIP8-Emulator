  
A [CHIP-8](https://en.wikipedia.org/wiki/CHIP-8) virtual machine/emulator.  
  
  
Specs:  
  
* Memory: 4096 bytes, of which the first 512 contain the character graphics.  
  
* Registers: 16 8-bit registers, labeled V0-VF. VF is used as a flag for some instructions. There is also a memory register, I, which is 16 bits.  
  
* Stack: The stack has 16 levels.  
  
* Timers: These count down by 1 at 60hz, until 0. The Delay timer can be both read and set. The Sound timer emits sound until it reaches 0.  
  
* Input: CHIP-8 uses a keypad with 16 keys, corresponding to 0-F in hexadecimal. Usually, 8, 4, 6, and 2 are used for directional control. The keypad is laid out like so: (with default keybindings on the right)  
  
```
	1 2 3 C  ->  7 8 9 *  
	4 5 6 D  ->  4 5 6 -  
	7 8 9 E  ->  1 2 3 +  
	A 0 B F  ->  R 0 . E  
```
  
	(Note: R = right arrow, E = enter. All keys are on the keyboard keypad)  
  
* Graphics: The display is 64x32 pixels monochrome. Sprites are 8 pixels wide, and between 1 and 15 pixels tall.  

