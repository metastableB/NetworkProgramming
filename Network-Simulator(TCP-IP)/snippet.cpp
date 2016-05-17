#include <iostream>
#include <chrono>

int main() {
	std::chrono::high_resolution_clock::time_point t = std::chrono::high_resolution_clock::now(); 
	std::chrono::duration<long,std::nano> t2 (5);
	t = t - t2;
}