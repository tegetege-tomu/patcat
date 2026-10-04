## INTRO

Why? because my Keystep Pro sends channel AT and my Ise-nin take PAT or MPE only. And because I have an uno + spark midishield lying around.

First things first, to get off my chest, MIDI is an incredibly frustrating and archaic standard that refuses to be superceded so we are stuck working with it. There, I said it, no more complaining going forward.

## FILES

.ino will just sit there for setup and serial init.



## BUTTONS

enable/ disable patcat - chord of 1 & 3, long-press

enter/ leave settings - chord of 2 & 3, long-press

toggle sustain on/ off - button 1, long-press: AT sustain; AT independent
 
toggle sustain type - chord of 1 & 2: AT max; AT lock

toggle note thru - button 2, long-press: AT only sent over midi; AT + note thru 

toggle trigger mode - button 3, long-press: zero AT at note-on; current AT at note-on

MIDI panic (quick) - button 2, short-press: send all notes off, all AT off

MIDI panic (full) - button 2, long-press: force all notes off (long cycle)




## ARCHITECTURE

````
MIDI IN
  |
  ↓
Parser (FSM) 
  |
  ↓
Router ─┬-→ pass-through queue ─┐
        │                       ├-→  TxScheduler -→  Writer (running status) -→ MIDI OUT
        └-→ Translator ─────────┘
                  │
                  └─ HeldNotes set, pending pressure
````
