/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/FS/VFS/VFS.cpp
	概要		：	仮想ファイルシステム
	詳細		：	ファイルシステムを統合管理します。
					このクラスに任せれば、様々なファイルシステムを意識せずにファイルが扱えます。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"VFS.h"
#include	"DM.h"

#include	<stdlib.h>
#include	<string.h>


#include	"Joliet.h"
#include	"ISO9660.h"

#include	"Keyboard.h"
#include	"Mouse.h"



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数・マクロ定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
extern VFS	G_VFS;
extern DM	G_DM;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	VFSクラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	VFSクラスの初期化
	説明	：
	Include	：	VFS.h
	引数	：	void
	戻り値	：	void
*******************************************************************************/
void	VFS::Init( void )
{
	DP( "VFS::Init()\t\t\t\t\t\t\t\t\t[  OK  ]" );
}


/*******************************************************************************
	概要	：	ファイル名変更
	説明	：	ファイル名を変更します。
	Include	：	VFS.h
	引数	：	const char* CPc_FilePath			対象のファイルパス(絶対)
				const char* CPc_NewFileName			新しいファイル名
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		VFS::Rename( const char* CPc_FilePath, const char* CPc_NewFileName )
{
	//未実装
	return 0;
}


/*******************************************************************************
	概要	：	ファイルシステム読み取り
	説明	：
	Include	：	VFS.h
	引数	：	const char* CPc_FilePath	'/'から始まる絶対パス
				ui ui_Offset				ファイル内のオフセット
				void* Pv_Addr				格納先アドレス
				ui ui_Size					処理するサイズ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		VFS::Read( const char* CPc_FilePath, ui ui_Offset, void* Pv_Addr, ui ui_Size )
{
	char	Ac_FilePath[256];		//ファイルパス
	DevInfo	DevInfo;
	u1		u1_DeviceID;
	s4		s4_Return;

	strncpy( Ac_FilePath, CPc_FilePath, 255 );

	//ファイルパス最適化
	if( ( s4_Return = OptimFP( Ac_FilePath ) ) < 0 )
		return s4_Return;

	u1_DeviceID = strtoul( Ac_FilePath + 1, NULL, 10 );	//デバイスID抜きだし
	DevInfo = G_DM.GetDevInfo( u1_DeviceID );			//デバイス情報取得

	if( DevInfo.P_Driver == NULL )		//ドライバのポインタがNULL
		return ERROR_DRIVER_NULL;

	//ファイルシステムを調べる。(正常に終了するとデバイス情報にファイルシステムが登録される)
	if( ( s4_Return = DistinctionFS( u1_DeviceID ) ) < 0 )
		return s4_Return;		//エラーが返ってきたら、そのまま返す。

	DevInfo = G_DM.GetDevInfo( u1_DeviceID );				//デバイス情報取得

	//ファイルシステム読み取り
	switch( DevInfo.P_FS->GetFSType() )
	{
//ファイルシステム読み込みマクロ
#define	READ__FS_TYPE_CASE( _FST_, _FS_ )\
	case _FST_:	return ( (_FS_*)DevInfo.P_FS )->Read( Ac_FilePath, ui_Offset, Pv_Addr, ui_Size );

	READ__FS_TYPE_CASE( FST::ISO9660,	ISO9660 )
//	READ__FS_TYPE_CASE( FST::JOLIET,	Joliet )
//	READ__FS_TYPE_CASE( FST::ROCKRIDGE,	RockRidge )

	READ__FS_TYPE_CASE( FST::KEYBOARD,	Keyboard )
	READ__FS_TYPE_CASE( FST::MOUSE,		Mouse )

	default:
		break;
	}

	//その他は無効
	return ERROR_INVALID_FSTYPE;

}


/*******************************************************************************
	概要	：	ファイルシステム書き込み
	説明	：
	Include	：	VFS.h
	引数	：	const char* CPc_FilePath	'/'から始まる絶対パス
				ui ui_Offset				ファイル内のオフセット
				void* Pv_Addr				格納先アドレス
				ui ui_Size					処理するサイズ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		VFS::Write( const char* CPc_FilePath, ui ui_Offset, void* Pv_Addr, ui ui_Size )
{
	char	Ac_FilePath[256];		//ファイルパス
	s4		s4_Return;

	strncpy( Ac_FilePath, CPc_FilePath, 255 );

	//ファイルパス最適化
	if( ( s4_Return = OptimFP( Ac_FilePath ) ) < 0 )
		return s4_Return;


	//まだ途中
	return 0;
}


/*******************************************************************************
	概要	：	ファイルシステム識別
	説明	：	デバイスIDから、ファイルシステムを識別します。
				識別したらデバイス情報にファイルシステムを登録します。
	Include	：	VFS.h
	引数	：	u1 u1_DeviceID		デバイスID
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		VFS::DistinctionFS( u1 u1_DeviceID )
{
	DevInfo	DevInfo = G_DM.GetDevInfo( u1_DeviceID );		//デバイス情報取得
	ui		ui_MemoryID;

	//ファイルシステムが登録されていれば、そこから調べる。
	if( DevInfo.P_FS != NULL )
	{
		switch( DevInfo.P_FS->GetFSType() )
		{
//ファイルシステム確認マクロ
#define	DISTFS__FS_TYPE_CASE( _FST_, _FS_ )\
	case _FST_:\
		if( ( (_FS_*)DevInfo.P_FS )->CheckFS( u1_DeviceID ) > 0 )\
			return SUCCESS;\
		else\
			G_KMA.Delete( DevInfo.P_FS->GetMemID() );\
		break;

		DISTFS__FS_TYPE_CASE( FST::MOUSE, Mouse )
		DISTFS__FS_TYPE_CASE( FST::KEYBOARD, Keyboard )
//		DISTFS__FS_TYPE_CASE( Joliet )
		DISTFS__FS_TYPE_CASE( FST::ISO9660, ISO9660 )

		default:		//その他は無効
			return ERROR_INVALID_FSTYPE;
		}
	}


	//各ファイルシステムに確認させる。
	//上位のファイルシステムから順に確認させる。( Joliet/RockRidge > ISO9660 )
//ファイルシステム確認マクロ
#define	CHECK_FS( _FS_ )\
	{\
		_FS_	_##_FS_##_;\
		_FS_*	_P_##_FS_##_;\
		if( _##_FS_##_.CheckFS( u1_DeviceID ) > 0 )\
		{\
			ui_MemoryID = G_KMA.Create( sizeof _##_FS_##_ );\
			_P_##_FS_##_ = (_FS_*)G_KMA.GetBase( ui_MemoryID );\
			if( _P_##_FS_##_->Init( u1_DeviceID, ui_MemoryID ) > 0 )\
			{\
				G_DM.DevInfoUpdate( u1_DeviceID, _P_##_FS_##_ );\
				return SUCCESS;\
			}\
		}\
	}

	CHECK_FS( Keyboard )
	CHECK_FS( Mouse )
//	CHECK_FS( Joliet )
	CHECK_FS( ISO9660 )


	return ERROR_NOTSUPPORT;
}


/*******************************************************************************
	概要	：	ファイルパスの正当性の検査
	説明	：	ファイルパスが正しいかどうか検査します。
				長さが255までであることも保証します。
				有効例：/10/ aa/aa/j/
						/10/a a/aa/j
						/10/aa /aa/
						/10/aaa/aa
						/10/aaa/
						/10/aaa
						/10/
						/10
						/
				無効例： /10/fff
						/10 /hhh
						'\0'
						/ 87/llll
						/a/aaa/aa/a
						/10/aaa/aa//
						/10//aaaa
						10/aaaa
						//fff
	Include	：	VFS.h
	引数	：	const char* CPc_FilePath		検査対象のファイルパス
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		VFS::CheckFP( const char* CPc_FilePath )
{
	ui		ui_Count;

	if( CPc_FilePath == NULL )			//NULLポインタエラー
		return ERROR_NULLPOINTER;
	if( CPc_FilePath[0] != '/' )		//先頭が'/'でなければエラー
		return ERROR_INVALID_FP;
	if( CPc_FilePath[1] == '\0' )		//"/"のみなら、さっさと正常終了。
		return SUCCESS;
	if( strstr( CPc_FilePath, "//" ) )		//"//"があったらエラー
		return ERROR_INVALID_FP;
	if( strtoul( CPc_FilePath + 1, NULL, 10 ) > 255 )	//デバイス番号が0から255以外ならエラー
		return ERROR_INVALID_DEVID;

	//デバイスIDに無効な文字が無いかチェック
	for( ui_Count = 1; CPc_FilePath[ui_Count] != '/' && CPc_FilePath[ui_Count] != '\0'; ui_Count++ )
	{
		if( CPc_FilePath[1] < '0' || '9' < CPc_FilePath[1] )	//数値以外があればエラー
			return ERROR_INVALID_FP;
		if( ui_Count >= 255 )			//ファイルパスが長すぎる
			return ERROR_FPTOOLONG;
	}

	return SUCCESS;
}


/*******************************************************************************
	概要	：	ファイルパス最適化
	説明	：	ファイルパスを簡単に扱えるように最適化します。
				"."(自己参照)や".."(親参照)などを最適化します。
	Include	：	VFS.h
	引数	：	char* Pc_FilePath		最適化対象のファイルパス
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		VFS::OptimFP( char* Pc_FilePath )
{
	char	Ac_FPWork[256];				//ファイルパスワーク
	char*	Pc_FPWork;					//ワークのポインタ
	char*	Pc_FPstrsep = Ac_FPWork + 1;	//strsep用のポインタ
	si		si_Offset = 0;				//'/'の場所のオフセット値
	s4		s4_Return;

	//ファイルパス検査
	if( ( s4_Return = CheckFP( Pc_FilePath ) ) < 0 )
		return s4_Return;

	strcpy( Ac_FPWork, Pc_FilePath );
	Pc_FilePath[0] = '\0';

	while( ( Pc_FPWork = strsep( &Pc_FPstrsep, "/" ) ) && Pc_FPWork[0] != '\0' )
	{
		if( !strcmp( Pc_FPWork, "." ) )		//自己参照
			continue;
		if( !strcmp( Pc_FPWork, ".." ) )	//親参照
		{
			Pc_FilePath[si_Offset] = '\0';			//現在の階層に終端を置く
			while( Pc_FilePath[si_Offset] != '/' )	//オフセットを親の階層まで移動
				if( --si_Offset < 0 )				//オフセットがマイナスになったらエラー
					return ERROR_INVALID_FP;
			continue;
		}

		//'/'の場所を更新
		si_Offset += strlen( Pc_FilePath + si_Offset );

		Pc_FilePath[si_Offset] = '/';
		strcpy( Pc_FilePath + si_Offset + 1, Pc_FPWork );
	}

	return SUCCESS;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

