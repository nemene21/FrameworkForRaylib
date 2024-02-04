#ifndef MISC_H
#define MISC_H

#include <raylib.h>
#include <raymath.h>
#include <iostream>

bool operator==(Color first, Color other);

Color Lerp(Color a, Color b, float c);

float RandF();

#endif