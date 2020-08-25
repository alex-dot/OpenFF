layout(location = 0) in vec2 position;
layout(location = 1) in vec2 textureCoordinates;

uniform ivec2 viewportSize;
uniform ivec2 boxSize;
uniform ivec2 boxOffset;
uniform lowp vec2 relativeBillboardRatio;

out lowp vec2 coords;
out lowp vec2 boxSizeFrag;
out lowp vec2 relativeBillboardRatioFrag;

void main() {
  boxSizeFrag = vec2(boxSize);
  relativeBillboardRatioFrag = relativeBillboardRatio;

  // Convert size from pixels to cartesian and modify by billboardratio
  vec2 size = vec2(boxSize)/vec2(viewportSize)*relativeBillboardRatio;

  // Convert offset from pixels to cartesian (w/ billboardratio modification)
  // ******************
  // *        |       *
  // *--[XXXXXXXXXX]--*
  // *        |       *
  // ******************
  // Since initial location is always in the center, calculate remaining pixels
  // to each side of the box, reduce that quotient by the offset in pixels and
  // calculate the cartesian offset.
  vec2 offset = vec2(((viewportSize-boxSize)/2.0-boxOffset)/viewportSize*2.0);
  offset = offset*relativeBillboardRatio;

  mat4 mvp = transpose(mat4(size.x,    0.0, 0.0, -1.0*offset.x,
                               0.0, size.y, 0.0,  1.0*offset.y,
                               0.0,    0.0, 0.0, 0.0,
                               0.0,    0.0, 0.0, 1.0));

  // Prepare in-box coordinates for [0.0;1.0] dimensions, starting top-left
  coords.x = (position.x+1.0)/2.0;
  coords.y = (position.y+1.0)/2.0;

  gl_Position = vec4(mvp*vec4(position,0.0,1.0));
}
