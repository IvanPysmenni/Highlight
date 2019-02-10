#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;


struct myRange
{
	size_t start = 0;
	size_t end = 0;

	myRange(size_t inStart, size_t inEnd) : start(inStart), end(inEnd) {}
};


// Generate vector of indexes which don't contain sybmols which must be ignore
const vector<int> GenerateVectorOfIndexes(const wstring& str, const wstring& ignoreSymbols)
{
	vector<int> res;
	for (size_t i = 0; i < str.size(); ++i)
	{
		if (ignoreSymbols.find(str[i]) == wstring::npos)
		{
			res.push_back(i);
		}
	}

	return res;
}

vector<myRange> getHighlight(wstring str, const wstring& pattern, const wstring& ignoreSymbols)
{
	const vector<int> arrRanges = GenerateVectorOfIndexes(str, ignoreSymbols);

	str.erase(
		remove_if(str.begin(), str.end(),
			[=](auto ch) {return ignoreSymbols.find(ch) != wstring::npos; }),
		str.end());

	vector<myRange> res;

	size_t found = 0;
	while (found < str.size() && ((found = str.find(pattern, found)) != string::npos))
	{
		res.push_back(myRange(arrRanges[found], arrRanges[found]));
		for (size_t i = 0; i < pattern.size() - 1; ++i)
		{
			if (arrRanges[found + (i + 1)] - arrRanges[found + i] > 1)
			{
				res.back().end = arrRanges[found + i];
				res.push_back(myRange(arrRanges[found + (i + 1)], arrRanges[found + (i + 1)]));

				continue;
			}
		}
		res.back().end = arrRanges[found + (pattern.size() - 1)];

		found += pattern.size();
	}

	return res;
}

void printResult(const vector<myRange>& inRes)
{
	for (size_t i = 0; i < inRes.size(); ++i)
	{
		wcout << "[" << inRes[i].start << ", " << inRes[i].end << "] ";
	}

	wcout << endl;
}


int main()
{
	wstring str = L"+49 (0) 56 56 5618";
	wstring searchPattern = L"565618";
	wstring ignoreSymbols = L" -()#,.";
	vector<myRange> res = getHighlight(str, searchPattern, ignoreSymbols);
	printResult(res);


	str = L"1111111111";
	searchPattern = L"1";
	ignoreSymbols = L" -()#,.";
	res = getHighlight(str, searchPattern, ignoreSymbols);
	printResult(res);


	str = L"111 11111 11";
	searchPattern = L"1";
	ignoreSymbols = L" -()#,.";
	res = getHighlight(str, searchPattern, ignoreSymbols);
	printResult(res);


	str = L"8991891";
	searchPattern = L"891";
	ignoreSymbols = L" -()#,.";
	res = getHighlight(str, searchPattern, ignoreSymbols);
	printResult(res);

	str = L"09340932";
	searchPattern = L"0932";
	ignoreSymbols = L" -()#,.";
	res = getHighlight(str, searchPattern, ignoreSymbols);
	printResult(res);

	str = L"0934093257";
	searchPattern = L"0932";
	ignoreSymbols = L" -()#,.";
	res = getHighlight(str, searchPattern, ignoreSymbols);
	printResult(res);

	str = L"+38 (0) 97 3545 123";
	searchPattern = L"+380973545123";
	ignoreSymbols = L" -()#,.";
	res = getHighlight(str, searchPattern, ignoreSymbols);
	printResult(res);

	str = L"+38\u00A0(0)\u00A097\u00A03545\u00A0123";
	searchPattern = L"+380973545123";
	ignoreSymbols = L" -()#,.\u00A0";
	res = getHighlight(str, searchPattern, ignoreSymbols);
	printResult(res);

	system("pause");
}
