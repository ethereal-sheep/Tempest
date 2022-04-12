#version 460
/**********************************************************************************
* \author		Lim Yong Kiang, Darren (lim.y@digipen.edu)
* \author		Tiong Jun Ming, Jerome (j.tiong@digipen.edu)
* \version		1.0
* \date			2022
* \note			Course: GAM350
* \copyright	Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
				or disclosure of this file or its contents without the prior
				written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
in vec2 TexCoords;
out vec4 colorOutput;

float FXAA_SPAN_MAX = 8.0f;
float FXAA_REDUCE_MUL = 1.0f/8.0f;
float FXAA_REDUCE_MIN = 1.0f/128.0f;
float middleGrey = 0.18f;
float bluramount  = 1.0;
float center      = 1.1;
float stepSize    = 0.004;
float steps       = 3.0;
float minOffs     = (float(steps-1.0)) / -2.0;
float maxOffs     = (float(steps-1.0)) / +2.0;

uniform sampler2D screenTexture;
uniform sampler2D sao;
uniform sampler2D gEffects;
uniform sampler2D bloomBlur;

uniform int gBufferView;
uniform int tonemappingMode;
uniform bool saoMode;
uniform bool fxaaMode;
uniform bool tiltShiftMode;
uniform float cameraAperture;
uniform float cameraShutterSpeed;
uniform float cameraISO;
uniform vec2 screenTextureSize;
uniform float gammaValue;

vec3 colorLinear(vec3 colorVector);
vec3 colorSRGB(vec3 colorVector);
vec3 ReinhardTM(vec3 color);
vec3 FilmicTM(vec3 color);
vec3 UnchartedTM(vec3 color);
float computeSOBExposure(float aperture, float shutterSpeed, float iso);
vec3 computeFxaa();


void main()
{
    vec3 color;
	float amount = pow((TexCoords.y * center) * 2.0 - 1.0, 2.0) * bluramount;
    vec4 blurredColor = vec4(0.0, 0.0, 0.0, 1.0);
    vec3 bloomColor = texture(bloomBlur, TexCoords).rgb;

    if(gBufferView == 1)
    {
		if(tiltShiftMode)
		{
			for(float offsX = minOffs; offsX <= maxOffs; ++offsX) 
			{
				for(float offsY = minOffs; offsY <= maxOffs; ++offsY) 
				{				
					vec2 temp_tcoord = TexCoords.xy;
					
					temp_tcoord.x += offsX * amount * stepSize;
					temp_tcoord.y += offsY * amount * stepSize;
					
					blurredColor += texture2D(screenTexture, temp_tcoord);			
				} // for y
			} // for x
			blurredColor /= float(steps * steps);
			color = blurredColor.rgb;
		}
		else
		{
			// FXAA computation
			if(fxaaMode)
				color = computeFxaa();  // Don't know if applying FXAA first is a good idea, especially with effects such as motion blur and DoF...
			else
				color = texture(screenTexture, TexCoords).rgb;
		}
		color += bloomColor;
        // SAO computation
        if(saoMode)
        {
            float sao = texture(sao, TexCoords).r;
            color *= sao;
        }

        // Exposure computation
        color *= computeSOBExposure(cameraAperture, cameraShutterSpeed, cameraISO);

        // Tonemapping computation
        if(tonemappingMode == 1)
        {
            color = ReinhardTM(color);
            colorOutput = vec4(colorSRGB(color), 1.0f);
        }
        else if(tonemappingMode == 2)
        {
            color = FilmicTM(color);
			color = colorSRGB(color);
            colorOutput = vec4(color, 1.0f);
        }
        else if(tonemappingMode == 3)
        {
            float W = 11.2f;
            color = UnchartedTM(color);
            vec3 whiteScale = 1.0f / UnchartedTM(vec3(W));

            color *= whiteScale;
            colorOutput = vec4(colorSRGB(color), 1.0f);
        }
    }

    else    // No tonemapping or linear/sRGB conversion if we want to visualize the different buffers
    {
        color = texture(screenTexture, TexCoords).rgb;
		color = colorSRGB(color);
        colorOutput = vec4(color, 1.0f);
    }
}



vec3 computeFxaa()
{
    vec2 screenTextureOffset = screenTextureSize;
    vec3 luma = vec3(0.299f, 0.587f, 0.114f);

    vec3 offsetNW = texture(screenTexture, TexCoords.xy + (vec2(-1.0f, -1.0f) * screenTextureOffset)).xyz;
    vec3 offsetNE = texture(screenTexture, TexCoords.xy + (vec2(1.0f, -1.0f) * screenTextureOffset)).xyz;
    vec3 offsetSW = texture(screenTexture, TexCoords.xy + (vec2(-1.0f, 1.0f) * screenTextureOffset)).xyz;
    vec3 offsetSE = texture(screenTexture, TexCoords.xy + (vec2(1.0f, 1.0f) * screenTextureOffset)).xyz;
    vec3 offsetM  = texture(screenTexture, TexCoords.xy).xyz;

    float lumaNW = dot(luma, offsetNW);
    float lumaNE = dot(luma, offsetNE);
    float lumaSW = dot(luma, offsetSW);
    float lumaSE = dot(luma, offsetSE);
    float lumaM  = dot(luma, offsetNW);

    vec2 dir = vec2(-((lumaNW + lumaNE) - (lumaSW + lumaSE)),
                     ((lumaNW + lumaSW) - (lumaNE + lumaSE)));

    float dirReduce = max((lumaNW + lumaNE + lumaSW + lumaSE) * (FXAA_REDUCE_MUL * 0.25f), FXAA_REDUCE_MIN);
    float dirCorrection = 1.0f / (min(abs(dir.x), abs(dir.y)) + dirReduce);

    dir = min(vec2(FXAA_SPAN_MAX), max(vec2(-FXAA_SPAN_MAX), dir * dirCorrection)) * screenTextureOffset;

    vec3 resultA = 0.5f * (texture(screenTexture, TexCoords.xy + (dir * vec2(1.0f / 3.0f - 0.5f))).xyz +
                                    texture(screenTexture, TexCoords.xy + (dir * vec2(2.0f / 3.0f - 0.5f))).xyz);

    vec3 resultB = resultA * 0.5f + 0.25f * (texture(screenTexture, TexCoords.xy + (dir * vec2(0.0f / 3.0f - 0.5f))).xyz +
                                             texture(screenTexture, TexCoords.xy + (dir * vec2(3.0f / 3.0f - 0.5f))).xyz);

    float lumaMin = min(lumaM, min(min(lumaNW, lumaNE), min(lumaSW, lumaSE)));
    float lumaMax = max(lumaM, max(max(lumaNW, lumaNE), max(lumaSW, lumaSE)));
    float lumaResultB = dot(luma, resultB);

    if(lumaResultB < lumaMin || lumaResultB > lumaMax)
        return vec3(resultA);
    else
        return vec3(resultB);
}



vec3 colorLinear(vec3 colorVector)
{
  vec3 linearColor = pow(colorVector.rgb, vec3(gammaValue));

  return linearColor;
}


vec3 colorSRGB(vec3 colorVector)
{
  vec3 srgbColor = pow(colorVector.rgb, vec3(1.0f / gammaValue));

  return srgbColor;
}


vec3 ReinhardTM(vec3 color)
{
    return color / (color + vec3(1.0f));
}


vec3 FilmicTM(vec3 color)
{
    color = max(vec3(0.0f), color - vec3(0.004f));
    color = (color * (6.2f * color + 0.5f)) / (color * (6.2f * color + 1.7f) + 0.06f);

    return color;
}


vec3 UnchartedTM(vec3 color)
{
  const float A = 0.15f;
  const float B = 0.50f;
  const float C = 0.10f;
  const float D = 0.20f;
  const float E = 0.02f;
  const float F = 0.30f;
  const float W = 11.2f;

  color = ((color * (A * color + C * B) + D * E) / (color * ( A * color + B) + D * F)) - E / F;

  return color;
}


float computeSOBExposure(float aperture, float shutterSpeed, float iso)
{
    float lAvg = (1000.0f / 65.0f) * sqrt(aperture) / (iso * shutterSpeed);

    return middleGrey / lAvg;
}
