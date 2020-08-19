#include "stdafx.h"
#include "xData.h"
#include "xDataBaseDll.h"
#include <string>
#include <vector>

#define XML_WAIT_BEGIN 1
#define XML_READ_NAME  2
#define XML_WAIT_TYPE_OR_VALUE  3
#define XML_READ_LONG_NAME  4
#define XML_READ_TYPE  5
#define XML_WAIT_END_HEADER  6 // Ожидание конца заголовка ('>' или "\>")
#define XML_PARSE_HEADER  7 // Обработка заголовка
#define XML_PARSE_BODY  8 
#define XML_READ_HEADER_VALUE 9
#define XML_READ_HEADER_LONG_VALUE 10
#define XML_WAIT_VALUE_OR_SUBXML_OR_ENDTAG 11 // Заголовок прочитан. Ожидается значение, субъобъект или завержающий тег
#define XML_READ_END_BODY 12
#define XML_READ_LONG_END_BODY 13
#define XML_PARSE_BODY_AS_VALUE 14

// Флаги
#define WAIT_NAME     0x100 // Ожидание имени при обработке заголовка
#define WAIT_OBJ_NAME 0x200 // Ожидание имени объекта при обработке заголовка
#define WAIT_TYPE     0x400 // Ожидание типа при обработке заголовка (ожидание символа ':')
#define WAIT_VALUE    0x800 // Ожидание значения при обработке заголовка (ожидание символа '=')

#define SPACE(x) (x==' ' || x=='\n' || x==13 || x==10 || x==0x8)

