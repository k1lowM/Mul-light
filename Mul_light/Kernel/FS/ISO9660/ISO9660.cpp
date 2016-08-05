/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Kernel/FS/ISO9600/ISO9600.cpp
	概要		：	ISO9660(ファイルシステム)
	詳細		：
	責任者		：
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	インクルード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
#include	"Common.h"
#include	"ISO9660.h"
#include	"Define.h"
#include	"DM.h"
#include	"ATA.h"
#include	<Math.h>

#include	<string.h>
#include	<stdlib.h>



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	グローバル
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
extern DM	G_DM;



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ISO9660(FS)クラス：パブリックメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	ISO9660(FS)クラスの初期化
	説明	：
	Include	：	ISO9660.h
	引数	：	u1 u1_DeviceID		デバイスID
				ui ui_FSMemID		ファイルシステムクラスのメモリ領域ID
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		ISO9660::Init( u1 u1_DeviceID, ui ui_FSMemID )
{
	VD		PVD;		//PVDのバッファ
	DevInfo	DevInfo = G_DM.GetDevInfo( u1_DeviceID );		//デバイス情報取得
	s4		s4_Return;

	//以前の情報を削除
	G_KMA.Delete( Mui_PDT_KMID );			//パスデータテーブルのメモリ領域を削除
	Mui_PDT_KMID = 0;

	//PVD読み取り
	if( ( s4_Return = Read( u1_DeviceID, LBA_PVD, 0, &PVD, sizeof PVD ) ) < 0 )
		return s4_Return;

	if( PVD.u4_PTSize == 0 )		//パステーブルのサイズが0ならエラー
		return ERROR_PT_SIZE_0;

	//CDのパステーブルからパス・データ・テーブルを構築する。
	{
		ui		ui_PTMemID;			//パステーブルのバッファのメモリID
		u1*		Pu1_PTBase;			//パステーブルのバッファアドレス
		PTR*	P_PTR;				//パステーブル・レコードのポインタ
		ui		ui_PTOffset;		//パステーブルのオフセット
		ui		ui_NumPR;			//パスレコードの数
		ui		ui_PRCount;			//パスレコードのカウンタ
		PathData*	P_PDTBase;		//パスデータテーブルのベースアドレス
		ui		ui_ParentOld = 0;	//一つ前の親ディレクトリ番号

		//パステーブルのサイズ分メモリ確保
		Pu1_PTBase = (u1*)G_KMA.GetBase( ui_PTMemID = G_KMA.Create( PVD.u4_PTSize ) );

		//パステーブル読み取り
		if( ( s4_Return = Read( u1_DeviceID, PVD.u4_PT_LBA, 0, Pu1_PTBase, PVD.u4_PTSize ) ) < 0 )
			return s4_Return;

		//パステーブル・レコードの数を算出
		for( ui_PTOffset = 0, ui_NumPR = 0; ui_PTOffset < PVD.u4_PTSize; ui_NumPR++ )
			ui_PTOffset += INT_CEIL_BIN( Pu1_PTBase[ui_PTOffset] + 8, 1 );		//ディレクトリ識別子の長さ + その他のヘッダ情報8バイト分

		//パスデータテーブルのメモリ領域を確保。
		P_PDTBase = (PathData*)G_KMA.GetBase( Mui_PDT_KMID = G_KMA.Create( sizeof (PathData) * ui_NumPR ) );

		//パステーブル・レコードからパス・データを構築。
		for( ui_PRCount = 0, ui_PTOffset = 0; ui_PRCount < ui_NumPR; ui_PRCount++ )
		{
			P_PTR = (PTR*)Pu1_PTBase;
			ui		ui_ParentCache = P_PTR->u2_Parent - 1;		//親番号のキャッシュ

			strncpy( P_PDTBase[ui_PRCount].Ac_DirName, P_PTR->Ac_Name, P_PTR->u1_NameLen );		//ディレクトリ名を格納
			P_PDTBase[ui_PRCount].Ac_DirName[31]	= 0;		//文字列の終端を設定
			P_PDTBase[ui_PRCount].u4_LBA			= P_PTR->u4_DirLBA;			//ディレクトリのLBAを格納
			P_PDTBase[ui_PRCount].u2_FirstChiled	= 0;		//子ディレクトリの先頭要素番号を初期化
			P_PDTBase[ui_PRCount].u2_NumChiled		= 0;		//子ディレクトリ数を初期化

			//親ディレクトリの設定
			if( ui_ParentOld != ui_ParentCache )	//前のディレクトリと親が違う(子の先頭)
				P_PDTBase[ui_ParentCache].u2_FirstChiled = ui_PRCount;
			P_PDTBase[ui_ParentCache].u2_NumChiled++;		//親の子の数を加算
			ui_ParentOld = ui_ParentCache;

			//ディレクトリ識別子の長さ + その他のヘッダ情報8バイト分進める
			Pu1_PTBase += INT_CEIL_BIN( P_PTR->u1_NameLen + 8, 1 );
		}

		//パステーブル用のメモリ領域を削除
		G_KMA.Delete( ui_PTMemID );
	}

	Mui_FSMemID		= ui_FSMemID;
	M_FSType		= FST::ISO9660;
	return SUCCESS;
}


