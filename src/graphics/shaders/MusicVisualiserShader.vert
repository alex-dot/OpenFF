layout(location = 0) in vec2 position;

uniform float index;
uniform float max_index;
uniform float channel;
uniform float height;
uniform float height_prev1;
uniform float height_prev2;

void main() {
  float c = -1.0 + 2*channel;
  float h = (height + height_prev1 + height_prev2) / 3.0;

  mat4 mvp = transpose(mat4(0.15/max_index,    0.0, 0.0, c*(0.05+(0.9/max_index)*index),
                               0.0, 0.8*h, 0.0, 0.0,
                               0.0,    0.0, 0.0, 0.0,
                               0.0,    0.0, 0.0, 1.0));

  gl_Position = vec4(mvp*vec4(position,0.0,1.0));
}