xaData XML(char*& Xml, xData& ErrDescr, unsigned int &row, unsigned int &col)
{
char* str = Xml;
using namespace std;
string error;
string name;
string type;
string value;
unsigned __int8 datatype=DT_STRING;
xData res;
xData cur;
bool HexPresent=false;
xData add;

    int mode = XML_WAIT_BEGIN;
    do
    {
        if (*str==13){row++;col=1;}
        if (*str!=13 && *str!=10) col++;
        if (*str!=0 && (!SPACE(*str)) && *(unsigned __int8*)str<32) goto ERR;
        switch(mode & 0xFF)
        {
            case XML_WAIT_BEGIN:
                if (SPACE(*str))
                    break;
                if (*str=='<')
                {
                    mode=XML_PARSE_HEADER | WAIT_NAME | WAIT_TYPE | WAIT_VALUE | WAIT_OBJ_NAME;
                    break;
                }
                if (*str==0)
                    goto END;
                goto ERR;

            case XML_PARSE_BODY_AS_VALUE:
                if (*str=='<' && !value.empty())
                {
                    xData tp(value.data());
                    tp.ToNumber(datatype);
                    add = tp;
                    value.clear();
                    if (*str!='<')
                        break;
                }

                if (*str=='<')
                {
                    char* s = str+1;
                    while (SPACE(*s))
                        s++;
                    if (*s=='/')
                    {
                        if (datatype==DT_STRING && !value.empty())
                            add = value.data();

                        if (!name.empty()) add.SetName(name.data());
                        if (!cur.DataSize && (!cur.Named || name.empty()))
                            cur = add;
                        else
                            cur << add;
                        name.clear();
                        value.clear();
                        mode = XML_READ_END_BODY;
                        str=s;
                        break;
                    }
                }

                value += *str;
                break;

            case XML_PARSE_HEADER:
                if (*str==0)
                    goto ERR;
                if (SPACE(*str))
                    break;
                if (*str=='<')
                {
                    ErrDescr=xData(": Invalid start of new tag");
                    goto ERR;
                }
                if ((mode & WAIT_TYPE) && *str==':')
                {
                    mode = XML_READ_TYPE;
                    break;
                }
                if ((mode & WAIT_VALUE) && *str=='=')
                {
                    mode = XML_READ_HEADER_VALUE;
                    break;
                }
                if (mode & WAIT_NAME && *str=='"')
                {
                    mode = XML_READ_LONG_NAME | (mode & WAIT_OBJ_NAME);;
                    break;
                }
                if (*str=='>')
                {
                    if (!name.empty())
                    {
                        if (cur.Named)
                        {
                            xData d;
                            d.SetName(name.data());
                            cur << d;
                        } else 
                            cur.SetName(name.data());
                        name.clear();
                    }
                    mode = XML_PARSE_BODY;
                    break;
                }
                if (*str=='/' && *(str+1)=='>')
                {
                    if (!name.empty())
                    {
                        if (cur.Named)
                        {
                            xData d;
                            d.SetName(name.data());
                            cur << d;
                        } else 
                            cur.SetName(name.data());
                        name.clear();
                    }
                    str += 2;
                    res = cur;
                    goto END;
                }
                if (mode & WAIT_NAME)
                {
                    mode = XML_READ_NAME | (mode & WAIT_OBJ_NAME);
                    continue;
                }
                if (!name.empty() && (mode & (WAIT_TYPE|WAIT_VALUE)))
                {
                    xData d;
                    d.SetName(name.data());
                    cur << d;
                    name.clear();
                    mode = XML_READ_NAME;
                    continue;
                }
                break;

            case XML_READ_NAME:
                if (SPACE(*str) && name.empty())
                    break;
                if (SPACE(*str) || *str==':' || *str=='=' || *str=='/')
                {
                    if (mode & WAIT_OBJ_NAME)
                    {
                        mode=XML_PARSE_HEADER | WAIT_NAME | WAIT_TYPE | WAIT_VALUE;
                        cur.SetName(name.data());
                        name.clear();
                        continue;
                    }
                }
                if (name.empty() && *str=='"')
                {
                    mode=XML_READ_LONG_NAME;
                    break;
                }
                if (*str==':')
                {
                    mode=XML_READ_TYPE;
                    break;
                }
                if (*str=='=')
                {
                    mode=XML_READ_HEADER_VALUE;
                    break;
                }
                if (SPACE(*str) || *str=='>')
                {
                    mode=XML_PARSE_HEADER | WAIT_TYPE | WAIT_VALUE;
                    continue;
                }
                name+=*str;
                break;

            case XML_READ_LONG_NAME:
                if (*str==0)
                    goto ERR;
                if (*str=='"')
                {
                    if (mode & WAIT_OBJ_NAME)
                    {
                        mode=XML_PARSE_HEADER | WAIT_NAME | WAIT_TYPE | WAIT_VALUE;
                        cur.SetName(name.data());
                        name.clear();
                        break;
                    }
                    mode=XML_PARSE_HEADER | WAIT_TYPE | WAIT_VALUE;
                    break;
                }
                name+=*str;
                break;

            case XML_READ_TYPE:
                if (type.empty() && *str == ' ')
                    break;
                if (*str == ' ' || *str=='=' || *str=='>' || *str=='/')
                {
                    if (type=="B") datatype=DT_BOOL; else
                    if (type=="I8") datatype=DT_INT8; else
                    if (type=="I16") datatype=DT_INT16; else
                    if (type=="I32") datatype=DT_INT32; else
                    if (type=="I64") datatype=DT_INT64; else
                    if (type=="UI8") datatype=DT_UINT8; else
                    if (type=="UI16") datatype=DT_UINT16; else
                    if (type=="UI32") datatype=DT_UINT32; else
                    if (type=="UI64") datatype=DT_UINT64; else
                    if (type=="F") datatype=DT_FLOAT; else
                    if (type=="D") datatype=DT_DOUBLE; else
                        goto ERR;
                    type.clear();
                    mode=XML_PARSE_HEADER | WAIT_VALUE;
                    continue;
                }
                type += *str;
                break;

            case XML_READ_HEADER_VALUE:
                if (*str==0)
                    goto ERR;
                if (*str=='"')
                {
                    mode = XML_READ_HEADER_LONG_VALUE;
                    break;
                }
                if (value.empty() && *str == ' ')
                    break;
                if (*str == ' ' || *str == '>' || (*str == '/' && *(str+1) == '>'))
                {
                    if (value.empty())
                        goto ERR;
                    xData tp = value.data();
                    switch(datatype)
                    {
                        case DT_BOOL:   if (value.find("true")!=-1) add = true; else add = (tp.operator __int32()!=0);break;
                        case DT_INT8:   add = tp.operator __int8();break;
                        case DT_INT16:  add = (const __int16) tp;break;
                        case DT_INT32:  add = (const __int32) tp;break;
                        case DT_INT64:  add = (const __int64) tp;break;
                        case DT_UINT8:  add = tp.operator unsigned __int8();break;
                        case DT_UINT16: add = (const unsigned __int16) tp;break;
                        case DT_UINT32: add = (const unsigned __int32) tp;break;
                        case DT_UINT64: add = (const unsigned __int64) tp;break;
                        case DT_FLOAT:  add = (const float) tp;break;
                        case DT_DOUBLE: add = (const double) tp;break;
                        case DT_STRING: add = (const char*) tp;break;
                    }
                    if (!name.empty()) add.SetName(name.data());
                    if ((cur.Named && !name.empty()) || cur.DataSize>0)
                        cur << add;
                    else
                        cur = add;
                    value.clear();
                    name.clear();
                    datatype=DT_STRING;
                    if (SPACE(*str))
                    {
                        mode = mode=XML_PARSE_HEADER | WAIT_NAME | WAIT_TYPE | WAIT_VALUE;
                        break;
                    }
                    else
                        if (*str=='>')
                        {
                            mode = XML_PARSE_BODY;
                            break;
                        }
                        else
                            {
                                str += 2;
                                res = cur;
                                goto END;
                            }
                }
                value += *str;
                break;

            case XML_READ_HEADER_LONG_VALUE:
                if (*str==0)
                    goto ERR;
                if (*str == '"')
                {
                    xData tp = value.data();
                    tp.ToNumber(datatype);
                    add = tp;
                    //switch(datatype)
                    //{
                    //    case DT_BOOL:   if (value.find("true")!=-1) add = true; else add = (tp.operator __int32()!=0);break;
                    //    case DT_INT8:   add = tp.operator __int8();break;
                    //    case DT_INT16:  add = (const __int16) tp;break;
                    //    case DT_INT32:  add = (const __int32) tp;break;
                    //    case DT_INT64:  add = (const __int64) tp;break;
                    //    case DT_UINT8:  add = tp.operator unsigned __int8();break;
                    //    case DT_UINT16: add = (const unsigned __int16) tp;break;
                    //    case DT_UINT32: add = (const unsigned __int32) tp;break;
                    //    case DT_UINT64: add = (const unsigned __int64) tp;break;
                    //    case DT_FLOAT:  add = (const float) tp;break;
                    //    case DT_DOUBLE: add = (const double) tp;break;
                    //    case DT_STRING: add = (const char*) tp;break;
                    //}
                    if (!name.empty()) add.SetName(name.data());
                    if ((cur.Named && !name.empty()) || cur.DataSize>0)
                        cur << add;
                    else
                        cur = add;
                    value.clear();
                    name.clear();
                    datatype=DT_STRING;
                    mode = mode=XML_PARSE_HEADER | WAIT_NAME | WAIT_TYPE | WAIT_VALUE;
                    break;
                }
                value += *str;
                break;

            case XML_PARSE_BODY:
                if (*str == 0)
                    goto ERR;
                if (SPACE(*str))
                {
                    value += *str;
                    break;
                }
                if (*str == '<')
                {
                    char* s = str+1;
                    while (*s==' ')
                        s++;
                    if (*s=='/')
                    {
                        mode = XML_READ_END_BODY;
                        str = s;
                        break;
                    }
                    xaData r(XML(str,ErrDescr,row,col));
                    if (r.IsEmpty() && !r.Named) 
                        goto ERR;
//                    if (r.DataSize || r.DataType==DT_UNDEFINE)
                    cur <<r;// *(xData*)(r.DataValue+unsigned __int32(&r));
                    continue;
                }
                HexPresent = false;
                if (datatype!=DT_STRING)
                    value.clear();
                add.Invalidate(false,true,false);
                mode = XML_PARSE_BODY_AS_VALUE;
                continue;

            case XML_READ_END_BODY:
                if (*str == 0)
                    goto ERR;
                if (name.empty() && *str == ' ')
                    break;
                if (name.empty() && *str == '"')
                {
                    mode = XML_READ_LONG_END_BODY;
                    break;
                }
                if (SPACE(*str) || *str=='>')
                {
                    if (name=="") name.clear();
                    if ((cur.Named && name.empty()) || (!cur.Named && !name.empty()) || (!name.empty() && !cur.CompareWithName(name.data())))
                    {
                        ErrDescr=xData(": Begin tag '")+
                            xData(cur.GetName())+
                            xData("' not equal end tag '")+
                            name.data()+
                            xData("'");
                        goto ERR;
                    }
                    name.clear();
                    if (SPACE(*str))
                    {
                        char* s = str;
                        while (*s!='>') {
                            if (*s == 0)
                                goto ERR;
                            s++;
                        }
                        str = s;
                    }
                    str += 1;
                    res = cur;
                    goto END;
                }
                name += *str;
                break;

            case XML_READ_LONG_END_BODY:
                if (*str == 0)
                    goto ERR;
                if (*str=='"')
                {
                    if (cur.GetName() && cur.GetName()!=name)
                        goto ERR;
                    if (SPACE(*str))
                    {
                        char* s = str;
                        while (*s!='>') {
                            if (*s == 0)
                                goto ERR;
                            s++;
                        }
                        str = s;
                    }
                    str += 1;
                    res = cur;
                    goto END;
                }
                name += *str;
                break;

            case XML_WAIT_END_HEADER:
                break;

        }
        str++;
    } while (true);
END:
//    pos = int(str) - int(Xml);
    Xml = str;
    {
    //xData r = res.GetObjectDescriptor();
    //char* c = r;
    //c=c;
    }
    return res;
ERR:
    return xData();
}

