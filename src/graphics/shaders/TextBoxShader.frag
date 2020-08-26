uniform sampler2D textureData;

uniform int boxColorMode = 0;                         // 0=unicolor; 1=gradient
uniform lowp vec3 boxUniColor = vec3(0.0,0.0,0.8);
uniform lowp vec3 boxTopLeftColor = vec3(1.0,0.0,0.0);
uniform lowp vec3 boxTopRightColor = vec3(0.0,1.0,0.0);
uniform lowp vec3 boxBottomLeftColor = vec3(0.0,0.0,0.0);
uniform lowp vec3 boxBottomRightColor = vec3(0.0,0.0,1.0);

in lowp vec2 coords;
in lowp vec2 boxSizeFrag;
in lowp vec2 relativeBillboardRatioFrag;

out vec4 fragmentColor;

float lookupBorder(float, float, float);
vec2  lookupBorder( vec2,  vec2,  vec2);

void main() {
  vec2 pixelRatio = 1.0/vec2(boxSizeFrag);

  // Not in border-area, draw prepared color
  if( coords.x >= 3.0*pixelRatio.x && coords.y >= 3.0*pixelRatio.y
   && coords.x <= (boxSizeFrag.x-3.0)*pixelRatio.x
   && coords.y <= (boxSizeFrag.y-3.0)*pixelRatio.y ) {

    if(      boxColorMode == 0 ) fragmentColor.rgb = boxUniColor;
    else if( boxColorMode == 1 ) {
      fragmentColor.rgb  = (1.0-coords.x)*(1.0-coords.y)*boxTopLeftColor;
      fragmentColor.rgb += coords.x*(1.0-coords.y)*boxTopRightColor;
      fragmentColor.rgb += (1.0-coords.x)*coords.y*boxBottomLeftColor;
      fragmentColor.rgb += coords.x*coords.y*boxBottomRightColor;
    }
    else fragmentColor.rgb = vec3(0.0);

    fragmentColor.a = 1.0;
  } else { // Draw border
    fragmentColor = texture(textureData, lookupBorder(coords,pixelRatio,boxSizeFrag));
  }
}

float lookupBorder(float coord, float ratio, float size) {
  if( coord < 4.0*ratio ) {
    return floor(coord/ratio);
  } else if ( coord > (size-4.0)*ratio ) {
    return 9.1-ceil((1.0-coord)/ratio);
  } else {
    return 4.0;
  }
}
vec2 lookupBorder(vec2 coord, vec2 ratio, vec2 size) {
  return vec2((lookupBorder(coord.x,ratio.x,size.x))/8.0,
              (lookupBorder(1.0-coord.y,ratio.y,size.y))/8.0);
}
