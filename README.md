cs354_project2_lsystem
======================

Project 2 of CS354 Computer Graphics.
The executable implements a recursive LSystem used to draw plants in realtime
using OpenGL and GLUT.

Features
========
The executable:
 * Prints the LSystem being used.
 * Allows redefining the LSystem Axiom using the -a option.
 * Can be manipulated by the mouse (left-click and drag).
 * Can be rotated using the '?' and '/' keys.
 * Can change the LSystem depth with the 'a' and 's' keys.
 * Can quit by pressing 'q'.
 * Can redraw the LSystem using a new random state by pressing 'r'.
 * Can change between orthographic and perspective projections by pressing 'p'.
 * No OpenGL matrix manipulation is done for the ModelView matrix.
 * Uses color interpolation on the leaves and branches of the LSystem.
 * Uses a stochastic LSystem.
 * Draws a background that rotates with the tree.

