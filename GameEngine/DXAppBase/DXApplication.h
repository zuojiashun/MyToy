#pragma once

#include"Application.h"
#include"Window.h"
#include"Timer.h"
#include"DXGraphics.h"

namespace dxgame
{

	class DXApplication : public Application, public IWindowEventHandler
	{
	public:
		explicit DXApplication();
		virtual ~DXApplication();

		virtual void init() override;
		virtual void run() override;

		void update(float deltaTime);
		void render();

		/*---------------Window Event Functions-----------------*/
		virtual void OnCreated() override;
		virtual void OnSizeChanged(int width, int height) override;
		virtual void OnActivate(bool active) override;
		virtual void OnDestroy() override;

	private:
		std::shared_ptr<Window>		m_Window;
		std::shared_ptr<ITimer>		m_Timer;
		std::shared_ptr<DXGraphics>	m_DXGraphics;
		bool						m_Running;
		bool						m_Exiting;
	};

}