//
//	Copylight (C) 2003 MakKi
//
//	Avisynth��GPL�Ȃ̂ŁA���̃\�t�g��GPL�ɂ��܂��B
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA, or visit
// http://www.gnu.org/copyleft/gpl.html .
//
//
/** @file
 *	@brief ���N���X
 */
#ifndef ___DELOGO_H
#define ___DELOGO_H

#include <windows.h>
#include <math.h>
#include "avisynth.h"
#include "../logo.h"

enum {
	LOGO_FADE_MAX = 256,	/// �t�F�[�h���̕s�����x�ő�l
	LOGO_DEFAULT_DEPTH = 128,	/// �s�����x�i�[�x�j�����l
};

/*---------------------------------------------------------------------------*/
/** TYPE
 *@{
 */
/// for EraseLOGO
struct Erase {
	static const char *Name(void){ return "EraseLOGO"; }
};
/// for AddLOGO
struct Add {
	static const char *Name(void){ return "AddLOGO"; }
};

//@}
/*****************************************************************************/
/** ���N���X
 */
template <class TYPE,class COLOR>
class deLOGO : public GenericVideoFilter {
protected:
	/// �z��|�C���^�Ǘ��pauto_ptr
	template<class T>
	class aptr{
		T *ptr;
	public:
		aptr(void):ptr(NULL){}
		aptr(T *t):ptr(t){}
		~aptr(){ delete[] ptr; }
		aptr<T> &operator =(T *t){
			delete[] ptr; ptr=t; return *this;
		}
		aptr<T> &operator =(aptr<T> &t){
			delete[] ptr; ptr=t.ptr; t.ptr=NULL; return *this;
		}
		T* get(void){ return ptr; }
		aptr<T> &ref(void){ return *this; }
		bool operator ==(T* t){ return ptr==t; }
		bool operator !(void){ return !ptr; }
	};
	::LOGO_HEADER lgh;
	aptr<typename COLOR::LOGO_PIXEL> logodata;
	int start, fadein, fadeout, end;

public:
	deLOGO(const PClip &clip,const char *logofile,const char *logoname,
				int pos_x,int pos_y,int depth,int y,int u,int v,
				int _start,int _fadein,int _fadeout,int _end,
				IScriptEnvironment *env)
		: GenericVideoFilter(clip), logodata(NULL),
		  start(_start),fadein(_fadein),fadeout(_fadeout),end(_end)
	{
		const VideoInfo &vi = clip->GetVideoInfo();
		child->SetCacheHints(CACHE_NOTHING,0);

		try{
			aptr<::LOGO_PIXEL> srcdata;
			// �ǂݍ���
			srcdata = ReadLogoData(logofile,logoname);
			// �ʒu�A�[�x����
			if(pos_x!=0 || pos_y!=0 || depth!=LOGO_DEFAULT_DEPTH){
				AdjustLogo(srcdata.ref(),pos_x,pos_y,depth);
			}
			// �F����
			if(y || u || v){
				ColorTuning(srcdata.ref(),y,u,v);
			}

			// COLOR::Convert�̓��S�f�[�^��new[]���ĕԂ�
			logodata = COLOR::Convert(srcdata.get(),lgh);
		}
		catch(const char *err){
			env->ThrowError("%s: %s",TYPE::Name(),err);
		}
	}
	~deLOGO(){}

