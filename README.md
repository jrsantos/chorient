# chorient
Arduino Multi-Monitor auto orientation changer for Linux

Simple quick hack to do auto monitor orienatation using a Arduino mini and three ADXL345 accelerometers. The ADXL345 is conected through the SPI bus because I could not find a module that would allow 3 different i2c address to controll all three monitors. The arduinor code implements algorithm that prints the number of sensors used and the orieantation in a bitmap. On the computer side, there is a simple Python program that reads the input from the Arduino board and then uses xrandr to set all three monitors to the new orientation setting.

The main cheapo components used:
--------------
Arduino Mini compatible board:
http://www.amazon.com/gp/product/B00KKPBG5S/ref=oh_aui_detailpage_o05_s01?ie=UTF8&psc=1

3x ADXL345 Accelerometer modules
http://www.amazon.com/gp/product/B008B4W1QS/ref=oh_aui_detailpage_o05_s01?ie=UTF8&psc=1

Demo:
http://www.youtube.com/watch?v=33lEa3g3GPg
