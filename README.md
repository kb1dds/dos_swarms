# dos_swarms
Various swarm simulations for DOS 

Copyright (c) 2022, Michael Robinson

## Synopsis

This repository contains several simple swarm simulations I made for DOS using Microsoft QuickC.  Since they all rely on that compiler's `<graph.h>` and `<conio.h>` libraries, they cannot be built using modern `gcc`.  However, the DOS `.exe` files work under DOSBox.

## Simulations

* `bees` : The baseline simulator, in which a swarm of bees follows a random point around the screen.  (This may have actually been written by my father.)

* `rob1` : Robots chasing a random point.  Similar to `bees`, but there are now obstacles that the robots can collide with and must avoid.

* `coimm` : A swarm of bacteria (red) being eaten by macrophages (green), B-cells (cyan), T-cells (blue), and antibodies (purple)

The `coimm` was too visually complicated, so I also made two simpler versions with slightly different runtime behavior:

* `coimm2` and `coimm3`: A swarm of bacteria (red) being eaten by macrophages (green)