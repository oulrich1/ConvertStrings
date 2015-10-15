#include <afx.h>
//#include <afxwin.h>

#include <string>
#include <vector>

#include "StringConversion.h"



/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -

namespace StringConversion {

	CString chars2cstring(const char* str)
	{
		CString newStr = _T("");

		MultiByteToWideCharUTF8(str, newStr);

		return newStr;
	}

	/* Must be deleted */
	char* cstring2chars(CString str)
	{
		char* newStr = "";
		int _len = 0;
		if (!str.IsEmpty()) {
			_len = WideCharToMultiByte(CP_UTF8, 0, str,
				str.GetLength(), NULL, 0, NULL, NULL);
			newStr = new char[_len + 1];
			WideCharToMultiByte(CP_UTF8, 0, str, str.GetLength(),
				newStr, _len, NULL, NULL);

			newStr[_len] = 0; //Null terminator
		}
		return newStr;
	}


	/* String Helpers */
	void stdstring2cstring(std::string const& src, CString& dst)
	{
		dst = tstring2cstring(rawbytes2tstring(src.c_str()));
	}

	void cstring2stdstring(CString const& src, std::string& dst)
	{
		tstring2string(cstring2tstring(src), dst);
	}


	void string2tstring(const std::string& src, tstring& dst)
	{
#ifdef UNICODE
		dst = rawbytes2tstring(src.c_str());
#else
		dst = src;
#endif
	}

	void tstring2string(const tstring& src, std::string& dst)
	{
#ifdef UNICODE
		{
			char* newStr = "";
			int _len = 0;
			if (!src.empty()) {
				_len = WideCharToMultiByte(CP_UTF8, 0, src.c_str(),
					src.length(), NULL,
					0, NULL, NULL);
				newStr = new char[_len + 1];

				WideCharToMultiByte(CP_UTF8, 0, src.c_str(),
					src.length(), newStr,
					_len, NULL, NULL);

				newStr[_len] = 0; //Null terminator
			}
			dst = newStr;
			delete newStr;
		}
#else
		dst = src;
#endif
	}

	tstring cstring2tstring(CString const& src)
	{
		tstring dst;
#ifdef UNICODE
		dst = src; // hopefully this does a copy see wcscpy_s for c-style wide chars
#else
		cstring2stdstring(src, dst);
#endif
		return dst;
	}

	CString tstring2cstring(const tstring& src)
	{
		CString dst;
#ifdef UNICODE
		dst = CString(src.c_str());
#else
		stdstring2cstring(src, dst);
#endif
		return dst;
	}


	tstring rawbytes2tstring(const char* str) {
		CString text = _T("");
		MultiByteToWideCharUTF8(str, text);
		return cstring2tstring(text);
	}

	// takes src char array of UTF8 and converts it into a wide-char wstring and stores in dst.
	void MultiByteToWideCharUTF8_Tstring(const char* src, tstring& dst) {
		dst = rawbytes2tstring(src);
	}

	/* --------------------------------------------------------- */
	/* converts the src multibyte string WHICH IS ALREADY ENCODED
	as UTF8 to a UTF8 Widechar string in 'dst'
	@returns 'dst' the string to be converted
	@return int - the number of characters in dst.
	if returned value == 1 then dst is valid but
	no characters were copied.
	if returned value == 0 then it failed to copy
	any characters, dst is invalid
	if returned valud == -1 then src is 0 */
	int MultiByteToWideCharUTF8(const char* src, CString& dst)
	{
		if (!src) {
			return -1;
		}

		int tlen = MultiByteToWideChar(CP_UTF8, 0, src, -1, NULL, 0);
		if (tlen > 1) {
			MultiByteToWideChar(CP_UTF8, 0, src, -1, dst.GetBufferSetLength(tlen), tlen);

			// sets the length of the string. if no params are passed 
			// then strlen is used which expects a NULL terminator 
			dst.ReleaseBuffer();
		} else {
			dst = _T(""); /// return empty string
		}

		return tlen; // number of characters including the NULL terminator
	}

	CString Int2CString(int i)
	{
		CString str;
		str.Format(_T("%d"), i);
		return str;
	}
}



/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -


namespace StringHelpers {

	/// given:   "\\\\hey/\/hey\\/\hey\"  
	/// returns: "////hey///hey////hey/"
	std::string BackSlashesToForwardSlashes(std::string str)
	{
		const size_t end = std::string::npos;

		size_t pos = 0;

		/// find next '\'
		while (end != (pos = str.find('\\', pos))) {
			/// replace it with '/'
			str.replace(pos, 1, "/");

			/// update position in str
			pos++;
		}

		return str;
	}

