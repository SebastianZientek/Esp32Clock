#pragma once

#include <cstdint>

namespace
{
/* clang-format off */
uint8_t LT[8] = {
    0b00111,
    0b01111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111
};

uint8_t UB[8] = {
    0b11111,
    0b11111,
    0b11111,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000
};

uint8_t RT[8] =
{
    0b11100,
    0b11110,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111
};

uint8_t LL[8] =
{
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b11111,
    0b01111,
    0b00000
};

uint8_t LB[8] =
{
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11111,
    0b11111,
    0b11111
};

uint8_t LR[8] =
{
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11111,
	0b11100,
	0b00000
};

uint8_t UMB[8] =
{
    0b11111,
    0b11111,
    0b11111,
    0b00000,
    0b00000,
    0b00000,
    0b11111,
    0b11111
};

uint8_t LMB[8] =
{
    0b11111,
    0b00000,
    0b00000,
    0b00000,
    0b00000,
    0b11111,
    0b11111,
    0b11111
};
/* clang-format on */
}  // namespace