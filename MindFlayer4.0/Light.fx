//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D gBufferPosition : register(t0);
Texture2D gBufferAlbedo : register(t1);
Texture2D gBufferSpecular : register(t2);
Texture2D gBufferNormal : register(t3);
SamplerState samLinear : register(s0);

cbuffer mfLightShaderAttributes : register(b0)
{
  float4 Position;                          // Camera position
  float4 lightDir;                          // Light direction in world space
  float4 lightDirColor;                     // Light direction color in linear space
  float4 ambientColor;                      // Ambient color linear space
  float4 specularColor;                     // Specular color in linear space
  float specPower;                          // Mult specular factor
  float kDiffuse;                           // Diffuse mult constant
  float kAmbient;                           // Ambient mult constant    
  float kSpecular;                          // Specular mult constant
};

//--------------------------------------------------------------------------------------
/**
 * Shader vertex input
 */
struct VS_INPUT
{
  float3 msPos             : POSITION0;  // Vertex position in model space
  float2 texCoord          : TEXCOORD0;  // Vertex texcoord in normalized space
};

/**
 * Shader vertex output
 */
struct VS_OUTPUT
{
  float4 psPos             : SV_POSITION0;  // Position in projected space
  float2 texCoord          : TEXCOORD0;     // Texcoord in normalized space
};

/**
 * Shader pixel input
 */
struct PS_INPUT
{
  float4 psPos             : SV_POSITION0;  // Position in projected space
  float2 texCoord          : TEXCOORD0;  // Texcoord in normalized space 
};

/**
 * Shader pixel output
 */
struct PS_OUTPUT
{
  float4 color             : SV_TARGET0;     // Position in projected space
};


//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT Input)
{
  // Output
  VS_OUTPUT Output;
  Output.psPos = float4(Input.msPos.xyz, 1.0);
  Output.texCoord = Input.texCoord;

  return Output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
PS_OUTPUT PS(PS_INPUT Input) : SV_Target
{
  // Position texture sampler   
   float3 posMap = gBufferPosition.Sample(samLinear, Input.texCoord.xy).xyz; 

   // Albedo texture sampler   
   float3 albedoMap = gBufferAlbedo.Sample(samLinear, Input.texCoord.xy).xyz;

   // Specular texture sampler   
   float specularMap = gBufferSpecular.Sample(samLinear, Input.texCoord.xy).x;

   // Position texture sampler   
   float3 normalMap = gBufferNormal.Sample(samLinear, Input.texCoord.xy).xyz;

   /**
    * Directional Light
    */
    // Constant
    float3 wsLightDir = normalize(lightDir.xyz);
    float NdL = clamp(dot(-wsLightDir.xyz, normalMap.xyz), 0.0, 1.0);
    float3 wsViewDir = normalize(posMap.xyz - Position.xyz);

    // Diffuse 
    float3 diffuse = albedoMap.xyz * lightDirColor.xyz * NdL * kDiffuse;

    // Specular 
    float3 wsReflect = normalize(reflect(wsLightDir.xyz, normalMap.xyz));
    float VdR = max(dot(-wsViewDir.xyz, wsReflect.xyz), 0.0);
    float specFactor = pow(VdR, specPower);
    float3 specular = specularColor.xyz * specFactor * NdL * kSpecular;

    // Ambient 
    float3 ambient = ambientColor.xyz * (1.0 - NdL) * kAmbient;

    // Output 
    PS_OUTPUT Output;
    Output.color = float4(pow(ambient + diffuse + specular.xyz, 1 / 2.2), 1.0);//float4(pow((diffuseColor.xyz + specularColor.xyz + ambientColor.xyz) * 10.0, 1/2.2), 1.0 - wsPos.w);

    return Output;
}
