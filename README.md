# Procedural-Walk
Actor Components for Bipedal procedural walk animations in UE5

## Overview

This is an actor component I made for our procedurally animated player character for our game [Sclorp & the crash on planet Snalien](https://oskarwistedt.itch.io/sclorp-the-crash-on-planet-snalien)

## Features

- **Bipedal walk**: Highly customizable walk animation logic for bipedal characters using Curve assets for easy visualization.

## Setup
_might expand on this in the future_

1. Place two **Leg** components and a **Leg manager** in your character hierarchy and set values and curves to your liking.

2. These components then need to be connected to an animation blueprint which in turn feeds certain values into a control rig using an IK node.

3. Using the control rig, bind foot bones to the positions provided by the **FootPosition** vectors from each **Leg** component.
