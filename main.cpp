#include<bits/stdc++.h>

using namespace std;

//Node Structure
struct Node
{
    int num;
    Node * left;
    vector<int> right;
    Node(int n)
    {
        num=n;
        left=NULL;
    }
};

//Function to print data dependency between instruction
void data_depend(Node * root,vector<int> v, unordered_map<int,vector<int>>  & dd,int cond){

    if(root==NULL){
        return ;
    }
    else{
        v.push_back(root->num);
        vector<int> chk;
        chk=root->right;
        for(int i=0;i<chk.size();i++){
            for(int j=0;j<v.size();j++){
                if(cond==1)
                    cout<<"Instruction "<<chk[i]<<" is data dependent on Instruction "<<v[j]<<"\n";
                dd[v[j]].push_back(chk[i]);
            }
        }
       data_depend(root->left,v,dd,cond && 1);
    }
}



//Function to tokenizing the line/string
vector<string> token(string ln){

  vector<string> ans;
  stringstream ss(ln);
  string temp;
  ss>>temp;
  ans.push_back(temp);
  while(getline(ss,temp,',')){
    int pos=temp.find('(');
    if(pos!=-1){
        int end=temp.find(')');
        temp=temp.substr(pos+1,end-pos-1);
    }
    temp.erase(remove(temp.begin(), temp.end(), ' '), temp.end());
    ans.push_back(temp);
  }
  return ans;
}

//another function for tokenizing the line/string
vector<string> imp_token(string ln){

  vector<string> ans;
  stringstream ss(ln);
  string temp;
  ss>>temp;
  ans.push_back(temp);
  while(getline(ss,temp,',')){
        string a="";
    for(int i=0;i<temp.size();i++){
        if(temp[i]!=' ') a=a+temp[i];
    }
    ans.push_back(a);
  }
  return ans;
}


//This function will produce a unscheduled assembly code output file
string unsh(unordered_map<string,vector<pair<string,int>>> stall,unordered_map<int,vector<int>> dd,string path){

    ifstream fr;
    fr.open(path);
    string line;
    ofstream op;
    op.open("Unscheduled.txt");
    getline(fr,line);
    op<<line<<endl;
    int prev_ins=1;
    transform(line.begin(), line.end(), line.begin(), ::toupper);
    vector<string> ans;
    ans=token(line);
    string prev=ans[0];
    int curr_ins=0;
    string curr;
    while(fr){
        getline(fr,line);
        curr_ins=prev_ins+1;
        transform(line.begin(), line.end(), line.begin(), ::toupper);
        vector<string> g;
        g=token(line);
        curr=g[0];
        int dep=0;
        vector<int> test;
        if(dd.find(prev_ins)!=dd.end()){
            test=dd[prev_ins];
            for(int i=0;i<test.size();i++){
                if(test[i]==curr_ins){
                    dep=1;
                    break;
                }
            }
        }
        if(dep==1){
            int n=0;
            if(stall.find(prev)!=stall.end()){
                vector<pair<string,int>> src;
                src=stall[prev];
                for(int i=0;i<src.size();i++){
                    if(src[i].first==curr){
                        n=src[i].second;
                        break;
                    }
                }
            }
            while(n--){
                op<<"stall"<<endl;
            }
        }
        op<<line<<endl;
        prev=curr;
        prev_ins=curr_ins;
        if(fr.eof()) break;
    }
    return "Unscheduled.txt";
}

