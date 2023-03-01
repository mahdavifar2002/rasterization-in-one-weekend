#include <bits/stdc++.h>
#include <glm/glm.hpp>
#include "color.h"

using namespace std;
using namespace glm;

const float aspect_ratio = 3.0 / 2.0;
const int image_width = 1080;
const int image_height = image_width / aspect_ratio;

#define TO_RASTER(v) vec3(image_width * (v.x + 1.0f) / 2, image_height * (v.y + 1.0f) / 2, 1.0f)


bool left(vec3 p_k, vec3 p_i, vec3 p_j) {
	return (p_j.x - p_i.x)*(p_k.y - p_i.y) - (p_k.x - p_i.x)*(p_j.y - p_i.y) >= 0;
}

bool in_triangle(vec3 p, vec3 v0, vec3 v1, vec3 v2)
{
	return left(p, v0, v1) && left(p, v1, v2) && left(p, v2, v0);
}

int main()
{
	vec3 v0(-0.5, -0.5, 1.0);
	vec3 v1( 0.5, -0.5, 1.0);
	vec3 v2( 0.0,  0.5, 1.0);

	v0 = TO_RASTER(v0);
	v1 = TO_RASTER(v1);
	v2 = TO_RASTER(v2);
// Render

	cout << "P3\n" << image_width << " " << image_height << "\n255\n";

	for (int y = image_height - 1; y >= 0; y--) {
		cerr << "\rScanlines remaining: " << y * 100 / image_height << "% " << flush;

		for (int x = 0; x < image_width; x++) {

			vec3 triangle_color(1.0, 0, 0);
			vec3 background_color(0, 0, 0);
			vec3 sample = { x + 0.5f, y + 0.5f, 1.0f };

			if (in_triangle(sample, v0, v1, v2))
				write_color(cout, triangle_color);
			else
				write_color(cout, background_color);
		}
	}

	cerr << "\nDone\n";
}