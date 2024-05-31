#version 440 core

uniform mat4 Model;
uniform mat4 View;
uniform mat4 ModelView;		// View * Model
uniform mat4 Projection;
uniform mat3 NormalMatrix;

in vec3 vPosition;			// Coordenadas locais do v�rtice
in vec3 vNormal;			// Normal do v�rtice
in vec3 InTextCoord;		// UVs  

out vec3 vPositionEyeSpace;
out vec3 vNormalEyeSpace;
out vec2 OutTextCoord;


void main()
{ 
	vPositionEyeSpace = (ModelView * vec4(vPosition, 1.0)).xyz;


	vNormalEyeSpace = normalize(NormalMatrix * vNormal);
	OutTextCoord = InTextCoord.xy;	//MUITO MAIS SIMPLES DO QUE A STRUCT MESH
	gl_Position = Projection * ModelView * vec4(vPosition, 1.0f);
}