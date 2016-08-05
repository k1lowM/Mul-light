/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/KLib/Image/Bmp.cpp
	概要		：
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"Bmp.h"
#include	"VFS.h"
#include	<string.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
extern VFS		G_VFS;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	Bmpクラス：静的パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	Bmp画像読み取り
	説明	：
	Include	：	Bmp.h
	引数	：	const char* CPc_ImageFP		ファイルパス
				void* Pv_Dest				格納先バッファ
				const ui Cui_BufSize		バッファサイズ(byte)
				[FileHead* P_FileHead = NULL]
				[WinBmpInfoHead* P_InfoHead = NULL]
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Bmp::Read( const char* CPc_ImageFP, void* Pv_Dest, const ui Cui_BufSize, FileHead* P_FileHead, WinBmpInfoHead* P_InfoHead )
{
	u1		Au1_BmpHeadBuf[256];
	FileHead*	P_FileHeadBuf = (FileHead*)Au1_BmpHeadBuf;		//Bmpファイルヘッダ
	WinBmpInfoHead*	P_InfoHeadBuf = (WinBmpInfoHead*)( Au1_BmpHeadBuf + sizeof (FileHead) );	//Bmp情報ヘッダ

	//ファイルヘッダ、情報ヘッダ読み込み
	if( G_VFS.Read( CPc_ImageFP, 0, Au1_BmpHeadBuf,sizeof Au1_BmpHeadBuf ) < 0 )
		return ERROR_READFAILED;

	//画像読み取り
	if( ( G_VFS.Read( CPc_ImageFP, P_FileHeadBuf->u4_ImageOffset, Pv_Dest, Cui_BufSize ) ) < 0 )
		return ERROR_READFAILED;

	//ファイルヘッダコピー
	if( P_FileHead != NULL )
		memcpy( P_FileHead, P_FileHeadBuf, sizeof (FileHead) );

	//情報ヘッダコピー
	if( P_InfoHead != NULL )
		memcpy( P_InfoHead, P_InfoHeadBuf, sizeof (WinBmpInfoHead) );

	return SUCCESS;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