	PVideoFrame __stdcall GetFrame(int n,IScriptEnvironment *env);

protected:
	/// ���S�f�[�^�t�@�C���ǂݍ���
	::LOGO_PIXEL *ReadLogoData(const char *logofile,const char *logoname)
	{
		class FILE_HANDLE{
			HANDLE h;
		public:
			FILE_HANDLE(void):h(INVALID_HANDLE_VALUE){}
			FILE_HANDLE(HANDLE _h):h(_h){}
			~FILE_HANDLE(){
				CloseHandle(h);
			}
			operator HANDLE(void){ return h; }
			operator bool(void){ return h!=INVALID_HANDLE_VALUE; }
			bool operator==(HANDLE hh){ return h==hh; }
			bool operator!=(HANDLE hh){ return h!=hh; }
		};

		if(logofile==NULL){
			throw "The logofile is not specified. - ���S�f�[�^�t�@�C�����w�肳��Ă��܂���";
		}

		// �t�@�C�����J��
		FILE_HANDLE hfile = CreateFile(logofile,GENERIC_READ,0,0,OPEN_EXISTING,
									   FILE_ATTRIBUTE_NORMAL,NULL);
		if(hfile==INVALID_HANDLE_VALUE){
			throw "The logofile is not found. - ���S�f�[�^�t�@�C����������܂���";
		}
		if(GetFileSize(hfile,NULL) < sizeof(LOGO_HEADER)+LOGO_FILE_HEADER_STR_SIZE){
			throw "The logo file is irregular. -  ���S�f�[�^�t�@�C�����s���ł�";
		}

		// ���S�f�[�^���擾
		LOGO_FILE_HEADER lfh;
		DWORD readed = 0;
		SetFilePointer(hfile,0,0,FILE_BEGIN);
		ReadFile(hfile,&lfh,sizeof(LOGO_FILE_HEADER),&readed,NULL);
		if(readed!=sizeof(LOGO_FILE_HEADER)){
			throw "Failed in reading logofile. - ���S�f�[�^�̓ǂݍ��݂Ɏ��s���܂���";
		}
		unsigned int num = SWAP_ENDIAN(lfh.logonum.l);

		// �Y�����S��T��
		int i;
		for(i=num;i;--i){
			// LOGO_HEADER�ǂݍ���
			readed = 0;
			ReadFile(hfile,&lgh,sizeof(LOGO_HEADER),&readed,NULL);
			if(readed!=sizeof(LOGO_HEADER)){
				throw "Failed in reading logofile. - ���S�f�[�^�̓ǂݍ��݂Ɏ��s���܂���";
			}

			//�w�胍�S����
			if(logoname==NULL || lstrcmp(logoname,lgh.name)==0){
				break;
			}

			// ����
			SetFilePointer(hfile,LOGO_PIXELSIZE(&lgh),0,FILE_CURRENT);
		}
		if(i==0){
			throw "The specified data is not found. - �w�肳�ꂽ���S�f�[�^��������܂���";
		}

		// �������m��
		LOGO_PIXEL *ptr = new ::LOGO_PIXEL[lgh.h * lgh.w];
		if(ptr==NULL){
			throw "Failed in memory allocation. - �������m�ۂɎ��s���܂���";
		}

		// �f�[�^�ǂݍ���
		memset(ptr,0,LOGO_PIXELSIZE(&lgh));
		ReadFile(hfile,ptr,LOGO_PIXELSIZE(&lgh),&readed,NULL);

		return ptr;
	}

