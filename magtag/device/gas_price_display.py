# SPDX-FileCopyrightText: 2020 ladyada, written for Adafruit Industries
#
# SPDX-License-Identifier: Unlicense
import time
import alarm
import supervisor
import alarm
from adafruit_magtag.magtag import MagTag

# Change this to the hour you want to check the data at, for us its 8pm
# local time (eastern), which is 20:00 hrs
DAILY_UPDATE_HOUR = 20

# Set up where we'll be fetching data from
DATA_SOURCE = "http://gas-monitor.isiain.workers.dev/get-gas-level"
SAFE_GAS_PRICE_LOCATION = ['SafeGasPrice']
PROPOSE_GAS_PRICE_LOCATION = ['ProposeGasPrice']
FAST_GAS_PRICE_LOCATION = ['FastGasPrice']

magtag = MagTag(
    url=DATA_SOURCE,
    json_path=(SAFE_GAS_PRICE_LOCATION, PROPOSE_GAS_PRICE_LOCATION, FAST_GAS_PRICE_LOCATION),
)


# Date stamp of info
magtag.add_text(
    text_font="/fonts/Arial-Bold-12.pcf",
    text_position=(10, 15),
    text_transform=lambda x: "Safe gas price: {}".format(x),
)
# Positive increase
magtag.add_text(
    text_font="/fonts/Arial-Bold-12.pcf",
    text_position=(10, 35),
    text_transform=lambda x: "Proposed Gas Price: {}".format(x),
)
# Curr hospitalized
magtag.add_text(
    text_font="/fonts/Arial-Bold-12.pcf",
    text_position=(10, 55),
    text_transform=lambda x: "Fast gas pricel: {}".format(x),
)


# updated time
magtag.add_text(
    text_font="/fonts/Arial-Bold-12.pcf",
    text_position=(245, 30),
    line_spacing=0.75,
    is_data=False
)

magtag.peripherals.neopixels.brightness = 0.1
magtag.peripherals.neopixel_disable = False # turn on lights
magtag.peripherals.neopixels.fill(0x0F0000) # red!

#magtag.get_local_time()
try:
    #now = time.localtime()
    #print("Now: ", now)

    # display the current time since its the last-update
    #updated_at = "%d/%d\n%d:%02d" % now[1:5]
    #magtag.set_text(updated_at, 6, False)

    # get data from the Covid Tracking Project
    value = magtag.fetch()
    print("Response is", value)

    # OK we're done!
    magtag.peripherals.neopixels.fill(0x000F00) # greten
except (ValueError, RuntimeError) as e:
    print("Some error occured, trying again later -", e)

time.sleep(2) # let screen finish updating

remaining = 60 # update 60 seconds
time.sleep(remaining)

# Turn it all off and go to bed till the next update time
#magtag.exit_and_deep_sleep(remaining)

