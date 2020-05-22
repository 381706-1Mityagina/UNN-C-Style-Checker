#include <iostream>
#include <cstdint>

int main() {
	std::uint64_t diff = 0;
	std::uint64_t diff0 = 0;
	std::uint64_t diff1 = 0;
	std::uint64_t diff2 = 0;
	std::uint64_t diff3 = 0;
	std::uint64_t diff4 = 0;
	std::uint64_t diff5 = 0;

	for (std::uint64_t i = 0; i < 1000000; ++i) {
		diff0 -= 0;
		diff1 -= 0;
		diff2 -= 0;
		diff3 -= 0;
		diff4 -= 0;
		diff5 -= 0;
		
		diff -= 0;
		diff -= 0;
	}
	std::cout << "Result : " << diff << "\n";

	return 0;
}