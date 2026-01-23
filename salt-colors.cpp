#include "./libmgui/include/MColor.hpp"

#include <cstdint>
#include <iostream>

constexpr std::uint8_t operator"" _uint8(unsigned long long v)
{
	return static_cast<std::int8_t>(v);
}

int main()
{
	const MColor k256AnsiColors[] = {

		{ uint8_t(0), uint8_t(0), uint8_t(0) },
		{ uint8_t(197), uint8_t(0), uint8_t(13) },
		{ uint8_t(0), uint8_t(172), uint8_t(92) },
		{ uint8_t(177), uint8_t(128), uint8_t(30) },
		{ uint8_t(0), uint8_t(63), uint8_t(141) },
		{ uint8_t(157), uint8_t(28), uint8_t(190) },
		{ uint8_t(15), uint8_t(168), uint8_t(192) },
		{ uint8_t(208), uint8_t(207), uint8_t(204) },

		{ uint8_t(94), uint8_t(92), uint8_t(100) },
		{ uint8_t(255), uint8_t(74), uint8_t(54) },
		{ uint8_t(28), uint8_t(223), uint8_t(113) },
		{ uint8_t(244), uint8_t(185), uint8_t(22) },
		{ uint8_t(18), uint8_t(122), uint8_t(249) },
		{ uint8_t(223), uint8_t(63), uint8_t(241) },
		{ uint8_t(32), uint8_t(214), uint8_t(241) },
		{ uint8_t(255), uint8_t(255), uint8_t(255) },
	};

    for (auto c : k256AnsiColors)
        std::cout << c << '\n';

	return 0;
}