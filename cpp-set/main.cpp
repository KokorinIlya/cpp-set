#include "set.h"
#include <string>

using namespace std;

int main()
{
	set <int> mySet;
	while (true)
	{
		string s;
		cin >> s;
		if (s == "insert")
		{
			int x;
			cin >> x;
			mySet.insert(x);
		}
		else if (s == "find")
		{	
			int x;
			cin >> x;
			for (auto res = mySet.find(x); res != mySet.end(); ++res)
			{
				cout << *(res) << " ";
			}
			cout << endl;
		}
		else if (s == "delete")
		{
			int x;
			cin >> x;
			auto res = mySet.erase(mySet.find(x));
			for (; res != mySet.end(); ++res)
			{
				cout << *(res) << " ";
			}
			cout << endl;
		}
		else if (s == "lower")
		{
			int x;
			cin >> x;
			auto res = mySet.lower_bound(x);
			for (; res != mySet.end(); ++res)
			{
				cout << *(res) << " ";
			}
			cout << endl;
		}
		else if (s == "upper")
		{
			int x;
			cin >> x;
			auto res = mySet.upper_bound(x);
			for (; res != mySet.end(); ++res)
			{
				cout << *(res) << " ";
			}
			cout << endl;
		}
		else if (s == "print")
		{
			for (auto i = mySet.begin(); i != mySet.end(); i++)
			{
				cout << *i << " ";
			}
			cout << endl;
		}
		else if (s == "reverse")
		{
			for (auto i = mySet.rbegin(); i != mySet.rend(); i++)
			{
				cout << *i << " ";
			}
			cout << endl;
		}
		else if (s == "exit")
		{
			break;
		}
	}
	system("pause");
	return 0;
}