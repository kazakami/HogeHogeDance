// texture.frag

uniform sampler2D texture;

varying vec4 position;
varying vec3 normal;

void main (void)
{
  vec4 color = texture2DProj(texture, gl_TexCoord[0]);
  vec3 light = normalize((gl_LightSource[0].position * position.w - gl_LightSource[0].position.w * position).xyz);
  vec3 fnormal = normalize(normal);
  float diffuse = max(dot(light, fnormal), 0.0);

  vec3 view = -normalize(position.xyz);
  vec3 halfway = normalize(light + view);
  float specular = pow(max(dot(fnormal, halfway), 0.0), gl_FrontMaterial.shininess);
  vec4 fragColor = color * (gl_LightSource[0].diffuse * diffuse + gl_LightSource[0].ambient)
    + gl_FrontLightProduct[0].specular * specular;
  float p = 0.5;
  fragColor = vec4(1.0) - (vec4(1.0) - fragColor) * pow(dot(view, fnormal), p);
  /*
  fragColor.x = 1.0 - (1.0 - fragColor.x) * pow(dot(view, fnormal), p);
  fragColor.y = 1.0 - (1.0 - fragColor.y) * pow(dot(view, fnormal), p);
  fragColor.z = 1.0 - (1.0 - fragColor.z) * pow(dot(view, fnormal), p);
  fragColor.w = 1.0 - (1.0 - fragColor.w) * pow(dot(view, fnormal), p);
  */
  gl_FragColor = fragColor;
}