	/// �ʒu�E�[�x����
	void AdjustLogo(aptr<::LOGO_PIXEL> &logodata,int x,int y,int depth)
	{

		int adjx,adjy;
		if(x>=0){
			lgh.x = lgh.x + int(x/4);
			adjx = x %4;
		}
		else{
			lgh.x = lgh.x + int((x-3)/4);
			adjx = (4+ (x%4)) %4;
		}
		if(y>=0){
			lgh.y = lgh.y + int(y/4);
			adjy = y % 4;
		}
		else{
			lgh.y = lgh.y + int((y-3)/4);
			adjy = (4+ (y%4)) %4;
		}

		if(depth==LOGO_DEFAULT_DEPTH && adjx==0 && adjy==0) return;

		int pitch = lgh.w;
		// 1/4�P�ʒ��߂̂��߁A1���₷
		int w = ++lgh.w;
		int h = ++lgh.h;

		aptr<::LOGO_PIXEL> dstdata;
		dstdata = new ::LOGO_PIXEL[(lgh.h+1) * (lgh.w+1)];
		if(dstdata==NULL){
			throw "Failed on memory allocation. - �������m�ۂɎ��s���܂���";
		}

		::LOGO_PIXEL *df = logodata.get();
		::LOGO_PIXEL *ex = dstdata.get();
		int i,j;

		//----------------------------------------------------- ��ԏ�̗�
		ex[0].dp_y  = df[0].dp_y *(4-adjx)*(4-adjy)*depth/128/16;	// ���[
		ex[0].dp_cb = df[0].dp_cb*(4-adjx)*(4-adjy)*depth/128/16;
		ex[0].dp_cr = df[0].dp_cr*(4-adjx)*(4-adjy)*depth/128/16;
		ex[0].y  = df[0].y;
		ex[0].cb = df[0].cb;
		ex[0].cr = df[0].cr;
#pragma parallel
		for(i=1;i<w-1;i++){									//��
			// Y
			ex[i].dp_y = (df[i-1].dp_y*adjx*(4-adjy)
						  + df[i].dp_y*(4-adjx)*(4-adjy)) *depth/128/16;
			if(ex[i].dp_y)
				ex[i].y  = (df[i-1].y*Abs(df[i-1].dp_y)*adjx*(4-adjy)
							+ df[i].y * Abs(df[i].dp_y)*(4-adjx)*(4-adjy))
					/(Abs(df[i-1].dp_y)*adjx*(4-adjy) + Abs(df[i].dp_y)*(4-adjx)*(4-adjy));
			// Cb
			ex[i].dp_cb = (df[i-1].dp_cb*adjx*(4-adjy)
						   + df[i].dp_cb*(4-adjx)*(4-adjy)) *depth/128/16;
			if(ex[i].dp_cb)
				ex[i].cb = (df[i-1].cb*Abs(df[i-1].dp_cb)*adjx*(4-adjy)
							+ df[i].cb * Abs(df[i].dp_cb)*(4-adjx)*(4-adjy))
					/ (Abs(df[i-1].dp_cb)*adjx*(4-adjy)+Abs(df[i].dp_cb)*(4-adjx)*(4-adjy));
			// Cr
			ex[i].dp_cr = (df[i-1].dp_cr*adjx*(4-adjy)
						   + df[i].dp_cr*(4-adjx)*(4-adjy)) *depth/128/16;
			if(ex[i].dp_cr)
				ex[i].cr = (df[i-1].cr*Abs(df[i-1].dp_cr)*adjx*(4-adjy)
							+ df[i].cr * Abs(df[i].dp_cr)*(4-adjx)*(4-adjy))
					/ (Abs(df[i-1].dp_cr)*adjx*(4-adjy)+Abs(df[i].dp_cr)*(4-adjx)*(4-adjy));
		}
		ex[i].dp_y  = df[i-1].dp_y * adjx *(4-adjy)*depth/128/16;	// �E�[
		ex[i].dp_cb = df[i-1].dp_cb* adjx *(4-adjy)*depth/128/16;
		ex[i].dp_cr = df[i-1].dp_cr* adjx *(4-adjy)*depth/128/16;
		ex[i].y  = df[i-1].y;
		ex[i].cb = df[i-1].cb;
		ex[i].cr = df[i-1].cr;

		//----------------------------------------------------------- ��
#pragma parallel
		for(j=1;j<h-1;j++){
			// �P�xY		//---------------------- ���[
			ex[j*w].dp_y = (df[(j-1)*pitch].dp_y*(4-adjx)*adjy
							+ df[j*pitch].dp_y*(4-adjx)*(4-adjy)) *depth/128/16;
			if(ex[j*w].dp_y)
				ex[j*w].y = (df[(j-1)*pitch].y*Abs(df[(j-1)*pitch].dp_y)*(4-adjx)*adjy
							 + df[j*pitch].y*Abs(df[j*pitch].dp_y)*(4-adjx)*(4-adjy))
					/ (Abs(df[(j-1)*pitch].dp_y)*(4-adjx)*adjy+Abs(df[j*pitch].dp_y)*(4-adjx)*(4-adjy));
			// �F��(��)Cb
			ex[j*w].dp_cb = (df[(j-1)*pitch].dp_cb*(4-adjx)*adjy
							 + df[j*pitch].dp_cb*(4-adjx)*(4-adjy)) *depth/128/ 16;
			if(ex[j*w].dp_cb)
				ex[j*w].cb = (df[(j-1)*pitch].cb*Abs(df[(j-1)*pitch].dp_cb)*(4-adjx)*adjy
							  + df[j*pitch].cb*Abs(df[j*pitch].dp_cb)*(4-adjx)*(4-adjy))
					/ (Abs(df[(j-1)*pitch].dp_cb)*(4-adjx)*adjy+Abs(df[j*pitch].dp_cb)*(4-adjx)*(4-adjy));
			// �F��(��)Cr
			ex[j*w].dp_cr = (df[(j-1)*pitch].dp_cr*(4-adjx)*adjy
							 + df[j*pitch].dp_cr*(4-adjx)*(4-adjy)) *depth/128/ 16;
			if(ex[j*w].dp_cr)
				ex[j*w].cr = (df[(j-1)*pitch].cr*Abs(df[(j-1)*pitch].dp_cr)*(4-adjx)*adjy
							  + df[j*pitch].cr*Abs(df[j*pitch].dp_cr)*(4-adjx)*(4-adjy))
					/ (Abs(df[(j-1)*pitch].dp_cr)*(4-adjx)*adjy+Abs(df[j*pitch].dp_cr)*(4-adjx)*(4-adjy));
			for(i=1;i<w-1;i++){	//------------------ ��
				// Y
				ex[j*w+i].dp_y = (df[(j-1)*pitch+i-1].dp_y*adjx*adjy
								  + df[(j-1)*pitch+i].dp_y*(4-adjx)*adjy
								  + df[j*pitch+i-1].dp_y*adjx*(4-adjy)
								  + df[j*pitch+i].dp_y*(4-adjx)*(4-adjy) ) *depth/128/16;
				if(ex[j*w+i].dp_y)
					ex[j*w+i].y = (df[(j-1)*pitch+i-1].y*Abs(df[(j-1)*pitch+i-1].dp_y)*adjx*adjy
								   + df[(j-1)*pitch+i].y*Abs(df[(j-1)*pitch+i].dp_y)*(4-adjx)*adjy
								   + df[j*pitch+i-1].y*Abs(df[j*pitch+i-1].dp_y)*adjx*(4-adjy)
								   + df[j*pitch+i].y*Abs(df[j*pitch+i].dp_y)*(4-adjx)*(4-adjy) )
						/ (Abs(df[(j-1)*pitch+i-1].dp_y)*adjx*adjy + Abs(df[(j-1)*pitch+i].dp_y)*(4-adjx)*adjy
						   + Abs(df[j*pitch+i-1].dp_y)*adjx*(4-adjy)+Abs(df[j*pitch+i].dp_y)*(4-adjx)*(4-adjy));
				// Cb
				ex[j*w+i].dp_cb = (df[(j-1)*pitch+i-1].dp_cb*adjx*adjy
								   + df[(j-1)*pitch+i].dp_cb*(4-adjx)*adjy
								   + df[j*pitch+i-1].dp_cb*adjx*(4-adjy)
								   + df[j*pitch+i].dp_cb*(4-adjx)*(4-adjy) ) *depth/128/16;
				if(ex[j*w+i].dp_cb)
					ex[j*w+i].cb = (df[(j-1)*pitch+i-1].cb*Abs(df[(j-1)*pitch+i-1].dp_cb)*adjx*adjy
									+ df[(j-1)*pitch+i].cb*Abs(df[(j-1)*pitch+i].dp_cb)*(4-adjx)*adjy
									+ df[j*pitch+i-1].cb*Abs(df[j*pitch+i-1].dp_cb)*adjx*(4-adjy)
									+ df[j*pitch+i].cb*Abs(df[j*pitch+i].dp_cb)*(4-adjx)*(4-adjy) )
						/ (Abs(df[(j-1)*pitch+i-1].dp_cb)*adjx*adjy + Abs(df[(j-1)*pitch+i].dp_cb)*(4-adjx)*adjy
						   + Abs(df[j*pitch+i-1].dp_cb)*adjx*(4-adjy) + Abs(df[j*pitch+i].dp_cb)*(4-adjx)*(4-adjy));
				// Cr
				ex[j*w+i].dp_cr = (df[(j-1)*pitch+i-1].dp_cr*adjx*adjy
								   + df[(j-1)*pitch+i].dp_cr*(4-adjx)*adjy
								   + df[j*pitch+i-1].dp_cr*adjx*(4-adjy)
								   + df[j*pitch+i].dp_cr*(4-adjx)*(4-adjy) ) *depth/128/16;
				if(ex[j*w+i].dp_cr)
					ex[j*w+i].cr = (df[(j-1)*pitch+i-1].cr*Abs(df[(j-1)*pitch+i-1].dp_cr)*adjx*adjy
									+ df[(j-1)*pitch+i].cr*Abs(df[(j-1)*pitch+i].dp_cr)*(4-adjx)*adjy
									+ df[j*pitch+i-1].cr*Abs(df[j*pitch+i-1].dp_cr)*adjx*(4-adjy)
									+ df[j*pitch+i].cr*Abs(df[j*pitch+i].dp_cr)*(4-adjx)*(4-adjy) )
						/ (Abs(df[(j-1)*pitch+i-1].dp_cr)*adjx*adjy +Abs(df[(j-1)*pitch+i].dp_cr)*(4-adjx)*adjy
						   + Abs(df[j*pitch+i-1].dp_cr)*adjx*(4-adjy)+Abs(df[j*pitch+i].dp_cr)*(4-adjx)*(4-adjy));
			}
			// Y		//----------------------- �E�[
			ex[j*w+i].dp_y = (df[(j-1)*pitch+i-1].dp_y*adjx*adjy
							  + df[j*pitch+i-1].dp_y*adjx*(4-adjy)) *depth/128/ 16;
			if(ex[j*w+i].dp_y)
				ex[j*w+i].y = (df[(j-1)*pitch+i-1].y*Abs(df[(j-1)*pitch+i-1].dp_y)*adjx*adjy
							   + df[j*pitch+i-1].y*Abs(df[j*pitch+i-1].dp_y)*adjx*(4-adjy))
					/ (Abs(df[(j-1)*pitch+i-1].dp_y)*adjx*adjy+Abs(df[j*pitch+i-1].dp_y)*adjx*(4-adjy));
			// Cb
			ex[j*w+i].dp_cb = (df[(j-1)*pitch+i-1].dp_cb*adjx*adjy
							   + df[j*pitch+i-1].dp_cb*adjx*(4-adjy)) *depth/128/ 16;
			if(ex[j*w+i].dp_cb)
				ex[j*w+i].cb = (df[(j-1)*pitch+i-1].cb*Abs(df[(j-1)*pitch+i-1].dp_cb)*adjx*adjy
								+ df[j*pitch+i-1].cb*Abs(df[j*pitch+i-1].dp_cb)*adjx*(4-adjy))
					/ (Abs(df[(j-1)*pitch+i-1].dp_cb)*adjx*adjy+Abs(df[j*pitch+i-1].dp_cb)*adjx*(4-adjy));
			// Cr
			ex[j*w+i].dp_cr = (df[(j-1)*pitch+i-1].dp_cr*adjx*adjy
							   + df[j*pitch+i-1].dp_cr*adjx*(4-adjy)) *depth/128/ 16;
			if(ex[j*w+i].dp_cr)
				ex[j*w+i].cr = (df[(j-1)*pitch+i-1].cr*Abs(df[(j-1)*pitch+i-1].dp_cr)*adjx*adjy
								+ df[j*pitch+i-1].cr*Abs(df[j*pitch+i-1].dp_cr)*adjx*(4-adjy))
					/ (Abs(df[(j-1)*pitch+i-1].dp_cr)*adjx*adjy+Abs(df[j*pitch+i-1].dp_cr)*adjx*(4-adjy));
		}
		//--------------------------------------------------------- ��ԉ�
		ex[j*w].dp_y  = df[(j-1)*pitch].dp_y *(4-adjx)*adjy *depth/128/16;	// ���[
		ex[j*w].dp_cb = df[(j-1)*pitch].dp_cb*(4-adjx)*adjy *depth/128/16;
		ex[j*w].dp_cr = df[(j-1)*pitch].dp_cr*(4-adjx)*adjy *depth/128/16;
		ex[j*w].y  = df[(j-1)*pitch].y;
		ex[j*w].cb = df[(j-1)*pitch].cb;
		ex[j*w].cr = df[(j-1)*pitch].cr;
#pragma parallel
		for(i=1;i<w-1;i++){		// ��
			// Y
			ex[j*w+i].dp_y = (df[(j-1)*pitch+i-1].dp_y * adjx * adjy
							  + df[(j-1)*pitch+i].dp_y * (4-adjx) *adjy) *depth/128/16;
			if(ex[j*w+i].dp_y)
				ex[j*w+i].y = (df[(j-1)*pitch+i-1].y*Abs(df[(j-1)*pitch+i-1].dp_y)*adjx*adjy
							   + df[(j-1)*pitch+i].y*Abs(df[(j-1)*pitch+i].dp_y)*(4-adjx)*adjy)
					/ (Abs(df[(j-1)*pitch+i-1].dp_y)*adjx*adjy +Abs(df[(j-1)*pitch+i].dp_y)*(4-adjx)*adjy);
			// Cb
			ex[j*w+i].dp_cb = (df[(j-1)*pitch+i-1].dp_cb * adjx * adjy
							   + df[(j-1)*pitch+i].dp_cb * (4-adjx) *adjy) *depth/128/16;
			if(ex[j*w+i].dp_cb)
				ex[j*w+i].cb = (df[(j-1)*pitch+i-1].cb*Abs(df[(j-1)*pitch+i-1].dp_cb)*adjx*adjy
								+ df[(j-1)*pitch+i].cb*Abs(df[(j-1)*pitch+i].dp_cb)*(4-adjx)*adjy )
					/ (Abs(df[(j-1)*pitch+i-1].dp_cb)*adjx*adjy +Abs(df[(j-1)*pitch+i].dp_cb)*(4-adjx)*adjy);
			// Cr
			ex[j*w+i].dp_cr = (df[(j-1)*pitch+i-1].dp_cr * adjx * adjy
							   + df[(j-1)*pitch+i].dp_cr * (4-adjx) *adjy) *depth/128/16;
			if(ex[j*w+i].dp_cr)
				ex[j*w+i].cr = (df[(j-1)*pitch+i-1].cr*Abs(df[(j-1)*pitch+i-1].dp_cr)*adjx*adjy
								+ df[(j-1)*pitch+i].cr*Abs(df[(j-1)*pitch+i].dp_cr)*(4-adjx)*adjy)
					/ (Abs(df[(j-1)*pitch+i-1].dp_cr)*adjx*adjy +Abs(df[(j-1)*pitch+i].dp_cr)*(4-adjx)*adjy);
		}
		ex[j*w+i].dp_y  = df[(j-1)*pitch+i-1].dp_y *adjx*adjy *depth/128/16;	// �E�[
		ex[j*w+i].dp_cb = df[(j-1)*pitch+i-1].dp_cb*adjx*adjy *depth/128/16;
		ex[j*w+i].dp_cr = df[(j-1)*pitch+i-1].dp_cr*adjx*adjy *depth/128/16;
		ex[j*w+i].y  = df[(j-1)*pitch+i-1].y;
		ex[j*w+i].cb = df[(j-1)*pitch+i-1].cb;
		ex[j*w+i].cr = df[(j-1)*pitch+i-1].cr;

		logodata = dstdata;	// logodata�J��, dstdata���������Ďc��
	}

