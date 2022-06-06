# test-benches
some arduino sketches to test various pieces of equipment
Can test:
- I2C LCDs (16x2 by default)
- HC-SR04 ultrasonic sensors
- servo motors
Note: When testing servos w/o hc-sr04, servo motion will be jerky because the ultrasonic read will be timing out
## Assembly
attach components according to constants and pin markings
## Usage
The LCD will display its hex address on the first line and the distance reading from the HC-SR04 on the second. The servo will
sweep back and forth a default of once in 4 seconds. Hot-swap any component to test another.