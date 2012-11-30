/*
** Haaf's Game Engine 1.5
** Copyright (C) 2003-2004, Relish Games
** hge.relishgames.com
**
** Core functions implementation: audio routines
*/


#include "hge_impl.h"


#define BASSDEF(f) (WINAPI *f)	// define the functions as pointers
#include "BASS\bass.h"

#define LOADBASSFUNCTION(f) *((void**)&f)=(void*)GetProcAddress(hBass,#f)


HEFFECT CALL HGE_Impl::Effect_Load(const char *filename, DWORD size)
{
	DWORD _size, length, samples;
	HSAMPLE hs;
	HSTREAM hstrm;
	BASS_CHANNELINFO info;
	void *buffer, *data;

	if(hBass)
	{
		if(bSilent) return 1;

		if(size) { data=(void *)filename; _size=size; }
		else
		{
			data=Resource_Load(filename, &_size);
			if(!data) return NULL;
		}
	
		hs=BASS_SampleLoad(TRUE, data, 0, _size, 4, BASS_SAMPLE_OVER_VOL);
		if(!hs) {
			hstrm=BASS_StreamCreateFile(TRUE, data, 0, _size, BASS_STREAM_DECODE);
			if(hstrm) {
				length=(DWORD)BASS_ChannelGetLength(hstrm);
				BASS_ChannelGetInfo(hstrm, &info);
				samples=length;
				if(info.chans < 2) samples>>=1;
				if(info.flags & BASS_SAMPLE_8BITS == 0) samples>>=1;
				buffer=BASS_SampleCreate(samples, info.freq, 2, 4, info.flags | BASS_SAMPLE_OVER_VOL);
				if(!buffer)
				{
					BASS_StreamFree(hstrm);
					_PostError("Can't create sound effect: Not enough memory");
				}
				else
				{
					BASS_ChannelGetData(hstrm, buffer, length);
					hs=BASS_SampleCreateDone();
					BASS_StreamFree(hstrm);
					if(!hs)	_PostError("Can't create sound effect");
				}
			}
		}

		if(!size) Resource_Free(data);
		return hs;
	}
	else return 0;
}

HCHANNEL CALL HGE_Impl::Effect_Play(HEFFECT eff)
{
	if(hBass)
	{
		HCHANNEL chn;
		chn=BASS_SampleGetChannel(eff, FALSE);
		BASS_ChannelPlay(chn, TRUE);
		return chn;
	}
	else return 0;
}

HCHANNEL CALL HGE_Impl::Effect_PlayEx(HEFFECT eff, int volume, int pan, float pitch, bool loop)
{
	if(hBass)
	{
		BASS_SAMPLE info;
		HCHANNEL chn;
		BASS_SampleGetInfo(eff, &info);
		chn=BASS_SampleGetChannel(eff, FALSE);
		BASS_ChannelSetAttributes(chn, (int)(pitch*info.freq), volume, pan);
		BASS_ChannelSetFlags(chn, loop ? BASS_SAMPLE_LOOP:0);
		BASS_ChannelPlay(chn, TRUE);
		return chn;
	}
	else return 0;
}

void CALL HGE_Impl::Effect_Free(HEFFECT eff)
{
	if(hBass) BASS_SampleFree(eff);
}

HMUSIC CALL HGE_Impl::Music_Load(const char *filename, DWORD size)
{
	void *data;
	DWORD _size;
	HMUSIC hm;

	if(hBass)
	{
		if(size) { data=(void *)filename; _size=size; }
		else
		{
			data=Resource_Load(filename, &_size);
			if(!data) return 0;
		}
		hm=BASS_MusicLoad(TRUE, data, 0, _size, (BASS_MUSIC_PRESCAN | BASS_MUSIC_POSRESET), 0);
		if(!hm)	_PostError("Can't load music");
		if(!size) Resource_Free(data);
		return hm;
	}
	else return 0;
}

HCHANNEL CALL HGE_Impl::Music_Play(HMUSIC mus, bool loop)
{
	if(hBass)
	{
		BASS_CHANNELINFO info;
		BASS_ChannelGetInfo(mus, &info);
		BASS_ChannelSetAttributes(mus, info.freq, 100, 0);
		BASS_ChannelSetFlags(mus, loop ? BASS_SAMPLE_LOOP:0);
		BASS_ChannelPlay(mus, TRUE);
		return mus;
	}
	else return 0;
}

