/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/File.h
	概要		：	Mul light API - ファイルクラス
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

#pragma	once


/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	クラス・構造体定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	ファイル情報構造体
*******************************************************************************/
struct	FileInfo
{
public:
	ui		ui_Size;		//ファイルサイズ
};


/*******************************************************************************
	ファイルクラス
*******************************************************************************/
class	File
{
//定数
public:
	//エラー情報
	enum	Error
	{
		ERROR = _ERROR_,
		ERROR_NULLPOINTER,			//ファイルパスがNULL
		ERROR_INVALIDMODE,			//モードが無効
		ERROR_PATHOVERLONG,			//ファイルパスが長すぎる
		ERROR_FILENOTFOUND,			//ファイルが見つからない
		ERROR_NOTSETUP,				//セットアップが行われていない
		ERROR_NOAUTHORITY,			//権限が無い
		ERROR_READFAILER,			//読み込み失敗

		SUCCESS = _SUCCESS_,
		SUCCESS_MODE_R,				//読み込み専用でオープンした
		SUCCESS_ENDOFFILE,			//読み込み成功。(ファイル終端に達した)
	};

	//モード
	enum	Mode
	{
		MODE_R,			//読み取り
		MODE_RW,		//読み取り、書き込み
	};

	//シーク基準
	enum	SeekWhence
	{
		SEEK_HEAD,		//先頭を基準
		SEEK_CURRENT,	//現在のファイルポインタを基準
		SEEK_TAIL,		//末尾を基準
	};

//変数
private:
	ui		Mui_Offset;			//ファイル内オフセット(ファイルポインタ)
	u1		Mu1_Mode;			//操作モードフラグ
	char	Mc_FilePath[256\
				- sizeof (u4)\
				- sizeof (u1)];		//ファイルパス(デバイスIDから始まるパス。例："/0000/Mul_light/Main.cpp")

//静的メソッド
public:
	static s4		Create( const char* CPc_FilePath );		//ファイル新規作成
	static s4		Delete( const char* CPc_FilePath );		//ファイル削除
	static s4		GetFullPath( char* Pc_Dest, const char* CPc_Relative );

//メソッド
public:
	s4		Setup( const char* CPc_FilePath, Mode Mode = MODE_R );	//ファイルを操作するための準備を行う。この関数を呼ぶとクラス内の情報がリセットされる。
	s4		Clear( void );											//ファイル初期化(書き込み時のみ)
	s4		Rename( const char* CPc_NewFileName );					//ファイル名変更(書き込み時のみ)
	s4		GetInfo( FileInfo* P_FileInfo );						//ファイル情報取得
	s4		Read( void* Pv_Addr, ui ui_Size );						//読み取り
	s4		Write( void* Pv_Addr, ui ui_Size );						//書き込み
	s4		Seek( si si_Offset, SeekWhence Whence = SEEK_CURRENT );	//オフセット変更
	s4		GetOffset( void );										//オフセット取得
};


/*******************************************************************************
	(SCD)ファイル存在確認クラス
*******************************************************************************/
namespace	SCD
{
	class	PresentCheck : public SCDBase
	{
	public:
		const char*	MP_FilePath;	//ファイルパス
	};
}


/*******************************************************************************
	(SCD)ファイル読み込みクラス
*******************************************************************************/
namespace	SCD
{
	class	FileRead : public SCDBase
	{
	public:
		const char*	MP_FilePath;	//ファイルパス
		ui		Mui_Offset;		//ファイル内オフセット(ファイルポインタ)
		ui		Mui_Dest;		//データ格納先バッファ
		ui		Mui_Size;		//読み込みサイズ
	};
}


/*******************************************************************************
	(SCD)ファイル書き込みクラス
*******************************************************************************/
namespace	SCD
{
	class	FileWrite : public SCDBase
	{
	public:
		const char*	MP_FilePath;	//ファイルパス
		ui		Mui_Offset;		//ファイル内オフセット(ファイルポインタ)
		ui		Mui_Src;		//データ参照先バッファ
		ui		Mui_Size;		//書き込みサイズ
	};
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
