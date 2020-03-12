#version 330

out vec4 pixelColor; 
in vec3 vertexColorToFS;

uniform ivec2 dimensions;
uniform bool  isRGB;

float delta = 2.4 / dimensions.x;
float colorGray = 0.0;
float inverseGray = 0.0;

vec3 color1 = vec3(0.0, 0.0, 0.0);
vec3 color2 = vec3(0.33, 0.83, 0.83);
vec3 color3 = vec3(0.10, 0.02, 0.33);

vec3 color_any;

struct Complex
{
  float real;
  float imaginary;
};

float lengthComplex(Complex c)
{
	return sqrt((c.real * c. real) + (c.imaginary * c.imaginary));
}

Complex squareComplex(Complex c)
{
	float newReal = (c.real * c.real) - (c.imaginary * c.imaginary);
	float newImaginary = 2 * c.real * c.imaginary;
	
	Complex newComplex = Complex(newReal, newImaginary);
	
	return newComplex;
}

Complex sumComplex(Complex c1, Complex c2)
{
	Complex newComplex = Complex(c1.real + c2.real, c1.imaginary + c2.imaginary);
	return newComplex;
}


void main() {
	Complex X = Complex(-1.2 + (gl_FragCoord.x * delta), 1.2 - (gl_FragCoord.y * delta));
	Complex Z = Complex(-0.74543, 0.11301);

	int count = 0;
	
	while(lengthComplex(X) <= 2.0 && count < 128.0) {
		X = sumComplex(squareComplex(X), Z);
		count++;
	}
	
	if(lengthComplex(X) <= 2.0 ) {
		colorGray = 0;
		color_any = color1;
	}
	else {
		colorGray = count / 128.0;
		inverseGray = 1.0 - colorGray;
	
		color_any = vec3((colorGray * color2) + (inverseGray * color3));   	
	}
	
	if(!isRGB) {
		pixelColor = vec4(colorGray, colorGray, colorGray, 1.0);
	}
	else {
		pixelColor = vec4(color_any, 1.0);
	}
	
}
