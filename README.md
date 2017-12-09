# Graphic Equalizer Display
Drew Dunne (asd222), Jacob Glueck (jng55)

![](https://d2mxuefqeaa7sj.cloudfront.net/s_285E986361B89230ED0EA68AF8109650F6D162A938C4084A862406B4E6962E0C_1495121133721_Screen+Shot+2017-05-18+at+11.25.10+AM.png)

# Introduction

Our final project is an audio spectrum analyzer which reads in audio from a standard 3.5 mm audio port and displays amplitudes of 7 frequency bands in a bar chart on a 16x32 LED matrix display. The project is self contained, and powered by a 5V wall adapter. Plug in the power, and then plug in a phone or computer and start playing music! The display will have a bar chart representing each frequency band’s amplitude. The display also has different color modes. Switch between modes with a red button on the side. The first few modes are gradient bars where as the bar gets taller, the higher parts change color. The remaining are solid colors, and a rainbow mode. Our driver can drive the display at 12 bit color (4 bits per channel), allowing for a full range of colors and beautiful gradients. The frequency analysis is performed by a MSGEQ7 integrated circuit.

https://youtu.be/SGRItKWKfTU

# System Design and Diagram
![Block Diagram of System](https://d2mxuefqeaa7sj.cloudfront.net/s_285E986361B89230ED0EA68AF8109650F6D162A938C4084A862406B4E6962E0C_1495072114803_block_diagram.png)


Our system works has four main components: the main loop, the display driver, the MSGEQ7 driver, and the button. Each component is described below.


- **Display**: The display component consists of the graphic equalizer display driver as well as the underlying LED matrix driver. 
  - `led_matrix` : This class interfaces directly with the GPIO pins connected to the display. Once initiated, the begin function is called which creates an interrupt timer at 15 kHz. The driver supported 12-bit color, and refreshed the entire display at 117 Hz, providing a fluid picture. The timer interrupts make it easy for us the run the refreshes with precise timing, and to easily run them concurrently with the rest of the project.
  - `graphic_equalizer` : This class uses the `led_matrix` class to provide a simple interface for drawing bars. It provides simple functions for initiating the matrix and maintains an array of the current bar heights. Setting a bar is accomplished with a single function: `setBarHeight(GraphicEq_t *geq, size_t bar, coord_t height,` `M``atrixColor_t *color)` . 
- **MSGEQ7**: This component uses interrupts to interface with the chip and read its output. This component is represented by the `msgeq7` class which behaves similarly to the others. It has a begin function which starts the timers. The ISR acts as a state machine due to the way the chip outputs the data. It reads 7 bands, then resets the chip and starts again. To read each band, it pulses the strobe line 7 times, one for each channel. After each strobe it does an analog read. The interrupts come allow us run this entire system, with the appropriate delays between each pulse, concurrently with the rest of the system.
- **Button**: The button component is represented by the `button` class, which manages an ISR on the rising edge of a specified input pin. When the button is pushed, the ISR is called activates and changes the color scheme. The interrupts are helpful here because we don’t have to pull, and can keep this code out of the main loop. 
- **Main**: The main process initiates and begins each of the other three components. Then it enters an infinite while loop where it reads the data stored in the chip state, then changes the color based on the data (only for the gradient ones). It formats the data to be in the range of 0 to 15 (max height for a bar) and then sets the bar height.

We chose to split up the components this way because it is very object-oriented (as far as objects go in C). Each component (besides Main) is represented by a struct and then all the methods take the struct as their first argument. This is a fairly standard convention for using objects in C. With this method, we had clean, simple, interfaces for interacting with all the components. Writing the main loop was easy as it was simply calling each of our methods.

# Hardware Description

The main external components were a LED dot matrix display, and an integrated circuit which performed the frequency analysis. Below is bill of materials:

| Quantity | Part Number       | Manufacturer Part Number | Description                      | Unit Price | Extended Price |
| -------- | ----------------- | ------------------------ | -------------------------------- | ---------- | -------------- |
| 1        | CP-SJ1-43502PM-ND | SJ1-43502PM              | CONN JACK 3COND 3.5MM PNL MT     | 2.27       | $2.27          |
| 1        | 1528-1412-ND      | 420                      | LED MATRIX PANEL MED RGB 16X32   | 24.95      | $24.95         |
| 1        | EJ508A-ND         | EJ508A                   | CONN PWR JACK 2.1X5.5MM SOLDER   | 0.83       | $0.83          |
| 1        | 1568-1335-ND      | COM-10468                | GRAPHIC EQUALIZER DISPLAY FILTER | 4.95       | $4.95          |
| 1        | 993-1296-ND       | PSC15A-050               | AC/DC WALL MOUNT ADAPTER 5V 15W  | 13.38      | $13.38         |
| 1        | EG2025-ND         | PS1024ALRED              | SWITCH PUSH SPST-NO 3A 125V      | 1.15       | $1.15          |

In addition, we used the following passive components:

| Part            | Quantity |
| --------------- | -------- |
| 0.01 $$\mu$$F   | 1        |
| 0.1 $$\mu$$F    | 2        |
| 33 pF           | 1        |
| 22 k$$\Omega$$  | 2        |
| 200 k$$\Omega$$ | 1        |


The schematic below shows all the connections:

![Full Schematic](https://d2mxuefqeaa7sj.cloudfront.net/s_834581BD57803A6D41CE22CF41937180BBDCF065F14023D4CE9AA85BFF7C6BA5_1495105627306_project_schem.svg.png)

# Software Description

Our software is organized into components as described above. In order to make this simple we chose an object-oriented design architecture. This required making our own object “classes” for the display, chip, etc. to handle the interfacing. Each class was designed with the same general structure:

- A struct to store the current state.
- A init method to initialize the state struct. This is like a constructor.
- A begin method to start the associated interrupts for whatever the class does.
- A end method to stop the associated interrupts.
- All other methods take the state struct as the first argument.
## Matrix Driver

Our first class contained in `led_matrix` consisted of a `MatrixState_t`. The state held a buffer containing the colors at each LED coordinate, the row pair the interrupt would next write to and the cycle index (explained later).

    typedef struct MatrixColor {
      uint8_t r;
      uint8_t g;
      uint8_t b;
    } MatrixColor_t;
    
    typedef struct MatrixState {
      MatrixColor_t buffer[MATRIX_ROW_COUNT][MATRIX_COL_COUNT];
      coord_t row_pair;
      coord_t cycle_index;
    } MatrixState_t;

The `led_matrix` classes had the standard init, begin and end functions, as well as functions for drawing a pixels and rectangles. Each one took in a `MatrixState_t` to act on. The drawing functions just updated the buffer according the the coordinates in the arguments. All digital IO was done using macros we wrote in `gpio.h`. We provided macros to configure a pin as a digital output, and to write to it.
The most interested part of the matrix driver was the ISR handler. It ran at 15 kHz. In each run, it updated 2 rows (row `i`  and `i + 8`) of the matrix, and advanced the row index for the next interrupt. In order to provide 12-bit color with 16 levels per channel, each row pair was driven in cycles 16 iterations long, where a color is on for the appropriate fraction of the 16 cycles. With 8 row pairs and each row pair requiring 16 cycles for a full refresh, the entire matrix refreshes at 117 Hz, providing a fluid picture.

## Graphic Equalizer Driver

The `graphic_equalizer` class uses a `GraphicEq_t` that holds the current bar heights and a reference to the matrix state that it displays on. 

    typedef struct GraphicEqualizer {
      coord_t bars[7];
      MatrixState_t matrixState;
    } GraphicEq_t;

The graphic equalizer class had the standard init and begin functions. The init sets up the bar heights initially and initializes the led matrix. Begin just begins the matrix state by calling the led matrix begin function. The graphic equalizer class has one other function `setBarHeight` that accepts a `GraphicEq_t` and updates the bar height inside as well as draws a new rectangle on the matrix state in the appropriate area. 

## MSGEQ7 Driver

The `msgeq7` class uses a struct similar to the equalizer and led matrix, the struct maintains the current state machine state, the previously read data, and the current data read index in the array. 

    typedef struct MSGEQ7 {
      uint16_t data[7];
      ReadState readState;
      uint8_t index;
    } MSGEQ7_t;

The read state is represented by an enum that has each of the different states. 

    typedef enum MSGEQ7_state {
      RESET_ON,
      RESET_OFF,
      STROBE_ON,
      STROBE_OFF,
      READ
    } ReadState;

In the init function the GPIO pins are set up accordingly with `gpio.h` and then begin starts the timer just like the others. In each interrupt, there is a switch statement that handles each case and performs an action. The flow of the chip reading goes as follows: Reset ON, Reset OFF, Strobe ON, Strobe OFF, read analog output and increment bar index. If the index is 7, reset to 0 and start again; otherwise start at Strobe for the next one until 7. This clocks in each band stored in the chip. There are delays between each of those states, hence why we need the interrupts. The chip is driven with 3.3 V and then the pins on the board are connected to the Strobe and Reset parts of the chip. The analog value is read in through A0 by configuring the ADC mux to only sample from A0. The ADC converts the signal to an value between 0x000 and 0xFFF. We filter this value with a simple exponential filter: $$y[n] = \alpha x[n] + (1-\alpha) x[n-1]$$. Using $$\alpha=0.1$$ we removed most of the noise. This made the display look much better as it removed flickering, and makes the bar movements look very smooth. As we have a very high sampling frequency, it doesn’t hurt the response time.

## Button Driver

The `button` class has an init function and begin just like the others, and it uses a button struct that contains the callback function. 

    typedef struct ButtonState { 
      void (*callback)(void); 
    } ButtonState_t;

The button init sets up the defined pin and port as a GPIO input and sets up the interrupt handler for port B so that on the rising edge of the button press the interrupt occurs. In main we have to debounce the button, but we just used an integer that counted up to a number in the main while loop such that when it reached that number the button could be pushed again. The button class also contains macros for enabling pins as GPIO inputs and enabling interrupts on those pins. 

## GPIO Driver

Our GPIO header file handles the initialization of digital output pins. We have defined a series of macros that make it easy to configure pins as outputs and drive them high and low. These macros are very fast because at compile time the code is replaced with the full code but it is expandable to any port letter and any pin. This way we did not have to use any libraries or write any lookup functions that take more time. With the frequency at which we are running the display we needed this to be as fast as possible. 

## Analog Driver

Our analog class handles similar things as the GPIO class but we did not use macros here because the reading of the analog input was a little more complicated. We just defined everything only for pin A0 as that was the only one needed. 

## Color Utilities

We also created a color utilities class to convert HSV to RGB because the gradients were easier to calculate in HSV format but our matrix takes RGB values. This also has a map function which we used to change one color range (say 0 to 360) to another. This was useful if we only wanted our range of gradients to go from blue to red rather than around the full color wheel. 

## Main

Finally our `main` file just handled the initialization of all the classes and structs, as well as beginning each process. As described in the **System Design** section the main loop read the values and transformed the analog data from 0x000-0xFFF to a value between 0 and 15 for the height. The height was then given to the color map function to get the proper hue for the color. The bar is then set to that height with the new color. Due to the matrix being scanned out row by row, the gradients are actually made from the display not updating the full bars color in time before it changes again. The main also handles the button callback which just increments a color scheme counter to cycle through the different schemes.   

# Testing

We tested each component of the project individually, and then tested that they entire system functioned as expected. Each component was tested as follows:

- LED matrix: tested displaying individual pixels in multiple colors, then rectangles, and then different intensities of colors.
- Graphic equalizer driver functions: tested that we could create and color bars, as well as change their heights.
- MSGEQ7: sent audio input at specific frequencies and verified that it recognized them correctly. The chip had 7 channels at 63 Hz, 160 Hz, 400 Hz, 1 kHz, 2.5 kHz, and 6.25 kHz. We generated audio signals at each of those frequencies and verified that the chip reported high amplitude at the target frequency and low amplitude at the other frequencies.
- Button: wrote a test that made a pixel on the matrix light up when the button was pushed.

All these tests are in `test.c`.
Once all the individual components were working, we tested the system as a whole, checking the following things:

- The music aligned with the equalizer. We played *Closer* by the Chainsmokers as it had very clear bass. We manually verified that every time there was a beat, the appropriate channel reacted. 
- More tone generator tests. This verified that we still have every channel working properly.
- Tested that the button was able to switch color schemes.
# Results and Challenges

Our project was extremely successful; we achieved everything we planned to. The two most complicated parts of our design were the LED matrix display driver and the MSGEQ7 driver. Both required significant time debugging. The LED matrix however, was very tricky. There was not a datasheet available for the matrix; all Adafruit had was an Arduino library and a short description of the communication protocol. Our initial driver was based solely on the protocol description Adafruit had on their website. However, we ran into strange issues with only being able to control every other column, and not being able to drive every row. After about 10 hours of experimentation over 2 days, we were able to figure it out. Driving every row required a very careful sequencing of the output enable (OE) pin with the row select pins. Driving every column required that the data outputs be pulsed on every clock, even if the did not change (if 2 consecutive bits were both high, we had to pulse that line low when the clock went low; it couldn’t remain high). The MSGEQ7 was far faster to debug: we simply had the pin mappings wrong in our code.
This project worked out as expected, and we would not do anything different.

# Work Distribution

We each wrote the initial versions of each class individually. Drew wrote:

- `analog.c`
- `msgeq7.c`
- `graphic_equalizer.c`

Jacob wrote:

- `led_matrix.c`
- `gpio.h`
- `button.c`

After the initial writing, we did most of it together. As we had to fix many bugs in those original files, we each edited them in person so we ended up with equal understanding of each. We would get together and work for hours (once all day) on the project in person. There were a few times we coded or tested a few things at home, but it was small pieces and never enough to leave someone clueless as to what is going on. The majority of it was done through taking turns coding it on the computer as well as talking through all our ideas ahead of time.

# References

We wrote almost all of the code ourselves, manipulating all the low-level registers directly. The analog read function was taken from an example found in one of the K64f example packets (https://developer.mbed.org/media/uploads/defrost/frdm-k64_adc_configuration.pdf) and the HSV to RGB method was converted from one found online (https://www.ruinelli.ch/rgb-to-hsv). Otherwise the only libraries included were for the `hardware_init()` function to up the clock frequency to 120MHz. We used the K64f manual given in lab 2 to learn about the registers we needed to set for GPIO and analog use. And the other data sheets used were for the MSGEQ7 (https://www.sparkfun.com/datasheets/Components/General/MSGEQ7.pdf) and the led matrix just had a how it works page from Adafruit (https://learn.adafruit.com/32x16-32x32-rgb-led-matrix/how-the-matrix-works) along with some wiring diagrams. They provided Arduino libraries but those were not very useful as the code was very dense and we couldn’t figure out what was going on.