//This function will produce a unrolled unscheduled assembly code output file
string unroll(string path){

     vector<string> ins;
     vector<string> oth;
     ifstream fr;
     fr.open(path);
     string line;
     set<string> st;
     int chk=0;
     while(fr){
        getline(fr,line);
        if(line.empty()){
            continue;
        }
        transform(line.begin(), line.end(), line.begin(), ::toupper);
        vector<string> ans;
        ans=token(line);
        if(ans[0]=="ADD.I" || ans[0]=="BNE" || chk==1){
            oth.push_back(line);
            chk=1;
        }
        else{
            ins.push_back(line);
        }
        for(int i=0;i<ans.size();i++){
            if(ans[i][0]=='F'){
                st.insert(ans[i]);
            }
        }
        if(fr.eof()) break;
     }

     vector<string> vt;
     for(int i=0;i<33;i=i+2){
        vt.push_back("F"+to_string(i));
     }
     int len=st.size();
     int n=4;
     int curr=0;
     vector<string> v(st.begin(),st.end());
     sort(v.begin(),v.end());
     ofstream op;
     op.open("Unrolled_Unscheduled.txt");
     int reg=8;
     while(n--){
        reg=reg-8;
        unordered_map<string,string> mp;
        for(int i=0;i<v.size();i++){
            mp[v[i]]=vt[curr];
            curr++;
        }
        for(int i=0;i<ins.size();i++){
                if(ins[i]=="STALL"){
                    op<<"stall"<<endl;
                    continue;
                }
            vector<string> ans=imp_token(ins[i]);
            for(int j=0;j<ans.size();j++){
                if(ans[j][0]=='F'){
                    ans[j]=mp[ans[j]];
                }
                if(ans[j].find("(")!= string::npos){
                    string te=ans[j].substr(ans[j].find("("));
                    ans[j]=to_string(reg)+te;
                }
            }
            string res="";
            res=ans[0]+" ";
            res=res+ans[1];
            for(int j=2;j<ans.size();j++){
                res=res+",";
                res=res+ans[j];
            }
            op<<res<<endl;
        }

     }
     reg-=8;

     for(int i=0;i<oth.size();i++){
        if(oth[i]=="STALL"){
            op<<"stall"<<endl;
            continue;
        }
        vector<string> ans=imp_token(oth[i]);
        for(int j=0;j<ans.size();j++){
            if(ans[j].find("#")!= string::npos){
                ans[j]="#"+to_string(reg);
            }
        }

        string res="";
        res=ans[0]+" ";
        res=res+ans[1];
        for(int j=2;j<ans.size();j++){
            res=res+",";
            res=res+ans[j];
        }
        op<<res<<endl;
     }
     op.close();

     return "Unrolled_Unscheduled.txt";
}

//This Function will add stall between the instruction as per dependency and produce the output file
void temp_stall(vector<string> res,unordered_map<string,vector<pair<string,int>>> stall,
                unordered_map<int,vector<int>> dd,string path)
{

    string line;
    ofstream op;
    op.open(path);
    line=res[0];
    op<<line<<endl;
    int prev_ins=1;
    transform(line.begin(), line.end(), line.begin(), ::toupper);
    vector<string> ans;
    ans=token(line);
    string prev=ans[0];
    int curr_ins=0;
    string curr;
    for(int i=1;i<res.size();i++){
        line=res[i];
        curr_ins=prev_ins+1;
        transform(line.begin(), line.end(), line.begin(), ::toupper);
        vector<string> g;
        g=token(line);
        curr=g[0];
        int dep=0;
        vector<int> test;
        if(dd.find(prev_ins)!=dd.end()){
            test=dd[prev_ins];
            for(int i=0;i<test.size();i++){
                if(test[i]==curr_ins){
                    dep=1;
                    break;
                }
            }
        }
        if(dep==1){
            int n=0;
            if(stall.find(prev)!=stall.end()){
                vector<pair<string,int>> src;
                src=stall[prev];
                for(int i=0;i<src.size();i++){
                    if(src[i].first==curr){
                        n=src[i].second;
                        break;
                    }
                }
            }
            while(n--){
                op<<"stall"<<endl;
            }
        }
        op<<line<<endl;
        prev=curr;
        prev_ins=curr_ins;
    }
}