void CALL HGE_Impl::Music_Free(HMUSIC mus)
{
	if(hBass) BASS_MusicFree(mus);
}

HSTREAM CALL HGE_Impl::Stream_Load(const char *filename, DWORD size)
{
	void *data;
	DWORD _size;
	HSTREAM hs;
	CStreamList *stmItem;

	if(hBass)
	{
		if(bSilent) return 1;

		if(size) { data=(void *)filename; _size=size; }
		else
		{
			data=Resource_Load(filename, &_size);
			if(!data) return 0;
		}
		hs=BASS_StreamCreateFile(TRUE, data, 0, _size, 0);
		if(!hs)
		{
			_PostError("Can't load stream");
			if(!size) Resource_Free(data);
			return 0;
		}
		if(!size)
		{
			stmItem=new CStreamList;
			stmItem->hstream=hs;
			stmItem->data=data;
			stmItem->next=streams;
			streams=stmItem;
		}
		return hs;
	}
	else return 0;
}

void CALL HGE_Impl::Stream_Free(HSTREAM stream)
{
	CStreamList *stmItem=streams, *stmPrev=0;

	if(hBass)
	{
		while(stmItem)
		{
			if(stmItem->hstream==stream)
			{
				if(stmPrev) stmPrev->next=stmItem->next;
				else streams=stmItem->next;
				Resource_Free(stmItem->data);
				delete stmItem;
				break;
			}
			stmPrev=stmItem;
			stmItem=stmItem->next;
		}
		BASS_StreamFree(stream);
	}
}

HCHANNEL CALL HGE_Impl::Stream_Play(HSTREAM stream, bool loop, int volume)
{
	if(hBass)
	{
		BASS_CHANNELINFO info;
		BASS_ChannelGetInfo(stream, &info);
		BASS_ChannelSetAttributes(stream, info.freq, volume, 0);
		BASS_ChannelSetFlags(stream, loop ? BASS_SAMPLE_LOOP:0);
		BASS_ChannelPlay(stream, TRUE);
		return stream;
	}
	else return 0;
}

void CALL HGE_Impl::Channel_SetPanning(HCHANNEL chn, int pan)
{
	if(hBass) BASS_ChannelSetAttributes(chn, -1, -1, pan);
}

void CALL HGE_Impl::Channel_SetVolume(HCHANNEL chn, int volume)
{
	if(hBass) BASS_ChannelSetAttributes(chn, -1, volume, -101);
}

void CALL HGE_Impl::Channel_SetPitch(HCHANNEL chn, float pitch)
{
	if(hBass)
	{
		BASS_CHANNELINFO info;
		BASS_ChannelGetInfo(chn, &info);
		BASS_ChannelSetAttributes(chn, (int)(pitch*info.freq), -1, -101);
	}
}

void CALL HGE_Impl::Channel_Pause(HCHANNEL chn)
{
	if(hBass) BASS_ChannelPause(chn);
}

void CALL HGE_Impl::Channel_Resume(HCHANNEL chn)
{
	if(hBass) BASS_ChannelPlay(chn, FALSE);
}

void CALL HGE_Impl::Channel_Stop(HCHANNEL chn)
{
	if(hBass) BASS_ChannelStop(chn);
}

void CALL HGE_Impl::Channel_StopAll()
{
	if(hBass) { BASS_Stop();BASS_Start(); }
}

bool CALL HGE_Impl::Channel_IsPlaying(HCHANNEL chn)
{
	if(hBass)
	{
		if(BASS_ChannelIsActive(chn)==BASS_ACTIVE_PLAYING) return true;
		else return false;
	}
	else return false;
}

float CALL HGE_Impl::Channel_GetLength(HCHANNEL chn) {
	if(hBass)
	{
		return BASS_ChannelBytes2Seconds(chn, BASS_ChannelGetLength(chn));
	}
	else return -1;
}

float CALL HGE_Impl::Channel_GetPos(HCHANNEL chn) {
	if(hBass)
	{
		return BASS_ChannelBytes2Seconds(chn, BASS_ChannelGetPosition(chn));
	}
	else return -1;
}

