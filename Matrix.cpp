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

        Matrix operator!() const
        {
            if (rows!=cols)
            {
                throw std::runtime_error("Matrix is not square and can't be inverted");
            }

            Matrix extendedM(rows,2*cols);
            for (size_t i=0;i<rows;++i)
            {
                for (size_t j=0;j<cols;++j)
                {
                    extendedM.A[i][j]=A[i][j];
                    if (i==j)
                        extendedM.A[i][j+cols]=1;
                    else
                        extendedM.A[i][j+cols]=0;
                }
            }

            for (size_t i=0;i<rows;++i)
            {
                for (size_t j=0;j<cols;++j)
                {
                    double r=extendedM.A[j][i]/extendedM.A[i][i];
                    for (size_t k=0;k<2*rows;++k)
                    {
                        extendedM.A[j][k]-=r*extendedM.A[i][k];
                    }
                }
            }

            for (size_t i=0;i<rows;++i)
            {
                double d=extendedM.A[i][i];
                for (size_t j=0;j<2*cols;++j)
                {
                    extendedM.A[i][j]/=d;
                }
            }

            Matrix result(rows,cols);
            for (size_t i=0;i<rows;++i)
            {
                for (size_t j=0;j<cols;++j)
                {
                    result.A[i][j]=extendedM.A[i][j+cols];
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
};

int main()
{
    Matrix<double> M1("input1.txt");
    Matrix<double> M2("input2.txt");
    Matrix<int> M3=Matrix<int>::identity(3);
    
    std::cout << M1 << std::endl;
    std::cout << M2 << std::endl;
    std::cout << M3 << std::endl;
}
