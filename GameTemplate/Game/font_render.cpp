#include "stdafx.h"
#include "font_render.h"

FontRender::FontRender()
{

}

FontRender::~FontRender()
{

}

bool FontRender::Start()
{


	return true;
}

void FontRender::Update()
{

}

////////////////////////////////////////////////////////////
// ‰Šú‰»
////////////////////////////////////////////////////////////


void FontRender::Init(
	const wchar_t* text,
	const Vector2 position,
	const float scale,
	const Vector4 color,
	const float rotation,
	const Vector2 pivot
)
{
	//‚»‚ê‚¼‚ê‚ğƒƒ“ƒo•Ï”‚É•Û‚³‚¹‚éB
	swprintf_s(m_text, text);
	m_position = position;
	m_color = color;
	m_rotation = rotation;
	m_scale = scale;
	m_pivot = pivot;
}

////////////////////////////////////////////////////////////
// •`‰æˆ—
////////////////////////////////////////////////////////////


void FontRender::Render(RenderContext& renderContext)
{
	//•`‰æŠJn
	m_font.Begin(renderContext);

	//•`‰æˆ—
	m_font.Draw(m_text,
		m_position,
		m_color,
		m_rotation,
		m_scale,
		m_pivot);

	//•`‰æI—¹
	m_font.End(renderContext);
}