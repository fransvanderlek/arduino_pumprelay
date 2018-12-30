# Plant Watering

Watering a plant automatically

## Behavior

When a user presses/releases the button, two things happen:
- watering starts on button press and ends on button release
- the system remembers the moisture level at the time of press (moistureMin) and at the time of release (moistureMax)

In effect, the system has 'learned' what the correct moisture levels should be by way of example. From that point on the system will use the stored moisturelevels as setpoints. If the button is not being pressed, the system will determine if moisture levels are acceptable. 

If the soil is then too dry ( measured value from moisture sensor < moistureMin ), watering will start until the moisture has the same level als the learned value ( measured value from moisture sensor >= moistureMax ).

The user can 'retrain' the system by pressing/releasing the button, at which point pumping will happen and the setpoints will relearned. 


