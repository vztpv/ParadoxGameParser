
#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#include <wiz/LOAD_DATA.H>


int main(int argc, char* argv[])
{
	string fileName;


	if (argc == 1) {
		cout << "FileName: ";
		getline(cin, fileName);
	}
	else
	{
		fileName = string(argv[1]);
	}

	wiz::load_data::UserType global;
	
	try {

		wiz::load_data::LoadData::LoadDataFromFile(fileName, global);

		wiz::load_data::LoadData::SaveWizDB(global, "test.txt", "1");
		//_getch(); // pause..
	}
	catch (const char* str) {
		cout << str << endl;
	}
	catch (const string& str) {
		cout << str << endl;
	}
	catch (const wiz::Error& e) {
		cout << e << endl;
	}
	//catch (...) {
	//	cout << "Error.." << endl;
	//}

	return 0;
}
