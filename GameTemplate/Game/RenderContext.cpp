#include "stdafx.h"
#include "../../MiniEngine/RenderContext.h"

/// <summary>
/// �r���[�|�[�g�ƃV�U�����O��`���Z�b�g�Őݒ�
/// </summary>
/// <param name="viewport">�r���[�|�[�g</param>
void RenderContext::SetViewportAndScissor(D3D12_VIEWPORT& viewport)
{
	//�V�U�����O��`���ݒ肷��B
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
/// �����_�����O�^�[�Q�b�g���X���b�g0�ɐݒ肷��B
/// </summary>
/// <remarks>
/// �{�֐��̓r���[�|�[�g�̐ݒ���s���܂���B
/// ���[�U�[���œK�؂ȃr���[�|�[�g���w�肷��K�v������܂��B
/// </remarks>
/// <param name="renderTarget">�����_�����O�^�[�Q�b�g</param>
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
/// �����_�����O�^�[�Q�b�g�̃N���A�B
/// </summary>
/// <param name="renderTarget"></param>
void RenderContext::ClearRenderTargetView(RenderTarget& renderTarget)
{
	RenderTarget* rtArray[] = { &renderTarget };
	ClearRenderTargetViews(1, rtArray);
}
