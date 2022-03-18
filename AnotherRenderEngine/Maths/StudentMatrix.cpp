#include<iostream>
using namespace std;

class Matrix
{
private:
    int row, list;
    double** HL;
public:
    Matrix(int r_ = 0, int l_ = 0);
    Matrix(int r_, int l_, double** newone);
    Matrix(const Matrix& rhs);
    ~Matrix();
    Matrix operator + (const Matrix& rhs);
    Matrix operator - (const Matrix& rhs);
    Matrix operator = (const Matrix& rhs);
    friend ostream& operator << (ostream& os, const Matrix& rhs);
};

int i, j;

Matrix::Matrix(int r_, int l_) :row(r_), list(l_)//构造函数
{
    HL = new double* [row];
    for (i = 0; i < row; i++)
        HL[i] = new double[list];
    cout << "please enter Matrix :" << endl;
    for (i = 0; i < row; i++)
        for (j = 0; j < list; j++)
            cin >> HL[i][j];
}

Matrix::Matrix(int r_, int l_, double** newone) :row(r_), list(l_)//构造函数重载，主要用于加法减法中的return使用
{
    HL = new double* [row];
    for (i = 0; i < row; i++)
        HL[i] = new double[list];
    for (i = 0; i < row; i++)
        for (j = 0; j < list; j++)
            HL[i][j] = newone[i][j];
}

Matrix::Matrix(const Matrix& rhs)
{
    if (this != &rhs)
    {
        this->row = rhs.row;
        this->list = rhs.list;
        HL = new double* [row];
        for (i = 0; i < row; i++)
            HL[i] = new double[list];
        for (i = 0; i < row; i++)
            for (j = 0; j < list; j++)
                this->HL[i][j] = rhs.HL[i][j];
    }
}

Matrix::~Matrix()//　析构函数，删除开辟的空间
{
    cout << "~ Matrix : row =" << row << " , list = " << list << endl << endl;
    for (i = 0; i < row; i++)
        delete[] HL[i];
    delete[] HL;
}

Matrix Matrix::operator + (const Matrix& rhs)
{
    if ((this->row == rhs.row) && (this->list == rhs.list))
    {
        double** newone;
        int r_, l_;
        r_ = row; l_ = list;
        newone = new double* [row];
        for (i = 0; i < row; i++)
            newone[i] = new double[list];
        for (i = 0; i < row; i++)
            for (j = 0; j < list; j++)
                newone[i][j] = HL[i][j] + rhs.HL[i][j];
        return Matrix(r_, l_, newone);
    }
    //    else
    //        cout<<"error ——矩阵类型不符 "<<endl;
}

Matrix Matrix::operator - (const Matrix& rhs)
{
    if ((this->row == rhs.row) && (this->list == rhs.list))
    {
        double** newone;
        int r_, l_;
        r_ = row; l_ = list;
        newone = new double* [row];
        for (i = 0; i < row; i++)
            newone[i] = new double[list];
        for (i = 0; i < row; i++)
            for (j = 0; j < list; j++)
                newone[i][j] = HL[i][j] - rhs.HL[i][j];
        return Matrix(r_, l_, newone);
    }
    //    else
    //        cout<<"error ——矩阵类型不符 "<<endl;
}

Matrix Matrix::operator = (const Matrix& rhs)
{
    if ((this->row == rhs.row) && (this->list == rhs.list))
    {
        for (i = 0; i < row; i++)
            for (j = 0; j < list; j++)
                this->HL[i][j] = rhs.HL[i][j];
        return (*this);
    }
    //    else
    //        cout<<"error ——矩阵类型不符 "<<endl;
}

ostream& operator << (ostream& os, const Matrix& rhs)
{
    os << "Matrix : row =" << rhs.row << " , list = " << rhs.list << endl;
    for (i = 0; i < rhs.row; i++)
    {
        for (j = 0; j < rhs.list; j++)
            os << rhs.HL[i][j] << " ";
        os << endl;
    }
    return os;
}
//出处
// https://www.cnblogs.com/2015-16/p/11782566.html
//int main()
//{
//    int m, n, x, y;
//    cin >> n >> m >> x >> y;
//    Matrix aa(n, m), bb(n, m), cc(n, m), dd(x, y);
//    cout << endl << aa << endl << bb << endl << cc << endl << dd << endl;
//    cout << (aa + bb + cc) << endl << (cc - bb) << endl;
//    return 0;
//}