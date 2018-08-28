//程序5.17
#include <iostream>

int main()
{
    using namespace std;
    char ch;
    int count = 0;
    cin.get(ch);
    cout<<"Enter characters, enter # to quit:"<<endl;
    while(ch!='#')
    {
        cout<<ch;
        count++;
        cin.get(ch);
    }
    cout<<endl<<"count number:"<<count<<endl;
    return 0;
}