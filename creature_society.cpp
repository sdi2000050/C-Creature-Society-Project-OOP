#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sstream>

using namespace std;

class creature_society;

class creature{
    private:
        string name;
        int lifetime;
        creature_society* d; 
    public:
        creature(int L, string n,creature_society* d):lifetime(L),name(n),d(d){}
        creature& operator=(const creature& c){
            if (this != &c){
                name=c.name;
                lifetime=c.lifetime;
                d=c.d;
            }
            return *this;
        }
        
        bool is_a_zobie();
        virtual bool is_a_good(creature* c){}
        virtual void clone(creature* c);
        void bless();
        void beat();
        virtual void robust_behavior(){}
        int get_life(){return lifetime;}
        creature_society* get_pointer(){return d;}
        string get_name(){return name;}
};


class good_creature :public creature{
    private:
        int good_thrsh;
    public:
        good_creature(int L,string n,creature_society* d,int gt):creature(L,n,d),good_thrsh(gt){cout<<"The good creature "<<n<<"  was just created\n";}
        good_creature& operator=(const good_creature& g){
            if(this!=&g){
                this->creature::operator=(g);
                good_thrsh=g.good_thrsh;
            }
            return *this;
        }  

        void clone(good_creature* g){creature::clone(g); g->good_thrsh=this->good_thrsh;}
        void robust_behavior();
        bool is_a_good(creature* c){return true;}
};


class bad_creature :public creature{
    private:
        int bad_thrsh;
    public:
        bad_creature(int L,string n,creature_society* d,int bt):creature(L,n,d),bad_thrsh(bt){cout<<"The bad creature "<<n<<" was just created\n";}
        bad_creature& operator=(const bad_creature& b){
            if(this!=&b){
                this->creature::operator=(b);
                bad_thrsh=b.bad_thrsh;
            }
            return *this;
        } 

        void clone(bad_creature* b){creature::clone(b); b->bad_thrsh=this->bad_thrsh;}
        void robust_behavior();
        bool is_a_good(creature* c){return false;}
};

class creature_society{
    private:
        int n;
        creature** top;
        good_creature* cg;
        bad_creature* cb;
    public:
        creature_society(int N,int L,int gt,int bt);
        ~creature_society();

        string get_name(string name,int i);
        void clone_next(good_creature* gc);
        void clone_zobies(bad_creature* bc);
        void beat(int i);
        void bless(int i);
        int no_of_good();
        int no_of_zobies();
};

creature_society::creature_society(int N,int L,int gt,int bt)
{   int i,gc=0,bc=0,g;
    n=N;
    top=new creature* [n];
    string gnames[n],bnames[n];
    for(i=0;i<n;i++){
        gnames[i]="good name";
        bnames[i]="bad name";
    }   
    for(i=0;i<n;i++){
        g=rand()%2;
        if(g==1){
            string c=get_name(gnames[gc],i);
            top[i]=new good_creature(L,c,this,gt);
        }
        else{
            string c=get_name(bnames[bc],i);
            top[i]=new bad_creature(L,c,this,bt);
        }
    }
    cout<<"A creature society was just created\n";
}

creature_society::~creature_society()
{
    delete[] top;
    cout<<"A creature society was just destroyed\n";
}

string creature_society::get_name(string name,int i){
    string c,num;
    c=name;
    stringstream ss;  
    ss << i;  
    ss >> num;  
    c=c+num;
    return c;
}


void creature::clone(creature* c){
    c->lifetime=this->lifetime;
    c->name=this->name;
    c->d=this->d;
}

void creature::bless(){
    lifetime++;
    cout<<"The creature "<< name << " has just been blessed\n";
}

void creature::beat(){
    if(lifetime>0){
        lifetime--;
        cout<<"The creature "<< name << " has just been beaten\n";
    }
}

bool creature::is_a_zobie(){
    if (lifetime==0)
        return true;
    return false;
}


void good_creature::robust_behavior(){
    if(get_life()>good_thrsh){
        get_pointer()->clone_next(this);
    }
}


void bad_creature::robust_behavior(){
    if(get_life()>bad_thrsh){
        get_pointer()->clone_zobies(this);
    }
}


void creature_society::clone_next(good_creature* gc){
    for(int i=0; i<n;i++){
        if (top[i]==gc){
            top[i]->clone(top[i+1]);
            cout<<"The creature "<<top[i]->get_name()<<" has just been cloned in the next position of the society\n";
            break;
        }
    }
}

void creature_society::clone_zobies(bad_creature* bc){
    for(int i=0; i<n;i++){
        if (top[i]==bc){
            if(top[i+1]->is_a_zobie()==true){
                top[i]->clone(top[i+1]);
                cout<<"The creature "<<top[i]->get_name()<<" has just been cloned in the next position with a zobie of the society\n";

            }
            else{
                break;
            }
        }
    }
}

void creature_society::beat(int i){
    (*top[i]).beat();
}

void creature_society::bless(int i){
    (*top[i]).bless();
    if((*top[i]).is_a_good(top[i])==true){
        (*top[i]).robust_behavior();
    }
    else{
        (*top[i]).robust_behavior();
    }
}

int creature_society::no_of_good(){
    int i,count=0;
    for(i=0; i<n;i++){
        if(this->top[i]->is_a_good(top[i])==true){
            count++;
        }
    }
    return count;
}

int creature_society::no_of_zobies(){
    int i,count=0;
    for(i=0; i<n;i++){
        if (top[i]->is_a_zobie()==true){
            count++;
        }  
    }
    return count;
}


int main(int argc, char *argv[]){
    int N,M,L,good_thrsh,bad_thrsh,i,b,j,g,z;

    if (argc!=6)
        return 1;
    N=atoi(argv[1]);
    M=atoi(argv[2]);
    L=atoi(argv[3]);
    good_thrsh=atoi(argv[4]);
    bad_thrsh=atoi(argv[5]);

    creature_society S(N,L,good_thrsh,bad_thrsh);

    for(j=0;j<M;j++){
        i=rand()%(N-1);
        b=rand()%(2+1);
        if(b==1){
            S.beat(i);
        }
        else{
            
            S.bless(i);
        }
    }

    g=S.no_of_good();
    z=S.no_of_zobies();
    if(g==N){
        cout<<"Good Dominates in the World!\n";
    }
    else if(z==N){
        cout<<"This is a dead society\n";
    }
    else{
        cout<<"Try again to improve the world\n";
    }


}