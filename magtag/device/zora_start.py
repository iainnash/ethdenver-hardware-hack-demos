# SPDX-FileCopyrightText: 2017 Scott Shawcroft, written for Adafruit Industries
#
# SPDX-License-Identifier: Unlicense
import time
from adafruit_magtag.magtag import MagTag

magtag = MagTag()

magtag.add_text(
    text_position=(
        20,
        (magtag.graphics.display.height // 2) - 1,
    ),
    text_scale=2,
    text_transform=lambda x: '{}'.format(x),
)

magtag.add_text(
    text_position=(
        20,
        (magtag.graphics.display.height // 4) - 1,
    ),
    text_scale=1,
    text_transform=lambda x: '{}'.format(x)
)


magtag.graphics.qrcode(b"https://docs.zora.co/",qr_size=2,x=240,y=70)

magtag.set_text("ZORA", 0, False)
magtag.set_text("the NFT marketplace protocol", 1, True)

button_colors = ((255, 0, 0), (255, 150, 0), (0, 255, 255), (180, 0, 255))
button_tones = (1047, 1318, 1568, 2093)

while True:
    for i, b in enumerate(magtag.peripherals.buttons):
        if not b.value:
            print("Button %c pressed" % chr((ord("A") + i)))
            magtag.peripherals.neopixel_disable = False
            magtag.peripherals.neopixels.fill(button_colors[i])
            magtag.peripherals.play_tone(button_tones[i], 0.25)
            break
    else:
        magtag.peripherals.neopixel_disable = True
    time.sleep(0.01)

