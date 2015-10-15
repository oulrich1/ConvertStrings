#pragma once
#include <string>

/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

namespace StringConversion {

#ifndef tstring
#ifdef UNICODE
	typedef std::wstring tstring;
#else
	typedef std::string tstring;
#endif
#endif


	CString chars2cstring(const char* str);

	/// returns a new char array. MUST BE DELETED 
	char*   cstring2chars(CString str);

	/* converts a std::string into a CString */
	void stdstring2cstring(const std::string& src, CString& dst);

	/* converts a CString into a std::string */
	void cstring2stdstring(const CString& src, std::string& dst);

#ifndef STD2CSTR
#define STD2CSTR( stdstring )                                   \
        ([](const std::string& s) -> CString {                  \
            CString retval;                                     \
            stdstring2cstring( s, retval );      \
            return retval;                                      \
		        }(stdstring))
#endif

#ifndef CSTR2STD
#define CSTR2STD( cstring )                                     \
        ([](const CString& s) -> std::string {                  \
            std::string retval;                                 \
            cstring2stdstring( s, retval );      \
            return retval;                                      \
		        }( cstring ))
#endif


	void string2tstring(const std::string& src, tstring& dst);
	void tstring2string(const tstring& src, std::string& dst);

#ifndef STD2TSTR
#define STD2TSTR( stdstring )                                   \
        ([](const std::string& s) -> tstring {                  \
            tstring retval;                                     \
            string2tstring( s, retval );         \
            return retval;                                      \
		        }(stdstring))
#endif

#ifndef TSTR2STD
#define TSTR2STD( tstr )                                        \
        ([](const tstring& s) -> std::string {                  \
            std::string retval;                                 \
            tstring2string( s, retval );         \
            return retval;                                      \
		        }( tstr ))
#endif


	tstring cstring2tstring(const CString& src);
	CString tstring2cstring(const tstring& src);

#ifndef CSTR2TSTR
#define CSTR2TSTR( tstr )                                       \
        ([](const CString& s) -> tstring {						\
			return cstring2tstring(s);                          \
		        }( tstr ))
#endif

#ifndef TSTR2CSTR
#define TSTR2CSTR( tstr )                                       \
        ([](const tstring& s) -> CString {						\
			return tstring2cstring(s);                          \
		        }( tstr ))
#endif


	tstring rawbytes2tstring(const char* str);

	void MultiByteToWideCharUTF8_Tstring(const char* src, tstring& dst);

	/* converts the src multibyte string WHICH IS ALREADY ENCODED
	as UTF8 to a UTF8 Widechar string in 'dst'
	@returns 'dst' the string to be converted
	@return int - the number of characters in dst.
	returns 1: dst is valid but no characters were copied
	returns 0: failed to copy any characters.
	return -1: src is 0.. */
	int MultiByteToWideCharUTF8(const char* src, CString& dst);

	CString Int2CString(int i);

}



/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

namespace StringHelpers {
	
	using StringConversion::tstring;

	/// given:   "\\\\hey/\/hey\\/\hey\"  
	/// returns: "////hey///hey////hey/"
	std::string BackSlashesToForwardSlashes(std::string str);

	/// given: "///hey//there/"
	/// returns: "/hey/there/"
	std::string SquashForwardSlashes(std::string str);
	std::string SquashSlashes(std::string str);

	// Wraps the string with Quotes.
	bool WrapQuotes(tstring& text);

	// Strips the beginning and ending quotes, but only if both are in the string
	bool UnwrapQoutes(tstring& text);

	/* --------------------------------------------------------- */
	/* transforms pos from being count of non whitespace
	chars to count of characters to get to the pos number of
	non whitespace characters in a string ( 0 - indexed ) */
	int GetIndexPthAlphaNumeric(const CString& str, int p);

	/* --------------------------------------------------------- */
	/* Get the p-th non whitespace character in the string						 */
	CString GetCharPthAlphaNumeric(const CString& str, int p);


	/// Path-String related helpers.. 
	CString RemoveCharAt(const CString& str, int idx);

	/// Removes char at idx_in_str from str if it is in delimiters. 
	/// Returns the new string. NOTE: idx_in_str must be >= 0
	CString RemoveCharAtIfEqualsAny(const CString& str,
		std::vector<CString> delimiters, int idx_in_str = -1); 

	/// For each character in str, if it equals delimiters then remove that char.. 
	/// returns the final string.
	CString RemoveCharsIfEqualsAny(const CString& str, std::vector<CString> delimiters);

	/// Removes last character if equals any in delimiters
	CString RemoveLastCharIfEqualsAny(const CString& str,
		std::vector<CString> delimiters);

	// Returns the last "Word" in path. Word is delimited by any in delimiter
	CString RetrieveLastWord(const CString& path,
		std::vector<CString> delimiters);

	// returns true if is is in list
	bool IsStrInList(const CString& s, std::vector<CString> list);

	// returns true if s1 and s2 are in list
	bool IsBothStrInList(const CString& s1, const CString& s2, std::vector<CString> list);

	/* Returns the index in string where an item was found, first item found in delimiters. */
	int FindFirstOccurance(const CString& s, std::vector<CString> items);

	/* Returns the character in string that first matches the delimiters in order.
	If no string in s matches any in delimiters then empty string is returned */
	CString GetFirstOccurance(const CString& s, std::vector<CString> items);


	// --------------- Path specific string helpers --------------- // 
	bool IsAtEndOfPath(const CString& _path, const CString& query, std::vector<CString> delimiters = {_T("/")});

	// given paths p1 and p2, find the first occuance of any delimiter in delimiters.. 
	// If delimiter doesnt exist in either p1 or p2 then "/" is used as a default delimiter
	CString GetDelimiterFromPaths(const CString& p1, const CString& p2, std::vector<CString> delimiters = {_T("/")});

	// given two paths, joins on common delimiter or "/" and returns the joined string.
	CString JoinPaths(const CString& p1, const CString& p2, std::vector<CString> delimiters = {_T("/")});

	CString AppendWordIfNotAtEndOfPath(const CString& _path, const CString& word,
		std::vector<CString> delimiters = std::vector<CString>()); // 
}
