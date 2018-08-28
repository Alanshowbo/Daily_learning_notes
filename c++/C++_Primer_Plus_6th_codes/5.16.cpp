//程序5.16
#include <iostream>

int main()
{
    using namespace std;
    char ch;
    int count = 0;
    cin>>ch;
    cout<<"Enter characters, enter # to quit:"<<endl;
    while(ch!='#')
    {
        cout<<ch;
        count++;
        cin>>ch;
    }
    cout<<endl<<"count number:"<<count<<endl;
	cin>>ch;
    return 0;
}