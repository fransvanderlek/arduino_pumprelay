/* Sweep
 using the digitalWrite to set/unset relay pins.
 */

int setPin = 12;
int unsetPin = 13;
int buttonReadPin = 2; //need GPIO 2, as it has pull up resistor
bool watering = false;
int moistureSensorPin = A0;

int moistureMin = -1;
int moistureMax = -1;

void setup() {

	Serial.begin(9600);
	pinMode(setPin, OUTPUT);
	pinMode(unsetPin, OUTPUT);

	digitalWrite(setPin, LOW);
	digitalWrite(unsetPin, LOW);

	activateRelayPin(unsetPin);

	pinMode(buttonReadPin, INPUT_PULLUP); //default will be HIGH

}

/*
 *
 when the button is pressed, we want the pump to go on
 pump should keep going as long as button is being pressed
 therefore then the button is released, pumping should stop.

 once on, the pump keeps going until it is told to stop, since we use a latching relay
 we introduce the 'pumping' variable to keep track of this state
 and avoid setting/unsetting the relay unnecesserily.

 * */
void loop() {

	delay(100);

	Serial.println( analogRead(A0));


	if (buttonPressed()) {
		storeMoistureMin();
		giveWaterWhile( buttonPressed );
		storeMoistureMax();
	}

	if (wateringLearned() && soilTooDry()) {
		giveWaterWhile ( soilNotMoisturized );
	}
}

bool buttonPressed() {
	return digitalRead(buttonReadPin) == LOW;
//since default is HIGH, then HIGH means the button is not pressed
//and therefore LOW means the buttons was pressed ( button is connected to GND )

}

void activateRelayPin(int pin) {
	digitalWrite(pin, HIGH);
	delay(10);
	digitalWrite(pin, LOW);
}

void startWatering() {
	activateRelayPin(setPin);
	watering = true;
	Serial.println("Watering");
}

void stopWatering() {
	activateRelayPin(unsetPin);
	watering = false;
	Serial.println("not watering");

}

void storeMoistureMin() {
	moistureMin = analogRead(moistureSensorPin);
	Serial.println("Moisture Min set to: ");
	Serial.println(moistureMin);
}

void storeMoistureMax() {
	moistureMax = analogRead(moistureSensorPin);
	Serial.println("Moisture Max set to: ");
	Serial.println(moistureMax);

}

void resetMoistureLevels() {
	moistureMin = -1;
	moistureMax = -1;
}

bool soilTooDry() {
	return analogRead(moistureSensorPin) < moistureMin;
}

bool soilNotMoisturized() {
	return analogRead(moistureSensorPin) <= moistureMax;
}

bool wateringLearned() {
	return moistureMin > 0 && moistureMax > 0;
}

void giveWaterWhile( bool (*keepWatering)(void)){
	startWatering();

	while (keepWatering()) {
		delay(100);

	}
	stopWatering();
}

