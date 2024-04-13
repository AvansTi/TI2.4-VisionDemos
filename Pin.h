#pragma once

#include <opencv2/core.hpp>

enum class PinType
{
    Unknown = 1<<0,
    Image1 = 1<<1,
    Image3 = 1<<2,
    Float = 1<<3,
    Int = 1<<4,

    Image = Image1 | Image3,

    Any = Image1 | Image3 | Float | Int
};



class Pin
{
public:
	int id = 0;
	int connection = 0;
    PinType type = PinType::Unknown;
};