//This function will generate a scheduled output file for the given input file
string sched(string o_path,string d_path,unordered_map<string,vector<pair<string,int>>> stall){

    ifstream fr;
    fr.open("input.txt");
    string line;
    int count=0;
    unordered_map<int,vector<string>> mp;
    while(fr){
        getline(fr,line);
        if(line.empty()){
            continue;
        }
        transform(line.begin(), line.end(), line.begin(), ::toupper);
        vector<string> ans;
        ans=token(line);
        if(ans[0]!="ADD.I" && ans[0]!="BNE"){
            count++;
        }
        if(fr.eof()) break;
    }
    fr.close();
    fr.open(o_path);
    int i=0;
    vector<string> oth;
    while(fr){
        getline(fr,line);
        transform(line.begin(), line.end(), line.begin(), ::toupper);
        vector<string> ans;
        ans=token(line);
        if(ans[0]=="STALL" || ans[0]==""){
            continue;
        }
        else if(ans[0]=="ADD.I" || ans[0]=="BNE"){
            oth.push_back(line);
        }
        else{
            mp[i%count].push_back(line);
            i++;

        }
    }
    vector<string> res;

    for(int j=0;j<count;j++){
        for(int i=0;i<mp[j].size();i++){
                //cout<<mp[j][i]<<"\n";
                res.push_back(mp[j][i]);
        }
    }
    /*for(int i=0;i<oth.size();i++){
        res.push_back(oth[i]);
    }
    */
    for(int i=0;i<oth.size();i++){
        if(oth[i].find("ADD.I")!=string::npos){
              for(int j=res.size()-1;j>=0;j--){
                if(res[j].find("ADD.")!=string::npos){
                    res.insert(res.begin()+j+1,oth[i]);
                    break;
                }
              }
        }
        else{
            res.push_back(oth[i]);
        }
    }

    int l=res.size()-1;
    int t_sum=0;
    while(res[l].find("ADD.I")==string::npos){
        l--;
    }
    string s=res[l].substr(res[l].find("#")+1);
    t_sum=stoi(s);
    for(int h=l+1;h<res.size();h++){
        if(res[h].find("L.")!=string::npos || res[h].find("S.")!=string::npos){
            vector<string> ans=imp_token(res[h]);
            for(int j=0;j<ans.size();j++){
                if(ans[j].find("(")!= string::npos){
                    string ret=ans[j].substr(0,ans[j].find("("));
                    int num=stoi(ret);
                    num=num-t_sum;
                    ans[j]=to_string(num)+ans[j].substr(ans[j].find("("));
                }
            }
            string conv="";
            conv=ans[0]+" ";
            conv=conv+ans[1];
            for(int j=2;j<ans.size();j++){
                conv=conv+",";
                conv=conv+ans[j];
            }
            res[h]=conv;

        }
    }


    //cout<<"\n"<<mp.size()<<" "<<oth.size()<<" "<<res.size()<<"\n";
    unordered_map<string,pair<Node *, Node *>> vp;
    vector<pair<string,vector<string>>> store;
    for(int i=0;i<res.size();i++){
        string line=res[i];
        transform(line.begin(), line.end(), line.begin(), ::toupper);
        vector<string> ans;
        ans=token(line);
        if(ans[0]=="L.D"){
            store.push_back({ans[1],{ans[2]}});
        }
        else if(ans[0]=="L.S"){
            store.push_back({ans[1],{ans[2]}});
        }
        else if(ans[0]=="ADD.D"){
            store.push_back({ans[1],{ans[2],ans[3]}});
        }
        else if(ans[0]=="ADD.S"){
            store.push_back({ans[1],{ans[2],ans[3]}});
        }
        else if(ans[0]=="S.D"){
            store.push_back({"",{ans[1],ans[2]}});
        }
        else if(ans[0]=="S.S"){
            store.push_back({"",{ans[1],ans[2]}});
        }
        else if(ans[0]=="ADD.I"){
            store.push_back({ans[1],{ans[2]}});
        }
        else if(ans[0]=="SUB.I"){
            store.push_back({ans[1],{ans[2]}});
        }
        else if(ans[0]=="BNE"){
            store.push_back({"",{ans[1],ans[2]}});
        }
        else if(ans[0]=="BEQZ"){
            store.push_back({"",{ans[1]}});
        }
        else if(ans[0]=="ADD.U"){
            store.push_back({ans[1],{ans[2],ans[3]}});
        }
        else if(ans[0]=="SUB.U"){
            store.push_back({ans[1],{ans[2],ans[3]}});
        }
        else if(ans[0]=="LW"){
            store.push_back({ans[1],{""}});
        }
    }

    for(int i=0;i<store.size();i++){
        string out=store[i].first;
        vector<string> v=store[i].second;
        for(int j=0;j<v.size();j++){
            if(vp.find(v[j])!=vp.end()){
                Node * curr=vp[v[j]].second;
                (curr->right).push_back(i+1);
            }
        }
        if(out!=""){
        if(vp.find(out)==vp.end()){
            Node * temp=new Node(i+1);
            vp[out]={temp,temp};
        }
        else{
            Node * temp=new Node(i+1);
            Node * curr=vp[out].second;
            curr->left=temp;
            curr=curr->left;
            vp[out].second=curr;
        }
        }
    }

    unordered_map<int,vector<int>> dd;
    unordered_map<string,pair<Node *, Node *>> :: iterator itr;
    for(itr=vp.begin();itr!=vp.end();itr++){
        vector<int> v;
        data_depend((itr->second).first,v,dd,0);
    }
    temp_stall(res,stall,dd,d_path);
    return d_path;

}

