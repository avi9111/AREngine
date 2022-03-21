Shader "GramGames/Ghost Ship" {
	Properties {
		_MainTex ("Base (RGB)", 2D) = "white" {}
		_RimColor ("Rim Color", Vector) = (1,1,1,1)
		_BaseColor ("Base Color", Vector) = (1,1,1,1)
		_OutlineColor ("Outline Color", Vector) = (0,0,0,1)
		_Lerp ("Ghost Lerp", Range(0, 1)) = 0
		_Offset ("Outline Width", Range(0, 1)) = 0.042
		_MultiplyColor ("Multiply Color", Vector) = (1,1,1,1)
		_MultiplyColorAmount ("Multiply Color Amount", Range(0, 1)) = 0
		_SubmergeColor ("Underwater Color", Vector) = (0,0.2235,0.3568,1)
	}
	SubShader {
		Tags { "QUEUE" = "Transparent" "RenderType" = "Transparent" }
		Pass {
			Tags { "QUEUE" = "Transparent" "RenderType" = "Transparent" }
			Blend SrcAlpha OneMinusSrcAlpha, SrcAlpha OneMinusSrcAlpha
			GpuProgramID 26039
			Program "vp" {
				SubProgram "gles hw_tier00 " {
					"!!GLES
					#ifdef VERTEX
					#version 100
					
					uniform 	vec3 _WorldSpaceCameraPos;
					uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
					uniform 	vec4 hlslcc_mtx4x4unity_WorldToObject[4];
					uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
					uniform 	vec4 _MainTex_ST;
					attribute highp vec4 in_POSITION0;
					attribute highp vec2 in_TEXCOORD0;
					attribute highp vec3 in_NORMAL0;
					varying highp vec2 vs_TEXCOORD0;
					varying mediump vec3 vs_TEXCOORD1;
					varying mediump vec4 vs_COLOR0;
					vec4 u_xlat0;
					vec4 u_xlat1;
					mediump vec2 u_xlat16_2;
					bvec2 u_xlatb3;
					mediump float u_xlat16_5;
					mediump float u_xlat16_8;
					float u_xlat9;
					void main()
					{
					    u_xlat0 = in_POSITION0.yyyy * hlslcc_mtx4x4unity_ObjectToWorld[1];
					    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[0] * in_POSITION0.xxxx + u_xlat0;
					    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[2] * in_POSITION0.zzzz + u_xlat0;
					    u_xlat0 = u_xlat0 + hlslcc_mtx4x4unity_ObjectToWorld[3];
					    u_xlat1 = u_xlat0.yyyy * hlslcc_mtx4x4unity_MatrixVP[1];
					    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[0] * u_xlat0.xxxx + u_xlat1;
					    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[2] * u_xlat0.zzzz + u_xlat1;
					    gl_Position = hlslcc_mtx4x4unity_MatrixVP[3] * u_xlat0.wwww + u_xlat1;
					    vs_TEXCOORD0.xy = in_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
					    u_xlat0.xyz = _WorldSpaceCameraPos.yyy * hlslcc_mtx4x4unity_WorldToObject[1].xyz;
					    u_xlat0.xyz = hlslcc_mtx4x4unity_WorldToObject[0].xyz * _WorldSpaceCameraPos.xxx + u_xlat0.xyz;
					    u_xlat0.xyz = hlslcc_mtx4x4unity_WorldToObject[2].xyz * _WorldSpaceCameraPos.zzz + u_xlat0.xyz;
					    u_xlat0.xyz = u_xlat0.xyz + hlslcc_mtx4x4unity_WorldToObject[3].xyz;
					    u_xlat0.xyz = u_xlat0.xyz + (-in_POSITION0.xyz);
					    u_xlat9 = dot(u_xlat0.xyz, u_xlat0.xyz);
					    u_xlat9 = inversesqrt(u_xlat9);
					    u_xlat0.xyz = vec3(u_xlat9) * u_xlat0.xyz;
					    u_xlat0.x = dot(in_NORMAL0.xyz, u_xlat0.xyz);
					    u_xlat0.x = (-u_xlat0.x) + 1.0;
					    u_xlat0.x = clamp(u_xlat0.x, 0.0, 1.0);
					    vs_TEXCOORD1.z = u_xlat0.x;
					    u_xlat0.x = in_POSITION0.y * hlslcc_mtx4x4unity_ObjectToWorld[1].y;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[0].y * in_POSITION0.x + u_xlat0.x;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[2].y * in_POSITION0.z + u_xlat0.x;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[3].y * in_POSITION0.w + u_xlat0.x;
					    u_xlatb3.xy = greaterThanEqual(vec4(-0.00999999978, -2.0, 0.0, 0.0), u_xlat0.xxxx).xy;
					    u_xlat16_2.x = (u_xlatb3.x) ? float(1.0) : float(0.0);
					    u_xlat16_2.y = (u_xlatb3.y) ? float(1.0) : float(0.0);
					    u_xlat16_5 = (-u_xlat0.x) * u_xlat16_2.y;
					    u_xlat16_8 = (-u_xlat0.x) * 0.333333343 + 0.5;
					    u_xlat16_2.x = u_xlat16_8 * u_xlat16_2.x;
					    u_xlat16_2.x = clamp(u_xlat16_2.x, 0.0, 1.0);
					    vs_TEXCOORD1.y = u_xlat16_5 * 0.5;
					    vs_TEXCOORD1.y = clamp(vs_TEXCOORD1.y, 0.0, 1.0);
					    vs_TEXCOORD1.x = u_xlat16_2.x;
					    vs_COLOR0.xyz = u_xlat16_2.xxx;
					    vs_COLOR0.w = 1.0;
					    return;
					}
					
					#endif
					#ifdef FRAGMENT
					#version 100
					
					#ifdef GL_FRAGMENT_PRECISION_HIGH
					    precision highp float;
					#else
					    precision mediump float;
					#endif
					precision highp int;
					uniform 	mediump vec4 _RimColor;
					uniform 	mediump vec4 _BaseColor;
					uniform 	mediump vec4 _SubmergeColor;
					uniform 	mediump vec4 _MultiplyColor;
					uniform 	float _Lerp;
					uniform 	float _MultiplyColorAmount;
					uniform lowp sampler2D _MainTex;
					varying highp vec2 vs_TEXCOORD0;
					varying mediump vec3 vs_TEXCOORD1;
					#define SV_Target0 gl_FragData[0]
					vec4 u_xlat0;
					lowp vec3 u_xlat10_0;
					mediump vec3 u_xlat16_1;
					mediump vec3 u_xlat16_2;
					float u_xlat3;
					float u_xlat12;
					mediump float u_xlat16_13;
					void main()
					{
					    u_xlat10_0.xyz = texture2D(_MainTex, vs_TEXCOORD0.xy).xyz;
					    u_xlat0.xyz = u_xlat10_0.xyz + (-_MultiplyColor.xyz);
					    u_xlat12 = (-_MultiplyColorAmount) + 1.0;
					    u_xlat0.xyz = vec3(u_xlat12) * u_xlat0.xyz + _MultiplyColor.xyz;
					    u_xlat16_1.xyz = (-u_xlat0.xyz) + _SubmergeColor.xyz;
					    u_xlat16_13 = vs_TEXCOORD1.x * _SubmergeColor.w;
					    u_xlat16_1.xyz = vec3(u_xlat16_13) * u_xlat16_1.xyz + u_xlat0.xyz;
					    u_xlat16_2.xyz = _RimColor.xyz + (-_BaseColor.xyz);
					    u_xlat16_2.xyz = vs_TEXCOORD1.zzz * u_xlat16_2.xyz + _BaseColor.xyz;
					    u_xlat0.xyz = (-u_xlat16_1.xyz) + u_xlat16_2.xyz;
					    u_xlat0.xyz = vec3(_Lerp) * u_xlat0.xyz + u_xlat16_1.xyz;
					    u_xlat16_1.x = vs_TEXCOORD1.z + _BaseColor.w;
					    u_xlat3 = u_xlat16_1.x + -1.0;
					    u_xlat0.w = _Lerp * u_xlat3 + 1.0;
					    SV_Target0 = u_xlat0;
					    return;
					}
					
					#endif"
				}
				SubProgram "gles hw_tier01 " {
					"!!GLES
					#ifdef VERTEX
					#version 100
					
					uniform 	vec3 _WorldSpaceCameraPos;
					uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
					uniform 	vec4 hlslcc_mtx4x4unity_WorldToObject[4];
					uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
					uniform 	vec4 _MainTex_ST;
					attribute highp vec4 in_POSITION0;
					attribute highp vec2 in_TEXCOORD0;
					attribute highp vec3 in_NORMAL0;
					varying highp vec2 vs_TEXCOORD0;
					varying mediump vec3 vs_TEXCOORD1;
					varying mediump vec4 vs_COLOR0;
					vec4 u_xlat0;
					vec4 u_xlat1;
					mediump vec2 u_xlat16_2;
					bvec2 u_xlatb3;
					mediump float u_xlat16_5;
					mediump float u_xlat16_8;
					float u_xlat9;
					void main()
					{
					    u_xlat0 = in_POSITION0.yyyy * hlslcc_mtx4x4unity_ObjectToWorld[1];
					    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[0] * in_POSITION0.xxxx + u_xlat0;
					    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[2] * in_POSITION0.zzzz + u_xlat0;
					    u_xlat0 = u_xlat0 + hlslcc_mtx4x4unity_ObjectToWorld[3];
					    u_xlat1 = u_xlat0.yyyy * hlslcc_mtx4x4unity_MatrixVP[1];
					    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[0] * u_xlat0.xxxx + u_xlat1;
					    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[2] * u_xlat0.zzzz + u_xlat1;
					    gl_Position = hlslcc_mtx4x4unity_MatrixVP[3] * u_xlat0.wwww + u_xlat1;
					    vs_TEXCOORD0.xy = in_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
					    u_xlat0.xyz = _WorldSpaceCameraPos.yyy * hlslcc_mtx4x4unity_WorldToObject[1].xyz;
					    u_xlat0.xyz = hlslcc_mtx4x4unity_WorldToObject[0].xyz * _WorldSpaceCameraPos.xxx + u_xlat0.xyz;
					    u_xlat0.xyz = hlslcc_mtx4x4unity_WorldToObject[2].xyz * _WorldSpaceCameraPos.zzz + u_xlat0.xyz;
					    u_xlat0.xyz = u_xlat0.xyz + hlslcc_mtx4x4unity_WorldToObject[3].xyz;
					    u_xlat0.xyz = u_xlat0.xyz + (-in_POSITION0.xyz);
					    u_xlat9 = dot(u_xlat0.xyz, u_xlat0.xyz);
					    u_xlat9 = inversesqrt(u_xlat9);
					    u_xlat0.xyz = vec3(u_xlat9) * u_xlat0.xyz;
					    u_xlat0.x = dot(in_NORMAL0.xyz, u_xlat0.xyz);
					    u_xlat0.x = (-u_xlat0.x) + 1.0;
					    u_xlat0.x = clamp(u_xlat0.x, 0.0, 1.0);
					    vs_TEXCOORD1.z = u_xlat0.x;
					    u_xlat0.x = in_POSITION0.y * hlslcc_mtx4x4unity_ObjectToWorld[1].y;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[0].y * in_POSITION0.x + u_xlat0.x;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[2].y * in_POSITION0.z + u_xlat0.x;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[3].y * in_POSITION0.w + u_xlat0.x;
					    u_xlatb3.xy = greaterThanEqual(vec4(-0.00999999978, -2.0, 0.0, 0.0), u_xlat0.xxxx).xy;
					    u_xlat16_2.x = (u_xlatb3.x) ? float(1.0) : float(0.0);
					    u_xlat16_2.y = (u_xlatb3.y) ? float(1.0) : float(0.0);
					    u_xlat16_5 = (-u_xlat0.x) * u_xlat16_2.y;
					    u_xlat16_8 = (-u_xlat0.x) * 0.333333343 + 0.5;
					    u_xlat16_2.x = u_xlat16_8 * u_xlat16_2.x;
					    u_xlat16_2.x = clamp(u_xlat16_2.x, 0.0, 1.0);
					    vs_TEXCOORD1.y = u_xlat16_5 * 0.5;
					    vs_TEXCOORD1.y = clamp(vs_TEXCOORD1.y, 0.0, 1.0);
					    vs_TEXCOORD1.x = u_xlat16_2.x;
					    vs_COLOR0.xyz = u_xlat16_2.xxx;
					    vs_COLOR0.w = 1.0;
					    return;
					}
					
					#endif
					#ifdef FRAGMENT
					#version 100
					
					#ifdef GL_FRAGMENT_PRECISION_HIGH
					    precision highp float;
					#else
					    precision mediump float;
					#endif
					precision highp int;
					uniform 	mediump vec4 _RimColor;
					uniform 	mediump vec4 _BaseColor;
					uniform 	mediump vec4 _SubmergeColor;
					uniform 	mediump vec4 _MultiplyColor;
					uniform 	float _Lerp;
					uniform 	float _MultiplyColorAmount;
					uniform lowp sampler2D _MainTex;
					varying highp vec2 vs_TEXCOORD0;
					varying mediump vec3 vs_TEXCOORD1;
					#define SV_Target0 gl_FragData[0]
					vec4 u_xlat0;
					lowp vec3 u_xlat10_0;
					mediump vec3 u_xlat16_1;
					mediump vec3 u_xlat16_2;
					float u_xlat3;
					float u_xlat12;
					mediump float u_xlat16_13;
					void main()
					{
					    u_xlat10_0.xyz = texture2D(_MainTex, vs_TEXCOORD0.xy).xyz;
					    u_xlat0.xyz = u_xlat10_0.xyz + (-_MultiplyColor.xyz);
					    u_xlat12 = (-_MultiplyColorAmount) + 1.0;
					    u_xlat0.xyz = vec3(u_xlat12) * u_xlat0.xyz + _MultiplyColor.xyz;
					    u_xlat16_1.xyz = (-u_xlat0.xyz) + _SubmergeColor.xyz;
					    u_xlat16_13 = vs_TEXCOORD1.x * _SubmergeColor.w;
					    u_xlat16_1.xyz = vec3(u_xlat16_13) * u_xlat16_1.xyz + u_xlat0.xyz;
					    u_xlat16_2.xyz = _RimColor.xyz + (-_BaseColor.xyz);
					    u_xlat16_2.xyz = vs_TEXCOORD1.zzz * u_xlat16_2.xyz + _BaseColor.xyz;
					    u_xlat0.xyz = (-u_xlat16_1.xyz) + u_xlat16_2.xyz;
					    u_xlat0.xyz = vec3(_Lerp) * u_xlat0.xyz + u_xlat16_1.xyz;
					    u_xlat16_1.x = vs_TEXCOORD1.z + _BaseColor.w;
					    u_xlat3 = u_xlat16_1.x + -1.0;
					    u_xlat0.w = _Lerp * u_xlat3 + 1.0;
					    SV_Target0 = u_xlat0;
					    return;
					}
					
					#endif"
				}
				SubProgram "gles hw_tier02 " {
					"!!GLES
					#ifdef VERTEX
					#version 100
					
					uniform 	vec3 _WorldSpaceCameraPos;
					uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
					uniform 	vec4 hlslcc_mtx4x4unity_WorldToObject[4];
					uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
					uniform 	vec4 _MainTex_ST;
					attribute highp vec4 in_POSITION0;
					attribute highp vec2 in_TEXCOORD0;
					attribute highp vec3 in_NORMAL0;
					varying highp vec2 vs_TEXCOORD0;
					varying mediump vec3 vs_TEXCOORD1;
					varying mediump vec4 vs_COLOR0;
					vec4 u_xlat0;
					vec4 u_xlat1;
					mediump vec2 u_xlat16_2;
					bvec2 u_xlatb3;
					mediump float u_xlat16_5;
					mediump float u_xlat16_8;
					float u_xlat9;
					void main()
					{
					    u_xlat0 = in_POSITION0.yyyy * hlslcc_mtx4x4unity_ObjectToWorld[1];
					    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[0] * in_POSITION0.xxxx + u_xlat0;
					    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[2] * in_POSITION0.zzzz + u_xlat0;
					    u_xlat0 = u_xlat0 + hlslcc_mtx4x4unity_ObjectToWorld[3];
					    u_xlat1 = u_xlat0.yyyy * hlslcc_mtx4x4unity_MatrixVP[1];
					    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[0] * u_xlat0.xxxx + u_xlat1;
					    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[2] * u_xlat0.zzzz + u_xlat1;
					    gl_Position = hlslcc_mtx4x4unity_MatrixVP[3] * u_xlat0.wwww + u_xlat1;
					    vs_TEXCOORD0.xy = in_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
					    u_xlat0.xyz = _WorldSpaceCameraPos.yyy * hlslcc_mtx4x4unity_WorldToObject[1].xyz;
					    u_xlat0.xyz = hlslcc_mtx4x4unity_WorldToObject[0].xyz * _WorldSpaceCameraPos.xxx + u_xlat0.xyz;
					    u_xlat0.xyz = hlslcc_mtx4x4unity_WorldToObject[2].xyz * _WorldSpaceCameraPos.zzz + u_xlat0.xyz;
					    u_xlat0.xyz = u_xlat0.xyz + hlslcc_mtx4x4unity_WorldToObject[3].xyz;
					    u_xlat0.xyz = u_xlat0.xyz + (-in_POSITION0.xyz);
					    u_xlat9 = dot(u_xlat0.xyz, u_xlat0.xyz);
					    u_xlat9 = inversesqrt(u_xlat9);
					    u_xlat0.xyz = vec3(u_xlat9) * u_xlat0.xyz;
					    u_xlat0.x = dot(in_NORMAL0.xyz, u_xlat0.xyz);
					    u_xlat0.x = (-u_xlat0.x) + 1.0;
					    u_xlat0.x = clamp(u_xlat0.x, 0.0, 1.0);
					    vs_TEXCOORD1.z = u_xlat0.x;
					    u_xlat0.x = in_POSITION0.y * hlslcc_mtx4x4unity_ObjectToWorld[1].y;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[0].y * in_POSITION0.x + u_xlat0.x;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[2].y * in_POSITION0.z + u_xlat0.x;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[3].y * in_POSITION0.w + u_xlat0.x;
					    u_xlatb3.xy = greaterThanEqual(vec4(-0.00999999978, -2.0, 0.0, 0.0), u_xlat0.xxxx).xy;
					    u_xlat16_2.x = (u_xlatb3.x) ? float(1.0) : float(0.0);
					    u_xlat16_2.y = (u_xlatb3.y) ? float(1.0) : float(0.0);
					    u_xlat16_5 = (-u_xlat0.x) * u_xlat16_2.y;
					    u_xlat16_8 = (-u_xlat0.x) * 0.333333343 + 0.5;
					    u_xlat16_2.x = u_xlat16_8 * u_xlat16_2.x;
					    u_xlat16_2.x = clamp(u_xlat16_2.x, 0.0, 1.0);
					    vs_TEXCOORD1.y = u_xlat16_5 * 0.5;
					    vs_TEXCOORD1.y = clamp(vs_TEXCOORD1.y, 0.0, 1.0);
					    vs_TEXCOORD1.x = u_xlat16_2.x;
					    vs_COLOR0.xyz = u_xlat16_2.xxx;
					    vs_COLOR0.w = 1.0;
					    return;
					}
					
					#endif
					#ifdef FRAGMENT
					#version 100
					
					#ifdef GL_FRAGMENT_PRECISION_HIGH
					    precision highp float;
					#else
					    precision mediump float;
					#endif
					precision highp int;
					uniform 	mediump vec4 _RimColor;
					uniform 	mediump vec4 _BaseColor;
					uniform 	mediump vec4 _SubmergeColor;
					uniform 	mediump vec4 _MultiplyColor;
					uniform 	float _Lerp;
					uniform 	float _MultiplyColorAmount;
					uniform lowp sampler2D _MainTex;
					varying highp vec2 vs_TEXCOORD0;
					varying mediump vec3 vs_TEXCOORD1;
					#define SV_Target0 gl_FragData[0]
					vec4 u_xlat0;
					lowp vec3 u_xlat10_0;
					mediump vec3 u_xlat16_1;
					mediump vec3 u_xlat16_2;
					float u_xlat3;
					float u_xlat12;
					mediump float u_xlat16_13;
					void main()
					{
					    u_xlat10_0.xyz = texture2D(_MainTex, vs_TEXCOORD0.xy).xyz;
					    u_xlat0.xyz = u_xlat10_0.xyz + (-_MultiplyColor.xyz);
					    u_xlat12 = (-_MultiplyColorAmount) + 1.0;
					    u_xlat0.xyz = vec3(u_xlat12) * u_xlat0.xyz + _MultiplyColor.xyz;
					    u_xlat16_1.xyz = (-u_xlat0.xyz) + _SubmergeColor.xyz;
					    u_xlat16_13 = vs_TEXCOORD1.x * _SubmergeColor.w;
					    u_xlat16_1.xyz = vec3(u_xlat16_13) * u_xlat16_1.xyz + u_xlat0.xyz;
					    u_xlat16_2.xyz = _RimColor.xyz + (-_BaseColor.xyz);
					    u_xlat16_2.xyz = vs_TEXCOORD1.zzz * u_xlat16_2.xyz + _BaseColor.xyz;
					    u_xlat0.xyz = (-u_xlat16_1.xyz) + u_xlat16_2.xyz;
					    u_xlat0.xyz = vec3(_Lerp) * u_xlat0.xyz + u_xlat16_1.xyz;
					    u_xlat16_1.x = vs_TEXCOORD1.z + _BaseColor.w;
					    u_xlat3 = u_xlat16_1.x + -1.0;
					    u_xlat0.w = _Lerp * u_xlat3 + 1.0;
					    SV_Target0 = u_xlat0;
					    return;
					}
					
					#endif"
				}
				SubProgram "gles3 hw_tier00 " {
					"!!GLES3
					#ifdef VERTEX
					#version 300 es
					
					#define HLSLCC_ENABLE_UNIFORM_BUFFERS 1
					#if HLSLCC_ENABLE_UNIFORM_BUFFERS
					#define UNITY_UNIFORM
					#else
					#define UNITY_UNIFORM uniform
					#endif
					#define UNITY_SUPPORTS_UNIFORM_LOCATION 1
					#if UNITY_SUPPORTS_UNIFORM_LOCATION
					#define UNITY_LOCATION(x) layout(location = x)
					#define UNITY_BINDING(x) layout(binding = x, std140)
					#else
					#define UNITY_LOCATION(x)
					#define UNITY_BINDING(x) layout(std140)
					#endif
					uniform 	vec3 _WorldSpaceCameraPos;
					uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
					uniform 	vec4 hlslcc_mtx4x4unity_WorldToObject[4];
					uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
					uniform 	vec4 _MainTex_ST;
					in highp vec4 in_POSITION0;
					in highp vec2 in_TEXCOORD0;
					in highp vec3 in_NORMAL0;
					out highp vec2 vs_TEXCOORD0;
					out mediump vec3 vs_TEXCOORD1;
					out mediump vec4 vs_COLOR0;
					vec4 u_xlat0;
					vec4 u_xlat1;
					mediump vec2 u_xlat16_2;
					bvec2 u_xlatb3;
					mediump float u_xlat16_5;
					mediump float u_xlat16_8;
					float u_xlat9;
					void main()
					{
					    u_xlat0 = in_POSITION0.yyyy * hlslcc_mtx4x4unity_ObjectToWorld[1];
					    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[0] * in_POSITION0.xxxx + u_xlat0;
					    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[2] * in_POSITION0.zzzz + u_xlat0;
					    u_xlat0 = u_xlat0 + hlslcc_mtx4x4unity_ObjectToWorld[3];
					    u_xlat1 = u_xlat0.yyyy * hlslcc_mtx4x4unity_MatrixVP[1];
					    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[0] * u_xlat0.xxxx + u_xlat1;
					    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[2] * u_xlat0.zzzz + u_xlat1;
					    gl_Position = hlslcc_mtx4x4unity_MatrixVP[3] * u_xlat0.wwww + u_xlat1;
					    vs_TEXCOORD0.xy = in_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
					    u_xlat0.xyz = _WorldSpaceCameraPos.yyy * hlslcc_mtx4x4unity_WorldToObject[1].xyz;
					    u_xlat0.xyz = hlslcc_mtx4x4unity_WorldToObject[0].xyz * _WorldSpaceCameraPos.xxx + u_xlat0.xyz;
					    u_xlat0.xyz = hlslcc_mtx4x4unity_WorldToObject[2].xyz * _WorldSpaceCameraPos.zzz + u_xlat0.xyz;
					    u_xlat0.xyz = u_xlat0.xyz + hlslcc_mtx4x4unity_WorldToObject[3].xyz;
					    u_xlat0.xyz = u_xlat0.xyz + (-in_POSITION0.xyz);
					    u_xlat9 = dot(u_xlat0.xyz, u_xlat0.xyz);
					    u_xlat9 = inversesqrt(u_xlat9);
					    u_xlat0.xyz = vec3(u_xlat9) * u_xlat0.xyz;
					    u_xlat0.x = dot(in_NORMAL0.xyz, u_xlat0.xyz);
					    u_xlat0.x = (-u_xlat0.x) + 1.0;
					#ifdef UNITY_ADRENO_ES3
					    u_xlat0.x = min(max(u_xlat0.x, 0.0), 1.0);
					#else
					    u_xlat0.x = clamp(u_xlat0.x, 0.0, 1.0);
					#endif
					    vs_TEXCOORD1.z = u_xlat0.x;
					    u_xlat0.x = in_POSITION0.y * hlslcc_mtx4x4unity_ObjectToWorld[1].y;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[0].y * in_POSITION0.x + u_xlat0.x;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[2].y * in_POSITION0.z + u_xlat0.x;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[3].y * in_POSITION0.w + u_xlat0.x;
					    u_xlatb3.xy = greaterThanEqual(vec4(-0.00999999978, -2.0, 0.0, 0.0), u_xlat0.xxxx).xy;
					    u_xlat16_2.x = (u_xlatb3.x) ? float(1.0) : float(0.0);
					    u_xlat16_2.y = (u_xlatb3.y) ? float(1.0) : float(0.0);
					    u_xlat16_5 = (-u_xlat0.x) * u_xlat16_2.y;
					    u_xlat16_8 = (-u_xlat0.x) * 0.333333343 + 0.5;
					    u_xlat16_2.x = u_xlat16_8 * u_xlat16_2.x;
					#ifdef UNITY_ADRENO_ES3
					    u_xlat16_2.x = min(max(u_xlat16_2.x, 0.0), 1.0);
					#else
					    u_xlat16_2.x = clamp(u_xlat16_2.x, 0.0, 1.0);
					#endif
					    vs_TEXCOORD1.y = u_xlat16_5 * 0.5;
					#ifdef UNITY_ADRENO_ES3
					    vs_TEXCOORD1.y = min(max(vs_TEXCOORD1.y, 0.0), 1.0);
					#else
					    vs_TEXCOORD1.y = clamp(vs_TEXCOORD1.y, 0.0, 1.0);
					#endif
					    vs_TEXCOORD1.x = u_xlat16_2.x;
					    vs_COLOR0.xyz = u_xlat16_2.xxx;
					    vs_COLOR0.w = 1.0;
					    return;
					}
					
					#endif
					#ifdef FRAGMENT
					#version 300 es
					
					precision highp float;
					precision highp int;
					#define HLSLCC_ENABLE_UNIFORM_BUFFERS 1
					#if HLSLCC_ENABLE_UNIFORM_BUFFERS
					#define UNITY_UNIFORM
					#else
					#define UNITY_UNIFORM uniform
					#endif
					#define UNITY_SUPPORTS_UNIFORM_LOCATION 1
					#if UNITY_SUPPORTS_UNIFORM_LOCATION
					#define UNITY_LOCATION(x) layout(location = x)
					#define UNITY_BINDING(x) layout(binding = x, std140)
					#else
					#define UNITY_LOCATION(x)
					#define UNITY_BINDING(x) layout(std140)
					#endif
					uniform 	mediump vec4 _RimColor;
					uniform 	mediump vec4 _BaseColor;
					uniform 	mediump vec4 _SubmergeColor;
					uniform 	mediump vec4 _MultiplyColor;
					uniform 	float _Lerp;
					uniform 	float _MultiplyColorAmount;
					UNITY_LOCATION(0) uniform mediump sampler2D _MainTex;
					in highp vec2 vs_TEXCOORD0;
					in mediump vec3 vs_TEXCOORD1;
					layout(location = 0) out mediump vec4 SV_Target0;
					vec4 u_xlat0;
					mediump vec3 u_xlat16_0;
					mediump vec3 u_xlat16_1;
					mediump vec3 u_xlat16_2;
					float u_xlat3;
					float u_xlat12;
					mediump float u_xlat16_13;
					void main()
					{
					    u_xlat16_0.xyz = texture(_MainTex, vs_TEXCOORD0.xy).xyz;
					    u_xlat0.xyz = u_xlat16_0.xyz + (-_MultiplyColor.xyz);
					    u_xlat12 = (-_MultiplyColorAmount) + 1.0;
					    u_xlat0.xyz = vec3(u_xlat12) * u_xlat0.xyz + _MultiplyColor.xyz;
					    u_xlat16_1.xyz = (-u_xlat0.xyz) + _SubmergeColor.xyz;
					    u_xlat16_13 = vs_TEXCOORD1.x * _SubmergeColor.w;
					    u_xlat16_1.xyz = vec3(u_xlat16_13) * u_xlat16_1.xyz + u_xlat0.xyz;
					    u_xlat16_2.xyz = _RimColor.xyz + (-_BaseColor.xyz);
					    u_xlat16_2.xyz = vs_TEXCOORD1.zzz * u_xlat16_2.xyz + _BaseColor.xyz;
					    u_xlat0.xyz = (-u_xlat16_1.xyz) + u_xlat16_2.xyz;
					    u_xlat0.xyz = vec3(_Lerp) * u_xlat0.xyz + u_xlat16_1.xyz;
					    u_xlat16_1.x = vs_TEXCOORD1.z + _BaseColor.w;
					    u_xlat3 = u_xlat16_1.x + -1.0;
					    u_xlat0.w = _Lerp * u_xlat3 + 1.0;
					    SV_Target0 = u_xlat0;
					    return;
					}
					
					#endif"
				}
				SubProgram "gles3 hw_tier01 " {
					"!!GLES3
					#ifdef VERTEX
					#version 300 es
					
					#define HLSLCC_ENABLE_UNIFORM_BUFFERS 1
					#if HLSLCC_ENABLE_UNIFORM_BUFFERS
					#define UNITY_UNIFORM
					#else
					#define UNITY_UNIFORM uniform
					#endif
					#define UNITY_SUPPORTS_UNIFORM_LOCATION 1
					#if UNITY_SUPPORTS_UNIFORM_LOCATION
					#define UNITY_LOCATION(x) layout(location = x)
					#define UNITY_BINDING(x) layout(binding = x, std140)
					#else
					#define UNITY_LOCATION(x)
					#define UNITY_BINDING(x) layout(std140)
					#endif
					uniform 	vec3 _WorldSpaceCameraPos;
					uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
					uniform 	vec4 hlslcc_mtx4x4unity_WorldToObject[4];
					uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
					uniform 	vec4 _MainTex_ST;
					in highp vec4 in_POSITION0;
					in highp vec2 in_TEXCOORD0;
					in highp vec3 in_NORMAL0;
					out highp vec2 vs_TEXCOORD0;
					out mediump vec3 vs_TEXCOORD1;
					out mediump vec4 vs_COLOR0;
					vec4 u_xlat0;
					vec4 u_xlat1;
					mediump vec2 u_xlat16_2;
					bvec2 u_xlatb3;
					mediump float u_xlat16_5;
					mediump float u_xlat16_8;
					float u_xlat9;
					void main()
					{
					    u_xlat0 = in_POSITION0.yyyy * hlslcc_mtx4x4unity_ObjectToWorld[1];
					    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[0] * in_POSITION0.xxxx + u_xlat0;
					    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[2] * in_POSITION0.zzzz + u_xlat0;
					    u_xlat0 = u_xlat0 + hlslcc_mtx4x4unity_ObjectToWorld[3];
					    u_xlat1 = u_xlat0.yyyy * hlslcc_mtx4x4unity_MatrixVP[1];
					    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[0] * u_xlat0.xxxx + u_xlat1;
					    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[2] * u_xlat0.zzzz + u_xlat1;
					    gl_Position = hlslcc_mtx4x4unity_MatrixVP[3] * u_xlat0.wwww + u_xlat1;
					    vs_TEXCOORD0.xy = in_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
					    u_xlat0.xyz = _WorldSpaceCameraPos.yyy * hlslcc_mtx4x4unity_WorldToObject[1].xyz;
					    u_xlat0.xyz = hlslcc_mtx4x4unity_WorldToObject[0].xyz * _WorldSpaceCameraPos.xxx + u_xlat0.xyz;
					    u_xlat0.xyz = hlslcc_mtx4x4unity_WorldToObject[2].xyz * _WorldSpaceCameraPos.zzz + u_xlat0.xyz;
					    u_xlat0.xyz = u_xlat0.xyz + hlslcc_mtx4x4unity_WorldToObject[3].xyz;
					    u_xlat0.xyz = u_xlat0.xyz + (-in_POSITION0.xyz);
					    u_xlat9 = dot(u_xlat0.xyz, u_xlat0.xyz);
					    u_xlat9 = inversesqrt(u_xlat9);
					    u_xlat0.xyz = vec3(u_xlat9) * u_xlat0.xyz;
					    u_xlat0.x = dot(in_NORMAL0.xyz, u_xlat0.xyz);
					    u_xlat0.x = (-u_xlat0.x) + 1.0;
					#ifdef UNITY_ADRENO_ES3
					    u_xlat0.x = min(max(u_xlat0.x, 0.0), 1.0);
					#else
					    u_xlat0.x = clamp(u_xlat0.x, 0.0, 1.0);
					#endif
					    vs_TEXCOORD1.z = u_xlat0.x;
					    u_xlat0.x = in_POSITION0.y * hlslcc_mtx4x4unity_ObjectToWorld[1].y;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[0].y * in_POSITION0.x + u_xlat0.x;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[2].y * in_POSITION0.z + u_xlat0.x;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[3].y * in_POSITION0.w + u_xlat0.x;
					    u_xlatb3.xy = greaterThanEqual(vec4(-0.00999999978, -2.0, 0.0, 0.0), u_xlat0.xxxx).xy;
					    u_xlat16_2.x = (u_xlatb3.x) ? float(1.0) : float(0.0);
					    u_xlat16_2.y = (u_xlatb3.y) ? float(1.0) : float(0.0);
					    u_xlat16_5 = (-u_xlat0.x) * u_xlat16_2.y;
					    u_xlat16_8 = (-u_xlat0.x) * 0.333333343 + 0.5;
					    u_xlat16_2.x = u_xlat16_8 * u_xlat16_2.x;
					#ifdef UNITY_ADRENO_ES3
					    u_xlat16_2.x = min(max(u_xlat16_2.x, 0.0), 1.0);
					#else
					    u_xlat16_2.x = clamp(u_xlat16_2.x, 0.0, 1.0);
					#endif
					    vs_TEXCOORD1.y = u_xlat16_5 * 0.5;
					#ifdef UNITY_ADRENO_ES3
					    vs_TEXCOORD1.y = min(max(vs_TEXCOORD1.y, 0.0), 1.0);
					#else
					    vs_TEXCOORD1.y = clamp(vs_TEXCOORD1.y, 0.0, 1.0);
					#endif
					    vs_TEXCOORD1.x = u_xlat16_2.x;
					    vs_COLOR0.xyz = u_xlat16_2.xxx;
					    vs_COLOR0.w = 1.0;
					    return;
					}
					
					#endif
					#ifdef FRAGMENT
					#version 300 es
					
					precision highp float;
					precision highp int;
					#define HLSLCC_ENABLE_UNIFORM_BUFFERS 1
					#if HLSLCC_ENABLE_UNIFORM_BUFFERS
					#define UNITY_UNIFORM
					#else
					#define UNITY_UNIFORM uniform
					#endif
					#define UNITY_SUPPORTS_UNIFORM_LOCATION 1
					#if UNITY_SUPPORTS_UNIFORM_LOCATION
					#define UNITY_LOCATION(x) layout(location = x)
					#define UNITY_BINDING(x) layout(binding = x, std140)
					#else
					#define UNITY_LOCATION(x)
					#define UNITY_BINDING(x) layout(std140)
					#endif
					uniform 	mediump vec4 _RimColor;
					uniform 	mediump vec4 _BaseColor;
					uniform 	mediump vec4 _SubmergeColor;
					uniform 	mediump vec4 _MultiplyColor;
					uniform 	float _Lerp;
					uniform 	float _MultiplyColorAmount;
					UNITY_LOCATION(0) uniform mediump sampler2D _MainTex;
					in highp vec2 vs_TEXCOORD0;
					in mediump vec3 vs_TEXCOORD1;
					layout(location = 0) out mediump vec4 SV_Target0;
					vec4 u_xlat0;
					mediump vec3 u_xlat16_0;
					mediump vec3 u_xlat16_1;
					mediump vec3 u_xlat16_2;
					float u_xlat3;
					float u_xlat12;
					mediump float u_xlat16_13;
					void main()
					{
					    u_xlat16_0.xyz = texture(_MainTex, vs_TEXCOORD0.xy).xyz;
					    u_xlat0.xyz = u_xlat16_0.xyz + (-_MultiplyColor.xyz);
					    u_xlat12 = (-_MultiplyColorAmount) + 1.0;
					    u_xlat0.xyz = vec3(u_xlat12) * u_xlat0.xyz + _MultiplyColor.xyz;
					    u_xlat16_1.xyz = (-u_xlat0.xyz) + _SubmergeColor.xyz;
					    u_xlat16_13 = vs_TEXCOORD1.x * _SubmergeColor.w;
					    u_xlat16_1.xyz = vec3(u_xlat16_13) * u_xlat16_1.xyz + u_xlat0.xyz;
					    u_xlat16_2.xyz = _RimColor.xyz + (-_BaseColor.xyz);
					    u_xlat16_2.xyz = vs_TEXCOORD1.zzz * u_xlat16_2.xyz + _BaseColor.xyz;
					    u_xlat0.xyz = (-u_xlat16_1.xyz) + u_xlat16_2.xyz;
					    u_xlat0.xyz = vec3(_Lerp) * u_xlat0.xyz + u_xlat16_1.xyz;
					    u_xlat16_1.x = vs_TEXCOORD1.z + _BaseColor.w;
					    u_xlat3 = u_xlat16_1.x + -1.0;
					    u_xlat0.w = _Lerp * u_xlat3 + 1.0;
					    SV_Target0 = u_xlat0;
					    return;
					}
					
					#endif"
				}
				SubProgram "gles3 hw_tier02 " {
					"!!GLES3
					#ifdef VERTEX
					#version 300 es
					
					#define HLSLCC_ENABLE_UNIFORM_BUFFERS 1
					#if HLSLCC_ENABLE_UNIFORM_BUFFERS
					#define UNITY_UNIFORM
					#else
					#define UNITY_UNIFORM uniform
					#endif
					#define UNITY_SUPPORTS_UNIFORM_LOCATION 1
					#if UNITY_SUPPORTS_UNIFORM_LOCATION
					#define UNITY_LOCATION(x) layout(location = x)
					#define UNITY_BINDING(x) layout(binding = x, std140)
					#else
					#define UNITY_LOCATION(x)
					#define UNITY_BINDING(x) layout(std140)
					#endif
					uniform 	vec3 _WorldSpaceCameraPos;
					uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
					uniform 	vec4 hlslcc_mtx4x4unity_WorldToObject[4];
					uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
					uniform 	vec4 _MainTex_ST;
					in highp vec4 in_POSITION0;
					in highp vec2 in_TEXCOORD0;
					in highp vec3 in_NORMAL0;
					out highp vec2 vs_TEXCOORD0;
					out mediump vec3 vs_TEXCOORD1;
					out mediump vec4 vs_COLOR0;
					vec4 u_xlat0;
					vec4 u_xlat1;
					mediump vec2 u_xlat16_2;
					bvec2 u_xlatb3;
					mediump float u_xlat16_5;
					mediump float u_xlat16_8;
					float u_xlat9;
					void main()
					{
					    u_xlat0 = in_POSITION0.yyyy * hlslcc_mtx4x4unity_ObjectToWorld[1];
					    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[0] * in_POSITION0.xxxx + u_xlat0;
					    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[2] * in_POSITION0.zzzz + u_xlat0;
					    u_xlat0 = u_xlat0 + hlslcc_mtx4x4unity_ObjectToWorld[3];
					    u_xlat1 = u_xlat0.yyyy * hlslcc_mtx4x4unity_MatrixVP[1];
					    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[0] * u_xlat0.xxxx + u_xlat1;
					    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[2] * u_xlat0.zzzz + u_xlat1;
					    gl_Position = hlslcc_mtx4x4unity_MatrixVP[3] * u_xlat0.wwww + u_xlat1;
					    vs_TEXCOORD0.xy = in_TEXCOORD0.xy * _MainTex_ST.xy + _MainTex_ST.zw;
					    u_xlat0.xyz = _WorldSpaceCameraPos.yyy * hlslcc_mtx4x4unity_WorldToObject[1].xyz;
					    u_xlat0.xyz = hlslcc_mtx4x4unity_WorldToObject[0].xyz * _WorldSpaceCameraPos.xxx + u_xlat0.xyz;
					    u_xlat0.xyz = hlslcc_mtx4x4unity_WorldToObject[2].xyz * _WorldSpaceCameraPos.zzz + u_xlat0.xyz;
					    u_xlat0.xyz = u_xlat0.xyz + hlslcc_mtx4x4unity_WorldToObject[3].xyz;
					    u_xlat0.xyz = u_xlat0.xyz + (-in_POSITION0.xyz);
					    u_xlat9 = dot(u_xlat0.xyz, u_xlat0.xyz);
					    u_xlat9 = inversesqrt(u_xlat9);
					    u_xlat0.xyz = vec3(u_xlat9) * u_xlat0.xyz;
					    u_xlat0.x = dot(in_NORMAL0.xyz, u_xlat0.xyz);
					    u_xlat0.x = (-u_xlat0.x) + 1.0;
					#ifdef UNITY_ADRENO_ES3
					    u_xlat0.x = min(max(u_xlat0.x, 0.0), 1.0);
					#else
					    u_xlat0.x = clamp(u_xlat0.x, 0.0, 1.0);
					#endif
					    vs_TEXCOORD1.z = u_xlat0.x;
					    u_xlat0.x = in_POSITION0.y * hlslcc_mtx4x4unity_ObjectToWorld[1].y;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[0].y * in_POSITION0.x + u_xlat0.x;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[2].y * in_POSITION0.z + u_xlat0.x;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[3].y * in_POSITION0.w + u_xlat0.x;
					    u_xlatb3.xy = greaterThanEqual(vec4(-0.00999999978, -2.0, 0.0, 0.0), u_xlat0.xxxx).xy;
					    u_xlat16_2.x = (u_xlatb3.x) ? float(1.0) : float(0.0);
					    u_xlat16_2.y = (u_xlatb3.y) ? float(1.0) : float(0.0);
					    u_xlat16_5 = (-u_xlat0.x) * u_xlat16_2.y;
					    u_xlat16_8 = (-u_xlat0.x) * 0.333333343 + 0.5;
					    u_xlat16_2.x = u_xlat16_8 * u_xlat16_2.x;
					#ifdef UNITY_ADRENO_ES3
					    u_xlat16_2.x = min(max(u_xlat16_2.x, 0.0), 1.0);
					#else
					    u_xlat16_2.x = clamp(u_xlat16_2.x, 0.0, 1.0);
					#endif
					    vs_TEXCOORD1.y = u_xlat16_5 * 0.5;
					#ifdef UNITY_ADRENO_ES3
					    vs_TEXCOORD1.y = min(max(vs_TEXCOORD1.y, 0.0), 1.0);
					#else
					    vs_TEXCOORD1.y = clamp(vs_TEXCOORD1.y, 0.0, 1.0);
					#endif
					    vs_TEXCOORD1.x = u_xlat16_2.x;
					    vs_COLOR0.xyz = u_xlat16_2.xxx;
					    vs_COLOR0.w = 1.0;
					    return;
					}
					
					#endif
					#ifdef FRAGMENT
					#version 300 es
					
					precision highp float;
					precision highp int;
					#define HLSLCC_ENABLE_UNIFORM_BUFFERS 1
					#if HLSLCC_ENABLE_UNIFORM_BUFFERS
					#define UNITY_UNIFORM
					#else
					#define UNITY_UNIFORM uniform
					#endif
					#define UNITY_SUPPORTS_UNIFORM_LOCATION 1
					#if UNITY_SUPPORTS_UNIFORM_LOCATION
					#define UNITY_LOCATION(x) layout(location = x)
					#define UNITY_BINDING(x) layout(binding = x, std140)
					#else
					#define UNITY_LOCATION(x)
					#define UNITY_BINDING(x) layout(std140)
					#endif
					uniform 	mediump vec4 _RimColor;
					uniform 	mediump vec4 _BaseColor;
					uniform 	mediump vec4 _SubmergeColor;
					uniform 	mediump vec4 _MultiplyColor;
					uniform 	float _Lerp;
					uniform 	float _MultiplyColorAmount;
					UNITY_LOCATION(0) uniform mediump sampler2D _MainTex;
					in highp vec2 vs_TEXCOORD0;
					in mediump vec3 vs_TEXCOORD1;
					layout(location = 0) out mediump vec4 SV_Target0;
					vec4 u_xlat0;
					mediump vec3 u_xlat16_0;
					mediump vec3 u_xlat16_1;
					mediump vec3 u_xlat16_2;
					float u_xlat3;
					float u_xlat12;
					mediump float u_xlat16_13;
					void main()
					{
					    u_xlat16_0.xyz = texture(_MainTex, vs_TEXCOORD0.xy).xyz;
					    u_xlat0.xyz = u_xlat16_0.xyz + (-_MultiplyColor.xyz);
					    u_xlat12 = (-_MultiplyColorAmount) + 1.0;
					    u_xlat0.xyz = vec3(u_xlat12) * u_xlat0.xyz + _MultiplyColor.xyz;
					    u_xlat16_1.xyz = (-u_xlat0.xyz) + _SubmergeColor.xyz;
					    u_xlat16_13 = vs_TEXCOORD1.x * _SubmergeColor.w;
					    u_xlat16_1.xyz = vec3(u_xlat16_13) * u_xlat16_1.xyz + u_xlat0.xyz;
					    u_xlat16_2.xyz = _RimColor.xyz + (-_BaseColor.xyz);
					    u_xlat16_2.xyz = vs_TEXCOORD1.zzz * u_xlat16_2.xyz + _BaseColor.xyz;
					    u_xlat0.xyz = (-u_xlat16_1.xyz) + u_xlat16_2.xyz;
					    u_xlat0.xyz = vec3(_Lerp) * u_xlat0.xyz + u_xlat16_1.xyz;
					    u_xlat16_1.x = vs_TEXCOORD1.z + _BaseColor.w;
					    u_xlat3 = u_xlat16_1.x + -1.0;
					    u_xlat0.w = _Lerp * u_xlat3 + 1.0;
					    SV_Target0 = u_xlat0;
					    return;
					}
					
					#endif"
				}
			}
			Program "fp" {
				SubProgram "gles hw_tier00 " {
					"!!GLES"
				}
				SubProgram "gles hw_tier01 " {
					"!!GLES"
				}
				SubProgram "gles hw_tier02 " {
					"!!GLES"
				}
				SubProgram "gles3 hw_tier00 " {
					"!!GLES3"
				}
				SubProgram "gles3 hw_tier01 " {
					"!!GLES3"
				}
				SubProgram "gles3 hw_tier02 " {
					"!!GLES3"
				}
			}
		}
		Pass {
			Tags { "QUEUE" = "Transparent" "RenderType" = "Transparent" }
			Blend SrcAlpha OneMinusSrcAlpha, SrcAlpha OneMinusSrcAlpha
			GpuProgramID 71213
			Program "vp" {
				SubProgram "gles hw_tier00 " {
					"!!GLES
					#ifdef VERTEX
					#version 100
					
					uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
					uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
					uniform 	mediump vec4 _OutlineColor;
					uniform 	float _Offset;
					uniform 	float _Lerp;
					attribute highp vec4 in_POSITION0;
					attribute highp vec2 in_TEXCOORD0;
					attribute highp vec3 in_NORMAL0;
					varying highp vec2 vs_TEXCOORD0;
					varying mediump float vs_TEXCOORD1;
					varying mediump vec4 vs_COLOR0;
					vec4 u_xlat0;
					bool u_xlatb0;
					vec4 u_xlat1;
					void main()
					{
					    u_xlat0.xyz = in_NORMAL0.xyz * vec3(_Offset) + in_POSITION0.xyz;
					    u_xlat1 = u_xlat0.yyyy * hlslcc_mtx4x4unity_ObjectToWorld[1];
					    u_xlat1 = hlslcc_mtx4x4unity_ObjectToWorld[0] * u_xlat0.xxxx + u_xlat1;
					    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[2] * u_xlat0.zzzz + u_xlat1;
					    u_xlat0 = u_xlat0 + hlslcc_mtx4x4unity_ObjectToWorld[3];
					    u_xlat1 = u_xlat0.yyyy * hlslcc_mtx4x4unity_MatrixVP[1];
					    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[0] * u_xlat0.xxxx + u_xlat1;
					    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[2] * u_xlat0.zzzz + u_xlat1;
					    u_xlat0 = hlslcc_mtx4x4unity_MatrixVP[3] * u_xlat0.wwww + u_xlat1;
					    gl_Position = u_xlat0 + vec4(-0.0, -0.0, 0.00700000022, -0.0);
					    vs_TEXCOORD0.xy = in_TEXCOORD0.xy;
					    u_xlat0 = (-_OutlineColor) + vec4(1.0, 1.0, 1.0, 1.0);
					    u_xlat0 = vec4(vec4(_Lerp, _Lerp, _Lerp, _Lerp)) * u_xlat0 + _OutlineColor;
					    vs_COLOR0 = u_xlat0;
					    u_xlat0.x = in_POSITION0.y * hlslcc_mtx4x4unity_ObjectToWorld[1].y;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[0].y * in_POSITION0.x + u_xlat0.x;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[2].y * in_POSITION0.z + u_xlat0.x;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[3].y * in_POSITION0.w + u_xlat0.x;
					    u_xlatb0 = -0.00999999978>=u_xlat0.x;
					    vs_TEXCOORD1 = (u_xlatb0) ? 1.0 : 0.0;
					    return;
					}
					
					#endif
					#ifdef FRAGMENT
					#version 100
					
					#ifdef GL_FRAGMENT_PRECISION_HIGH
					    precision highp float;
					#else
					    precision mediump float;
					#endif
					precision highp int;
					varying mediump float vs_TEXCOORD1;
					varying mediump vec4 vs_COLOR0;
					#define SV_Target0 gl_FragData[0]
					mediump vec4 u_xlat16_0;
					void main()
					{
					    u_xlat16_0.xyz = vs_COLOR0.xyz;
					    u_xlat16_0.w = 0.0;
					    u_xlat16_0 = u_xlat16_0 + (-vs_COLOR0);
					    SV_Target0 = vec4(vs_TEXCOORD1) * u_xlat16_0 + vs_COLOR0;
					    return;
					}
					
					#endif"
				}
				SubProgram "gles hw_tier01 " {
					"!!GLES
					#ifdef VERTEX
					#version 100
					
					uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
					uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
					uniform 	mediump vec4 _OutlineColor;
					uniform 	float _Offset;
					uniform 	float _Lerp;
					attribute highp vec4 in_POSITION0;
					attribute highp vec2 in_TEXCOORD0;
					attribute highp vec3 in_NORMAL0;
					varying highp vec2 vs_TEXCOORD0;
					varying mediump float vs_TEXCOORD1;
					varying mediump vec4 vs_COLOR0;
					vec4 u_xlat0;
					bool u_xlatb0;
					vec4 u_xlat1;
					void main()
					{
					    u_xlat0.xyz = in_NORMAL0.xyz * vec3(_Offset) + in_POSITION0.xyz;
					    u_xlat1 = u_xlat0.yyyy * hlslcc_mtx4x4unity_ObjectToWorld[1];
					    u_xlat1 = hlslcc_mtx4x4unity_ObjectToWorld[0] * u_xlat0.xxxx + u_xlat1;
					    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[2] * u_xlat0.zzzz + u_xlat1;
					    u_xlat0 = u_xlat0 + hlslcc_mtx4x4unity_ObjectToWorld[3];
					    u_xlat1 = u_xlat0.yyyy * hlslcc_mtx4x4unity_MatrixVP[1];
					    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[0] * u_xlat0.xxxx + u_xlat1;
					    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[2] * u_xlat0.zzzz + u_xlat1;
					    u_xlat0 = hlslcc_mtx4x4unity_MatrixVP[3] * u_xlat0.wwww + u_xlat1;
					    gl_Position = u_xlat0 + vec4(-0.0, -0.0, 0.00700000022, -0.0);
					    vs_TEXCOORD0.xy = in_TEXCOORD0.xy;
					    u_xlat0 = (-_OutlineColor) + vec4(1.0, 1.0, 1.0, 1.0);
					    u_xlat0 = vec4(vec4(_Lerp, _Lerp, _Lerp, _Lerp)) * u_xlat0 + _OutlineColor;
					    vs_COLOR0 = u_xlat0;
					    u_xlat0.x = in_POSITION0.y * hlslcc_mtx4x4unity_ObjectToWorld[1].y;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[0].y * in_POSITION0.x + u_xlat0.x;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[2].y * in_POSITION0.z + u_xlat0.x;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[3].y * in_POSITION0.w + u_xlat0.x;
					    u_xlatb0 = -0.00999999978>=u_xlat0.x;
					    vs_TEXCOORD1 = (u_xlatb0) ? 1.0 : 0.0;
					    return;
					}
					
					#endif
					#ifdef FRAGMENT
					#version 100
					
					#ifdef GL_FRAGMENT_PRECISION_HIGH
					    precision highp float;
					#else
					    precision mediump float;
					#endif
					precision highp int;
					varying mediump float vs_TEXCOORD1;
					varying mediump vec4 vs_COLOR0;
					#define SV_Target0 gl_FragData[0]
					mediump vec4 u_xlat16_0;
					void main()
					{
					    u_xlat16_0.xyz = vs_COLOR0.xyz;
					    u_xlat16_0.w = 0.0;
					    u_xlat16_0 = u_xlat16_0 + (-vs_COLOR0);
					    SV_Target0 = vec4(vs_TEXCOORD1) * u_xlat16_0 + vs_COLOR0;
					    return;
					}
					
					#endif"
				}
				SubProgram "gles hw_tier02 " {
					"!!GLES
					#ifdef VERTEX
					#version 100
					
					uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
					uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
					uniform 	mediump vec4 _OutlineColor;
					uniform 	float _Offset;
					uniform 	float _Lerp;
					attribute highp vec4 in_POSITION0;
					attribute highp vec2 in_TEXCOORD0;
					attribute highp vec3 in_NORMAL0;
					varying highp vec2 vs_TEXCOORD0;
					varying mediump float vs_TEXCOORD1;
					varying mediump vec4 vs_COLOR0;
					vec4 u_xlat0;
					bool u_xlatb0;
					vec4 u_xlat1;
					void main()
					{
					    u_xlat0.xyz = in_NORMAL0.xyz * vec3(_Offset) + in_POSITION0.xyz;
					    u_xlat1 = u_xlat0.yyyy * hlslcc_mtx4x4unity_ObjectToWorld[1];
					    u_xlat1 = hlslcc_mtx4x4unity_ObjectToWorld[0] * u_xlat0.xxxx + u_xlat1;
					    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[2] * u_xlat0.zzzz + u_xlat1;
					    u_xlat0 = u_xlat0 + hlslcc_mtx4x4unity_ObjectToWorld[3];
					    u_xlat1 = u_xlat0.yyyy * hlslcc_mtx4x4unity_MatrixVP[1];
					    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[0] * u_xlat0.xxxx + u_xlat1;
					    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[2] * u_xlat0.zzzz + u_xlat1;
					    u_xlat0 = hlslcc_mtx4x4unity_MatrixVP[3] * u_xlat0.wwww + u_xlat1;
					    gl_Position = u_xlat0 + vec4(-0.0, -0.0, 0.00700000022, -0.0);
					    vs_TEXCOORD0.xy = in_TEXCOORD0.xy;
					    u_xlat0 = (-_OutlineColor) + vec4(1.0, 1.0, 1.0, 1.0);
					    u_xlat0 = vec4(vec4(_Lerp, _Lerp, _Lerp, _Lerp)) * u_xlat0 + _OutlineColor;
					    vs_COLOR0 = u_xlat0;
					    u_xlat0.x = in_POSITION0.y * hlslcc_mtx4x4unity_ObjectToWorld[1].y;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[0].y * in_POSITION0.x + u_xlat0.x;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[2].y * in_POSITION0.z + u_xlat0.x;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[3].y * in_POSITION0.w + u_xlat0.x;
					    u_xlatb0 = -0.00999999978>=u_xlat0.x;
					    vs_TEXCOORD1 = (u_xlatb0) ? 1.0 : 0.0;
					    return;
					}
					
					#endif
					#ifdef FRAGMENT
					#version 100
					
					#ifdef GL_FRAGMENT_PRECISION_HIGH
					    precision highp float;
					#else
					    precision mediump float;
					#endif
					precision highp int;
					varying mediump float vs_TEXCOORD1;
					varying mediump vec4 vs_COLOR0;
					#define SV_Target0 gl_FragData[0]
					mediump vec4 u_xlat16_0;
					void main()
					{
					    u_xlat16_0.xyz = vs_COLOR0.xyz;
					    u_xlat16_0.w = 0.0;
					    u_xlat16_0 = u_xlat16_0 + (-vs_COLOR0);
					    SV_Target0 = vec4(vs_TEXCOORD1) * u_xlat16_0 + vs_COLOR0;
					    return;
					}
					
					#endif"
				}
				SubProgram "gles3 hw_tier00 " {
					"!!GLES3
					#ifdef VERTEX
					#version 300 es
					
					#define HLSLCC_ENABLE_UNIFORM_BUFFERS 1
					#if HLSLCC_ENABLE_UNIFORM_BUFFERS
					#define UNITY_UNIFORM
					#else
					#define UNITY_UNIFORM uniform
					#endif
					#define UNITY_SUPPORTS_UNIFORM_LOCATION 1
					#if UNITY_SUPPORTS_UNIFORM_LOCATION
					#define UNITY_LOCATION(x) layout(location = x)
					#define UNITY_BINDING(x) layout(binding = x, std140)
					#else
					#define UNITY_LOCATION(x)
					#define UNITY_BINDING(x) layout(std140)
					#endif
					uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
					uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
					uniform 	mediump vec4 _OutlineColor;
					uniform 	float _Offset;
					uniform 	float _Lerp;
					in highp vec4 in_POSITION0;
					in highp vec2 in_TEXCOORD0;
					in highp vec3 in_NORMAL0;
					out highp vec2 vs_TEXCOORD0;
					out mediump float vs_TEXCOORD1;
					out mediump vec4 vs_COLOR0;
					vec4 u_xlat0;
					bool u_xlatb0;
					vec4 u_xlat1;
					void main()
					{
					    u_xlat0.xyz = in_NORMAL0.xyz * vec3(_Offset) + in_POSITION0.xyz;
					    u_xlat1 = u_xlat0.yyyy * hlslcc_mtx4x4unity_ObjectToWorld[1];
					    u_xlat1 = hlslcc_mtx4x4unity_ObjectToWorld[0] * u_xlat0.xxxx + u_xlat1;
					    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[2] * u_xlat0.zzzz + u_xlat1;
					    u_xlat0 = u_xlat0 + hlslcc_mtx4x4unity_ObjectToWorld[3];
					    u_xlat1 = u_xlat0.yyyy * hlslcc_mtx4x4unity_MatrixVP[1];
					    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[0] * u_xlat0.xxxx + u_xlat1;
					    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[2] * u_xlat0.zzzz + u_xlat1;
					    u_xlat0 = hlslcc_mtx4x4unity_MatrixVP[3] * u_xlat0.wwww + u_xlat1;
					    gl_Position = u_xlat0 + vec4(-0.0, -0.0, 0.00700000022, -0.0);
					    vs_TEXCOORD0.xy = in_TEXCOORD0.xy;
					    u_xlat0 = (-_OutlineColor) + vec4(1.0, 1.0, 1.0, 1.0);
					    u_xlat0 = vec4(vec4(_Lerp, _Lerp, _Lerp, _Lerp)) * u_xlat0 + _OutlineColor;
					    vs_COLOR0 = u_xlat0;
					    u_xlat0.x = in_POSITION0.y * hlslcc_mtx4x4unity_ObjectToWorld[1].y;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[0].y * in_POSITION0.x + u_xlat0.x;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[2].y * in_POSITION0.z + u_xlat0.x;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[3].y * in_POSITION0.w + u_xlat0.x;
					#ifdef UNITY_ADRENO_ES3
					    u_xlatb0 = !!(-0.00999999978>=u_xlat0.x);
					#else
					    u_xlatb0 = -0.00999999978>=u_xlat0.x;
					#endif
					    vs_TEXCOORD1 = (u_xlatb0) ? 1.0 : 0.0;
					    return;
					}
					
					#endif
					#ifdef FRAGMENT
					#version 300 es
					
					precision highp float;
					precision highp int;
					in mediump float vs_TEXCOORD1;
					in mediump vec4 vs_COLOR0;
					layout(location = 0) out mediump vec4 SV_Target0;
					mediump vec4 u_xlat16_0;
					void main()
					{
					    u_xlat16_0.xyz = vs_COLOR0.xyz;
					    u_xlat16_0.w = 0.0;
					    u_xlat16_0 = u_xlat16_0 + (-vs_COLOR0);
					    SV_Target0 = vec4(vs_TEXCOORD1) * u_xlat16_0 + vs_COLOR0;
					    return;
					}
					
					#endif"
				}
				SubProgram "gles3 hw_tier01 " {
					"!!GLES3
					#ifdef VERTEX
					#version 300 es
					
					#define HLSLCC_ENABLE_UNIFORM_BUFFERS 1
					#if HLSLCC_ENABLE_UNIFORM_BUFFERS
					#define UNITY_UNIFORM
					#else
					#define UNITY_UNIFORM uniform
					#endif
					#define UNITY_SUPPORTS_UNIFORM_LOCATION 1
					#if UNITY_SUPPORTS_UNIFORM_LOCATION
					#define UNITY_LOCATION(x) layout(location = x)
					#define UNITY_BINDING(x) layout(binding = x, std140)
					#else
					#define UNITY_LOCATION(x)
					#define UNITY_BINDING(x) layout(std140)
					#endif
					uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
					uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
					uniform 	mediump vec4 _OutlineColor;
					uniform 	float _Offset;
					uniform 	float _Lerp;
					in highp vec4 in_POSITION0;
					in highp vec2 in_TEXCOORD0;
					in highp vec3 in_NORMAL0;
					out highp vec2 vs_TEXCOORD0;
					out mediump float vs_TEXCOORD1;
					out mediump vec4 vs_COLOR0;
					vec4 u_xlat0;
					bool u_xlatb0;
					vec4 u_xlat1;
					void main()
					{
					    u_xlat0.xyz = in_NORMAL0.xyz * vec3(_Offset) + in_POSITION0.xyz;
					    u_xlat1 = u_xlat0.yyyy * hlslcc_mtx4x4unity_ObjectToWorld[1];
					    u_xlat1 = hlslcc_mtx4x4unity_ObjectToWorld[0] * u_xlat0.xxxx + u_xlat1;
					    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[2] * u_xlat0.zzzz + u_xlat1;
					    u_xlat0 = u_xlat0 + hlslcc_mtx4x4unity_ObjectToWorld[3];
					    u_xlat1 = u_xlat0.yyyy * hlslcc_mtx4x4unity_MatrixVP[1];
					    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[0] * u_xlat0.xxxx + u_xlat1;
					    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[2] * u_xlat0.zzzz + u_xlat1;
					    u_xlat0 = hlslcc_mtx4x4unity_MatrixVP[3] * u_xlat0.wwww + u_xlat1;
					    gl_Position = u_xlat0 + vec4(-0.0, -0.0, 0.00700000022, -0.0);
					    vs_TEXCOORD0.xy = in_TEXCOORD0.xy;
					    u_xlat0 = (-_OutlineColor) + vec4(1.0, 1.0, 1.0, 1.0);
					    u_xlat0 = vec4(vec4(_Lerp, _Lerp, _Lerp, _Lerp)) * u_xlat0 + _OutlineColor;
					    vs_COLOR0 = u_xlat0;
					    u_xlat0.x = in_POSITION0.y * hlslcc_mtx4x4unity_ObjectToWorld[1].y;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[0].y * in_POSITION0.x + u_xlat0.x;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[2].y * in_POSITION0.z + u_xlat0.x;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[3].y * in_POSITION0.w + u_xlat0.x;
					#ifdef UNITY_ADRENO_ES3
					    u_xlatb0 = !!(-0.00999999978>=u_xlat0.x);
					#else
					    u_xlatb0 = -0.00999999978>=u_xlat0.x;
					#endif
					    vs_TEXCOORD1 = (u_xlatb0) ? 1.0 : 0.0;
					    return;
					}
					
					#endif
					#ifdef FRAGMENT
					#version 300 es
					
					precision highp float;
					precision highp int;
					in mediump float vs_TEXCOORD1;
					in mediump vec4 vs_COLOR0;
					layout(location = 0) out mediump vec4 SV_Target0;
					mediump vec4 u_xlat16_0;
					void main()
					{
					    u_xlat16_0.xyz = vs_COLOR0.xyz;
					    u_xlat16_0.w = 0.0;
					    u_xlat16_0 = u_xlat16_0 + (-vs_COLOR0);
					    SV_Target0 = vec4(vs_TEXCOORD1) * u_xlat16_0 + vs_COLOR0;
					    return;
					}
					
					#endif"
				}
				SubProgram "gles3 hw_tier02 " {
					"!!GLES3
					#ifdef VERTEX
					#version 300 es
					
					#define HLSLCC_ENABLE_UNIFORM_BUFFERS 1
					#if HLSLCC_ENABLE_UNIFORM_BUFFERS
					#define UNITY_UNIFORM
					#else
					#define UNITY_UNIFORM uniform
					#endif
					#define UNITY_SUPPORTS_UNIFORM_LOCATION 1
					#if UNITY_SUPPORTS_UNIFORM_LOCATION
					#define UNITY_LOCATION(x) layout(location = x)
					#define UNITY_BINDING(x) layout(binding = x, std140)
					#else
					#define UNITY_LOCATION(x)
					#define UNITY_BINDING(x) layout(std140)
					#endif
					uniform 	vec4 hlslcc_mtx4x4unity_ObjectToWorld[4];
					uniform 	vec4 hlslcc_mtx4x4unity_MatrixVP[4];
					uniform 	mediump vec4 _OutlineColor;
					uniform 	float _Offset;
					uniform 	float _Lerp;
					in highp vec4 in_POSITION0;
					in highp vec2 in_TEXCOORD0;
					in highp vec3 in_NORMAL0;
					out highp vec2 vs_TEXCOORD0;
					out mediump float vs_TEXCOORD1;
					out mediump vec4 vs_COLOR0;
					vec4 u_xlat0;
					bool u_xlatb0;
					vec4 u_xlat1;
					void main()
					{
					    u_xlat0.xyz = in_NORMAL0.xyz * vec3(_Offset) + in_POSITION0.xyz;
					    u_xlat1 = u_xlat0.yyyy * hlslcc_mtx4x4unity_ObjectToWorld[1];
					    u_xlat1 = hlslcc_mtx4x4unity_ObjectToWorld[0] * u_xlat0.xxxx + u_xlat1;
					    u_xlat0 = hlslcc_mtx4x4unity_ObjectToWorld[2] * u_xlat0.zzzz + u_xlat1;
					    u_xlat0 = u_xlat0 + hlslcc_mtx4x4unity_ObjectToWorld[3];
					    u_xlat1 = u_xlat0.yyyy * hlslcc_mtx4x4unity_MatrixVP[1];
					    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[0] * u_xlat0.xxxx + u_xlat1;
					    u_xlat1 = hlslcc_mtx4x4unity_MatrixVP[2] * u_xlat0.zzzz + u_xlat1;
					    u_xlat0 = hlslcc_mtx4x4unity_MatrixVP[3] * u_xlat0.wwww + u_xlat1;
					    gl_Position = u_xlat0 + vec4(-0.0, -0.0, 0.00700000022, -0.0);
					    vs_TEXCOORD0.xy = in_TEXCOORD0.xy;
					    u_xlat0 = (-_OutlineColor) + vec4(1.0, 1.0, 1.0, 1.0);
					    u_xlat0 = vec4(vec4(_Lerp, _Lerp, _Lerp, _Lerp)) * u_xlat0 + _OutlineColor;
					    vs_COLOR0 = u_xlat0;
					    u_xlat0.x = in_POSITION0.y * hlslcc_mtx4x4unity_ObjectToWorld[1].y;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[0].y * in_POSITION0.x + u_xlat0.x;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[2].y * in_POSITION0.z + u_xlat0.x;
					    u_xlat0.x = hlslcc_mtx4x4unity_ObjectToWorld[3].y * in_POSITION0.w + u_xlat0.x;
					#ifdef UNITY_ADRENO_ES3
					    u_xlatb0 = !!(-0.00999999978>=u_xlat0.x);
					#else
					    u_xlatb0 = -0.00999999978>=u_xlat0.x;
					#endif
					    vs_TEXCOORD1 = (u_xlatb0) ? 1.0 : 0.0;
					    return;
					}
					
					#endif
					#ifdef FRAGMENT
					#version 300 es
					
					precision highp float;
					precision highp int;
					in mediump float vs_TEXCOORD1;
					in mediump vec4 vs_COLOR0;
					layout(location = 0) out mediump vec4 SV_Target0;
					mediump vec4 u_xlat16_0;
					void main()
					{
					    u_xlat16_0.xyz = vs_COLOR0.xyz;
					    u_xlat16_0.w = 0.0;
					    u_xlat16_0 = u_xlat16_0 + (-vs_COLOR0);
					    SV_Target0 = vec4(vs_TEXCOORD1) * u_xlat16_0 + vs_COLOR0;
					    return;
					}
					
					#endif"
				}
			}
			Program "fp" {
				SubProgram "gles hw_tier00 " {
					"!!GLES"
				}
				SubProgram "gles hw_tier01 " {
					"!!GLES"
				}
				SubProgram "gles hw_tier02 " {
					"!!GLES"
				}
				SubProgram "gles3 hw_tier00 " {
					"!!GLES3"
				}
				SubProgram "gles3 hw_tier01 " {
					"!!GLES3"
				}
				SubProgram "gles3 hw_tier02 " {
					"!!GLES3"
				}
			}
		}
	}
}