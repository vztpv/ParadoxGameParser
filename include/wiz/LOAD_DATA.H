
#ifndef LOAD_DATA_H_INCLUDED
#define LOAD_DATA_H_INCLUDED


#include <vector>
#include <stack>
#include <fstream>
#include <string>
#include <utility>
#include <algorithm>
using namespace std;

#include <wiz/global.h>
#include <wiz/cpp_string.h>
#include <wiz/queues.h>
#include <wiz/stacks.h>


namespace wiz {
	class Token
	{
	public:
		string str;
		bool isComment;
	public:
		Token& operator=(const Token& token) {
			str = token.str;
			isComment = token.isComment;
			return *this;
		}
		void operator=(Token&& token) {
			str = move(token.str);
			isComment = token.isComment;
		}
		Token(Token&& token) : str(move(token.str)), isComment(token.isComment) { }
		Token(const Token& token) : str(token.str), isComment(token.isComment) { }
		explicit Token() : isComment(false) { }
		explicit Token(string&& str, bool isComment = false) : str(move(str)), isComment(isComment) { }
		explicit Token(const string& str, bool isComment = false) : str(str), isComment(isComment) { }
	};
}

#include <wiz/load_data_types.h>
#include <wiz/load_data_utility.h>
#include <wiz/load_data_reservers.h>

namespace wiz {
	namespace load_data {
		const string LEFT = "{";
		const string RIGHT = "}";
		const string EQ = "=";

		class LoadData
		{
		private:
			static bool isState0(const int state_reserve)
			{
				return 1 == state_reserve;
			}
			/// core
		public:
			template <class Reserver>
			static bool _LoadData(deque<Token>& strVec, Reserver& reserver, UserType& global) // first, strVec.empty() must be true!!
			{
				int state = 0;
				int braceNum = 0;
				long long state_reserve=0;
				vector< UserType* > nestedUT(1);
				string var1, var2, val;

				bool varOn = false;

				nestedUT[0] = &global;
				{
					reserver(strVec);

					while (strVec.empty())
					{
						reserver(strVec);
						if (
							strVec.empty() &&
							reserver.end()
							) {
							return false; // throw "Err nextToken does not exist"; // cf) or empty file or empty string!
						}
					}
				}

				
				while (false == strVec.empty()) {
					switch (state)
					{
					case 0:
						if (LEFT == Utility::Top(strVec, nestedUT[braceNum], reserver)) {
							state = 2;
						}
						else {
 							pair<bool, Token> bsPair = Utility::LookUp(strVec, nestedUT[braceNum], reserver);
							if (bsPair.first) {
								if (EQ == bsPair.second.str) {
									Utility::Pop(strVec, &var2, nestedUT[braceNum], reserver);
									Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver);
									state = 2;
								}
								else {
									if (Utility::Pop(strVec, &var1, nestedUT[braceNum], reserver)) {
										nestedUT[braceNum]->AddItem("", move(var1));
										state = 0;
									}
								}
							}
							else {
								if (Utility::Pop(strVec, &var1, nestedUT[braceNum], reserver)) {
									nestedUT[braceNum]->AddItem("", move(var1));
									state = 0;
								}
							}
						}
						break;
					case 1:
						if (RIGHT == Utility::Top(strVec, nestedUT[braceNum], reserver)) {
							Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver);
							state = 0;
						}
						else {
							// syntax error.
							throw "syntax error 1 ";
						}
						break;
					case 2:
						if (LEFT == Utility::Top(strVec, nestedUT[braceNum], reserver)) {
							Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver);

							///
							nestedUT[braceNum]->AddUserTypeItem(UserType(var2));
							UserType* pTemp;
							nestedUT[braceNum]->GetLastUserTypeItemRef(var2, pTemp);

							braceNum++;

							/// new nestedUT
							if (nestedUT.size() == braceNum) /// changed 2014.01.23..
								nestedUT.push_back(nullptr);

							/// initial new nestedUT.
							nestedUT[braceNum] = pTemp;
							///
							state = 3;
						}
						else {
							if (Utility::Pop(strVec, &val, nestedUT[braceNum], reserver)) {

								nestedUT[braceNum]->AddItem(move(var2), move(val));
								var2 = "";
								val = "";

								state = 0;
							}
						}
						break;
					case 3:
						if (RIGHT == Utility::Top(strVec, nestedUT[braceNum], reserver)) {
							Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver);

							nestedUT[braceNum] = nullptr;
							braceNum--;

							state = 0;
						}
						else {
							{
								/// uisng struct
								state_reserve++;
								state = 4;
							}
							//else
							{
								//	throw  "syntax error 2 ";
							}
						}
						break;
					case 4:
						if (LEFT == Utility::Top(strVec, nestedUT[braceNum], reserver)) {
							Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver);

							UserType temp("");