/*******************************************************************************
	概要	：	ファイルシステム確認
	説明	：	対象のデバイスのファイルシステムがISO9660か調べます。
	Include	：	ISO9660.h
	引数	：	u1 u1_DeviceID		確認するデバイスのID
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		ISO9660::CheckFS( u1 u1_DeviceID )
{
	VD		PVD;		//PVDのバッファ
	DevInfo	DevInfo = G_DM.GetDevInfo( u1_DeviceID );		//デバイス情報取得
	const u1	CAu1_PVDSignature[] = { 1, 'C', 'D', '0', '0', '1', 1, 0 };		//PVDのシグネチャ
	s4		s4_Return;

	//エラー処理
	if( DevInfo.P_Driver->GetDeviceType() != DT::CDD )		//CDドライブでなければエラー
		return ERROR_NOTSUPPORT;

	//PVD読み取り
	if( ( s4_Return = Read( u1_DeviceID, LBA_PVD, 0, &PVD, sizeof PVD ) ) < 0 )
		return s4_Return;

	//PVDのシグネチャと一致しなければエラー。
	if( strncmp( (char*)&PVD, (char*)CAu1_PVDSignature, sizeof CAu1_PVDSignature ) )
		return ERROR_NOTSUPPORT;


	return SUCCESS;
}


/*******************************************************************************
	概要	：	読み取り
	説明	：	ISO9660のファイルシステムからデータを読み取ります。
	Include	：	ISO9660.h
	引数	：	const char* CPc_FilePath		'/'から始まる絶対パス
				const char* CPc_NewFileName		新しい名前
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		ISO9660::Rename( const char* CPc_FilePath, const char* CPc_NewFileName )
{
	return ERROR_NOTSUPPORT;
}


/*******************************************************************************
	概要	：	読み取り
	説明	：	ISO9660のファイルシステムからデータを読み取ります。
	Include	：	ISO9660.h
	引数	：	const char* CPc_FilePath		'/'から始まる絶対パス
				ui ui_Offset		ファイル内のオフセット
				void* Pv_Addr		格納先アドレス
				ui ui_Size			処理するサイズ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		ISO9660::Read( const char* CPc_FilePath, ui ui_Offset, void* Pv_Addr, ui ui_Size )
{
	PathData*	P_PDT = (PathData*)G_KMA.GetBase( Mui_PDT_KMID );
	char	c_FPWork[256];			//ファイルパスワーク
	char*	Pc_FPWork;				//ワークのポインタ
	ui		ui_PDNo = 0;			//パスデータ番号
	u1		u1_DeviceID;

	//エラー処理
	if( Pv_Addr == NULL )		//ヌルポインタエラー
		return ERROR_NULLPOINTER;
	if( Mui_PDT_KMID == 0 )			//未初期化
		return ERROR_UNINITIALIZED;


	//ディスクの入れ替えが発生していないか調べる。

	//入れ替わっていれば、最初からやり直し。終了。



	//指定のファイルパスのディレクトリ・ファイルがあるか確認
	{
		char*	Pc_FPstrsep = c_FPWork;	//strsep用のポインタ
		ui		ui_ParentNo;

		strncpy( c_FPWork, CPc_FilePath, 255 );		//ファイルパスをワークにコピー
		strsep( &Pc_FPstrsep, "/" );		//空読み

		//デバイスID取得
		u1_DeviceID = strtoul( Pc_FPWork = strsep( &Pc_FPstrsep, "/" ), NULL, 10 );

		//ディレクトリを順番に探索
		while( true )
		{
			Pc_FPWork = strsep( &Pc_FPstrsep, "/" );	//ファイル・ディレクトリ名取得
			if( Pc_FPstrsep == NULL )					//目的のファイル・ディレクトリ名(次がNULL)まで達したら抜ける。
				break;

			ui_ParentNo = ui_PDNo;								//親ディレクトリ番号をセット
			ui_PDNo		= P_PDT[ui_ParentNo].u2_FirstChiled;	//パスデータ番号を子の先頭に設定

			//ディレクトリパスが一致するまで検索
			for( ; strcmp( Pc_FPWork, P_PDT[ui_PDNo].Ac_DirName ); ui_PDNo++ )
			{
				//子ディレクトリをすべて比較してもダメ。
				if( ui_PDNo - P_PDT[ui_ParentNo].u2_FirstChiled >= P_PDT[ui_ParentNo].u2_NumChiled )
					return ERROR_FILENOTFOUND;
			}
		}
	}

	//ディレクトリ・レコードから目的のファイルを探索
	{
		u1		Au1_DR[0x100];		//ディレクトリ・レコードのバッファ(サイズがu1だから、MAXが255)
		DR*		P_DR = (DR*)Au1_DR;	//ディレクトリ・レコード構造体(システム領域の読み取り不可)
		ui		ui_DROffset;		//ディレクトリ・レコードのオフセット値
		char*	Pc_Name = P_DR->Ac_Name;

		//目的のファイル・ディレクトリを検索
		for( ui_DROffset = 0, P_DR->u1_NameLen = 15; ; ui_DROffset += P_DR->u1_Length )
		{
			//ディレクトリ・レコード読み取り
			Read( u1_DeviceID, P_PDT[ui_PDNo].u4_LBA, ui_DROffset, Au1_DR, sizeof Au1_DR );

			//すべてのファイル・ディレクトリと一致しない。
			if( P_DR->u1_Length == 0 )
				return ERROR_FILENOTFOUND;		//ファイル見つからず・・・

			//ファイルならファイル名を修正
			if( ( P_DR->u1_AttrFlags & B1_DIR_MASK ) == B1_FILE )
			{
				while( Pc_Name[--P_DR->u1_NameLen] != ';' )
				{}
				Pc_Name[P_DR->u1_NameLen] = '\0';
			}

			//一致したら抜ける
			if( !strncmp( Pc_FPWork, P_DR->Ac_Name, P_DR->u1_NameLen ) )
				break;
		}

		if( P_DR->u4_Size < ui_Size )
			ui_Size = P_DR->u4_Size;
		//読み取り、終了
		return Read( u1_DeviceID, P_DR->u4_LBA, ui_Offset, Pv_Addr, ui_Size );
	}
}


/*******************************************************************************
	概要	：	書き込み
	説明	：
	Include	：	ISO9660.h
	引数	：	const char* CPc_FilePath		'/'から始まる絶対パス
				ui ui_Offset		ファイル内のオフセット
				void* Pv_Addr		格納先アドレス
				ui ui_Size			処理するサイズ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		ISO9660::Write( const char* CPc_FilePath, ui ui_Offset, void* Pv_Addr, ui ui_Size )
{
	return ERROR_NOTSUPPORT;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ISO9660(FS)クラス：プライベートメソッド
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	概要	：	読み込み
	説明	：	実際にデバイスから読み込みを行う関数です。
	Include	：	ISO9660.h
	引数	：	u1 u1_DeviceID		デバイスID
				ui ui_LBA			LBA
				ui ui_Offset		ファイル内のオフセット
				void* Pv_Addr		格納先アドレス
				ui ui_Size			処理するサイズ
	戻り値	：	s4		エラー情報
*******************************************************************************/
s4		ISO9660::Read( u1 u1_DeviceID, ui ui_LBA, ui ui_Offset, void* Pv_Addr, ui ui_Size )
{
	ui		ui_TempGMID;
	u1*		Pu1_BufBase;
	DCD_Data	CD;						//デバイス制御情報
	DevInfo	DevInfo = G_DM.GetDevInfo( u1_DeviceID );

	//無駄に読み取らないように、オフセットが2048以上ならLBAとオフセットを調整する。
	ui_LBA		+= ui_Offset >> 11;
	ui_Offset	&= SECTOR_SIZE - 1;

	//一時バッファ確保
	if( !( ui_TempGMID = G_GMA.Create( ui_Offset + ui_Size ) ) )
		return ERROR_MEMORYALLOC;		//メモリ領域確保エラー

	Pu1_BufBase = (u1*)G_GMA.GetBase( ui_TempGMID );

	CD.Init( CD.CTRL_READ, u1_DeviceID );	//デバイス制御情報初期化
	CD.ui_LBA		= ui_LBA;		//論理ブロックアドレス
	CD.Pv_Buffer	= Pu1_BufBase;	//バッファアドレス
	CD.ui_NumSector	= BYTE2SECTOR_CEIL( ui_Offset + ui_Size );	//セクタ数

	if( ( (ATA*)DevInfo.P_Driver )->Ctrl( &CD ) < 0 )
	{
		G_GMA.Delete( ui_TempGMID );		//一時バッファ削除
		return ERROR_READ_FAILED;		//読み取り失敗
	}

	memcpy( Pv_Addr, Pu1_BufBase + ui_Offset, ui_Size );
	G_GMA.Delete( ui_TempGMID );		//一時バッファ削除

	return SUCCESS;
}



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/

