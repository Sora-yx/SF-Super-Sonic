#pragma once

void PrintInfo(const char* text, ...);
bool isInGame();
void Init_Util();

void SetInGameFalse();
int GetKey(std::string s);


template <typename Tret = size_t, typename T, size_t N>
static constexpr Tret LengthOfArray(const T(&)[N]) noexcept
{
	return (Tret)N;
}