//Main function
int main()
{
    ifstream fr;
    fr.open("input.txt");
    string line;
    unordered_map<int,string> ins;
    unordered_map<string,pair<Node *, Node *>> mp;
    vector<pair<string,vector<string>>> store;
    int k=1;
    while(fr){
        getline(fr,line);
        ins[k++]=line;
        transform(line.begin(), line.end(), line.begin(), ::toupper);
        vector<string> ans;
        ans=token(line);
        if(ans[0]=="L.D"){
            store.push_back({ans[1],{ans[2]}});
        }
        else if(ans[0]=="L.S"){
            store.push_back({ans[1],{ans[2]}});
        }
        else if(ans[0]=="ADD.D"){
            store.push_back({ans[1],{ans[2],ans[3]}});
        }
        else if(ans[0]=="ADD.S"){
            store.push_back({ans[1],{ans[2],ans[3]}});
        }
        else if(ans[0]=="S.D"){
            store.push_back({"",{ans[1],ans[2]}});
        }
        else if(ans[0]=="S.S"){
            store.push_back({"",{ans[1],ans[2]}});
        }
        else if(ans[0]=="ADD.I"){
            store.push_back({ans[1],{ans[2]}});
        }
        else if(ans[0]=="SUB.I"){
            store.push_back({ans[1],{ans[2]}});
        }
        else if(ans[0]=="BNE"){
            store.push_back({"",{ans[1],ans[2]}});
        }
        else if(ans[0]=="BEQZ"){
            store.push_back({"",{ans[1]}});
        }
        else if(ans[0]=="ADD.U"){
            store.push_back({ans[1],{ans[2],ans[3]}});
        }
        else if(ans[0]=="SUB.U"){
            store.push_back({ans[1],{ans[2],ans[3]}});
        }
        else if(ans[0]=="LW"){
            store.push_back({ans[1],{""}});
        }
        if(fr.eof()) break;
    }

    //Creating the tree data structure for storing the dependencies
    for(int i=0;i<store.size();i++){
        string out=store[i].first;
        vector<string> v=store[i].second;
        for(int j=0;j<v.size();j++){
            if(mp.find(v[j])!=mp.end()){
                Node * curr=mp[v[j]].second;
                (curr->right).push_back(i+1);
            }
        }
        if(out!=""){
        if(mp.find(out)==mp.end()){
            Node * temp=new Node(i+1);
            mp[out]={temp,temp};
        }
        else{
            Node * temp=new Node(i+1);
            Node * curr=mp[out].second;
            curr->left=temp;
            curr=curr->left;
            mp[out].second=curr;
        }
        }
    }
    cout<<"-------------------------------------------------"<<"\n";
    cout<<"                  Instructions                   "<<"\n";
    cout<<"-------------------------------------------------"<<"\n";
    for(int i=1;i<=ins.size();i++){
        cout<<"     "<<i<<" : "<<ins[i]<<"\n";
    }
    cout<<"\n";
    cout<<"\n";
    cout<<"-------------------------------------------------"<<"\n";
    cout<<"                Data Dependency                  "<<"\n";
    cout<<"-------------------------------------------------"<<"\n";
    unordered_map<int,vector<int>> dd;
    unordered_map<int,vector<int>> od;
    unordered_map<int,vector<int>> ad;

    unordered_map<string,pair<Node *, Node *>> :: iterator it;
    for(it=mp.begin();it!=mp.end();it++){
        vector<int> v;
        data_depend((it->second).first,v,dd,1);
    }
    /*cout<<"\n";
    unordered_map<string,pair<Node *, Node *>> :: iterator itr;
    for(itr=mp.begin();itr!=mp.end();itr++){
        vector<int> v;
        waw((itr->second).first,v,od);
    }
    cout<<"\n";
    for(itr=mp.begin();itr!=mp.end();itr++){
        vector<int> v;
        raw((itr->second).first,v,ad);
    }*/

    //Mapping stall information between instruction
    unordered_map<string,vector<pair<string,int>>> stall;
    stall["ADD.D"].push_back({"ADD.D",3});
    stall["ADD.D"].push_back({"S.D",2});
    stall["ADD.D"].push_back({"L.D",1});
    stall["ADD.D"].push_back({"ADD.I",3});
    stall["ADD.D"].push_back({"BNE",1});
    stall["L.D"].push_back({"L.D",4});
    stall["L.D"].push_back({"ADD.D",1});
    stall["L.D"].push_back({"ADD.I",1});
    stall["ADD.I"].push_back({"BNE",1});
    stall["ADD.I"].push_back({"ADD.D",3});
    stall["ADD.I"].push_back({"ADD.I",3});
    stall["ADD.I"].push_back({"S.D",2});
    stall["ADD.I"].push_back({"L.D",1});

    string path1=unsh(stall,dd,"input.txt");                  //generating unscheduled output file
    string path2=unroll(path1);                               //generating unrolled unscheduled output file
    string path3=sched(path1,"Scheduled.txt",stall);          //generating scheduled output file
    string path4=sched(path2,"Unrolled_Scheduled.txt",stall); //generating unrolled scheduled output file

    cout<<"\n\n\n Four output text files have been generated using"
        <<"\n (i) "<<"Unscheduled strategy"
        <<"\n (ii) "<<"Scheduled strategy"
        <<"\n (iii) "<<"Loop Unrolling and Unscheduled strategy"
        <<"\n (iv) "<<"Loop Unrolling and Scheduled strategy\n\n";


    return 0;
}