							nestedUT[braceNum]->AddUserTypeItem(temp);
							UserType* pTemp;
							nestedUT[braceNum]->GetLastUserTypeItemRef("", pTemp);

							braceNum++;

							/// new nestedUT
							if (nestedUT.size() == braceNum) /// changed 2014.01.23..
								nestedUT.push_back(nullptr);

							/// initial new nestedUT.
							nestedUT[braceNum] = pTemp;
							///
							//}

							state = 5;
						}
						else if (RIGHT == Utility::Top(strVec, nestedUT[braceNum], reserver)) {
							Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver);
							state = isState0(state_reserve) ? 0 : 4;
							state_reserve--;

							{
								nestedUT[braceNum] = nullptr;
								braceNum--;
							}
						}
						else {
							pair<bool, Token> bsPair = Utility::LookUp(strVec, nestedUT[braceNum], reserver);
							if (bsPair.first) {
								if (EQ == bsPair.second.str) {
									// var2
									Utility::Pop(strVec, &var2, nestedUT[braceNum], reserver);
									Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver); // pass EQ
									state = 6;
								}
								else {
									// var1
									if (Utility::Pop(strVec, &var1, nestedUT[braceNum], reserver)) {
										nestedUT[braceNum]->AddItem("", move(var1));
										var1 = "";

										state = 4;
									}
								}
							}
							else
							{
								// var1
								if (Utility::Pop(strVec, &var1, nestedUT[braceNum], reserver))
								{
									nestedUT[braceNum]->AddItem("", move(var1));
									var1 = "";

									state = 4;
								}
							}
						}
						break;
					case 5:
						if (RIGHT == Utility::Top(strVec, nestedUT[braceNum], reserver)) {
							Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver);

							//if (flag1 == 0) {
							nestedUT[braceNum] = nullptr;
							braceNum--;
							// }
							//
							state = 4;
						}

						else {
							int idx = -1;
							int num = -1;

							
							{
								/// uisng struct
								state_reserve++;
								state = 4;
							}
							//else
							{
								//	throw "syntax error 4  ";
							}
						}
						break;
					case 6:
						if (LEFT == Utility::Top(strVec, nestedUT[braceNum], reserver)) {
							Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver);

							///
							{
								nestedUT[braceNum]->AddUserTypeItem(UserType(var2));
								UserType* pTemp;
								nestedUT[braceNum]->GetLastUserTypeItemRef(var2, pTemp);
								var2 = "";
								braceNum++;

								/// new nestedUT
								if (nestedUT.size() == braceNum) /// changed 2014.01.23..
									nestedUT.push_back(nullptr);

								/// initial new nestedUT.
								nestedUT[braceNum] = pTemp;
							}
							///
							state = 7;
						}
						else {
							if (Utility::Pop(strVec, &val, nestedUT[braceNum], reserver)) {

								nestedUT[braceNum]->AddItem(move(var2), move(val));
								var2 = ""; val = "";
								if (strVec.empty())
								{
									//
								}
								else if (RIGHT == Utility::Top(strVec, nestedUT[braceNum], reserver)) {
									Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver);

									{
										state = isState0(state_reserve) ? 0 : 4;
										state_reserve--;

										{
											nestedUT[braceNum] = nullptr;
											braceNum--;
										}
									}
									{
										//state = 4;
									}
								}
								else {
									state = 4;
								}
							}
						}
						break;
					case 7:
						if (RIGHT == Utility::Top(strVec, nestedUT[braceNum], reserver)) {
							Utility::Pop(strVec, nullptr, nestedUT[braceNum], reserver);
							//

							nestedUT[braceNum] = nullptr;
							braceNum--;
							//
							state = 4;
						}
						else {
							int idx = -1;
							int num = -1;
							
							{
								/// uisng struct
								state_reserve++;

								state = 4;
							}
							//else
							{
								//throw "syntax error 5 ";
							}
						}
						break;
					default:
						// syntax err!!

						throw "syntax error 6 ";
						break;
					}

					if (strVec.size() < 10) {
						reserver(strVec);

						while (strVec.empty()) // (strVec.empty())
						{
							reserver(strVec);
							if (
								strVec.empty() &&
								reserver.end()
								) {
								// throw "Err nextToken does not exist2";
								break;
							}
						}
					}
				}
				if (state != 0) {
					throw string("error final state is not 0!  : ") + toStr(state);
				}
				if (braceNum != 0) {
					throw string("chk braceNum is ") + toStr(braceNum);
				}
				
				return true;
			}

		public:
			static bool LoadDataFromFile(const string& fileName, UserType& global) /// global should be empty
			{
				ifstream inFile;
				inFile.open(fileName);
				if (true == inFile.fail())
				{
					inFile.close(); return false;
				}
				UserType globalTemp = global;
				deque<Token> strVec;

				try {
					InFileReserver ifReserver(inFile);

					ifReserver.Num = 100000;
					// cf) empty file..
					if (false == _LoadData(strVec, ifReserver, globalTemp))
					{
						return true;
					}

					inFile.close();
				}
				catch (Error e) { std::cout << e << endl; inFile.close(); return false; }
				catch (const char* err) { std::cout << err << endl; inFile.close(); return false; }
				catch (const string& e) { std::cout << e << endl; inFile.close(); return false; }
				catch (exception e) { std::cout << e.what() << endl; inFile.close(); return false; }
				catch (...) { std::cout << "not expected error" << endl; inFile.close(); return false; }

				global = move(globalTemp);
				return true;
			}

			static bool LoadDataFromString(const string& str, UserType& ut)
			{
				UserType utTemp = ut;
				deque<Token> strVec;

				string statement = str;
				int token_first = 0, token_last = 0; // idx of token in statement.
				int state = 0;


				for (int i = 0; i < statement.size(); ++i) {
					if (0 == state && '\"' == statement[i]) {
						token_last = i - 1;
						if (token_last >= 0 && token_last - token_first + 1 > 0) {
							strVec.emplace_back(statement.substr(token_first, token_last - token_first + 1));
						}
						state = 1;
						token_first = i; token_last = i;
					}
					else if (1 == state && '\"' == statement[i]) {
						state = 0; token_last = i;

						strVec.emplace_back(statement.substr(token_first, token_last - token_first + 1));
						token_first = i + 1;
					}

					if (0 == state && '=' == statement[i]) {
						token_last = i - 1;
						if (token_last >= 0 && token_last - token_first + 1 > 0) {
							strVec.emplace_back(statement.substr(token_first, token_last - token_first + 1));
						}
						strVec.emplace_back(("="));
						token_first = i + 1;
					}
					else if (0 == state && isWhitespace(statement[i])) { // isspace ' ' \t \r \n , etc... ?
						token_last = i - 1;
						if (token_last >= 0 && token_last - token_first + 1 > 0) {
							strVec.emplace_back(statement.substr(token_first, token_last - token_first + 1));
						}
						token_first = i + 1;
					}
					else if (0 == state && '{' == statement[i]) {
						token_last = i - 1;
						if (token_last >= 0 && token_last - token_first + 1 > 0) {
							strVec.emplace_back(statement.substr(token_first, token_last - token_first + 1));
						}
						strVec.emplace_back(("{"));
						token_first = i + 1;
					}
					else if (0 == state && '}' == statement[i]) {
						token_last = i - 1;
						if (token_last >= 0 && token_last - token_first + 1 > 0) {
							strVec.emplace_back(statement.substr(token_first, token_last - token_first + 1));
						}
						strVec.emplace_back(("}"));
						token_first = i + 1;
					}

					if (0 == state && '#' == statement[i]) { // different from load_data_from_file
						token_last = i - 1;
						if (token_last >= 0 && token_last - token_first + 1 > 0) {
							strVec.emplace_back((statement.substr(token_first, token_last - token_first + 1)));
						}
						int j = 0;
						for (j = i; j < statement.size(); ++j) {
							if (statement[j] == '\n') // cf) '\r' ?
							{
								break;
							}
						}
						--j; // "before enter key" or "before end"

						if (j - i + 1 > 0) {
							strVec.emplace_back(statement.substr(i, j - i + 1), true);
						}
						token_first = j + 2;
						i = token_first - 1;
					}
				}

				if (token_first < statement.size())
				{
					strVec.emplace_back(statement.substr(token_first));
				}

				try {
					// empty string!
					NoneReserver nonReserver;
					if (false == _LoadData(strVec, nonReserver, utTemp))
					{
						return true;
					}
				}
				catch (Error& e) { std::cout << e << endl; return false; }
				catch (const char* err) { std::cout << err << endl; return false; }
				catch (exception& e) { std::cout << e.what() << endl; return false; }
				catch (string str) { std::cout << str << endl; return false; }
				catch (...) { std::cout << "not expected error" << endl; return  false; }

				ut = std::move(utTemp);
				return true;
			}

			// SaveQuery
			static bool SaveWizDB(const UserType& global, const string& fileName, const string& option = "1", const string& option2 = "") { /// , int option
				ofstream outFile;
				if (fileName.empty()) { return false; }
				if (option2 == "") {
					outFile.open(fileName);
					if (outFile.fail()) { return false; }
				}
				else {
					outFile.open(fileName, ios::app);
					if (outFile.fail()) { return false; }

					outFile << "\n";
				}

				/// saveFile
				if (option == "1") // for eu4.
					global.Save1(outFile); // cf) friend
				else if (option == "2")
					global.Save2(outFile);

				outFile.close();

				return true;
			}
		};
	}
}


#endif // LOAD_DATA_H_INCLUDED
