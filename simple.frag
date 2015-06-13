// simple.frag
 
varying vec4 position;
varying vec3 normal;
 
void main (void)
{
  vec3 light = normalize((gl_LightSource[0].position * position.w - gl_LightSource[0].position.w * 
position).xyz);
  vec3 fnormal = normalize(normal);
  float diffuse = max(dot(light, fnormal), 0.0);
  
  vec3 view = -normalize(position.xyz);
  vec3 halfway = normalize(light + view);
  float specular = pow(max(dot(fnormal, halfway), 0.0), gl_FrontMaterial.shininess);
  gl_FragColor = gl_FrontLightProduct[0].diffuse * diffuse
                + gl_FrontLightProduct[0].specular * specular
                + gl_FrontLightProduct[0].ambient;
}
