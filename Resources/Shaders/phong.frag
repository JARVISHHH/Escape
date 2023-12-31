#version 330 core
// Uniforms for shape information
in vec3 worldSpace_pos;
in vec3 particular_norm;
in vec2 tex_coord;

in vec4 lightSpace_pos[16];

// Object Material Data
uniform int colorSource; // 0 = solid color (objColor), 1 = texture color (objTexture), 2 = per-vertex color (vertColor)
uniform vec3 objColor;
uniform sampler2D objTexture;
in vec3 vertColor;
uniform float shininess;
uniform float alpha;  // Object alpha value
uniform int fragNormalSource;
in mat3 TBN;
uniform sampler2D normalMap;

// Camera uniform
uniform vec3 worldSpace_camPos;

// Global Data
uniform vec3 coeffs; // vec3(ka, kd, ks)

// Light Data
uniform int lightType[16]; // 0 = point light, 1 = directional light
uniform vec3 lightColor[16];
uniform vec3 lightFunction[16]; // Attenuation coefficients
uniform vec3 worldSpace_lightPos[16]; //Light Positions
uniform vec3 worldSpace_lightDir[16]; //Light Directions
uniform int numLights; // Max number of lights = 16

// Shadow uniform
uniform sampler2D depthMaps[16];
uniform samplerCube depthCubeMaps[16];
uniform float farPlane;

out vec4 fragColor;

vec3 getToLight(int lightIndex) {
    int LIGHT_POINT = 0;
    int LIGHT_DIRECTIONAL = 1;

    if (lightType[lightIndex] == LIGHT_POINT) {
        return normalize(worldSpace_lightPos[lightIndex] - worldSpace_pos);
    }
    else if (lightType[lightIndex] == LIGHT_DIRECTIONAL) {
        return normalize(-worldSpace_lightDir[lightIndex]);
    }

    return vec3(0);
}

float attenuationFactor(int lightIndex) {
    int LIGHT_POINT = 0;

    if (lightType[lightIndex] == LIGHT_POINT) {
        vec3 coeffs = lightFunction[lightIndex];
        float d = length(worldSpace_lightPos[lightIndex] - worldSpace_pos);
        return 1.0 / (coeffs.x + coeffs.y * d + coeffs.z * d * d);
    }

    return 1;
}

float computeDiffuseIntensity(vec3 worldSpace_toLight, vec3 worldSpace_norm) {
    // Dot product to get diffuse intensity
    return max(dot(worldSpace_toLight, normalize(worldSpace_norm)), 0);
}

float computeSpecularIntensity(vec3 worldSpace_toLight, vec3 worldSpace_toEye, vec3 worldSpace_norm) {
    // Guard against pow weirdness when exponent is 0
    if (shininess == 0) {
        return 0;
    }

    //reflect toLight
    vec3 worldSpace_toLightReflected = reflect(-worldSpace_toLight, normalize(worldSpace_norm));

    //Compute specular intensity using toEye, reflected light, and shininess
    return pow(max(dot(worldSpace_toLightReflected, worldSpace_toEye), 0), shininess);
}

float shadowCalculation(int index) {
    int LIGHT_POINT = 0;
    int LIGHT_DIRECTIONAL = 1;

    vec4 fragPosLightSpace = lightSpace_pos[index];

    float closestDepth, currentDepth;

    if(lightType[index] == LIGHT_POINT) {
        vec3 frag2light = worldSpace_pos - worldSpace_lightPos[index];
        currentDepth = length(frag2light);
        closestDepth = texture(depthCubeMaps[index], frag2light).r * farPlane;
    } else {
        // perform perspective divide
        vec3 projCoords = fragPosLightSpace.xyz / fragPosLightSpace.w;
        // transform to [0,1] range
        projCoords = projCoords * 0.5 + 0.5;
        if(projCoords.x < 0 || projCoords.x > 1 || projCoords.y < 0 || projCoords.y > 1) return 1.0;
        // get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
        closestDepth = texture(depthMaps[index], projCoords.xy).x; 
        // get depth of current fragment from light's perspective
        currentDepth = projCoords.z;
    }
    // check whether current frag pos is in shadow
    float bias = 0.05;
    float shadow = currentDepth - bias > closestDepth  ? 1.0 : 0.0;

    return shadow;
}

void main() {

    vec3 worldSpace_norm = particular_norm;
    if(fragNormalSource == 1) {
        vec3 normal = texture(normalMap, tex_coord).rgb;
        normal = normal * 2.0 - 1.0;   
        worldSpace_norm = normalize(TBN * normal); 
    } else {
        worldSpace_norm = particular_norm;
    }
    // worldSpace_norm = particular_norm;

    // Declare ambient, diffuse, and specular terms
    vec3 ambi = vec3(coeffs.x);
    vec3 diff = vec3(0.0);
    vec3 spec = vec3(0.0);


    // Compute worldSpace_toEye Vector for specular intensity computation;
    vec3 worldSpace_toEye = normalize(worldSpace_camPos - worldSpace_pos);


    // Compute per-light diffuse and specular contribution
    for(int i = 0; i<numLights; i+= 1){

        float shadow = shadowCalculation(i);

        // get direction vector to light based on light type
        vec3 worldSpace_toLight = getToLight(i);

        float diffuse_intensity = computeDiffuseIntensity(worldSpace_toLight, worldSpace_norm);
        float specular_intensity = computeSpecularIntensity(worldSpace_toLight, worldSpace_toEye, worldSpace_norm);

        float att = attenuationFactor(i);


        diff = diff + diffuse_intensity * lightColor[i] * att * (1.0 - shadow);
        spec = spec + specular_intensity * lightColor[i] * att * (1.0 - shadow);
    }

    // Apply global coefficients and object color to the diffuse and specular components
    diff = diff * vec3(coeffs.y);
    spec = spec * vec3(coeffs.z);

    // Color generated only from light intensities and colors
    vec3 tempColor = clamp(ambi + diff + spec, 0, 1);

    // Apply correct object color
    if (colorSource == 0 ) {
        fragColor = vec4(tempColor * objColor, alpha);
    } 
    else if (colorSource == 1){
        fragColor = vec4(tempColor * vec3(texture(objTexture, tex_coord)), alpha);
    }
    else if (colorSource == 2) {
        fragColor = vec4(tempColor * vertColor, alpha);
    }
    else{
        fragColor = vec4(tempColor, alpha);
    }
}