	/// given: "///hey//there/"
	/// returns: "/hey/there/"
	std::string SquashForwardSlashes(std::string str)
	{
		const size_t end = std::string::npos;

		size_t pos = 0;

		/// find next '\'
		while (end != (pos = str.find('/', pos))) {
			/// update position in str
			pos++;
			/// if there are any other '\', remove it
			while (pos < str.length() && str[pos] == '/') {
				str.erase(pos, 1);	// erase that char
				pos++;				// next character
			}
		}

		return str;
	}

	std::string SquashSlashes(std::string str)
	{
		return 	SquashForwardSlashes(BackSlashesToForwardSlashes(str));
	}

	/// simply prepends and appends a _T("\") quote
	bool WrapQuotes(tstring& text)
	{
		text = _T('\"') + text + _T('\"');
		return true;
	}

	/// strips the beinning and ending quote only if both of them are there
	bool UnwrapQoutes(tstring& text)
	{
		bool bIsUnwrapped = false;
		const int text_length = text.length();
		if (text_length >= 2) {
			if (text[0] == _T('\"') && text[text_length - 1] == _T('\"')) {
				text = text.substr(1, text_length - 2);
				bIsUnwrapped = true;
			}
		}
		return bIsUnwrapped;
	}

	/* --------------------------------------------------------- */
	/* transforms pos from being count of non whitespace
	chars to count of characters to get to the pos number of
	non whitespace characters in a string ( 0 - indexed )
	on error returns -1 */
	int GetIndexPthAlphaNumeric(const CString& str, int p) {
		const int NOT_ENOUGH_CHARS = -1;

		if (p < 0 || p >= str.GetLength()) /// note enough chars
			return NOT_ENOUGH_CHARS;

		int newPos = -1;   // iterator over the str
		int numChars = -1; // keeps track of number of non whitespace
		int str_length = str.GetLength();

		// while count the nonewhitespace and new pos is not past last character
		while (numChars != p && newPos < str_length) {
			newPos++;    // because of 0 indexing, decided to start
			//  counting first to get out of -1 state (start state)
			// count non whitespace (whitespace includes ' ', '\n' and some others)
			if (!iswspace(str[newPos]))
				numChars++;
		}

		if (newPos >= str_length)
			return NOT_ENOUGH_CHARS;	/// NOT ENOUGH characters for there

		return newPos;
	}

	/* --------------------------------------------------------- */
	/* Get the p-th non whitespace character in the string		 */
	CString GetCharPthAlphaNumeric(const CString& str, int p) {
		int index = GetIndexPthAlphaNumeric(str, p);
		if (index < 0)
			return _T("");
		return str.GetAt(index);
	}

	/* General string helpers */
	CString RemoveCharAt(const CString& str, int idx) {
		if (idx < -1 || idx >= str.GetLength())
			return str;
		if (str.GetLength() <= 0)
			return str;
		if (idx == -1)
			idx = str.GetLength() - 1;
		return str.Mid(0, idx) + str.Mid(idx + 1, str.GetLength());
	}

	/// Removes char at idx_in_str from str if it is in delimiters. 
	/// Returns the new string. NOTE: idx_in_str must be >= 0
	CString RemoveCharAtIfEqualsAny(const CString& str,
		std::vector<CString> delimiters, int idx_in_str)
	{
		CString _str = str;
		if (idx_in_str >= 0 && idx_in_str < str.GetLength()) {
			CString target_char(str.GetAt(idx_in_str));
			// go through delimiters and determine if we should delete idx_in_str-th char
			for (UINT i = 0; i < delimiters.size(); ++i) {
				if (delimiters[i].Compare(target_char) == 0) {
					_str = RemoveCharAt(str, idx_in_str);
					break;
				}
			}
		}
		return _str;
	}

	/// For each character in str, if it equals delimiters then remove that char.. 
	/// returns the final string.
	CString RemoveCharsIfEqualsAny(const CString& str, std::vector<CString> delimiters)
	{
		CString _str = str;
		for (auto delimit : delimiters)
			_str.Remove(delimit.GetAt(0));
		return _str;
	}

