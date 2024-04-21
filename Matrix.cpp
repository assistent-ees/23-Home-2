#include <iostream>
#include <fstream>

template <typename T>
class Matrix
{
    private:
        size_t rows;
        size_t cols;
        T **A;

    public:
        ~Matrix()
        {
            if (A!=nullptr)
            {
                for (size_t i=0;i<rows;++i)
                    delete[] A[i];
            }
            delete[] A;
        }

        Matrix(): A(nullptr), rows(0), cols(0) {}

        Matrix(size_t newRows,size_t newCols) : rows(newRows), cols(newCols)
        {
            A=new T *[rows];
            for (size_t i=0;i<rows;++i)
            {
                A[i]=new T[cols];
            }
        }

        Matrix(const std::string& filename)
        {
            std::ifstream file(filename);
            if (!file.is_open())
            {
                throw std::runtime_error("Failed to open file");
            }

            file >> rows >> cols;
            A=new T*[rows];
            for (size_t i=0;i<rows;++i)
            {
                A[i]=new T[cols];
                for (size_t j=0;j<cols;++j)
                {
                    file >> A[i][j];
                }
            }
            file.close();
        }

        Matrix operator+(const Matrix& other) const
        {
            if (rows!=other.rows or cols!=other.cols)
            {
                throw std::invalid_argument("Matrices must have the same dimensions");
            }
            
            Matrix result(rows,cols);
            for (size_t i=0;i<rows;++i)
            {
                for (size_t j=0;j<cols;++j)
                {
                    result.A[i][j]=A[i][j]+other.A[i][j];
                }
            }
            
            return result;
        }
        Matrix operator-(const Matrix& other) const
        {
            if (rows!=other.rows or cols!=other.cols)
            {
                throw std::invalid_argument("Matrices must have the same dimensions");
            }

            Matrix result(rows,cols);
            for (size_t i=0;i<rows;++i)
            {
                for (size_t j=0;j<cols;++j)
                {
                    result.A[i][j]=A[i][j]-other.A[i][j];
                }
            }

            return result;
        }
        Matrix operator*(const Matrix& other) const
        {
            if (cols!=other.rows)
            {
                throw std::invalid_argument("Number of columns in the first matrix must be equal to the number of rows in the second matrix");
            }

            Matrix result(rows,cols);
            for (size_t i=0;i<rows;++i)
            {
                for (size_t j=0;j<other.cols;++j)
                {
                    result.A[i][j]=0;
                    for (size_t k=0;k<cols;++k)
                    {
                        result.A[i][j]+=A[i][k]*other.A[k][j];
                    }  
                }
            }

            return result;
        }
        Matrix operator*(const T& scalar) const
        {
            Matrix result(rows,cols);
            for (size_t i=0;i<rows;++i)
            {
                for (size_t j=0;j<cols;++j)
                {
                    result.A[i][j]=A[i][j]*scalar;
                }
            }

            return result;
        }

        Matrix& operator=(const Matrix& other) const
        {
            if (&other == this)
            {
                return *this;
            }

            if (A!=nullptr)
            {
                for (size_t i=0;i<rows;++i)
                {
                    delete[] A[i];
                }
                delete[] A;
            }

            rows=other.rows;
            cols=other.cols;
            A=new T*[rows];
            for (size_t i=0;i<rows;++i)
            {
                A[i]=new T[cols];
                for (size_t j=0;j<cols;++j)
                {
                    A[i][j]=other.A[i][j];
                }
            }

            return *this;
        }

        static Matrix zero(size_t size)
        {
            return Matrix(size,size);
        }
        static Matrix identity(size_t size)
        {
            Matrix result(size,size);
            for (size_t i=0;i<size;++i)
            {
                for (size_t j=0;j<size;++j)
                {
                    if (i==j)
                    {
                        result.A[i][j]=1;
                    }
                    else
                    {
                        result.A[i][j]=0;
                    }
                }
            }

            return result;  
        }

        friend std::ostream& operator<<(std::ostream& os,const Matrix& matrix)
        {
            for (size_t i=0;i<matrix.rows;++i)
            {
                for (size_t j=0;j<matrix.cols;++j)
                {
                    os << matrix.A[i][j]<< " ";
                }
                os << std::endl;
            }

            return os;
        }

        T determinant()
        {   
            return determ(A,rows);
        }

        void getMinor(T **matr,size_t n,T **minor,size_t row,size_t col)
        {
            size_t r=0, c=0;
            for (size_t i=0;i<n;++i)
            {
                if (i==row)
                    continue;
                for (size_t j=0;j<n;++j)
                {
                    if (j==col)
                        continue;
                    minor[r][c]=matr[i][j];
                    ++c;
                }
                ++r;
                c=0;
            }
        }

        Matrix<T> operator!()
        {
            T det=determinant();
            if (det==0)
            {
                std::cerr << "Matrix is singular, inverse matrix doesn't exist" << std::endl;
                return *this;
            }

            Matrix<T> inverseMatr(rows,cols);
            size_t n=rows;
            for (size_t i=0;i<n;++i)
            {
                for (size_t j=0;j<n;++j)
                {
                    T** minor =new T*[n-1];
                    for (size_t k=0;k<n-1;++k)
                    {
                        minor[k]=new T[n-1];
                    }
                    getMinor(A,n,minor,i,j);
                    T sign=((i+j)%2==0) ? 1 : -1;
                    inverseMatr.A[j][i]=sign*determ(minor,n-1)/det;
                    
                    for (size_t k=0;k<n-1;++k)
                    {
                        delete[] minor[k];
                    }
                    delete[] minor;
                }
            }

            return inverseMatr;
        }

        T determ(T** matr,size_t n)
        {
            if (n==1)
            {
                return matr[0][0];
            }
            
            T det=0;
            int sign=1;
            T** minor=new T*[n-1];
            for (size_t i=0;i<n-1;++i)
            {
                minor[i]=new T[n-1];
            }
            for (size_t i=0;i<n;++i)
            {
                getMinor(matr,n,minor,0,i);
                det+=sign*matr[0][i]*determ(minor,n-1);
                sign=-sign;
            }
            for (size_t i=0;i<n-1;++i)
            {
                delete[] minor[i];
            }  
            delete[] minor;

            return det;
        }
};

int main()
{
    Matrix<double> M1("input1.txt");
    Matrix<double> M2("input2.txt");
    Matrix<double> M3=Matrix<double>::identity(2);

    std::cout << M1 << std::endl;
    std::cout << M2 << std::endl;
    std::cout << M3.determinant() << std::endl;
}
