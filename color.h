#ifndef COLOR_H
#define COLOR_H

#include <bits/stdc++.h>
#include <glm/glm.hpp>

using namespace std;
using namespace glm;

void write_color(std::ostream &out, vec3 pixel_color) {
	auto r = pixel_color.x;
	auto g = pixel_color.y;
	auto b = pixel_color.z;

	// Divide the color by number of samples and gamma-correct for gamma=2.0.
	auto scale = 1.0;
	r = sqrt(scale * r);
	g = sqrt(scale * g);
	b = sqrt(scale * b);

	// Write the translated [0, 255] value of each color component.
	out << static_cast<int>(256 * clamp(r, 0.0f, 0.999f)) << ' '
		<< static_cast<int>(256 * clamp(g, 0.0f, 0.999f)) << ' '
		<< static_cast<int>(256 * clamp(b, 0.0f, 0.999f)) << '\n';
}

#endif
