uniform float f_time;
varying vec3 v_texCoord3D;
//varying float f_depth;

void main(void) {
  gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
  v_texCoord3D = gl_Vertex.xyz;
//  f_depth = gl_Position.w;
}
