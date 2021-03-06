/**
 * @file
 * @brief Geoscape fragment shader.
 */

#if r_postprocess
	/*
	 * Indicates that gl_FragData is written to, not gl_FragColor.
	 * #extension needs to be placed before all non preprocessor code.
	 */
	#extension GL_ARB_draw_buffers : enable
#endif

in_qualifier vec2 tex;

in_qualifier vec4 ambientLight;
in_qualifier vec4 diffuseLight;
in_qualifier vec4 specularLight;
in_qualifier vec4 diffuseLight2;

in_qualifier vec3 lightVec;
in_qualifier vec3 lightVec2;
in_qualifier vec3 eyeVec;

#ifndef glsl110
	#if r_postprocess
		/** After glsl1110 this need to be explicitly declared; used by fixed functionality at the end of the OpenGL pipeline.*/
		out vec4 gl_FragData[2];
	#else
		/** After glsl1110 this need to be explicitly declared; used by fixed functionality at the end of the OpenGL pipeline.*/
		out vec4 gl_FragColor;
	#endif
#endif

/** Diffuse.*/
uniform sampler2D SAMPLER_DIFFUSE;
/** Blend.*/
uniform sampler2D SAMPLER_BLEND;
/** Normalmap.*/
uniform sampler2D SAMPLER_NORMALMAP;

uniform float BLENDSCALE;
uniform float GLOWSCALE;
uniform vec4 DEFAULTCOLOR;
uniform vec4 CITYLIGHTCOLOR;

const float specularExp = 32.0;

/* index of refraction for water */
const float n1 = 1.0;
const float n2 = 1.333;
const float eta = n1 / n2;

/**
 * @brief Calculate reflection component of Frenel's equations.
 */
float fresnelReflect(in float cos_a) {
	float cos_b = sqrt(1.0 - ((eta * eta) * ( 1.0 - (cos_a * cos_a))));
	float rs = (n1 * cos_a - n2 * cos_b ) / (n1 * cos_a + n2 * cos_b);
	float rp = (n1 * cos_b - n2 * cos_a ) / (n1 * cos_b + n2 * cos_a);
	return ((rs * rs + rp * rp) / 2.0);
}


void main(void) {
	/* blend textures smoothly */
	vec3 diffuseColorA = texture2D(SAMPLER_DIFFUSE, tex).rgb;
	vec3 diffuseColorB = texture2D(SAMPLER_BLEND, tex).rgb;
	vec4 diffuseColor;
	diffuseColor.rgb = ((1.0 - BLENDSCALE) * diffuseColorA) + (BLENDSCALE * diffuseColorB);
	diffuseColor.a = 1.0;

	/* calculate diffuse reflections */
	vec3 V = vec3(normalize(eyeVec).rgb);
	vec3 L = vec3(normalize(lightVec).rgb);
	vec3 N = vec3(normalize(texture2D(SAMPLER_NORMALMAP, tex).rgb * 2.0 - 1.0).rgb);
	float NdotL = clamp(dot(N, L), 0.0, 1.0);
	vec4 reflectColor = diffuseColor * diffuseLight * NdotL;

	/* calculate specular reflections */
	float RdotL = clamp(dot(reflect(-L, N), V), 0.0, 1.0);
	float gloss = texture2D(SAMPLER_NORMALMAP, tex).a;
	/* NOTE: this "d" here is a hack to compensate for the fact
	 * that we're using orthographic projection.
	 */
	float d = clamp(pow(1.0 + dot(V, L), 0.4), 0.0, 1.0);
	float fresnel = 2.0 * fresnelReflect(NdotL);
	vec4 specularColor = (d * d * fresnel * fresnel) * gloss * pow(RdotL, specularExp) * specularLight;

	/* calculate night illumination */
	float diffuseNightColor = texture2D(SAMPLER_DIFFUSE, tex).a;
	float NdotL2 = clamp(dot(N, normalize(lightVec2)), 0.0, 1.0);
	vec4 nightColor = diffuseLight2 * CITYLIGHTCOLOR * diffuseNightColor * NdotL2;

	vec4 color = DEFAULTCOLOR + (ambientLight * diffuseColor) + reflectColor + (0.0 * specularColor) + nightColor;
	vec4 hdrColor = GLOWSCALE *
					( clamp((reflectColor - vec4(0.9, 0.9, 0.9, 0)), 0.0, GLOWSCALE) +
					  1.0 * specularColor + nightColor);
	hdrColor.a = 1.0;

	/* calculate final color */
#if r_postprocess
	gl_FragData[0] = color;
	gl_FragData[1] = hdrColor;
#else
	gl_FragColor.rgb = color.rgb + hdrColor.rgb;
	gl_FragColor.a = color.a;
#endif
}
