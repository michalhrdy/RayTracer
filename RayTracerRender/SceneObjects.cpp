#pragma once
#include "SceneObjects.h"

void CCanvas::Clear(void)
{
    pixels.clear();
    pixels.resize(width*height);
}