bool xDataBaseDll::FromXml(char* Xml, xData& ErrDescr)
{
    char* x = Xml;
    unsigned int row=1,col=0;
    xData err;
    xaData r(XML(x,err,row,col));
    if (!err.IsEmpty())
    {
        char* c = err;
        if (&ErrDescr)
        {
            ErrDescr+=xData(" (row %v, col %v)");
            ErrDescr+=row;
            ErrDescr+=(col-1);
        }
        return false;
    }
    Invalidate();
    ((xData*)this)->MoveData(r);
    return true;
//    if (r.DataSize || r.DataType==DT_UNDEFINE)
//        *this = r;//*(xData*)(r.DataValue+unsigned __int32(&r));
}

//_CrtMemState s1, s2, s3;

XDATA_API void start()
{
//	_CrtSetReportMode( _CRT_WARN, _CRTDBG_MODE_DEBUG );
//	_CrtSetReportMode( _CRT_ERROR, _CRTDBG_MODE_DEBUG);
//	_CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_DEBUG);	
//	_CrtMemCheckpoint( &s1 );
}

XDATA_API void end()
{
//    _CrtMemCheckpoint( &s2 );
//	_CrtDumpMemoryLeaks();
//	if ( _CrtMemDifference( &s3, &s1, &s2 ) )
//		_CrtMemDumpStatistics( &s3 );	
}

