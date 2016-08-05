/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/FS/VFS/VFS.h
	概要		：	仮想ファイルシステム
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	VFSクラス
*******************************************************************************/
class	VFS
{
//定数
public:
	//エラー情報
	enum	Error
	{
		ERROR = _ERROR_,
		ERROR_NULLPOINTER,			//ヌルポエラー
		ERROR_NOTSUPPORT,			//サポート対象外
		ERROR_INVALID_FSTYPE,		//無効ファイルシステムタイプ
		ERROR_INVALID_FP,			//無効ファイルパス
		ERROR_INVALID_DEVID,		//無効デバイスID
		ERROR_FILESYSTEM_NON,		//ファイルシステム無し。
		ERROR_DRIVER_NULL,			//ドライバNULL
		ERROR_FPTOOLONG,			//ファイルパスが長すぎる

		SUCCESS = _SUCCESS_,
	};

//メソッド
public:
	void	Init( void );

	s4		Rename( const char* CPc_FilePath, const char* CPc_NewFileName );		//ファイル名変更
	s4		Read( const char* CPc_FilePath, ui ui_Offset, void* Pv_Addr, ui ui_Size );		//読み取り
	s4		Write( const char* CPc_FilePath, ui ui_Offset, void* Pv_Addr, ui ui_Size );		//書き込み

	s4		DistinctionFS( u1 u1_DeviceID );		//ファイルシステム識別

	s4		CheckFP( const char* CPc_FilePath );	//ファイルパスの正当性の検査
	s4		OptimFP( char* Pc_FilePath );			//ファイルパス最適化
};



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
