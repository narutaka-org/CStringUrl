#pragma once

class CStringUtil
{
public:
  BOOL SplitUrL(CString strURL, CString& strProt, CString& strServer, CString& strPath);
	CString UrlEncode(CString strValue);
	CString UrlDecode(CString strValue);
protected:
	int UrlEncode(LPCWSTR lpszSrc, LPWSTR lpszDst);
	int UrlDecode(LPCWSTR lpszSrc, LPWSTR lpszDst);
};
