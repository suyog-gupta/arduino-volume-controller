# arduino-volume-controller
Arduino based auto volume controller for personal use. A simple device to automatically control television volume via Arduino Uno board.

There are three sensors connected to the Arduino board:
- Audio sensor
- IR Transmitter
- IR Receiver

What do sensors do:
- Audio sensor: Microphone that picks up the sound produced by TV and ambient sounds, and reports the reading to the board
- IR Transmitter: Sends out IR signals to TV to bring volume up or down
- IR Receiver: Checks incoming IR signals to identify if user wishes to enable or disable the volume controller

High level flow:
- Input: Volume reading is reported by audio sensor

- Processing:
- 01 Average of five volume readings is calculated
- 02 Average is then compared to a preset threshold value
- 02a If average is above threshold value, spike counter is incremented
- 02b If avaerge is below threshold value, low counter is incremented
- 03 Based on counter reading, a decision is made whether to increase or decrease the volume

- Output: IR Transmitter sends out IR signal to the TV
