# Getting started with the magtag and circuit python

Recommended editor for beginners: https://codewith.mu/

## Coding tips and guides
1. `code.py` runs automatically on boot.
2. `secrets.py` configures the WIFI network settings
   - this is 2.4ghz networks only, 5ghz (venue wifi) will not work

## Adafruit guides:
1. HTTP Fetch: https://learn.adafruit.com/magtag-covid-tracking-project-iot-display
2. General Intro: https://learn.adafruit.com/adafruit-magtag

## First time setup (already done for hackathon participants)
1. Click reset twice not too fast not too slow
2. Upload .u2f bootloader for circuitpython (can be found in this folder boot/)
3. Add lib folder from example repo (to CIRCUITPYTHON volume)
4. Update secrets.py with WIFI information (etc).
5. Copy code.py and secrets.py to root CIRCUITPYTHON volume
6. Edit code.py and device will auto-refresh
7. Use http://www.freeware.the-meiers.org/ CoolTerm or your favorite serial terminal to interact with device or see crash information beyond the screen information.