/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	ファイル名	：	/Mul_light/Boot/BootSector/Main.s
	概要		：	ブートセクタです。
	詳細		：	ブートセクタのメインプログラムです。
					El Toritoによって、メモリの0x0000 7c00〜0x0000 7dffに読み込まれます。
					BIOSの機能を利用してブートローダを読み込みます。
					ブートローダは0x0006 1000～0x0006 4fffに読み込みます。
	責任者		：	佐合 秀昭
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	定数定義
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
ORIGIN			= 0x7c00

SCT				= 0x200				#一セクタのバイト数

#スタック
_SS_			= 0x4000
_SP_			= 0x0000

#セグメント
SEG_REG			= ( ORIGIN >> 4 )	#リアルモード用セグメントレジスタ値

#ブートローダ
BL_DISC_SCT		= ( 0x0200 / SCT )	#ディスク上の論理セクタ
BL_BASE			= 0x60000			#読み込み先
BL_SIZE			= 0x4000 - 0x200	#サイズ
BL_SCT_BASE		= ( BL_BASE / SCT )	#セクタ単位の読み込み先
BL_SCT_SIZE		= ( BL_SIZE / SCT )	#セクタ単位のサイズ

#カーネル
K_DISC_SCT		= ( 0x4000 / SCT )	#ディスク上の論理セクタ
K1_BASE			= 0x64000			#前半の読み込み先
K1_SIZE			= 0xc000			#前半のサイズ
K2_BASE			= K1_SIZE			#後半の読み込み先
K2_SIZE			= ( 0x40000 - K1_SIZE )	#後半のサイズ
K1_SCT_BASE		= ( K1_BASE / SCT )	#前半のセクタ単位の読み込み先
K1_SCT_SIZE		= ( K1_SIZE / SCT )	#前半のセクタ単位のサイズ
K2_SCT_BASE		= ( K2_BASE / SCT )	#後半のセクタ単位の読み込み先
K2_SCT_SIZE		= ( K2_SIZE / SCT )	#後半のセクタ単位のサイズ



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	実行コード
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
	.section .text
	.code16
/*******************************************************************************
	初期化
*******************************************************************************/
_EntryPoint:
/*
	xorl		%eax,				%eax
	movw		%ax,				%es
	xorl		%edi,				%edi
	movw		$0x1000,			%cx
	rep			stosb
*/
	#セグメントレジスタの初期化
	movw		$SEG_REG,			%ax
	movw		%ax,				%ds
	movw		%ax,				%es
	movw		%ax,				%fs
	movw		%ax,				%gs

	#スタックを設定
	movw		$_SS_,				%ax
	movw		%ax,				%ss
	movw		$_SP_,				%sp




/*******************************************************************************
	ブートローダ読み込み
*******************************************************************************/
_BootLoaderLoad:
	#読み込み先アドレス
	movw		$BL_BASE >> 4,		%bx
	movw		%bx,				%es
	xorw		%bx,				%bx

	xorw		%dx,				%dx			#論理セクタ上位
	movw		$BL_DISC_SCT,		%ax			#論理セクタ下位
	movw		$BL_SCT_SIZE,		%di			#セクタ数
	xorw		%si,				%si			#ドライブ
	call		_FDDRead
	cmpw		$0x0000,			%cx
	jne			_TimeOut
_BootLoaderLoadFinish:


/*******************************************************************************
	カーネル読み込み
	メモリの先頭はBIOSが使ってるらしいので、前半部分はとりあえず違うところに置いておく。
*******************************************************************************/
_KernelLoad:
#前半
_KernelLoad1:
	#読み込み先アドレス
	movw		$K1_BASE >> 4,		%bx
	movw		%bx,				%es
	xorw		%bx,				%bx

	xorw		%dx,				%dx			#論理セクタ上位
	movw		$K_DISC_SCT,		%ax			#論理セクタ下位
	movw		$K1_SCT_SIZE,		%di			#セクタ数
	xorw		%si,				%si			#ドライブ
	call		_FDDRead
	cmpw		$0x0000,			%cx
	jne			_TimeOut
_KernelLoad1Finish:


#後半
_KernelLoad2:
	#読み込み先アドレス
	movw		$K2_BASE >> 4,		%bx
	movw		%bx,				%es
	xorw		%bx,				%bx

	xorw		%dx,				%dx			#論理セクタ上位
	movw		$K_DISC_SCT + K1_SCT_SIZE,		%ax			#論理セクタ下位
	movw		$K2_SCT_SIZE,		%di			#セクタ数
	xorw		%si,				%si			#ドライブ
	call		_FDDRead
	cmpw		$0x0000,			%cx
	jne			_TimeOut
