/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/FS/Template/Template.h
	概要		：	てんぷれ(ファイルシステム)
	詳細		：
	責任者		：
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"FS.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	てんぷれクラス
*******************************************************************************/
class	Template : public FS
{
//定数
	//エラー情報
	enum	Error
	{
		ERROR = _ERROR_,
		ERROR_NOTSUPPORT,		//サポート対象外

		SUCCESS = _SUCCESS_,
	};

//変数
private:

//メソッド
public:
	void	Init( void );

	s4		CheckFS( u1 u1_DeviceID );		//ファイルシステム確認

	s4		Rename( const char* CPc_FilePath, const char* CPc_NewFileName );				//ファイル名変更
	s4		Read( const char* CPc_FilePath, ui ui_Offset, u1* Pu1_Addr, ui ui_Size );		//読み取り
	s4		Write( const char* CPc_FilePath, ui ui_Offset, u1* Pu1_Addr, ui ui_Size );		//書き込み
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