XDATA_API bool ParseXmlFile(xData FileName, xData& RetData, xData& RetError)
{
    HANDLE hFile;
    xData error;
    RetData.Invalidate(false,true,true);
    hFile = CreateFile((char*)FileName,GENERIC_READ, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    unsigned __int32 FileSize;
    DWORD hr =  GetLastError();
    if (!hFile || hFile == INVALID_HANDLE_VALUE)
    {
        RetError = "Can't open file ";
        RetError += FileName;
        return false;
    }
    FileSize = GetFileSize(hFile,NULL);
    if (!FileSize)
    {
        RetError = "File ";
        RetError += FileName;
        RetError += "is empty";
        CloseHandle(hFile);
        return false;
    }

    char* buf = new char[FileSize+1];
    if (!buf)
    {
        RetError = "Can't allocate %i bytes in heap";
        RetError += FileSize;
        CloseHandle(hFile);
        delete buf;
        return false;
    }
    buf[FileSize]=0;

    DWORD read;
    if (!ReadFile(hFile, buf, FileSize,&read,NULL))
    {
        RetError = "Can't read data from ";
        RetError += FileName;
        CloseHandle(hFile);
        delete buf;
        return false;
    }

    CloseHandle(hFile);
    int i=FileSize-1;
    while ( i>=0 && SPACE(buf[i]) )
    {
        FileSize--;
        buf[FileSize]=0;
        i--;
    }
    // Удаление комментариев
    char* b = buf-1;
    bool remark=false;
    bool quote=false;
    while (*(++b))
    {
        if (*b==' ')
            continue;
        if (!remark && *b=='"')
        {
            quote=!quote;
            continue;
        }
        if (*b==10)
        {
            remark=false;
            continue;
        }
        if (*b==';' && !quote)
        {
            remark=true;
            if (*b!=13)
                *b=' ';
            continue;
        }
        if (remark && *b!=13)
            *b=' ';
    }


    b = buf;
    unsigned int row=1,col=0;

    xaData res(XML(b,error,row,col));
    if (!res.IsEmpty())
    {
        RetData = res;
    }
    else
    {
        RetError = error;
        RetError+=xData(" (row %v, col %v)");
        RetError+=row;
        RetError+=col;
        delete buf;
        return false;
    }

    //xData r = RetData.GetObjectDescriptor();
    //char* c = r;
    //c=c;
    delete buf;

    return true;
}

std::string SubHtml(const std::string& html, const xData& xml, const std::string& base)
{
	std::string ret;

	std::string word;
	std::string value;
	std::vector<std::string> tag;
	std::string body;
	std::string curtag;

	enum States{
		SearchBeginOfWord,
		SearchEndOfWord,
		SearchEqual,
		SearchValue,
		ParseWord,
		SearchValueQuote,
		SearchValueEnd,
		SearchBlockBody,
		SearchEndOfBlockBody
	} state = SearchBeginOfWord;
	char ch[2]={0};
	unsigned int index=0;
	bool FirstInBlock=false;
	int IndexBeginOfTag=0;

	while(index<html.size())
	{
				//xData d2 = xml.GetObjectDescriptor();
				//char* cc2 = d2;

		*ch = html[index];
		_strlwr_s(ch,2);
		if (*ch=='<' && (html.size()<=index+1 || html[index+1]!='/'))
		{
			IndexBeginOfTag = ret.size();
			FirstInBlock=true;
		}
		switch (state)
		{
		case SearchBeginOfWord:
			if (strchr("abcdefghijklmnopqrstuvwxyz",*ch))
			{
				word = ch;
				state = SearchEndOfWord;
			}
			break;
		case SearchEndOfWord:
			if (!strchr("abcdefghijklmnopqrstuvwxyz",*ch))
			{
				if (FirstInBlock)
					curtag = word;
				if (word.compare("xif")==0 || word.compare("xref")==0 || word.compare("xbranch")==0 || word.compare("xname")==0)
				{
					tag.push_back(curtag);
					state = SearchEqual;
					continue;
				}
				else
					state = SearchBeginOfWord;
				FirstInBlock=false;
			} else 
			{
				word += *ch;
			}
			break;
		case SearchEqual:
			if (*ch==' ')
				break;
			if (*ch=='=')
			{
				state = SearchValue;
				break;
			}
			state = ParseWord;
		case SearchValue:
			if (*ch==' ')
				break;
			if (*ch=='"')
			{
				state = SearchValueQuote;
				break;
			}
			state = SearchValueEnd;
			value = html[index];
			break;
		case SearchValueQuote:
			if (*ch=='"')
			{
				state = SearchBlockBody;
				break;
			}
			value += html[index];
			break;
		case SearchValueEnd:
			if (*ch==' ' || *ch=='/' || *ch=='>')
			{
				state = SearchBlockBody;
				continue;
			}
			value += html[index];
			break;
		case SearchBlockBody:
			if (*ch=='>')
			{
				state = SearchEndOfBlockBody;
				if (!tag.empty() && tag.back().compare(word)==0 && (word.compare("xif")==0 || word.compare("xref")==0 || word.compare("xbranch")==0 || word.compare("xname")==0))
				{
					ret.erase(IndexBeginOfTag,ret.size()-IndexBeginOfTag);
					index++;
					continue;
				}
			}
			if (*ch=='/' && html.size()>index+1 && html[index+1]=='>')
			{
				ret+="/>";
				index += 2;
				state = ParseWord;
				if (tag.back().compare(word)==0 && (word.compare("xif")==0 || word.compare("xref")==0 || word.compare("xbranch")==0 || word.compare("xname")==0))
					ret.erase(IndexBeginOfTag,ret.size()-IndexBeginOfTag);
				tag.pop_back();
			}
			break;
		case SearchEndOfBlockBody:
			std::string s;
			if (*ch=='<' && html.size()>index+1 && html[index+1]=='/')
			{
				if (!tag.empty())
				{
					std::string s;
					s = html.substr(index+2,tag.back().size());
					if (s.compare(tag.back())==0 && tag.size()==1)
					{
						state = ParseWord;
						if (tag.back().compare(word)==0 && (word.compare("xif")==0 || word.compare("xref")==0 || word.compare("xbranch")==0 || word.compare("xname")==0))
						{
							ret.erase(IndexBeginOfTag,ret.size()-IndexBeginOfTag);
							while (html[index]!='>') index++;
							index++;
						}
						tag.pop_back();
						break;
					}
					tag.pop_back();
				}
			} else
			if (*ch=='<')
			{
				unsigned int i=index+1;
				while (html[i]==' ') i++;
				while (html[i]!=' ' && html[i]!='=' && html[i]!='/' && html[i]!='>') s+=html[i++];
				while (html[i]==' ') i++;
				while (html[i]!='>') i++;
				if (html[i-1]!='/')
					tag.push_back(s);
			}
			body += html[index];
			index++;
			continue;
		}
		if (state == ParseWord)
		{
			if (word.compare("xif")==0)
			{
				body = SubHtml(body,xml,base);
				std::string elsestr;
				size_t i = body.find("<xelse/>");
				if (i!=std::string::npos)
				{
					elsestr = body.substr(i+8,body.size()-i-8);
					body = body.erase(i,body.size()-i);
				}
				std::string left,right;

				enum {
					act_none,
					act_equal,
					act_notequal,
					act_less,
					act_more,
					act_exist
				} action = act_exist;

				left = value;
				if ( (i = value.find("==")) != std::string::npos)
				{
					left = value.substr(0,i);
					right = value.substr(i+2,value.size()-i-2);
					action=act_equal;
				}
				if ( (i = value.find("!=")) != std::string::npos)
				{
					left = value.substr(0,i);
					right = value.substr(i+2,value.size()-i-2);
					action=act_notequal;
				}
				if ( (i = value.find("<")) != std::string::npos)
				{
					left = value.substr(0,i);
					right = value.substr(i+1,value.size()-i-1);
					action=act_less;
				}
				if ( (i = value.find(">")) != std::string::npos)
				{
					left = value.substr(0,i);
					right = value.substr(i+1,value.size()-i-1);
					action=act_more;
				}
				while(!left.empty() && (*left.begin())==' ') left.erase(left.begin());
				while(!left.empty() && (*(left.end()-1)==' ')) left.erase(left.end()-1);
				while(!right.empty() && (*right.begin())==' ') right.erase(left.begin());
				while(!right.empty() && (*(right.end()-1)==' ')) right.erase(left.end()-1);
				xpData leftxml,rightxml;
				if (left.size()>=2 && (*left.begin())=='[' && (*(left.end()-1))==']')
				{
					left.erase(left.begin());
					left.erase(left.end()-1);
					if (!left.empty() && left[0]=='.')
					{
						left.erase(0,1);
						left = base + left;
					}
					leftxml = xml[REF(left.data())];
				} else
					if (!left.empty())
						leftxml = left.data();
				if (right.size()>=2 && (*right.begin())=='[' && (*(right.end()-1))==']')
				{
					right.erase(right.begin());
					right.erase(right.end()-1);
					if (!right.empty() && right[0]=='.')
					{
						right.erase(0,1);
						right = base + right;
					}
					rightxml = xml[REF(right.data())];
				} else
					if (!right.empty())
						rightxml = right.data();

				bool result=false;
				// Сравнение
				switch (action)
				{
				case act_equal:
					result = leftxml == rightxml;
					break;
				case act_notequal:
					result = leftxml != rightxml;
					break;
				case act_exist:
					result = !leftxml.IsEmpty();
					break;
				}

				if (!result)
					body = SubHtml(elsestr,xml,base);
				ret += body;
//				xData dd = ret.GetObjectDescriptor();char*cc=dd;

				int a=0;
			}
			if (word.compare("xref")==0)
			{
				if (!value.empty() && value[0]=='.')
				{
					value.erase(0,1);
					value = base + value;
				}


				xaData r = xml[REF(value.data())];
				if (r.IsReference() && !r.GetReference()->IsEmpty())
				{
					ret += (char*)xData(r);	
				} else
				{
					ret += body;
				}
			}
			if (word.compare("xbranch")==0)
			{
				if (!value.empty() && value[0]=='.')
				{
					value.erase(0,1);
					value = base + value;
				}
				xaData r = xml[REF(value.data())];
				//xData d = r.GetObjectDescriptor();
				//char* cc = d;
				if (r.IsReference() && !r.GetReference()->IsEmpty())
				{
					unsigned int s = r.GetReference()->GetContainerSize();
					for (unsigned int i=0;i<s;i++)
					{
						ret += SubHtml(body,xml,value+'\\'+(char*)xData(i));	
					}
				}
			}
			if (word.compare("xname")==0)
			{
			}
//			state = SearchEndOfTag;
			word.clear();
			value.clear();
			body.clear();
			tag.clear();
			state = SearchBeginOfWord;
			continue;
		}
		ret += html[index];
		index++;
	}

	return ret;
}

XDATA_API bool CompileHtmlFile(xData InFileName, xData OutFileName, xData& Xml, xData& RetError)
{
	//xData d = Xml.GetObjectDescriptor();
	//char* cc = d;
	HANDLE InFile,OutFile;
	std::string instr,outstr;
	DWORD readed;

    InFile = CreateFile((char*)InFileName,GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD rh = GetLastError();
    if (!InFile || InFile == INVALID_HANDLE_VALUE)
    {
        RetError = "Can't open file ";
        RetError += InFileName;
        return false;
    }
	DWORD s = GetFileSize(InFile,0);
	instr.append(s,0);
	instr[0]='1';
	instr[1]='2';
	instr[2]='3';
	ReadFile(InFile,&instr[0],s,&readed,0);
	CloseHandle(InFile);
    if (!readed)
    {
        RetError = "Can't read file ";
        RetError += InFileName;
        return false;
    }
	std::string res = SubHtml(instr,Xml,"");

    OutFile = CreateFile((char*)OutFileName,GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (!OutFile || OutFile == INVALID_HANDLE_VALUE)
    {
        RetError = "Can't open file ";
        RetError += OutFileName;
        return false;
    }
	WriteFile(OutFile,res.data(),res.size(),&readed,0);
	CloseHandle(OutFile);

	return true;
}

XDATA_API xData CompileHtmlFile(xData InFileName, xData& Xml, xData& RetError)
{
	//xData d = Xml.GetObjectDescriptor();
	//char* cc = d;
	HANDLE InFile;
	std::string instr,outstr;
	DWORD readed;

    InFile = CreateFile((char*)InFileName,GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD rh = GetLastError();
    if (!InFile || InFile == INVALID_HANDLE_VALUE)
    {
        RetError = "Can't open file ";
        RetError += InFileName;
        return xData();
    }
	DWORD s = GetFileSize(InFile,0);
	instr.append(s,0);
	instr[0]='1';
	instr[1]='2';
	instr[2]='3';
	ReadFile(InFile,&instr[0],s,&readed,0);
	CloseHandle(InFile);
    if (!readed)
    {
        RetError = "Can't read file ";
        RetError += InFileName;
        return xData();
    }
	std::string res = SubHtml(instr,Xml,"");
	return res.data();
}
