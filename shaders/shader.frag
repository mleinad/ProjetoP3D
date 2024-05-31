#version 440

uniform mat4 Model;
uniform mat4 View;
uniform mat4 ModelView;


out vec4 color;


uniform vec4 u_Color;

struct AmbientLight {
	vec3 ambient;	
};

uniform AmbientLight ambientLight;

struct DirectionalLight	{
	vec3 direction;		
	
	vec3 ambient;		
	vec3 diffuse;		
	vec3 specular;		
};

uniform DirectionalLight directionalLight;

struct PointLight	{
	vec3 position;		
	
	vec3 ambient;		
	vec3 diffuse;		
	vec3 specular;		
	
	float constant;		
	float linear;		
	float quadratic;	
};

uniform PointLight pointLight;

struct SpotLight {
	vec3 position;		
	
	vec3 ambient;		
	vec3 diffuse;		
	vec3 specular;		
	
	float constant;		
	float linear;		
	float quadratic;	

	float spotCutoff, spotExponent;
	vec3 spotDirection;
};

uniform SpotLight spotLight;

struct Material{
	vec3 emissive;
	vec3 ambient;		// Ka
	vec3 diffuse;		// kd
	vec3 specular;		// ke
	float shininess;
};

uniform Material material;
vec3 diffuseColor;



uniform bool spotLightOn, pointLightOn, directionalLightOn, ambientLightOn;


uniform bool IsTable; //soluçao da treta -> implementar multiplos shaders



uniform sampler2D Texture;

in vec3 vPositionEyeSpace;
in vec3 vNormalEyeSpace;
//layout (location = 2) in vec2 InTextCoord;
in vec3 InTextCoord;

in vec3 vPositionWorldSpace;
in vec3  vNormalWorldSpace;


vec4 calcAmbientLight(AmbientLight light);
vec4 calcDirectionalLight(DirectionalLight light, out vec4 ambient);
vec4 calcPointLight(PointLight light, out vec4 ambient);
vec4 calcSpotLight(SpotLight light, out vec4 ambient);



void main()
{
	diffuseColor = material.diffuse;
	vec4 emissive = vec4(material.emissive, 1.0);

	vec4 ambient;
	vec4 light1, light2, light3;
	vec4 ambientTemp;

	if(ambientLightOn)
	{
		ambient = calcAmbientLight(ambientLight);
	}
	else 
	{
		ambient = vec4(0,0,0,0);
	}
	
	if(directionalLightOn)
	{
		light1 = calcDirectionalLight(directionalLight, ambientTemp);
		ambient += ambientTemp;
	}
	else
	{
		light1= vec4(0,0,0,0);
	}
	
	if(pointLightOn)
	{
		light2 = calcPointLight(pointLight, ambientTemp);
		ambient += ambientTemp;
	}
	else
	{
		light2 = vec4(0,0,0,0);
	}
	
	if(spotLightOn)
	{
		light3 = calcSpotLight(spotLight, ambientTemp);
		ambient += ambientTemp;
	}
	else
	{
		light3 = vec4(0,0,0,0);
	}

	//vec4 textColor = texture(Texture, InTextCoord);


	//exprimentar

	if(IsTable)
	{
		color = vec4(InTextCoord.x,InTextCoord.y ,1.0f,1.0f) *( emissive + (ambient/4) + light1 + light2 +light3);
	}
	else
	{

	//color = textColor * (emissive + (ambient/4) + light1 + light2 +light3);
	
	color = (emissive + (ambient/4) + light1 + light2 +light3);
	}
	
}


vec4 calcAmbientLight(AmbientLight light)
{
	vec4 ambient = vec4(material.ambient * light.ambient, 1.0);
	return ambient;
}

vec4 calcDirectionalLight(DirectionalLight light, out vec4 ambient)
{
	
	ambient = vec4(material.ambient * light.ambient, 1.0);
	vec3 lightDirectionEyeSpace = (View * vec4(light.direction, 0.0)).xyz;
	vec3 L = normalize(-lightDirectionEyeSpace);

	vec3 N = normalize(vNormalEyeSpace);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(diffuseColor * light.diffuse, 1.0) * NdotL;
		
	vec3 V = normalize(-vPositionEyeSpace);
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);

	vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);

	return (diffuse + specular);
}

vec4 calcPointLight(PointLight light, out vec4 ambient){

	ambient = vec4(material.ambient * light.ambient, 1.0);

	vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
	vec3 L = normalize(lightPositionEyeSpace - vPositionEyeSpace);
	vec3 N = normalize(vNormalEyeSpace);
	float NdotL = max(dot(N, L), 0.0);
	vec4 diffuse = vec4(diffuseColor * light.diffuse, 1.0) * NdotL;

	vec3 V = normalize(-vPositionEyeSpace);
	vec3 R = reflect(-L, N);
	float RdotV = max(dot(R, V), 0.0);
	vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);
	
	float dist = length(mat3(View) * light.position - vPositionEyeSpace);	
	float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

	return (attenuation * (diffuse + specular));
}


vec4 calcSpotLight(SpotLight light, out vec4 ambient) 
{
    ambient = vec4(material.ambient * light.ambient, 1.0);

    vec3 lightPositionEyeSpace = (View * vec4(light.position, 1.0)).xyz;
    vec3 spotDirectionEyeSpace = normalize((View * vec4(light.spotDirection, 0.0)).xyz);

    vec3 L = normalize(lightPositionEyeSpace - vPositionEyeSpace);
    vec3 N = normalize(vNormalEyeSpace);

    float theta = dot(L, -spotDirectionEyeSpace);
    float epsilon = light.spotCutoff - light.spotExponent;
    float intensity = clamp((theta - light.spotExponent) / epsilon, 0.0, 1.0);

    float NdotL = max(dot(N, L), 0.0);
    vec4 diffuse = vec4(diffuseColor * light.diffuse, 1.0) * NdotL;

    vec3 V = normalize(-vPositionEyeSpace);
    vec3 R = reflect(-L, N);
    float RdotV = max(dot(R, V), 0.0);
    vec4 specular = pow(RdotV, material.shininess) * vec4(light.specular * material.specular, 1.0);

    float dist = length(lightPositionEyeSpace - vPositionEyeSpace);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    diffuse *= intensity;
    specular *= intensity;

    return attenuation * (diffuse + specular);
}
