Simple class to improve handling keyboard/mouse input. Allows to register multiple key combinations uder one name.

#### Constructing key binding
Default action is PRESS-RELEASE to change to REPEAT-RELEASE use ```hold``` as first key.
In keybinding there should be:
- optional 'hold'
- modifier key if implementation allows
- key

For default implementation for keyboard there are no modifier keys, they should be handled by user via implementing custom action for pressing and releasing modifier key.

Also default behaviour for repeating keys is changed, now key is repeated imidiately and until release command. I found it especially useful for games.

So now to implement alt-f4 one have to do it in two stages:
registerAction("alt", [&alt]{ alt=true;}, [&alt]{alt=false; })
registerAction("f4", [&alt]{ if(alt) exit(); })
  
Probably there is some magic to awoid this shit, but I am content with this now. Probably later I will refactor this.

---

At one moment there can be active nly one context, to awoid repeating actions, context can derive action from another. So there could be many actions under one key.

#### Usage
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
