Simple class to improve handling keyboard/mouse input. Allows to register multiple key combinations uder one name.

#### Constructing key binding
Default action is PRESS-RELEASE to change to REPEAT-RELEASE use ```hold``` as first key.
In keybinding there should be:
- optional 'hold'
- modifier key if implementation allows
- key

#### How It works
Handlers are organised in hierarhical way in a tree. With only one nodea active, event if not consumed is propagated to parent of node. 


Also default behaviour for repeating keys is changed, now key is repeated imidiately and until release command. I found it especially useful for games.

So now to implement alt-f4 one have to do it in two stages:
registerAction("alt", [&alt]{ alt=true;}, [&alt]{alt=false; })
registerAction("f4", [&alt]{ if(alt) exit(); })
  
Probably there is some magic to awoid this shit, but I am content with this now. Probably later I will refactor this.

---



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
