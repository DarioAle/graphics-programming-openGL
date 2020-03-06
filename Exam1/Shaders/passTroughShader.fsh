#version 330


out vec4 pixelColor; 
in vec3 vertexColorToFS;

void main() {
	pixelColor = vec4(vertexColorToFS, 1.0);
	
}
