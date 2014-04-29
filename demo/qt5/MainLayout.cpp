/**
 * Main Layout
 */

#include <BlendInt/Gui/Widget.hpp>
#include <BlendInt/Core/String.hpp>
#include <BlendInt/Service/StockItems.hpp>
#include <BlendInt/Gui/HLayout.hpp>

#include "MainLayout.hpp"

MainLayout::MainLayout ()
	: m_view3d(0), m_button(0)
{
	set_margin(4, 4, 4, 4);
	set_space(4);

	InitOnce();
}

MainLayout::~MainLayout ()
{

}

void MainLayout::InitOnce ()
{
	using namespace BI;

	m_view3d = Manage(new BI::Viewport3D);
	m_view3d->set_name("Viewport3D");
	m_view3d->Resize(630, 445);
	m_view3d->SetPosition(5, 30);

	m_button = Manage(new BI::Button);
	m_button->set_name("Button");
	m_button->SetExpandX(false);
	
	m_space = Manage(new BI::SpaceArea);
	m_space->set_name("SpaceArea");
	m_space->SetExpandX(true);
	m_space->Resize(200, 10);
	
	HLayout* button_layout = Manage(new HLayout);
	button_layout->Add(m_space);
	button_layout->Add(m_button);

	Add(m_view3d);
	Add(button_layout);
}