_KernelLoad2Finish:


/*******************************************************************************
	読み込み完了。ブートセクタの仕事終了。ブートローダの実行に入ります。
*******************************************************************************/
_ReadFinish:
#	jmp		_Loop
	ljmp		$BL_BASE >> 4,		$0			#ブートローダに入る。

_Loop:
	hlt
	jmp			_Loop




/*******************************************************************************
	何回やってもダメ。エラー。
*******************************************************************************/
_TimeOut:
	movw		$_DiscReadError,	%si			#文字列のアドレス
	call		_PrintString
	jmp			_Loop






/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	関数
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
/*******************************************************************************
	FDD読み込み
	dx:ax:論理セクタ0～(1セクタ = 512Byte)
	di:セクタ数
	si:ドライブ
	es:bx:読み込み先アドレス
*******************************************************************************/
_FDDRead:
TIME_OUT		= 0xff			#CD(FDD)読み込み挑戦回数
ONE_CYLINDER	= 36			#1シリンダのセクタ数

#論理セクタをCHSに変換
	movw		$ONE_CYLINDER,		%bp
	divw		%bp								#論理セクタ / ONE_CYLINDER = シリンダ(ax) 余りセクタ(dx)
	movb		%al,				%ch			#シリンダを設定
	movw		%dx,				%ax			#ax:余りセクタ下位
	xorw		%dx,				%dx			#dx:余りセクタ上位(0)
	movw		$ONE_CYLINDER >> 1,	%bp
	divw		%bp								#余りセクタ / ( ONE_CYLINDER / 2 ) = ヘッド(ax) セクタ(dx)
	movb		%dl,				%cl			#セクタを設定
	movw		%si,				%dx			#ドライブを設定
	movb		%al,				%dh			#ヘッドを設定

#リードループ
_FDDReadLoop:
	incb		%cl								#セクタをインクリメント
	movw		$TIME_OUT,			%si			#タイムアウト回数を設定
	movw		$0x0201,			%ax			#ah:モード,		al:セクタ数

#失敗のためやり直し
_FDDReadRetry:
	decw		%si								#タイムアウト回数をデクリメント
	jz			_FDDReadTimeOut					#タイムアウト回数が0になったらエラー終了

	int			$0x13							#読み込み
	jc			_FDDReadRetry					#エラーならもう一度

	movw		%es,				%ax			#バッファアドレスを進める
	addw		$0x20,				%ax			#バッファアドレスを進める
	movw		%ax,				%es			#バッファアドレスを進める

	#指定セクタ数読み込んだら終了
	decw		%di
	jz			_FDDReadFinish					#if( --di == 0 ) goto _FDDReadFinish;

	#18セクタ読んでいなければもう一度読み込みを実行。
	cmpb		$18,				%cl
	jb			_FDDReadLoop					#if( cl < 18 ) goto _ReadLoop;
	xorb		%cl,				%cl			#セクタをリセット

	#表面を読み終わったら裏面を読み、裏も終わったら次のシリンダを読む。
	incb		%dh								#とりあえずヘッド番号加算
	cmpb		$1,					%dh
	je			_FDDReadLoop					#表から裏になったらそのまま読み込みを実行。if( dh == 1 ) goto _ReadLoop;
	xorb		%dh,				%dh			#裏面が終わるので、ヘッド番号を表にして次のシリンダを読む。

	#シリンダ番号を1加算してもう一度実行
	incb		%ch								#シリンダ番号加算
	jmp			_FDDReadLoop

#タイムアウト
_FDDReadTimeOut:
	movw		$0xffff,			%cx			#失敗の番号
	ret
#成功
_FDDReadFinish:
	xorw		%cx,				%cx			#成功
	ret


/*******************************************************************************
	文字列表示(終端 = 0)
	si:文字列の先頭アドレス#
*******************************************************************************/
_PrintString:
	movb		$0x0e,				%ah			#ah:0x0e
	movw		$0x0005,			%bx			#bh:0x00,		bl:カラーコード

_PrintStringLoop:
	movb		(%si),				%al
	cmpb		$0x00,				%al
	je			_PrintStringFinish
	int			$0x10							#表示
	incb		%dl
	incw		%si
	jmp			_PrintStringLoop

_PrintStringFinish:
	ret



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	データ
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
	.section .data

_DiscReadError:
	.ascii	"Disc Read Error!!"
	.byte	0

_Success:
	.ascii	"Success!"
	.byte	0



/*■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■
	End of file
■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■■*/
