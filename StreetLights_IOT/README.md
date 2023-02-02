# Street light Detection IoT project (S4/Smart industry)
## Summary
This project is project aims to create tech that will record GPS and light data and then save it to SD card. Then the data from the SD card will be imported to a Node-Red flow that will show it and compare it to the [Eindhoven Open Data: Street Lights](https://data.eindhoven.nl/explore/dataset/data-openbare-verlichting/information/?disjunctive.straatnaam_identificatie_mast&disjunctive.stadsdeel&disjunctive.wijk&disjunctive.buurt&disjunctive.straatnaam&disjunctive.identificatie&disjunctive.eigenaar&disjunctive.type_lamp&disjunctive.kleur_lamp&disjunctive.wattage_lamp&disjunctive.lumen_lamp&disjunctive.ra_waarde_lamp&disjunctive.schakelschema_lamp&disjunctive.type_armatuur)

## Hardware
- ESP32 C3 DevKitM1
- GY-NEO6MV2 ESP32 GPS
- SD Card Reader
- Simple light sensor
- 10k Ohm resistor
![](https://github.com/RusoDotExe/UniStuff/blob/main/StreetLights_IOT/Pictures/Hardware.jpg)

## Node-Red Flow
![](https://github.com/RusoDotExe/UniStuff/blob/main/StreetLights_IOT/Pictures/Node_red_flow.png)

## Final Result
The red dots are the data that was gathered and the green dots are sample from the dataset (this result could also be displayed at Node-Red UI)
![](https://github.com/RusoDotExe/UniStuff/blob/main/StreetLights_IOT/Pictures/Node_red_map.png)