void CALL HGE_Impl::Channel_SetPos(HCHANNEL chn, float fSeconds) {
	if(hBass)
	{
		BASS_ChannelSetPosition(chn, BASS_ChannelSeconds2Bytes(chn, fSeconds));
	}
}


//////// Implementation ////////


bool HGE_Impl::_SoundInit()
{
	if(!bUseSound || hBass) return true;

	hBass=LoadLibrary("bass.dll");
	if (!hBass)
	{
		_PostError("Can't load BASS.DLL");
		return false;
	}

	LOADBASSFUNCTION(BASS_GetVersion);

	if (BASS_GetVersion()!=MAKELONG(2,2))
	{
		_PostError("Incorrect BASS.DLL version");
		return false;
	}

	LOADBASSFUNCTION(BASS_GetDeviceDescription);
	LOADBASSFUNCTION(BASS_Init);
	LOADBASSFUNCTION(BASS_Free);
	LOADBASSFUNCTION(BASS_Start);
	LOADBASSFUNCTION(BASS_Stop);
	LOADBASSFUNCTION(BASS_SetConfig);

	LOADBASSFUNCTION(BASS_SampleLoad);
	LOADBASSFUNCTION(BASS_SampleCreate);
	LOADBASSFUNCTION(BASS_SampleCreateDone);
	LOADBASSFUNCTION(BASS_SampleGetInfo);
	LOADBASSFUNCTION(BASS_SampleGetChannel);
	LOADBASSFUNCTION(BASS_SampleFree);
	
	LOADBASSFUNCTION(BASS_MusicLoad);
	LOADBASSFUNCTION(BASS_MusicFree);

	LOADBASSFUNCTION(BASS_StreamCreateFile);
	LOADBASSFUNCTION(BASS_StreamFree);
	
	LOADBASSFUNCTION(BASS_ChannelGetInfo);
	LOADBASSFUNCTION(BASS_ChannelGetAttributes);
	LOADBASSFUNCTION(BASS_ChannelSetAttributes);
	LOADBASSFUNCTION(BASS_ChannelSetFlags);
	LOADBASSFUNCTION(BASS_ChannelGetData);
	LOADBASSFUNCTION(BASS_ChannelPlay);
	LOADBASSFUNCTION(BASS_ChannelPause);
	LOADBASSFUNCTION(BASS_ChannelStop);
	LOADBASSFUNCTION(BASS_ChannelIsActive);
	LOADBASSFUNCTION(BASS_ChannelGetLength);
	LOADBASSFUNCTION(BASS_ChannelGetPosition);
	LOADBASSFUNCTION(BASS_ChannelSetPosition);
	LOADBASSFUNCTION(BASS_ChannelSeconds2Bytes);
	LOADBASSFUNCTION(BASS_ChannelBytes2Seconds);

	bSilent=false;
	if (!BASS_Init(1,nSampleRate,0,hwnd,NULL))
	{
		System_Log("BASS Init failed, using no sound");
		BASS_Init(0,nSampleRate,0,hwnd,NULL);
		bSilent=true;
	}
	else
	{
		System_Log("Sound Device: %s",BASS_GetDeviceDescription(1));
		System_Log("Sample rate: %ld\n", nSampleRate);
	}

	_SetFXVolume(nFXVolume);
	_SetMusVolume(nMusVolume);

	return true;
}

void HGE_Impl::_SoundDone()
{
	CStreamList *stmItem=streams, *stmNext;
	
	if(hBass)
	{
		BASS_Stop();
		BASS_Free();
		FreeLibrary(hBass);
		hBass=0;

		while(stmItem)
		{
			stmNext=stmItem->next;
			Resource_Free(stmItem->data);
			delete stmItem;
			stmItem=stmNext;
		}
		streams=0;
	}
}

void HGE_Impl::_SetMusVolume(int vol)
{
	if(hBass) BASS_SetConfig(BASS_CONFIG_GVOL_MUSIC, vol);
}

void HGE_Impl::_SetFXVolume(int vol)
{
	if(hBass) BASS_SetConfig(BASS_CONFIG_GVOL_SAMPLE, vol);
}






