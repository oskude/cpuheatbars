# cpuheatbars

`cpuheatbars` is a prototype of a *linux desktop widget* to show current processor load as *bars that kinda fade in/out*.

## todo

- draw with qt
- user options

## depends

- qt5-core
- qt5-gui
- qt5-webengine
- qt5-websockets

## build

```
cd /your/build/dir
qmake /path/to/cpuheatbars.pro
make
```

## install

```
install -Dm755 cpuheatbars /usr/local/bin/cpuheatbars
```
