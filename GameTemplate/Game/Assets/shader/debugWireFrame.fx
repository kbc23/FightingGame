
/*!
 * @brief	���C���v���~�e�B�u�p�̃V�F�[�_�[�B
 */

cbuffer CB : register(b0) {
	float4x4 mView;
	//float4x4 mProj;
};

//���_�V�F�[�_�[�ւ̓��́B
struct SVSIn {
	float4 pos 		: POSITION;		//���f���̒��_���W�B
	float4 col		: COLOR;
};
//�s�N�Z���V�F�[�_�[�ւ̓��́B
struct SPSIn {
	float4 pos 			: SV_POSITION;	//�X�N���[����Ԃł̃s�N�Z���̍��W�B
	float4 col			: COLOR;
};


/*!
 * @brief	���_�V�F�[�_�[
 */
SPSIn VSMain(SVSIn vsIn)
{

	SPSIn psIn;

	psIn.pos = mul(mView, vsIn.pos);
	//psIn.pos = mul(mProj, psIn.pos);

	psIn.col = vsIn.col;
	return psIn;
}

float4 PSMain(SPSIn psIn) : SV_Target0
{
	return float4(1.0f, 0.0f, 0.0f, 1.0f);
//return psIn.col;
}