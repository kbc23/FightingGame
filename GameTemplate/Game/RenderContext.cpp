#include "stdafx.h"
#include "../../MiniEngine/RenderContext.h"

/// <summary>
/// ビューポートとシザリング矩形をセットで設定
/// </summary>
/// <param name="viewport">ビューポート</param>
void RenderContext::SetViewportAndScissor(D3D12_VIEWPORT& viewport)
{
	//シザリング矩形も設定する。
	D3D12_RECT scissorRect;
	scissorRect.bottom = static_cast<LONG>(viewport.Height);
	scissorRect.top = 0;
	scissorRect.left = 0;
	scissorRect.right = static_cast<LONG>(viewport.Width);
	SetScissorRect(scissorRect);

	m_commandList->RSSetViewports(1, &viewport);
	m_currentViewport = viewport;
}
/// <summary>
/// レンダリングターゲットをスロット0に設定する。
/// </summary>
/// <remarks>
/// 本関数はビューポートの設定を行いません。
/// ユーザー側で適切なビューポートを指定する必要があります。
/// </remarks>
/// <param name="renderTarget">レンダリングターゲット</param>
void RenderContext::SetRenderTarget(RenderTarget& renderTarget)
{
	RenderTarget* rtArray[] = { &renderTarget };
	SetRenderTargets(1, rtArray);
}
void RenderContext::SetRenderTargetAndViewport(RenderTarget& renderTarget)
{
	D3D12_VIEWPORT viewport;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = static_cast<float>(renderTarget.GetWidth());
	viewport.Height = static_cast<float>(renderTarget.GetHeight());
	viewport.MinDepth = D3D12_MIN_DEPTH;
	viewport.MaxDepth = D3D12_MAX_DEPTH;
	SetViewportAndScissor(viewport);

	SetRenderTarget(renderTarget);
}
/// <summary>
/// レンダリングターゲットのクリア。
/// </summary>
/// <param name="renderTarget"></param>
void RenderContext::ClearRenderTargetView(RenderTarget& renderTarget)
{
	RenderTarget* rtArray[] = { &renderTarget };
	ClearRenderTargetViews(1, rtArray);
}
