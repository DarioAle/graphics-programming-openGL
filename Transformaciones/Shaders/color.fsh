#version 330


out vec4 pixelColor; 
in vec3 vertexColorToFS;

uniform vec3 squareColor;

void main() {
	pixelColor = vec4(vertexColorToFS, 1);
}
