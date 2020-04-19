layout(location = 0) in vec4 position;
layout(location = 1) in vec2 textureCoordinates;

uniform vec2 relativeBillboardRatio;

out vec2 interpolatedTextureCoordinates;

void main() {
  interpolatedTextureCoordinates = textureCoordinates;

  gl_Position = vec4(position.xy * relativeBillboardRatio, position.zw);
}
