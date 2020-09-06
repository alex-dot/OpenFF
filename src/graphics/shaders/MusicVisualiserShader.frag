uniform vec3 color = vec3(1.0, 1.0, 1.0);
uniform float index;
uniform float max_index;

out vec4 fragmentColor;

void main() {
  fragmentColor.rgb = color.rgb;
  fragmentColor.a = 1.0-index/max_index;
}
