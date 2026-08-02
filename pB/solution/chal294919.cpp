#include <iostream>
#include <vector>
using namespace std;

char a[128], b[128];
char s[1000005];

int main()
{
	int i;
	scanf("%s%s%s", s, a, b);
	string ans;
	vector<bool> rem(128, false), rep(128, false);
	if (*a != '(')
	{
		for (i=0; a[i]; i++)
		{
			rem[a[i]] = true;
		}
	}
	if (*b != '(')
	{
		for (i=0; b[i]; i++)
		{
			rep[b[i]] = true;
		}
	}
	for (i=0; s[i]; i++)
	{
		if (!rem[s[i]])
		{
			if (rep[s[i]])
			{
				ans += string("Q") + s[i] + "Q";
			}
			else
			{
				ans += s[i];
			}
		}
	}
	puts(ans.c_str());
	return 0;
}