/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/TM/TSS.cpp
	概要		：	TSSヘッダ
	詳細		：	TSSのクラスです。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"TSS.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	TSSクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	TSSクラスの初期化
	説明	：
	Include	：	Task.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	TSS::Init( void )
{
	Mu2_BackLink =	Mu2_Rsv1 =
	Mu4_ESP0 =		M_SS0 =		Mu2_Rsv2 =
	Mu4_ESP1 =		M_SS1 =		Mu2_Rsv3 =
	Mu4_ESP2 =		M_SS2 =		Mu2_Rsv4 =
	Mu4_CR3 =	Mu4_EIP =
	Mu4_EAX =	Mu4_ECX =	Mu4_EDX =	Mu4_EBX =
	Mu4_ESP =	Mu4_EBP =	Mu4_ESI =	Mu4_EDI =
	M_ES =		Mu2_Rsv5 =	M_CS =		Mu2_Rsv6 =
	M_SS =		Mu2_Rsv7 =	M_DS =		Mu2_Rsv8 =
	M_FS =		Mu2_Rsv9 =	M_GS =		Mu2_Rsv10 =
	Mu2_LDT =		Mu2_Rsv11 =
	Mu2_DebugTrap = 0;

	Mu2_IOMapBase =0xdfff;
	Mu4_EFlags = 0x202;//EFLAGS_RESERVED( EFLAGS_IF );
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

