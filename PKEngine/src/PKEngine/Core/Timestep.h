#pragma once

namespace PKEngine {
	class Timestep {
	public:
		Timestep(float ts) :m_Timestep(ts) {}

		operator float()const { return m_Timestep; }
		
		inline float GetSeconds() const { return m_Timestep; }
		inline float GetMillionSeconds() const { return m_Timestep * 1000; }
	private:
		float m_Timestep;
	};
}