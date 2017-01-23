#include <QCoreApplication>
#include <qvector.h>
#include <QDebug>


class Operation
{
    QVector<int> val;
public:
    Operation(QVector<int> values)
    {
        val = values;
    }
    int op(int x, int y);

};

int Operation::op(int x, int y)
{
    return val[x*2+y];
}

class func
{
public:
    QVector<int> elem;
    QVector<int> values;
    QVector<int> op;
    func expansion(int i,int val);
    func(QVector<int> elements, QVector<int> operands)
    {
        elem=elements;
        op=operands;
    }

    func(const func& foo)
    {
        this->elem=foo.elem;
        this->op=foo.op;
        this->values=foo.values;
    }
    int test();
};

int func::test()
{
    int result=elem[0]*values[0];
    //for(int i = 1; i < elem.size(); i++)

}

func func::expansion(int i,int val)
{
    func foo(*this);
    foo.values[i]=val;
    return foo;

}

class Node
{
public:
    int index;
    int low;
    int high;
    Node(){}
    Node(int i, int l, int h)
    {
        index = i;
        low = l;
        high = h;
    }
    void Show()
    {
        qDebug()<<" "<<index<<low<<high;
    }
};

int index(Node u)
{
    return u.index;
}
int low(Node u)
{
    return u.low;
}
int high(Node u)
{
    return u.high;
}

Node ZeroNode(100,0,0);
Node OneNode(100,1,1);

class BDD
{
public:
    QVector<QVector<int>> table;
    QVector<Node> H;
    BDD()
    {
        H.append(ZeroNode);
        H.append(OneNode);
//        H[0].index=0;
//        H[0].high=0;
//        H[0].low=0;

//        H[1].index=0;
//        H[1].high=1;
//        H[1].low=1;
    }

    void Show();
};

void BDD::Show()
{
    for(int i =0;i<H.size();i++)
    {
        std::printf("%i",i);
        H[i].Show();
    }
}

bool member(BDD H,int i, int v0, int v1)
{
//    for (auto vec:H.table)
//        if (i==vec[0] && v0==vec[1] && v1==vec[2])
//            return true;
    for (auto u : H.H)
        if (i==index(u) && v0==low(u) && v1==high(u))
            return true;
    return false;
}

int lookup(BDD H,int i, int v0, int v1)
{
//    for (int j=0;j<H.table.size();j++)
//        if (i==H.table[j][0] && v0==H.table[j][1] && v1==H.table[j][2])
//            return j;

    for (int j=0;j<H.H.size();j++)
        if (i==index(H.H[j]) && v0==low(H.H[j])&& v1==high(H.H[j]))
            return j;
    return -1;
}

int insert(BDD &H, int i, int v0, int v1)
{
    //QVector<int> node={i,v0,v1};
    Node node(i,v0,v1);
    H.H.append(node);
    qDebug()<<H.H.size();
    node.Show();
    return H.H.size() - 1;
    //H.table.append(node);
    //return H.table.size() - 1;
}

int MakeNode(BDD &H, int i, int l, int h)
{
    if (l==h)
        return l;
//    if(member(H,i,l,h))
//        return lookup(H,i,l,h);
    return insert(H,i,l,h);
}



int App(int u1,int u2, QVector<QVector<int>> &G, BDD &H, Operation op)
{
    qDebug()<<u1<<u2;
    int u;
    if (G[u1][u2]>-1)
    {
        return G[u1][u2];
    }
    else
    {
        if((u1==0 || u1==1) && (u2==0 || u2==1))
            u = op.op(u1,u2);
        else
        {
            //if (index(u1)==index(u2))
            if (H.H[u1].index == H.H[u2].index)
                //u = MakeNode(H,index(u1),App(low(u1),low(u2),&G,H,op),App(high(u1),high(u2),&G,H,op));
                u = MakeNode(H,H.H[u1].index,App(H.H[u1].low,H.H[u2].low,G,H,op),App(H.H[u1].high,H.H[u2].high,G,H,op));
            else
            {
                if (H.H[u1].index < H.H[u2].index)
                    //u = MakeNode(H,index(u1),App(low(u1),u2,&G,H,op),App(high(u1),u2,&G,H,op));
                    u = MakeNode(H,H.H[u1].index,App(H.H[u1].low,u2,G,H,op),App(H.H[u1].high,u2,G,H,op));
                else
                    u = MakeNode(H,H.H[u2].index,App(u1,H.H[u2].low,G,H,op),App(u1,H.H[u2].high,G,H,op));
            }

        }
        G[u1][u2]=u;
        return u;
    }
}
int Apply(BDD &H, Operation op, int u1, int u2)
{
    QVector<QVector<int>> G;
//    int **G;
//    *G = new int[u1];

    QVector<int> tmp;
    for(int j=0;j<u2+1;j++)
        tmp.append(-1);
    for (int i=0;i<u1+1;i++)
    {
        G.append(tmp);
    }
    //G[3][4]=0;
    qDebug()<<G<<G[3][4];
    //return 0;
    int ret = App(u1,u2,G,H,op);
    for(int i = 0; i < u1+1;i++)
        qDebug()<< G[i];
    return ret;
}

void foo(int& test)
{
    test = 5;
}
void bar()
{
    int test = 1;
    qDebug()<<test;
    foo(test);
    qDebug()<<test;
}


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QVector<int> oper={0,1,1,1};
    Operation dis(oper);
    qDebug()<<dis.op(0,1);

    BDD H;
    MakeNode(H,4,0,1);//2
    MakeNode(H,3,2,1);
    MakeNode(H,2,2,3);
    MakeNode(H,1,4,3);//5

    MakeNode(H,4,0,1);//6
    MakeNode(H,3,1,6);//7
    MakeNode(H,1,6,7);//8

    H.Show();

    qDebug()<<Apply(H,dis,5,8);
    H.Show();

//    G = new int*[u1];


//    for (int i=0;i<u1;i++)
//    {
//        G[i] = new int[u2];
//        for(int j=0;j<u2;i++)
//            G[i][j]=-1;
//    }
//    qDebug()<<G;
    /*
    const unsigned int DIM1 = 7;
    const unsigned int DIM2 = 3;

    int **ary;    // (1)

    // создание
    ary = new int * [DIM1];    // массив указателей (2)
    for (int i = 0; i < DIM1; i++) {   // (3)
        ary[i] = new int [DIM2];     // инициализация указателей
    }

    // работа с массивом
    for (int i = 0; i < DIM1; i++) {
        for (int j = 0; j < DIM2; j++) {
            ary[i][j] = (i + 1) * 10 + (j + 1);
        }
    }
    for (int i = 0; i < DIM1; i++) {
            for (int j = 0; j < DIM2; j++) {
                qDebug() << ary[i][j];
            }
            qDebug() << endl;
        }

        */

    return a.exec();
}
