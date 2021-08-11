# RoboDesk
Read LOGICDATA protocol to control height of a powered sit/stand desk.

LOGICDATA protocol decoding from [phord's RoboDesk](https://github.com/phord/RoboDesk/tree/LogicData)

## Current features:
* Double tap a direction to go to a hardcoded target height
* Wifi connection
* MQTT publish/subscribe

## Files:
* `firmware`: platformio code for the d1 mini
* `schematic`: kicad schematic for the connections between the d1 mini and the desk
  * One thing I did is diagram both sides of a bunch of connectors.
  Is this the best way to do this?
* `robodesk-case.scad`: Enclosure using https://www.thingiverse.com/thing:1264391
* `logicdata-controller.xcf`: orthorectified top and bottom image of the logicdata controller `SMART-e-2-UNL-US`
  * Didn't fit anywhere else, so it might as well go here
