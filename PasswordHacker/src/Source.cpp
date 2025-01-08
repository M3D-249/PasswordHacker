#include <iostream>
#include <fstream>
#include <string>
#include <deque>
#include <functional>
#include <memory>
#include <filesystem>

static char symbols[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";

bool HackPassword(std::string pass, uint16_t min, uint16_t max)
{
	using std::string;

	std::deque<std::string*> words;
	time_t time0 = clock();

	for (char ch : symbols)
	{
		string* temp = new string;
		temp->operator+=(ch);
		words.push_back(temp);
	}

	while (words.size())
	{
		string* str = words.front();
		words.pop_front();

		if (str->size() >= min && str->size() <= max)
		{
			if ((*str) == pass)
			{
				std::clog << "Password hacked : " << (*str);
				std::cout << "\t Time taken : " << (double)(clock() - time0) / (double)CLOCKS_PER_SEC;

				for (string* s : words)
					delete s;

				return true;
			}
		}
		else if (str->size() > max)
		{
			std::clog << "Faild :(";

			for (string* s : words)
				delete s;

			return false;
		}

		for (char ch : symbols)
		{
			string* temp = new string((*str));
			(*temp) += ch;
			words.push_back(temp);
			//std::clog << (*temp) << '\n';
		}

		delete str;
	}

	return false;
}

bool HackPasswordOPT(std::string outFile, std::string pass, uint16_t min, uint16_t max)
{
	std::fstream fs;
	fs.open(outFile, std::ios::in | std::ios::out | std::ios::trunc);

	if (!fs.is_open() || fs.fail())
	{
		std::cerr << "Output file couldnt be open!";
		return false;
	}

	time_t time0 = clock();
	int64_t count = 0;
	std::string strbuff;
	int64_t gOffset = 0;
	
	fs.seekp(0, std::ios::end);
	for (int i = 0; i < 62; ++i)
	{
		fs << symbols[i] << '\n';
		++count;
	}
	fs << symbols[62];

	while (1)
	{
		try
		{	 
			fs.seekg(gOffset, std::ios::beg);
			std::getline(fs, strbuff);
			gOffset += strbuff.size() + 2;
			//std::clog << strbuff;
			//std::cin.get();
			
			if (strbuff == pass)
			{
				
				std::clog << "Password hacked : " << strbuff;
				std::cout << "\t Time taken : " << (double)(clock() - time0) / (double)CLOCKS_PER_SEC;
				return true;
			}

			fs.seekp(-1, std::ios::end);

			for (char ch : symbols)
			{
				fs << strbuff + std::string(1, ch) << '\n';
				++count;
			}
		}
		catch (const std::exception& e)
		{
			std::cerr << "Error Occured : " << e.what();
			fs.close();
			return false;
		}
	}
}

int main(int argc, char* argv[])
{
	std::string currentDir = std::filesystem::current_path().string();

	std::string pass;
	std::cout << "Enter password to hack :\n";

	while (getline(std::cin, pass))
	{
		HackPasswordOPT(currentDir+"/out.txt", pass, 2, 10);
		std::cout << "\n------------------------\n";
		std::cout << "Enter password to hack :\n";
	}
	return 0;
}