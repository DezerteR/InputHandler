Simple class to improve handling keyboard/mouse input. Allows to register multiple key combinations uder one name.
to register key combination: ``` InputHandler::registerKeyCombination("alt-f4:exit"); ```

#### Constructing key binding
Default action is PRESS-RELEASE to change to REPEAT-RELEASE use ```hold``` as first key.
In keybinding there should be:
- optional 'hold'
- at least one of
  - modifier keys: 'shift', 'ctrl', 'alt', 'super'
  - key
  
As key is taken last of keys.
Examples:
```
hold-shift : statistics
alt-f4 : exit
```

#### Using
Keybindings are defined in contexts in which should be unique. Active context could consume all events, or only these that are defined.
There are two ways to define keybinding in context:
- via registered combination
- locally with own combination and name

keys:
```
shift
ctrl
alt
super
space
esc
enter
tab
backspace
insert
delete
right
left
down
up
pageUp
pageDown
home
end
capsLock
scrollLock
printScreen
pause
f1
f2
f3
f4
f5
f6
f7
f8
f9
f10
f11
f12
menu
shift
ctrl
alt
super
hold
scrollUp
scrollDown
minus
```
