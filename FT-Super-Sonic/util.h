#pragma once

void PrintInfo(const char* text, ...);
bool isInGame();
void init_Util();


template <typename Tret = size_t, typename T, size_t N>
static constexpr Tret LengthOfArray(const T(&)[N]) noexcept
{
	return (Tret)N;
}

uint8_t CheckStatusFieldFlags(int64_t in_field, uint32_t in_flags);