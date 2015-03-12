These are three different versions relating to an autonomous car project in a class I took at UIUC called ECE 110, which is an introduction to electrical and computer engineering. 

The goal was to have a miniature "car" with four wheels travel alog a path marked by white tape on a black background. The wheels were powered by individual motors, and the main input was from light sensors that would use basic threshold detection to figure out where track was black and where it was white.

The Arduino code constantly polled the light sensor input, and figured out how to move by controlling the respective motors to move in the correct direction in order to follow the track.