	CString RemoveLastCharIfEqualsAny(const CString& str,
		std::vector<CString> delimiters)
	{
		if (str.GetLength() == 0)
			return str;
		return RemoveCharAtIfEqualsAny(str, delimiters, str.GetLength() - 1);
	}

	bool IsStrInList(const CString& s, std::vector<CString> list) {
		for (UINT i = 0; i < list.size(); ++i) {
			CString d = list[i];
			if (s.Compare(d) == 0) {
				return true;
			}
		}
		return false;
	}

	bool IsBothStrInList(const CString& s1, const CString& s2, std::vector<CString> list) {
		return IsStrInList(s1, list) && IsStrInList(s2, list);
	}

	/// Finds the last word in the path that is delimited by the end of 
	/// the string and the first delimiter that is found in the list
	CString RetrieveLastWord(const CString& sentence,
		std::vector<CString> delimiters)
	{
		CString last_word = _T("");
		for (UINT i = 0; i < delimiters.size(); ++i) {
			int idx_found = sentence.ReverseFind(delimiters[i].GetAt(0));
			if (idx_found != -1) {
				int idx_start_word = idx_found + 1;
				last_word = sentence.Mid(idx_start_word,
					sentence.GetLength() - idx_start_word);
				break;
			}
		}
		return last_word;
	}

	/* Returns the index in string where an item was found, first item found in delimiters. */
	int FindFirstOccurance(const CString& s, std::vector<CString> delimiters) {
		for (UINT i = 0; i < delimiters.size(); ++i) {
			int idx = s.Find(delimiters[i].GetAt(0));
			if (idx != -1) {
				return idx;
			}
		}
		return -1;
	}

	/* Returns the character in string that first matches the delimiters in order. 
	 If no string in s matches any in delimiters then empty string is returned */
	CString GetFirstOccurance(const CString& s, std::vector<CString> delimiters) {
		int idx = FindFirstOccurance(s, delimiters);
		if (idx != -1)
			return CString(s.GetAt(idx));
		return _T("");
	}

	/// End General String Helpers ///


	/// String Helpers that are more on the side of being Path related ///

	// returns true if query is the last folder/file at the end of the path string
	bool IsAtEndOfPath(const CString& _path, const CString& query, std::vector<CString> delimiters)
	{
		CString path = _path;
		path = path.Trim();
		path = RemoveLastCharIfEqualsAny(path, delimiters);

		/// Get the word from the end of the path
		CString last_word = RetrieveLastWord(path, delimiters);
		return last_word.Compare(query) == 0; // returns true if no differences
	}

	// given paths p1 and p2, find the first occuance of any delimiter in delimiters.. 
	// If delimiter doesnt exist in either p1 or p2 then "/" is used as a default delimiter
	CString GetDelimiterFromPaths(const CString& p1, const CString& p2, std::vector<CString> delimiters) {
		CString delimiter = GetFirstOccurance(p1, delimiters);
		if (delimiter.GetLength() == 0) {
			delimiter = GetFirstOccurance(p2, delimiters);
			if (delimiter.GetLength() == 0) {
				delimiter = _T("/"); // default path delimiter
			}
		}
		return delimiter;
	}

	CString JoinPaths(const CString& p1, const CString& p2, std::vector<CString> delimiters)
	{
		CString delimiter = GetDelimiterFromPaths(p1, p2, delimiters);
		return RemoveLastCharIfEqualsAny(p1, delimiters) + delimiter + RemoveCharAtIfEqualsAny(p2, delimiters, 0);
	}

	// appends word to end of path if it doesnt exist at the end of path
	// path = "my/path" or "my/path and word = "view" -> path == "my/path/view
	CString AppendWordIfNotAtEndOfPath(const CString& _path, const CString& word,
		std::vector<CString> delimiters)
	{
		// delimiters could be passed in with initializer list
		if (delimiters.size() == 0) {
			delimiters.push_back(_T("\\"));
			delimiters.push_back(_T("/"));
		}

		// word is stripped of any special path characters, since its a word in a path
		CString word_without_specials = RemoveCharsIfEqualsAny(word, delimiters);

		// if the word without special path chars is at the end of the path, then
		// nothing needs to be done here, return the original path
		if (IsAtEndOfPath(_path, word_without_specials, delimiters)) {
			return _path;
		}

		// otherwise join the path with the word and return it
		CString path = _path;
		path = path.Trim();
		path = JoinPaths(path, word_without_specials, delimiters);
		return path;
	}
	/// End Path String Helpers

} // end stringhelpers

/// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
