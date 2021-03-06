/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/FS/Joliet/Joliet.cpp
	概要		：	Joliet(ファイルシステム)
	詳細		：
	責任者		：
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"Joliet.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	Joliet(FS)クラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	Joliet(FS)クラスの初期化
	説明	：
	Include	：	Joliet.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	Joliet::Init( void )
{
}


/*******************************************************************************
	概要	：	ファイルシステム確認
	説明	：	対象のデバイスのファイルシステムがJolietか調べます。
	Include	：	Joliet.h
	引数	：	u1 u1_DeviceID		確認するデバイス番号
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Joliet::CheckFS( u1 u1_DeviceID )
{
	return SUCCESS;
}


/*******************************************************************************
	概要	：	読み取り
	説明	：	Jolietのファイルシステムからデータを読み取ります。
	Include	：	Joliet.h
	引数	：	const char* CPc_FilePath		'/'から始まる絶対パス
				ui ui_Offset		ファイル内のオフセット
				u1* Pu1_Addr		格納先アドレス
				ui ui_Size			処理するサイズ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		Joliet::Read( const char* CPc_FilePath, ui ui_Offset, u1* Pu1_Addr, ui ui_Size )
{
	return SUCCESS;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

