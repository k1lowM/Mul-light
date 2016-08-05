/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Include/Mul_light/File.cpp
	概要		：	Mul light API - ファイルクラス
	詳細		：
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"MLCommon.h"
#include	"File.h"
#include	<string.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイルクラス：静的パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	ファイル新規作成
	説明	：	ファイルシステム上にファイルを新規作成します。
				CD-Rなどの更新ができないデバイスに対しては無効です。
				すでに対象のファイルが存在している場合は何もしません。
	Include	：	File.h
	引数	：	const char* CPc_FilePath		対象のファイルパス
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		File::Create( const char* CPc_FilePath )
{
	return 0;
}


/*******************************************************************************
	概要	：	ファイル削除
	説明	：	ファイルシステム上からファイルを抹消します。
				ゴミ箱に入ったりはしません。
				CD-Rなどの更新ができないデバイスに対しては無効です。
	Include	：	File.h
	引数	：	const char* CPc_FilePath		対象のファイルパス
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		File::Delete( const char* CPc_FilePath )
{
	return 0;
}


/*******************************************************************************
	概要	：	フルパス取得
	説明	：	現在のパスと相対パスからフルパスを取得します。
	Include	：	File.h
	引数	：	char* Pc_Dest
				const char* CPc_Relative
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		File::GetFullPath( char* Pc_FPDest, const char* CPc_Relative )
{
	char	c_FPWork[256];		//ファイルパスワーク
	char*	Pc_FPWork;
	char*	Pc_FPstrsep = c_FPWork;
	ui		ui_StrCount;

	//エラー処理
	if( Pc_FPDest == NULL || CPc_Relative == NULL )	//NULLポインタ
		return ERROR_NULLPOINTER;

//	GetPath( Pc_Dest );			//現在のファイルパス取得

	strcpy( c_FPWork, CPc_Relative );		//相対パスをワークにコピー

	//相対パスを'/'で区切って、最初の文字列のポインタを取得(終端に達するとNULL)
	while( ( Pc_FPWork = strsep( &Pc_FPstrsep, "/" ) ) )
	{
		//親階層を指している
		if( strcmp( Pc_FPWork, ".." ) )
		{
			//一階層上に移動
			ui_StrCount = strlen( c_FPWork ) - 1;
			for( c_FPWork[ui_StrCount] = 0; c_FPWork[ui_StrCount] != '/'; ui_StrCount-- )
				c_FPWork[ui_StrCount] = 0;
			continue;
		}

		//自己階層を指している
		if( strcmp( Pc_FPWork, "." ) )
			continue;

		//通常のディレクトリやファイルなら絶対パスに繋げる
		if( Pc_FPDest[strlen( Pc_FPDest ) - 1] != '/' )		//最後に'/'が入っていなければ
			strcat( Pc_FPDest, "/" );						//'/'を繋げる
		strcat( Pc_FPDest, Pc_FPWork );
	}

	return SUCCESS;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイルクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	ファイル操作セットアップ
	説明	：	ファイルを読み書きするための準備を行います。
	Include	：	File.h
	引数	：	const char* CPc_FilePath		対象のファイルパス
				[Mode Mode = MODE_R]			操作モード
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		File::Setup( const char* CPc_FilePath, Mode Mode )
{
	u1		u1_Mode = Mode;

	//リセット
	Mu4_FilePointer = Mu1_Mode = Mc_FilePath[0] = 0;

	//エラー処理
	if( CPc_FilePath == NULL )		//ファイルパスがNULL
		return ERROR_NULLPOINTER;
	if( strlen( Mc_FilePath ) >= sizeof( Mc_FilePath ) )	//ファイルパスが長すぎる
		return ERROR_PATHOVERLONG;

	//相対パスなら絶対パスに変換
	if( CPc_FilePath[0] != '/' )
		GetFullPath( Mc_FilePath, CPc_FilePath );		//相対パスから絶対パスを取得


	//ファイルの有無確認
	if( SCPresentCheck( Mc_FilePath ) )
		;


	//書き込みモードのとき
	if( u1_Mode == MODE_RW )
	{
		//書き込み可能デバイスか？
	}


	//変数セット
	Mu1_Mode		= u1_Mode;
//	strncpy( Mc_FilePath, CPc_FilePath, sizeof( Mc_FilePath ) );

	return SUCCESS;
}


/*******************************************************************************
	概要	：	ファイル名変更
	説明	：	Setupで指定したファイルのファイル名を変更します。
				操作モードがMODE_Rだった場合は、エラーです。
	Include	：	File.h
	引数	：	const char* CPc_NewFileName		変更後のファイル名
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		File::Rename( const char* CPc_NewFileName )
{
	//エラー処理
	if( Mc_FilePath[0] == 0 )
		return ERROR_NOTSETUP;

	//まだ途中
	return 0;
}


/*******************************************************************************
	概要	：	ファイル情報取得
	説明	：	Setupで指定したファイルのファイル名を変更します。
				操作モードがMODE_Rだった場合は、エラーです。
	Include	：	File.h
	引数	：	FileInfo* P_FileInfo		ファイル情報を格納する構造体ポインタ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		File::GetInfo( FileInfo* P_FileInfo )
{
	//まだ途中
	return 0;
}


/*******************************************************************************
	概要	：	ファイル読み込み
	説明	：	Setupで指定したファイルからデータを読み込みます。
	Include	：	File.h
	引数	：	void* Pv_Addr
				ui ui_Size
	戻り値	：	s4		ファイルポインタの位置(負数ならエラー)
*******************************************************************************/
s4		File::Read( void* Pv_Addr, ui ui_Size )
{
	//エラー処理
	if( Mc_FilePath[0] == 0 )
		return ERROR_NOTSETUP;

	//デバイスの存在確認



	//ファイル存在確認
	if( SCPresentCheck( Mc_FilePath ) < 0 )		//ファイル見つからず。。
		return ERROR_FILENOTFOUND;

	//ファイル読み込み
	return SCRead( Pv_Addr, ui_Size );
}


