//--------------------------------------------------------------------------------------
// File: GBuffer.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D albedoTex : register(t0);
Texture2D normalTex : register(t1);
Texture2D specularTex : register(t2);
SamplerState samLinear : register(s0);

cbuffer camera : register(b0)
{
  float4x4 View;
  float4x4 Projection;
};

cbuffer cbChangesEveryFrame : register(b1)
{
  matrix World;
  float4 vMeshColor;
};


//--------------------------------------------------------------------------------------
struct VS_INPUT
{
  float3 msPos             : POSITION0;   // Vertex position in model space
  float2 texCoord          : TEXCOORD0;  // Vertex texcoord in normalized space
  float3 msNormal          : NORMAL0;    // Vertex normal in model space
  float3 msTangent         : TANGENT0;   // Vertex tangent in model space
};

/**
 * Shader vertex output
 */
struct VS_OUTPUT
{
  float4 psPos             : SV_POSITION0;  // Position in projected space
  float4 wsPos             : TEXCOORD0;  // Position in world space
  float2 texCoord          : TEXCOORD1;  // Texcoord in normalized space 
  float3 wsNormal          : NORMAL0;    // Normal in world space
  float3 wsTangent         : TANGENT0;   // Tangent in world space
};

/**
 * Shader pixel input
 */
struct PS_INPUT
{
  float4 psPos             : SV_POSITION0;  // Position in projected space
  float4 wsPos             : TEXCOORD0;  // Position in world space
  float2 texCoord          : TEXCOORD1;  // Texcoord in normalized space 
  float3 wsNormal          : NORMAL0;    // Normal in world space
  float3 wsTangent         : TANGENT0;   // Tangent in world space
};

/**
 * Shader pixel output
 */
struct PS_OUTPUT
{
  float4 posRT             : SV_TARGET0;     // Position in world space
  float4 normalRT          : SV_TARGET1;     // Normal in world space
  float4 colorRT           : SV_TARGET2;     // Albedo color in linear space
  float4 specularRT        : SV_TARGET3;     // Specular map in linear space
};



//--------------------------------------------------------------------------------------
// Vertex Shader
//--------------------------------------------------------------------------------------
VS_OUTPUT VS(VS_INPUT Input)
{
  // Position in world space
  float4 wsPos = mul(float4(Input.msPos.xyz, 1.0f), World);

  // Position in view space
  float4 vsPos = mul(wsPos.xyzw, View);

  // Position in projected space
  float4 psPos = mul(vsPos.xyzw, Projection);

  // Normal in world space
  float3 wsNormal = mul(float4(Input.msNormal.xyz, 0.0f), World).xyz;
  wsNormal = normalize(wsNormal);

  // Tangent in world space
  float3 wsTangent = mul(float4(Input.msTangent.xyz, 0.0f), World).xyz;
  wsTangent = normalize(wsTangent);

  // Output
  VS_OUTPUT Output;
  Output.psPos = psPos;
  Output.wsPos = wsPos;
  Output.texCoord = Input.texCoord;
  Output.wsNormal = wsNormal;
  Output.wsTangent = wsTangent;

  return Output;
}


//--------------------------------------------------------------------------------------
// Pixel Shader
//--------------------------------------------------------------------------------------
PS_OUTPUT PS(PS_INPUT Input)
{
  // Color texture sampler   
  float4 color = albedoTex.Sample(samLinear, Input.texCoord.xy).xyzw;
  color = pow(color.xyzw, 2.2f); // Se manda a espacio lineal 0 a 255, pasa a espacio de floatantes

  // Specular texture sampler   
  float specular = specularTex.Sample(samLinear, Input.texCoord.xy).x;
  specular.x = pow(specular.x, 2.2f);

  // World space normal
  float3 normal = normalTex.Sample(samLinear, Input.texCoord.xy).xyz;
  normal *= 2.0f - 1.0f;

  float3 wsBinormal = cross(Input.wsNormal.xyz, Input.wsTangent.xyz).xyz;
  wsBinormal = normalize(wsBinormal.xyz);

  float3 wsNormal = mul(normal, float3x3(Input.wsTangent.xyz, wsBinormal, Input.wsNormal.xyz));
  wsNormal = normalize(wsNormal.xyz);

  // Output 
  PS_OUTPUT Output;
  Output.posRT = float4(Input.wsPos.xyz, 1.0);
  Output.colorRT = float4(color.xyz, 1.0f);
  Output.normalRT = float4(wsNormal.xyz, 1.0f);
  Output.specularRT = float4(specular.xxx, 1.0f);

  //Output.posRT = float4(1,1,0, 0.0);
  //Output.colorRT = float4(1,0,1, 0.0f);
  //Output.normalRT = float4(0,1,1, 0.0f);
  //Output.specularRT = float4(1,1,1, 0.0f);
  return Output;    
}
