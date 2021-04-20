Please include your answers to the questions below with your submission, entering into the space below each question
See [Mastering Markdown](https://guides.github.com/features/mastering-markdown/) for github markdown formatting if desired.

**1. How much current does a single LED draw when the output drive is set to "Strong" with the original code?**

A single LED draws around 5.1mA when set with Strong.

**2. After commenting out the standard output drive and uncommenting "Weak" drive, how much current does a single LED draw?**

A single LED draws around 5.25mA when set with Weak

**3. Is there a meaningful difference in current between the answers for question 1 and 2? Please explain your answer,
referencing the [Mainboard Schematic](https://www.silabs.com/documents/public/schematic-files/WSTK-Main-BRD4001A-A01-schematic.pdf) and [AEM Accuracy](https://www.silabs.com/documents/login/user-guides/ug279-brd4104a-user-guide.pdf) section of the user's guide where appropriate.**

There is not a meaningful difference between setting the output drive strong or weak because the external resistor in series with the LED Diode is what controls the amount of current(not the drive strength of the pin).  There would be an issue if the low setting could not supply the required current for the LED but it clearly does since the LED is flashing.  The pins drive strength will affect the rise times of the output signals when driving a signal.  

**4. Using the Energy Profiler with "weak" drive LEDs, what is the average current and energy measured with only LED1 turning on in the main loop?**

The average current is 5.10mA and the average power is 16.98mW with only LED1 cycling on/off.

**5. Using the Energy Profiler with "weak" drive LEDs, what is the average current and energy measured with both LED1 and LED0 turning on in the main loop?**

The average current is 5.28mA and the average power is 17.57mW when the gpioLed0SetOn() function is uncommented, now turning both LED1 and LED0 on and off.
