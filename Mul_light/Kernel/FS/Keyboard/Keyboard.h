/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/FS/Keyboard/Keyboard.h
	概要		：	キーボード(ファイルシステム)
	詳細		：
	責任者		：
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"FS.h"
#include	<Device/DD.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	キーボードクラス
*******************************************************************************/
class	Keyboard : public FS
{
//定数
public:
	//エラー情報
	enum	Error
	{
		ERROR = _ERROR_,
		ERROR_NOTSUPPORT,		//サポート対象外
		ERROR_INVALIDKEYCODE,	//無効キーコード

		SUCCESS = _SUCCESS_,
	};

//変数
private:
	u4		MAu4_KeyState[8];		//全256ビットのキー押下状態
	u4		MAu4_KeyOld[8];			//プレス/リリース検出用

//メソッド
public:
	s4		Init( u1 u1_DeviceID, ui ui_FSMemID );

	s4		CheckFS( u1 u1_DeviceID );		//ファイルシステム確認

	s4		PassDD( DD* P_DeviceData );

	s4		Rename( const char* CPc_FilePath, const char* CPc_NewFileName );				//ファイル名変更
	s4		Read( const char* CPc_FilePath, ui ui_Offset, void* Pv_Addr, ui ui_Size );		//読み取り
	s4		Write( const char* CPc_FilePath, ui ui_Offset, void* Pv_Addr, ui ui_Size );		//書き込み

private:
	void	SetKeyState( u1 u1_KeyCode, bool b_Press );
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
