#pragma once
#include "PKEngine/Core/Input.h"
namespace PKEngine {
	class PKENGINE_API WindowsInput : public Input
	{
	public:

	protected:
		virtual bool IsKeyPressedImpl(int keycode)override;

		// ͨ�� Input �̳�
		virtual bool IsMouseButtonPressedImpl(int button) override;
		virtual std::pair<float, float> GetMousePositionImpl() override;
		virtual float GetMouseXImpl() override;
		virtual float GetMouseYImpl() override;
	};
}