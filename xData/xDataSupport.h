#ifndef XDATASUPPORT
#define XDATASUPPORT

inline bool CheckXVersion()
{
	const wchar_t V[] = XDATA_VERSION;
	const wchar_t *S = GetXVersion();
	for (int i=0; V[i]!=0; i++)
		if (V[i]!=S[i]) return false;
	return true;
};

#ifdef WINCE
	inline unsigned __int32 ansitextlength(const char* Text, unsigned __int32 MaxLength=65536*8){
	unsigned __int32 s;
		for (s=0;s<MaxLength;s++)
			if (!Text[s])
				break;
		return s;
	}
#else
	inline unsigned __int32 ansitextlength(const char* Text, unsigned __int32 MaxLength=65536*8){
	unsigned __int32 s=0;
		if (Text)
		{
			__asm{
				push eax
				push ecx
				push edi
				mov al,0
				mov ecx,MaxLength
				inc ecx
				mov edi,Text
				repne scas al
				not ecx
				inc ecx
				add ecx,MaxLength
				mov s, ecx
				pop edi
				pop ecx
				mov eax,MaxLength
				inc eax
				cmp s,eax 
				jne NOIF 
				mov s,0 
			NOIF:
				pop eax
			}
		}
		return s;
	}
#endif


#ifdef WINCE
	inline unsigned __int32 unicodetextlength(const wchar_t* Text, unsigned __int32 MaxLength=65536){
	unsigned __int32 s;
		for (s=0;s<MaxLength;s++)
			if (!Text[s])
				break;
		return s;
	}
#else
	inline unsigned __int32 unicodetextlength(const wchar_t* Text, unsigned __int32 MaxLength=65536){
	unsigned __int32 s=0;
		if (Text)
		{
			__asm{
				push eax
				push ecx
				push edi
				mov ax,0
				mov ecx,MaxLength
				inc ecx
				mov edi,Text
				repne scas ax
				not ecx
				inc ecx
				add ecx,MaxLength
				mov s, ecx
				pop edi
				pop ecx
				mov eax,MaxLength 
				inc eax
				cmp s,eax 
				jne NOIF 
				mov s,0 
			NOIF:
				pop eax
			}
		}
		return s;
	}
#endif

	template <class TYPE>
	inline void textcopy(TYPE* Dest, const TYPE* Src, unsigned __int32 MaxLength=65536){
		if (!Src) return;
		unsigned __int32 s=0;
		while (*Src) {
			*(Dest++) = *(Src++);
			if (++s == MaxLength)
				break;
		};
		*Dest=0;
	}

#endif // XDATASUPPORT