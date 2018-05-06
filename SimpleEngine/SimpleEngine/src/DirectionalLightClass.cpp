#include "DirectionalLightClass.h"


DirectionalLightClass::DirectionalLightClass()
{
}


DirectionalLightClass::DirectionalLightClass(const DirectionalLightClass& other)
{
}


DirectionalLightClass::~DirectionalLightClass()
{
}


void DirectionalLightClass::SetAmbientColor(float red, float green, float blue, float alpha)
{
	m_ambientColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}


void DirectionalLightClass::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	m_diffuseColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}


void DirectionalLightClass::SetDirection(float x, float y, float z)
{
	m_direction = D3DXVECTOR3(x, y, z);
	return;
}


void DirectionalLightClass::SetSpecularColor(float red, float green, float blue, float alpha)
{
	m_specularColor = D3DXVECTOR4(red, green, blue, alpha);
	return;
}


void DirectionalLightClass::SetSpecularPower(float power)
{
	m_specularPower = power;
	return;
}


D3DXVECTOR4 DirectionalLightClass::GetAmbientColor()
{
	return m_ambientColor;
}


D3DXVECTOR4 DirectionalLightClass::GetDiffuseColor()
{
	return m_diffuseColor;
}


D3DXVECTOR3 DirectionalLightClass::GetDirection()
{
	return m_direction;
}


D3DXVECTOR4 DirectionalLightClass::GetSpecularColor()
{
	return m_specularColor;
}


float DirectionalLightClass::GetSpecularPower()
{
	return m_specularPower;
}