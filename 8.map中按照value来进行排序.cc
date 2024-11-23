

#include<iostream>
#include<vector>
#include<map>
#include<algorithm>

using namespace std;

// map按照value进行排序

class Mycompare{
    public:
    bool operator()(pair<string,int> &a,pair<string,int> &b){
        return a.second < b.second; // 从小到大排序
    }
};

// 主函数
int main(){
    map<string,int>m1;
    m1["hello"]=1;
    m1["world"]=6;
    m1["haha"]=2;
    m1["yejie"] = 4;
    // 输出暂时没排序的值
    cout<<"-------排序前------------"<<endl;
    for(auto &num : m1)
    {
        cout << num.first << " : " << num.second << endl;
    }
    // 排序过后
    // 将数据放在vector中
    vector<pair<string,int>>v(m1.begin(),m1.end());
    sort(v.begin(),v.end(),Mycompare());
    cout<<"-------排序后------------"<<endl;
    for(auto &num : v)
    {
        cout << num.first << " : " << num.second << endl;
    }
    return 0;
}
