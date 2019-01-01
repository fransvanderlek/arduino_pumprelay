/* Sweep
 using the digitalWrite to set/unset relay pins.
 */

int setPin = 12;
int unsetPin = 13;
int buttonReadPin = 2; //need GPIO 2, as it has pull up resistor
bool watering = false;
int moistureSensorPin = A0;

int moistureMin = -1;
long pumpIntervalMillis = -1;

long wateringStartTime = -1;


void setup() {

	Serial.begin(115200);
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

	delay(1000);

	Serial.println("##loop");
    Serial.print("Moisture at: ");
	Serial.println( analogRead(A0));

	if (buttonPressed()) {
		storeMoistureMin();
		long timeButtonPressed = millis();
		giveWaterWhile( buttonPressed );
		pumpIntervalMillis = millis() - timeButtonPressed;

		Serial.print("pumpIntervalMillis set to ");
		Serial.println( pumpIntervalMillis);
	}

	if (wateringLearned() && soilTooDry()) {
		wateringStartTime = millis();
		giveWaterWhile ( pumpTimeNotFinished );
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
	Serial.print("##Watering at time ");
	Serial.println(millis());
}

void stopWatering() {
	activateRelayPin(unsetPin);
	watering = false;
	Serial.print("##stop watering at time ");
	Serial.println(millis());

}

void storeMoistureMin() {
	moistureMin = analogRead(moistureSensorPin);
	Serial.print("Moisture Min set to: ");
	Serial.println(moistureMin);
}



bool soilTooDry() {
	Serial.print("##Soil too dry");
	Serial.println( analogRead(moistureSensorPin) <= moistureMin);
	return analogRead(moistureSensorPin) <= moistureMin;
}


bool pumpTimeNotFinished(){
	return (millis()-wateringStartTime) <= pumpIntervalMillis;
}

bool wateringLearned() {

	return (moistureMin >= 0) && (pumpIntervalMillis >=0) ;
}

void giveWaterWhile( bool (*keepWatering)(void)){
	startWatering();

	while (keepWatering()) {
		delay(1000);

	}
	stopWatering();
}

