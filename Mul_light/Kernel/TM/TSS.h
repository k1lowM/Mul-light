/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/TM/TSS.h
	概要		：	TSSヘッダ
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	TSSクラス
*******************************************************************************/
class	TSS
{
//定数
protected:
	enum
	{
		KERNEL_CS		= 0x08,		//カーネルのコードセグメントのセレクタ
		KERNEL_DS		= 0x10,		//カーネルのデータセグメントのセレクタ
	};

//変数
protected:
	u2		Mu2_BackLink,	Mu2_Rsv1;

	u4		Mu4_ESP0;		SELECTOR	M_SS0,	Mu2_Rsv2;
	u4		Mu4_ESP1;		SELECTOR	M_SS1,	Mu2_Rsv3;
	u4		Mu4_ESP2;		SELECTOR	M_SS2,	Mu2_Rsv4;

	u4		Mu4_CR3;		u4		Mu4_EIP;		u4		Mu4_EFlags;

	u4		Mu4_EAX, Mu4_ECX, Mu4_EDX, Mu4_EBX, Mu4_ESP, Mu4_EBP, Mu4_ESI, Mu4_EDI;

	SELECTOR	M_ES,	Mu2_Rsv5;		SELECTOR	M_CS,	Mu2_Rsv6;
	SELECTOR	M_SS,	Mu2_Rsv7;		SELECTOR	M_DS,	Mu2_Rsv8;
	SELECTOR	M_FS,	Mu2_Rsv9;		SELECTOR	M_GS,	Mu2_Rsv10;

	u2		Mu2_LDT,		Mu2_Rsv11;
	u2		Mu2_DebugTrap,	Mu2_IOMapBase;

//関数
public:
	void	Init( void );
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
