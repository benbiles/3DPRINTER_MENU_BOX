# 3DPRINTER-MENU-BOX

concept: a low cost 3d printer AC fan controller and power switcher for enclosure tempterture regulation and saftey power shutdown 

mainboard uses ESP32 DevKit V1 with Arduino code
screen menu with 4 tactile buttons
1 x OLED screen
2 x DS18B20 Temp sensor channels
2 channels Variable AC 110v / 240v for fan speed control
2 channels 110V / 240V AC solid state relay SSR for shutdown on timer or over temp safety shutdown.

future Mobile APP alarm / notifications / warnings using MIT app inventor.

https://hackaday.io/project/190382-3dprinter-menu-box

current status:
basic menu , oled screen and button with debounce are working.
1 channel temp sensor is working

testing to do soon..
AC fan controller boards and enclosure temp regulation.
extracting the air cools the enclosure too much ?
install a heppa filter and recycle air back into enclosure with fan 1 and extract air with other
when enclosure becomes over temptetur with fan 2 ? 
test power shutdown in case of over temp.

to do later..
install smoke detector for emergency power down.
web cam image minitoring even if just stills taken at intervals.

