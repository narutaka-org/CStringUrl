#pragma once

class CStringUtil
{
public:
	void SplitUrL(CString strURL, CString& strProt, CString& strServer, CString& strPath);
	CStringW UrlEncode(CStringW strValue);
	CStringW UrlDecode(CStringW strValue);
protected:
	int UrlEncode(LPCWSTR lpszSrc, LPWSTR lpszDst);
	int UrlDecode(LPCWSTR lpszSrc, LPWSTR lpszDst);
};
