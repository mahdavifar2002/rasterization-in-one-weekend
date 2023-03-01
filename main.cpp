#include <bits/stdc++.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "color.h"
#include "cube.h"

using namespace std;
using namespace glm;

const float aspect_ratio = 3.0 / 2.0;
const int image_width = 1080;
const int image_height = image_width / aspect_ratio;

vec3 image[image_height][image_width];
float depth[image_height][image_width];

// #define TO_RASTER(v) vec3(image_width * (v.x + 1.0f) / 2, image_height * (v.y + 1.0f) / 2, 1.0f)
#define TO_RASTER(v) vec4(image_width * (v.x + v.w) / 2, image_height * (v.y + v.w) / 2, v.z, v.w)


bool left(vec3 p_k, vec3 p_i, vec3 p_j) {
	return (p_j.x - p_i.x)*(p_k.y - p_i.y) - (p_k.x - p_i.x)*(p_j.y - p_i.y) >= 0;
}

bool right(vec3 p_k, vec3 p_i, vec3 p_j) {
	return (p_j.x - p_i.x)*(p_k.y - p_i.y) - (p_k.x - p_i.x)*(p_j.y - p_i.y) <= 0;
}

bool in_triangle(vec3 p, vec3 v0, vec3 v1, vec3 v2)
{
	return left(p, v0, v1) && left(p, v1, v2) && left(p, v2, v0);
			// || right(p, v0, v1) && right(p, v1, v2) && right(p, v2, v0);
}

vector<mat4> objects;

void initialize_scene_objects(vector<mat4> &objects)
{
	// construct a scene of few cubrs randomly positioned

	const mat4 identity(1.0f);

	auto M0 = translate(identity, vec3(0, 0, 2));
	M0 = rotate(M0, radians(45.0f), vec3(0, 1, 0));
	objects.push_back(M0);

	auto M1 = translate(identity, vec3(-3.75f, 0, 0));
	M1 = rotate(M1, radians(30.0f), vec3(0, 1, 0));
	objects.push_back(M1);

	auto M2 = translate(identity, vec3(3.75f, 0, 2));
	M2 = rotate(M2, radians(60.0f), vec3(0, 1, 0));
	objects.push_back(M2);

	auto M3 = translate(identity, vec3(0, 0, -2));
	M3 = rotate(M3, radians(90.0f), vec3(0, 1, 0));
	objects.push_back(M3);

	auto M4 = translate(identity, vec3(0, 0, 2));
	M4 = rotate(M4, radians(0.0f), vec3(0, 1, 0));
	objects.push_back(M4);
}

vec4 VS(const vec3& pos, const mat4& M, const mat4& V, const mat4& P)
{
	return P * V * M * vec4(pos, 1.0f);
}

void print(vec4 v)
{
	std::cerr << "(" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << ")\n";
}

int main()
{
	float near_plane = 0.1f;
	float far_plane = 100.f;
	vec3 eye(0, 3.75, 6.5);
	vec3 lookat(0, 0, 0);
	vec3 up(0, 1, 0);

	mat4 view = lookAt(eye, lookat, up);
	mat4 proj = perspective(radians(60.0f), (float) image_width / image_height, near_plane, far_plane);


	// Render

	cout << "P3\n" << image_width << " " << image_height << "\n255\n";

	initialize_scene_objects(objects);


	vec3 triangle_color(1.0, 0, 0);
	vec3 background_color(0, 0, 0);

	for (int y = image_height - 1; y >= 0; y--)
		for (int x = 0; x < image_width; x++) {
			image[y][x] = background_color;
			depth[y][x] = FLT_MAX;
		}

	// Loop over objects in the scene
	for (int n = 0; n < objects.size(); n++)
	{
		int num_triangles = sizeof(indices)/sizeof(int)/3;

		for (int idx = 0; idx < num_triangles; idx++)
		{
			std::cerr << "\rTriangles remaining: " << ((objects.size() - n)*num_triangles - idx - 1) * 100 / (objects.size() * num_triangles) << "% " << flush;

			vec3& v0 = vertices[indices[idx*3]];
			vec3& v1 = vertices[indices[idx*3 + 1]];
			vec3& v2 = vertices[indices[idx*3 + 2]];

			// Invoke function for each vertex of the triangle to transform them from object-space to clip-space (-w, w)
			vec4 v0clip = VS(v0, objects[n], view, proj);
			vec4 v1clip = VS(v1, objects[n], view, proj);
			vec4 v2clip = VS(v2, objects[n], view, proj);

			// Apply viewport transformation
			vec4 v0homogen = TO_RASTER(v0clip);
			vec4 v1homogen = TO_RASTER(v1clip);
			vec4 v2homogen = TO_RASTER(v2clip);

			// Base vertex matrix
			mat3 M =
			{
				{ v0homogen.x, v1homogen.x, v2homogen.x },
				{ v0homogen.y, v1homogen.y, v2homogen.y },
				{ v0homogen.x, v1homogen.z, v2homogen.z },
			};

			// Calculate constant function to interpolate w
			vec3 C = M * vec3(1, 1, 1);

			for (int y = image_height - 1; y >= 0; y--) {
				for (int x = 0; x < image_width; x++) {

					vec3 sample = { x + 0.5f, y + 0.5f, 1.0f };
					float w = dot(C, sample);

					if (in_triangle(sample, v0homogen/v0homogen.w, v1homogen/v1homogen.w, v2homogen/v2homogen.w) && w <= depth[y][x])
					{
						depth[y][x] = w;
						image[y][x] = colors[indices[3 * idx] % 6];
					}
				}
			}
		}
	}

	for (int y = image_height - 1; y >= 0; y--)
		for (int x = 0; x < image_width; x++)
			write_color(cout, image[y][x]);


	std::cerr << "\nDone\n";
}