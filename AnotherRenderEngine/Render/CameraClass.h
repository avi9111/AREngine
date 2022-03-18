#pragma once

#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include<xnamath.h>

class CameraClass
{
private:
	float mPostionX, mPostionY, mPostionZ; //���������ռ��λ��
	float mRotationX, mRotationY, mRotationZ; //����ھֲ��ռ���ת��floatֵ(���������תŷ����)
	XMMATRIX mViewMatrix;

public:
	CameraClass()
	{
		mPostionX = 0;
		mPostionY = 0;
		mPostionZ = -5.0f;
		mRotationX = 0;
		mRotationY = 0;
		mRotationZ = 0;
	}

	CameraClass(const CameraClass&other)
	{

	}
	~CameraClass()
	{

	}

public:

	void SetPostion(float x, float y, float z)
	{
		mPostionX = x;
		mPostionY = y;
		mPostionZ = z;
	}

	void SetRotation(float x, float y, float z)
	{
		mRotationX = x;
		mRotationY = y;
		mRotationZ = z;
	}



	XMVECTOR GetPostion()
	{

		return XMVectorSet(mPostionX, mPostionY, mPostionZ, 0.0f);
	}

	XMVECTOR GetRotation()
	{
		return XMVectorSet(mRotationX, mRotationY, mRotationZ, 0.0f);
	}


	void Render()
	{
		//������,λ������,�۲�����
		XMVECTOR Up, Postion, LookAt;
		float yaw, pitch, roll; //ŷ����


								//����������
		Up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

		//���������λ��
		Postion = XMVectorSet(mPostionX, mPostionY, mPostionZ, 0.0f);

		//�������Ĭ������¿��ĵ�
		LookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

		//���� yaw(Y��), pitch(X��), roll(Z��),Ϊ����ֵ
		pitch = mRotationX*0.0174532925f;
		yaw = mRotationY*0.0174532925f;
		roll = mRotationZ*0.0174532925f;

		//������ת���󣬲���ת�ֲ��ռ������Up������LookAt����
		XMMATRIX RotateMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);
		LookAt = XMVector3TransformCoord(LookAt, RotateMatrix);
		Up = XMVector3TransformCoord(Up, RotateMatrix);

		//������Ӿֲ�ģ�Ϳռ��Ϊ����ռ�
		LookAt = LookAt + Postion;

		//��󴴽�ViewMatrix
		mViewMatrix = XMMatrixLookAtLH(Postion, LookAt, Up);
	}

	XMMATRIX GetViewMatrix()
	{
		return mViewMatrix;
	}


};
















#endif 
