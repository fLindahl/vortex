struct PointLight {
	vec4 color;
	vec4 position;
	vec4 radiusAndPadding;
};

struct VisibleIndex {
	int index;
};

// Shader storage buffer objects
layout(std430, binding = 1) readonly buffer LightBuffer{
	PointLight data[];
} lightBuffer;

layout(std430, binding = 2) readonly buffer VisibleLightIndicesBuffer{
	VisibleIndex data[];
} visibleLightIndicesBuffer;

uniform int totalLightCount;

out vec4 fragColor;

void main() {
	// Determine which tile this pixel belongs to
	ivec2 location = ivec2(gl_FragCoord.xy);
	ivec2 tileID = location / ivec2(16, 16);
	uint index = tileID.y * LightTileWorkGroups.x + tileID.x;

	uint offset = index * 1024;
	uint i;
	for (i = 0; i < 1024 && visibleLightIndicesBuffer.data[offset + i].index != -1; i++);

	float ratio = float(i) / float(totalLightCount);
	fragColor = vec4(vec3(ratio, ratio, ratio), 1.0);
}


