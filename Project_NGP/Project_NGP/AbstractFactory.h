#pragma once

// �߻� ���丮 ���� : ��ü ������ ����� �������� �ϳ��� ���ø����� ����
class GameObject;

template <class SubClass>
class AbstractFactory
{
public:
	static GameObject* CreateObj()
	{
		GameObject* pObj = new SubClass;
		pObj->Initialize();

		return pObj;
	}

	static GameObject* CreateObj(int posX, int posY)
	{
		GameObject* pObj = new SubClass;
		pObj->Initialize();
		pObj->SetPosition(posX, posY);

		return pObj;
	}

	static GameObject* CreateObj(int posX, int posY, float angle)
	{
		GameObject* pObj = new SubClass;
		pObj->Initialize();
		pObj->SetPosition(posX, posY);
		pObj->SetAngle(angle);

		return pObj;
	}
};
