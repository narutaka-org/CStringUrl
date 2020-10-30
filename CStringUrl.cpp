#include "CStringUrl.h"

void CStringUrl::SplitUrL(CString strURL, CString& strProt, CString& strServer, CString& strPath)
{
	CStringW strTemp = strURL;
	int nProt = strTemp.Find(_T("://"));
	if (nProt >= 0)
	{
		strProt = strTemp.Mid(0, nProt);
		strTemp = strTemp.Mid(nProt + 3);
	}
	int nServer = strTemp.Find(_T("/"));
	if (nServer >= 0)
	{
		strServer = strTemp.Mid(0, nServer);
		strPath = strTemp.Mid(nServer);
	}	else {
		strServer = strTemp;
		strPath = _T("");
	}
}

CStringW CStringUrl::UrlEncode(CStringW strValue)
{
	const int nSize = UrlEncode(strValue, 0);
	LPWSTR lpszEncodeText = (LPWSTR)GlobalAlloc(0, sizeof(WCHAR) * nSize);
	UrlEncode(strValue, lpszEncodeText);
	CStringW strRent = lpszEncodeText;
	GlobalFree(lpszEncodeText);
	return strRent;
}

CStringW CStringUrl::UrlDecode(CStringW strValue)
{
	const int nSize = UrlDecode(strValue, 0);
	LPWSTR lpszDecodeText = (LPWSTR)GlobalAlloc(0, sizeof(WCHAR) * nSize);
	UrlDecode(strValue, lpszDecodeText);
	CStringW strRent = lpszDecodeText;
	GlobalFree(lpszDecodeText);
	return strRent;
}

int CStringUrl::UrlEncode(LPCWSTR lpszSrc, LPWSTR lpszDst)
{
	DWORD iDst = 0;
	const DWORD dwTextLengthA = WideCharToMultiByte(CP_UTF8, 0, lpszSrc, -1, 0, 0, 0, 0);
	LPSTR szUTF8TextA = (LPSTR)GlobalAlloc(GMEM_FIXED, dwTextLengthA);
	if (szUTF8TextA)
	{
		if (WideCharToMultiByte(CP_UTF8, 0, lpszSrc, -1, szUTF8TextA, dwTextLengthA, 0, 0))
		{
			for (DWORD iSrc = 0; iSrc < dwTextLengthA && szUTF8TextA[iSrc] != '\0'; ++iSrc)
			{
				LPCSTR lpszUnreservedCharacters = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-._~";
				if (StrChrA(lpszUnreservedCharacters, szUTF8TextA[iSrc]))
				{
					if (lpszDst) lpszDst[iDst] = (WCHAR)szUTF8TextA[iSrc];
					++iDst;
				}
				else if (szUTF8TextA[iSrc] == ' ')
				{
					if (lpszDst) lpszDst[iDst] = L'+';
					++iDst;
				}
				else
				{
					if (lpszDst) wsprintfW(&lpszDst[iDst], L"%%%02X", szUTF8TextA[iSrc] & 0xFF);
					iDst += 3;
				}
			}
			if (lpszDst) lpszDst[iDst] = L'\0';
			++iDst;
		}
		GlobalFree(szUTF8TextA);
	}
	return iDst;
}

int CStringUrl::UrlDecode(LPCWSTR lpszSrc, LPWSTR lpszDst)
{
	const int nSrcLength = lstrlenW(lpszSrc);
	int iSrc, iBuf;
	for (iSrc = 0, iBuf = 0; iSrc < nSrcLength; ++iSrc)
	{
		if (lpszSrc[iSrc] == L'%' && isxdigit(lpszSrc[iSrc + 1]))
		{
			iSrc += 2;
		}
		++iBuf;
	}
	LPSTR szUTF8TextA = (LPSTR)GlobalAlloc(GMEM_FIXED, iBuf * 2 + 1);
	if (szUTF8TextA != NULL)
	{
		ZeroMemory(szUTF8TextA, iBuf + 1);
		for (iSrc = 0, iBuf = 0; iSrc < nSrcLength; ++iSrc)
		{
			if (lpszSrc[iSrc] == L'%' && isxdigit(lpszSrc[iSrc + 1]))
			{
				WCHAR num[3] = { lpszSrc[iSrc + 1], lpszSrc[iSrc + 2], L'\0' };
				szUTF8TextA[iBuf] = (CHAR)wcstol(num, 0, 16);
				iSrc += 2;
			}
			else if (lpszSrc[iSrc] == L'+')
			{
				szUTF8TextA[iBuf] = ' ';
			}
			else
			{
				szUTF8TextA[iBuf] = (CHAR)lpszSrc[iSrc];
			}
			++iBuf;
		}
		szUTF8TextA[iBuf] = '\0';
		const DWORD iDst = MultiByteToWideChar(CP_UTF8, 0, szUTF8TextA, -1, 0, 0);
		if (lpszDst)
		{
			MultiByteToWideChar(CP_UTF8, 0, szUTF8TextA, -1, lpszDst, iDst);
		}
		GlobalFree(szUTF8TextA);
		return iDst;
	}
	return -1;
}
