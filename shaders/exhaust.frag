//==============================================================================
// Combined rocket exhaust shader
//==============================================================================

uniform float time;
uniform vec3 v_cameraPos;
varying vec3 v_texCoord3D;


vec4 permute(vec4 x) { return mod(((x*34.0)+1.0)*x, 289.0); }
vec4 taylorInvSqrt(vec4 r) { return 1.79284291400159 - 0.85373472095314 * r; }

float snoise(vec3 v) { //Simplex 3D noise
  const vec2  C = vec2(1.0/6.0, 1.0/3.0);
  const vec4  D = vec4(0.0, 0.5, 1.0, 2.0);

  // First corner
  vec3 i  = floor(v + dot(v, C.yyy));
  vec3 x0 =   v - i + dot(i, C.xxx);

  // Other corners
  vec3 g = step(x0.yzx, x0.xyz);
  vec3 l = 1.0 - g;
  vec3 i1 = min( g.xyz, l.zxy );
  vec3 i2 = max( g.xyz, l.zxy );

  //   x0 = x0 - 0. + 0.0 * C
  vec3 x1 = x0 - i1 + 1.0 * C.xxx;
  vec3 x2 = x0 - i2 + 2.0 * C.xxx;
  vec3 x3 = x0 - 1. + 3.0 * C.xxx;

  // Permutations
  i = mod(i, 289.0 );
  vec4 p = permute( permute( permute(
             i.z + vec4(0.0, i1.z, i2.z, 1.0 ))
           + i.y + vec4(0.0, i1.y, i2.y, 1.0 ))
           + i.x + vec4(0.0, i1.x, i2.x, 1.0 ));

  // Gradients
  // ( N*N points uniformly over a square, mapped onto an octahedron.)
  float n_ = 1.0/7.0; // N=7
  vec3  ns = n_ * D.wyz - D.xzx;

  vec4 j = p - 49.0 * floor(p * ns.z *ns.z);  //  mod(p,N*N)

  vec4 x_ = floor(j * ns.z);
  vec4 y_ = floor(j - 7.0 * x_ );    // mod(j,N)

  vec4 x = x_ *ns.x + ns.yyyy;
  vec4 y = y_ *ns.x + ns.yyyy;
  vec4 h = 1.0 - abs(x) - abs(y);

  vec4 b0 = vec4( x.xy, y.xy );
  vec4 b1 = vec4( x.zw, y.zw );

  //vec4 s0 = vec4(lessThan(b0,0.0))*2.0 - 1.0;
  //vec4 s1 = vec4(lessThan(b1,0.0))*2.0 - 1.0;
  vec4 s0 = floor(b0)*2.0 + 1.0;
  vec4 s1 = floor(b1)*2.0 + 1.0;
  vec4 sh = -step(h, vec4(0.0));

  vec4 a0 = b0.xzyw + s0.xzyw*sh.xxyy ;
  vec4 a1 = b1.xzyw + s1.xzyw*sh.zzww ;

  vec3 p0 = vec3(a0.xy,h.x);
  vec3 p1 = vec3(a0.zw,h.y);
  vec3 p2 = vec3(a1.xy,h.z);
  vec3 p3 = vec3(a1.zw,h.w);

  //Normalise gradients
  vec4 norm = taylorInvSqrt(vec4(dot(p0,p0), dot(p1,p1), dot(p2, p2), dot(p3,p3)));
  p0 *= norm.x;
  p1 *= norm.y;
  p2 *= norm.z;
  p3 *= norm.w;

  // Mix final noise value
  vec4 m = max(0.6 - vec4(dot(x0,x0), dot(x1,x1), dot(x2,x2), dot(x3,x3)), 0.0);
  m = m * m;
  return 42.0 * dot( m*m, vec4( dot(p0,x0), dot(p1,x1),
                                dot(p2,x2), dot(p3,x3) ) );
}
//  float n = snoise(v_texCoord3D) + snoise(v_texCoord3D*2.0)*0.5 + snoise(v_texCoord3D*4.0)*0.25;
//
//  gl_FragColor = vec4(0.5 + 0.6 * vec3(n, n, n), 1.0);


//==============================================================================
void main(void) {
  const float num_samples = 8.0;

  //Compute ray start and end
  vec3 v_start = v_cameraPos;
  vec3 v_end = v_texCoord3D;
  vec3 v_ray = v_end - v_start;
  
  //Compute intersection of ray with infinite cylinder
  float A = pow(v_ray.y,2.0) + pow(v_ray.z,2.0);
  float B = 2.0*v_start.y*v_ray.y + 2.0*v_start.z*v_ray.z;
  float C = pow(v_start.y,2.0) + pow(v_start.z,2.0) - 1.0;
  float D = B*B-4.0*A*C;

  if (D >= 0.0) {
    //Intersection times
    float t1 = (-B - sqrt(D))/(2.0*A); //Near
    float t2 = (-B + sqrt(D))/(2.0*A); //Far
    float dt = (t2-t1)/num_samples;
    float dx = length(v_ray*dt);

    //Clamp in case camera dives inside the cylinder
    t1 = max(t1,0.0);
    t2 = max(t2,0.0);

    //Find intersection of ray with plane
//    float t0 = dot(vec3(1.0,0.0,0.0),-v_start) / dot(v_start,v_ray);
//    t1 = min(t0,t1);
//    t2 = max(t0,t2);

    //Hack to improve rendering when cross exhaust
    float falloff_hack = min(1.0,0.5*t1);

    //Ray-trace the exhaust
    vec3 v_totalEmission = vec3(0.0, 0.0, 0.0);
    for (int i=0; i < int(num_samples); i++) {
      //Ray point coordinates
      vec3 v_emission = vec3(0,0,0);
      vec3 v_position = v_start + v_ray*(t1+dt*float(i));
      float r2 = pow(v_position.y,2.0)+pow(v_position.z,2.0);
      float r = sqrt(r2);
      float x = v_position.x-1.0;
      
      //Noise coordinates
      vec3 v_noise = v_position;
      v_noise.x = v_noise.x/6.0 - 7.5*time;
      
      //Constants
      const float radius   = 0.5;  //Nozzle radius
      const float core_ef  = 4.0;  //Exponential falloff (radial)
      const float core_lf  = 0.0;  //Exponential falloff (length)
      const float core_cef = 4.0; //Cutoff falloff (radial)
      const float core_clf = 0.0;  //Cutoff falloff (length)
      
      
      //Intensity: exhaust core
      float r_cutoff = radius*exp(-core_clf*x);
      float r_falloff = exp(-core_ef*r2)*
                        min(1.0,exp(-core_cef*(r-r_cutoff)));
      float x_falloff = min(1.0,exp(-core_lf*x))* //Exhaust
                        min(1.0,exp(falloff_hack*4.0*x));     //Inside engine
      float core_intensity = x_falloff*r_falloff;
      
      
      //Modulate exhaust core
      float noise =
        2.50*snoise(v_noise*1.0)+
        0.80*snoise(v_noise*3.0)+
        0.50*snoise(v_noise*6.0);
        
      noise *= min(1.0,abs(0.5*x));
        
      noise = clamp(2.5+noise-0.4*x,0.0,1.0);
      core_intensity *= noise;
        
      
      //Return emission
      v_emission = 4.00*vec3(0.85,0.75,0.50)*core_intensity;
      
      v_totalEmission += 2.0*v_emission*dx;
    }
    
    gl_FragColor = vec4(v_totalEmission,1);
  } else {
    gl_FragColor = vec4(0,0,0,0);
  }
}