	/// �F����
	void ColorTuning(aptr<::LOGO_PIXEL> &logodata,int y,int u,int v)
	{
		::LOGO_PIXEL* lgp = logodata.get();
		for(int i=lgh.h;i;--i){
			for(int j=lgh.w;j;--j){
				lgp->y  += y * 16;
				lgp->cb += u * 16;
				lgp->cr += v * 16;
				++lgp;
			}
		}
	}

	/// �t�F�[�h�ɂ��[�x�v�Z
	int CalcFade(int n)
	{
		if(n<start || (end<n && end>=start)){	// �͈͊O
			return 0;
		}
		if(n < start+fadein){			// �t�F�[�h�C��
			return ((n - start)*2 +1)*LOGO_FADE_MAX / (fadein *2);
		}
		else if(n > end-fadeout && end>=0){		// �t�F�[�h�A�E�g
			return ((end - n)*2 +1)*LOGO_FADE_MAX / (fadeout *2);
		}
		// �ʏ�
		return LOGO_FADE_MAX;
	}

	int Abs(int x){ return abs(x); }
	int Clamp(int n, int l, int h){	return min(max(n,l),h); }

	int YtoAUY(int y){ return ((y*1197)>>6) - 299; }
	int CtoAUC(int c){ return (c*4681 - 128*4681 +164)>>8; }
	int AUYtoY(int y){ return Clamp((y*219 + 383 + (16<<12))>>12,0,255);}
	int AUCtoC(int c){ return Clamp((c*7 + 2048*7 + 66 + (16<<7))>>7,0,255); }
};


#endif
