#version 330

in vec2 vertexPosition;
in vec3 vertexColor;
in int radiusPos;

uniform float scaleExternal;
uniform float scaleInternal;

out vec3 vertexColorToFS;

void main() {
    if(radiusPos == 0)
    {
        gl_Position = vec4(scaleExternal * vertexPosition.x, scaleExternal * vertexPosition.y, 0.0, 1.0);
    }
    else
    {
        gl_Position = vec4(scaleInternal * vertexPosition.x, scaleInternal * vertexPosition.y, 0.0, 1.0);
    }
    // gl_Position = vec4(savertexPosition, 0.0, 1.0);
    if(scaleInternal == 0){
        vertexColorToFS = vec3(0.0, 0.0, 0.0);
    }else{
        vertexColorToFS = vertexColor;
    }
}