/*******************************************************************************
	概要	：	ファイル書き込み
	説明	：	Setupで指定したファイルにデータを書き込みます。
				操作モードがMODE_Rだった場合は、エラーです。
	Include	：	File.h
	引数	：	void* Pv_Addr
				ui ui_Size
	戻り値	：	s4		ファイルポインタの位置(負数ならエラー)
*******************************************************************************/
s4		File::Write( void* Pv_Addr, ui ui_Size )
{
	//エラー処理
	if( Mc_FilePath[0] == 0 )
		return ERROR_NOTSETUP;
	if(  )		//読み込み専用モードならエラー
		return ;


	//まだ途中
	return 0;
}


/*******************************************************************************
	概要	：	ファイルシーク
	説明	：	ファイルポインタをシークします。
	Include	：	File.h
	引数	：	s4 s4_Offset		シーク基準からのオフセット
				SeekWhence Whence	シークの基準(SEEK_HEAD,SEEK_CURRENT,etc...)
	戻り値	：	s4		ファイルポインタの位置(負数ならエラー)
*******************************************************************************/
s4		File::Seek( s4 s4_Offset, SeekWhence Whence )
{
	//エラー処理
	if( Mc_FilePath[0] == 0 )
		return ERROR_NOTSETUP;

	//まだ途中
	return 0;
}


/*******************************************************************************
	概要	：	オフセット取得
	説明	：	現在のファイル内のオフセットを取得する。
	Include	：	File.h
	引数	：	void
	戻り値	：	s4		ファイルポインタの位置(負数ならエラー)
*******************************************************************************/
s4		File::GetOffset( void )
{
	//エラー処理
	if( Mc_FilePath[0] == 0 )
		return ERROR_NOTSETUP;

	return (s4)Mu4_FilePointer;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイルクラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	ファイルの存在確認
	説明	：	ファイルの存在確認のシステムコールを発行します。
	Include	：	File.h
	引数	：	const char* CPc_FilePath		対象のファイルパス
	戻り値	：	s4		エラー情報
*******************************************************************************
s4		File::SCPresentCheck( const char* CPc_FilePath )
{
	s4		s4_RetValue;

	//ファイル存在確認
	__asm__
	(
			"pushl	%1;"
			"pushl	%2;"
			"lcall	%3,		$0x00;"
			: "=a"( s4_RetValue )
			: "i"( SC1::FILE_PRESENTCHECK ), "r"( CPc_FilePath ), "i"( SC_1 )
			:
	);
	return s4_RetValue;
}


/*******************************************************************************
	概要	：	ファイル読み込み
	説明	：	指定したサイズのデータをファイルから読み込みます
	Include	：	File.h
	引数	：	void* Pv_Addr				読み込んだデータを格納するアドレス
				ui ui_Size					読み込むサイズ
	戻り値	：	s4		エラー情報
*******************************************************************************
s4		File::SCRead( void* Pv_Addr, ui ui_Size )
{
	s4		s4_RetValue;

	//ファイル存在確認
	__asm__
	(
			"pushl	%1;"
			"pushl	%2;"
			"pushl	%3;"
			"pushl	%4;"
			"pushl	%5;"
			"lcall	%6,		$0x00;"
			: "=a"( s4_RetValue )
			: "i"( SC4::FILE_READ ), "r"( Mc_FilePath ), "r"( Mu4_FilePointer ), "r"( Pv_Addr ), "r"( ui_Size ), "i"( SC_4 )
			:
	);
	return s4_RetValue;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

