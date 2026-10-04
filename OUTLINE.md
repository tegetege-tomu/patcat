see https://learn.sparkfun.com/tutorials/midi-shield-hookup-guide


choices:
Shield config: hardware serial
MIDI jack mode: MIDI OUT
Power over MIDI: disabled

buttons:
b1 (short) - trigger
b1 (long) - enter settings mode; exit and save settings
b2 (short) - increment note 1 semitone
b2 (long) - increment note 1 8ve
b3 (short/long) - decrement

libraris:
eeprom.h
debounce?? YES
no need midi since will just use serial

buttons/pots in separate handler

