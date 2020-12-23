uniform sampler2D textureData;

uniform int windowColorMode = 0;                       // 0=unicolor; 1=gradient
uniform lowp vec3 windowUniColor = vec3(0.0,0.0,0.8);
uniform lowp vec3 windowTopLeftColor = vec3(1.0,0.0,0.0);
uniform lowp vec3 windowTopRightColor = vec3(0.0,1.0,0.0);
uniform lowp vec3 windowBottomLeftColor = vec3(0.0,0.0,0.0);
uniform lowp vec3 windowBottomRightColor = vec3(0.0,0.0,1.0);

uniform lowp float windowBorderTransparency = float(1.0);
uniform lowp float windowBodyTransparency = float(1.0);

uniform int windowBorderImageLength = 3;
uniform ivec2 windowSize;
uniform lowp vec2 relativeBillboardRatio;

in lowp vec2 coords;

out vec4 fragmentColor;

float lookupBorder(float, float, float);
vec2  lookupBorder( vec2,  vec2,  vec2);

void main() {
  vec2 pixelRatio = 1.0/vec2(windowSize);

  // Not in border-area, draw prepared color
  if( coords.x >= windowBorderImageLength*pixelRatio.x
   && coords.y >= windowBorderImageLength*pixelRatio.y
   && coords.x <= (windowSize.x-windowBorderImageLength)*pixelRatio.x
   && coords.y <= (windowSize.y-windowBorderImageLength)*pixelRatio.y ) {

    if(      windowColorMode == 0 ) fragmentColor.rgb = windowUniColor;
    else if( windowColorMode == 1 ) {
      fragmentColor.rgb  = (1.0-coords.x)*(1.0-coords.y)*windowTopLeftColor;
      fragmentColor.rgb += coords.x*(1.0-coords.y)*windowTopRightColor;
      fragmentColor.rgb += (1.0-coords.x)*coords.y*windowBottomLeftColor;
      fragmentColor.rgb += coords.x*coords.y*windowBottomRightColor;
    }
    else fragmentColor.rgb = vec3(0.0);

    fragmentColor.a = windowBodyTransparency;

  } else { // Draw border
    fragmentColor = texture(textureData, lookupBorder(coords,pixelRatio,windowSize));
    fragmentColor.a -= (1.0-windowBorderTransparency);
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
