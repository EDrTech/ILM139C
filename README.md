![Banner](docs/visual/ILM139C_banner.png)
![Banner](docs/visual/ILM139C_4asm_3d.png)
&nbsp;
###### <sub>Quick access</sub>
---------------------------------------------------------------------------------

<p align="center">
  <a href="/docs/datasheet_src/datasheet01.pdf"><img src="docs/visual/badges/module_datasheet.svg"></a>
  <a href="/docs/schematics"><img src="docs/visual/badges/module_schematic.svg"></a>
  <a href="/code_src/ilm139c_battiny_demo_allcombined/ilm139c_battiny_demo_allcombined.ino"><img src="docs/visual/badges/module_demo_code.svg"></a>
  <a href="/docs/bom/ILM139C_matrix_bom.csv"><img src="docs/visual/badges/module_bom.svg"></a>
</p>

---------------------------------------------------------------------------------
&nbsp;
&nbsp;
&nbsp;
---------------------------------------------------------------------------------
> [!WARNING]
> <p>
> This project is currently under active development. It is experimental, and may not function as intended. The source files might not exactly match tested hardware if you can see this message. Use at your own risk.
><br clear="right"/>
></p>

---------------------------------------------------------------------------------
# Illumicro ILM139C 13x9 RGB LED matrix

<p align="center">
  <img src="/docs/visual/ILM139C_bwig.png" align="center" width="80%"/>
</p>
ILM139C Is an RGB LED module with a matrix of 13x9 RGB LEDs.
It is based on the IS31FL3741A LED matrix driver, which provides an i2c interface for full control of
individual LEDs which is accessible with a standard QWIIC connector. 
QWIIC Connector allows for up to 4 modules to be connected together on a single i2c bus.
Small form factor of 26mm*18mm and 2mm pitch of LEDs allows for creating small 
high density and high brightness displays.

<p align="center">
  <video src='/docs/visual/ILM139C_demo.mp4'/>
</p>

&nbsp;
&nbsp;
&nbsp;
&nbsp;
&nbsp;
&nbsp;
&nbsp;
&nbsp;
&nbsp;
&nbsp;

# Features
&nbsp;
&nbsp;
&nbsp;
&nbsp;
&nbsp;
<p>

<img src="/docs/visual/ILM139C_asm_top_3d.png" align="right" width="50%"/>
  
- Very small footprint of 26mm*18mm
- 13x9 RBG LED matrix
- 1mm*1mm LEDs, with 2mm pitch
- IS31FL3741A Driver
- 2.7V~5.5V Operating voltage
- QWIIC Connector
- Selectable i2c address
- Removable driver/matrix boards
- Separate power supply connector
  
</p>
&nbsp;
&nbsp;
&nbsp;


# ILM139C Module pinout

<p align="center">
  <img src="/docs/visual/ILM139C_pinout.png" align="center" width="80%"/>
</p>

---------------------------------------------------------------------------------

# Installation and set up

Connect QWIIC cable to one of the available connectors on the bottom of the module. If you want to power the module over QWIIC, the jumper resistor should be soldered on the bottom of the module. If the jumper is present, DO NOT connect external power supply as it will short to 3.3V of the QWIIC!
The module can be programmed over i2c, a library is currently under development to allow straightforward integration with and i2c capable device.

&nbsp;
&nbsp;
&nbsp;


![illumicro Series](docs/visual/illumicro_fbanner.png)
