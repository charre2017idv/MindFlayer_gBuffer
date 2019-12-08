//--------------------------------------------------------------------------------------
// File: Tutorial07.fx
//
// Copyright (c) Microsoft Corporation. All rights reserved.
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------
// Constant Buffer Variables
//--------------------------------------------------------------------------------------
Texture2D LightTex : register(t0);
SamplerState samLinear : register(s0);

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
PS_OUTPUT PS(PS_INPUT Input)
{
  // Output 
  PS_OUTPUT Output;
  Output.color = LightTex.Sample(samLinear, Input.texCoord.xy).xyzw;
  return Output;
}
