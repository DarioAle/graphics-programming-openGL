#version 400

const int N = 3;

struct Light {
	vec3 lightColor;
	float subCutOff;
	vec3 lightPosition;
	float exponent;
	vec3 lightDirection;
	float cutoff;
};

uniform LightBlock {
   Light lights[N];
};

in vec3 worldVertexPosition;
in vec3 worldVertexNormal;

uniform vec3 cameraPosition;
uniform vec3 ambientLight;

uniform vec3 materialA;
uniform vec3 materialD;
uniform vec3 materialS;

out vec4 pixelColor;

void main() {
  vec3 n  = normalize(worldVertexNormal);
  vec3 tempPixelColor = ambientLight *  materialA;
  vec3 l, r, d;
  float factorD, factorS;
  vec3 v = normalize(cameraPosition - worldVertexPosition);

  int i;
  for(i = 0; i < N; i++) {    
  	l = normalize(lights[i].lightPosition - worldVertexPosition);  	
  	d = normalize(lights[i].lightDirection);
  	
  	float cosAlpha = clamp(dot(d, -l), 0, 1);
  	if(cosAlpha <  lights[i].cutoff) continue;
  	
  	float FDegradacion = 1.0;
  	if(cosAlpha >= lights[i].subCutOff) FDegradacion = 1.0;
  	else FDegradacion = (cosAlpha - lights[i].cutoff) / (lights[i].subCutOff - lights[i].cutoff);
  	
  	
  	factorD = clamp(dot(l, n), 0, 1);
  	r = normalize((2 * n)  * dot(n, l) - l);
  	factorS = clamp(pow(dot(r, v), lights[i].exponent), 0, 1);
 	tempPixelColor += FDegradacion * lights[i].lightColor * (materialD * factorD + materialS * factorS);
  }

  pixelColor = vec4(clamp(tempPixelColor, 0, 1), 1);
}

