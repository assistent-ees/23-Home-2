#include <iostream>
#include <vector>
#include <string>
#include <fstream>

using namespace std;

template<typename T>

class Matrix {
private:
    int stolby;
    int stroki;
    vector<vector<T>> matrix;  //двумерный вектор для хранения данных матрицы
public:

    Matrix(int kon_stroki, int kon_stolby) //конструктор для создания матрицы заданного размера
    {
        stroki = kon_stroki;
        stolby = kon_stolby;
        matrix = vector<vector<T>>(stroki, vector<T>(stolby));
    }
    
    Matrix(const string& filename) //конструктор для считывания матрицы из файла
    {
        ifstream file(filename);
        if (file.is_open()) {
            int str, cols;
            file >> str >> cols;
            stroki = str;
            stolby = cols;
            matrix.resize(stroki, vector<int>(stolby));

            for (int i = 0; i < stroki; i++) {
                for (int j = 0; j < stolby; j++) {
                    file >> matrix[i][j];
                }
            }
            file.close();
        }
        else {
            cerr << "не получилось открыть файл" << endl;
        }
    }

    void printMatrixToFile(string fileoutname) //вывод матрицы в файл
    {
        ofstream fileout(fileoutname);
        for (int i = 0; i < stroki; ++i)
        {
            for (int j = 0; j < stolby; ++j)
            {
                fileout << matrix[i][j] << " ";
            }
            fileout << endl;
        }
        fileout.close();
    }

    Matrix() : stroki(0), stolby(0) {} //конструктор для пустой матрицы

    Matrix(int kon_stroki, int kon_stolby, const T value) //конструктор для скаляра
    {
        stroki = kon_stroki;
        stolby = kon_stolby;
        matrix = vector<vector<T>>(stroki, vector<T>(stolby, value));
    }

    Matrix operator+(const Matrix& other)    //перегрузка +
    {
        if (stroki != other.stroki || stolby != other.stolby)
        {
            throw invalid_argument("не совпадает размерность матриц при операции +");
        }
        Matrix res(stroki, stolby);
        for (int i = 0; i < stroki; i++)
        {
            for (int j = 0; j < stolby; j++)
            {
                res.matrix[i][j] = matrix[i][j] + other.matrix[i][j];
            }
        }
        return res;
    }

    Matrix operator-(const Matrix& other) //перегрузка -
    {
        if (stroki != other.stroki || stolby != other.stolby)
        {
            throw invalid_argument("не совпадает размерность матриц при операции -");
        }
        Matrix res(stroki, stolby);
        for (int i = 0; i < stroki; i++)
        {
            for (int j = 0; j < stolby; j++)
            {
                res.matrix[i][j] = matrix[i][j] - other.matrix[i][j];
            }
        }
        return res;
    }

    Matrix operator*(const Matrix& other) //перегрузка * для матриц
    {
        if (stroki != other.stolby)
        {
            throw invalid_argument("не подходит размерность матриц при операции *");
        }
        Matrix res(stroki, other.stolby);
        for (int i = 0; i < stroki; i++)
        {
            for (int j = 0; j < other.stolby; j++)
            {
                T sum = 0;
                for (int k = 0; k < stolby; k++)
                {
                    sum += matrix[i][k] * other.matrix[k][j];
                }
                res.matrix[i][j] = sum;
            }
        }
        return res;
    }

    Matrix operator*(int scl) //перегрузка * для матрицы и скаляра
    {
        Matrix res(stroki, stolby);
        for (int i = 0; i < stroki; i++)
        {
            for (int j = 0; j < stolby; j++)
            {
                res.matrix[i][j] = matrix[i][j] * scl;
            }
        }
        return res;
    }

    friend istream& operator>>(istream& is, Matrix& other) //перегрузка >>
    {
        cout << "введите " << other.stolby * other.stroki << " значений" << endl;
        for (int i = 0; i < other.stroki; i++)
        {
            for (int j = 0; j < other.stolby; j++)
            {
                is >> other.matrix[i][j];
            }
        }
        return is;
    }

    friend ostream& operator<<(ostream& os, const Matrix& other) //перегрузка <<
    {
        for (auto str : other.matrix)
        {
            for (auto el : str)
            {
                os << el << " ";
            }
            os << endl;
        }
        return os;
    }

    Matrix operator=(const Matrix& other)  //перегрузка присваивания
    {
        if (stroki != other.stroki || stolby != other.stolby)
        {
            throw invalid_argument("присваивание не получится");
        }
        this->stroki = other.stroki;
        this->stolby = other.stolby;
        this->matrix = other.matrix;
        return *this;
    }

    T determinant(const vector<vector<T>>& mat) { //нахождение определителя для проверки нахождения обрат мат
        if (mat.size() != mat[0].size()) {
            throw invalid_argument("Error: Matrix is not square!");
        }

        int n = mat.size();

        if (n == 1) {
            return mat[0][0];
        }

        T det = 0;
        for (int i = 0; i < n; ++i) {
            vector<vector<T>> submatrix(n - 1, vector<T>(n - 1));
            for (int j = 1; j < n; ++j) {
                for (int k = 0; k < n; ++k) {
                    if (k < i) {
                        submatrix[j - 1][k] = mat[j][k];
                    }
                    else if (k > i) {
                        submatrix[j - 1][k - 1] = mat[j][k];
                    }
                }
            }
            T sign = (i % 2 == 0) ? 1 : -1;
            det += sign * mat[0][i] * determinant(submatrix);
        }

        return det;
    }

    Matrix<double> operator!()     //перегрузка оператора ! для нахождения обратной матрицы
    {

        //проверка определителя на равенство нулю
        if (abs(determinant(matrix)) < 1e-6) {
            throw invalid_argument("Определитель равен нулю, нет обратной матрицы.");
        }

        int n = this->stroki;

        vector<vector<double>> rash_matrix(n, vector<double>(n * 2, 0.0));
        for (int i = 0; i < n; i++) {
            rash_matrix[i][i + n] = 1.0;
            for (int j = 0; j < n; j++) {
                rash_matrix[i][j] = this->matrix[i][j];
            }
        }

        for (int i = 0; i < n; i++) {
            double p = rash_matrix[i][i];
            for (int j = 0; j < (2 * n); j++) {
                rash_matrix[i][j] /= p;
            }
            for (int k = 0; k < n; k++) {
                if (k != i) {
                    double f = rash_matrix[k][i];
                    for (int j = 0; j < (2 * n); j++) {
                        rash_matrix[k][j] -= rash_matrix[i][j] * f;
                    }
                }
            }
        }

        vector<vector<double>> dop_matrix(n, vector<double>(n, 0));
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                dop_matrix[i][j] = rash_matrix[i][j + n];
            }
        }

        return dop_matrix;
    }

    Matrix(vector<vector<T>> matrix)
    {
        this->matrix = matrix;
        stroki = matrix.size();
        stolby = matrix[0].size();
    }

    static Matrix zero_matrix(int stroki, int stolby) //нулевая матрица
    {
        return Matrix(vector<vector<T>>(stroki, vector<T>(stolby)));
    }
    
    static Matrix ed_matrix(int stroki) //единичная матрица
    {
        vector<vector<T>> result = vector<vector<T>>(stroki, vector<T>(stroki, 0));
        for (int i = 0; i < stroki; i++)
        {
            result[i][i] = 1;
        }
        return result;
    }
};

int main()
{
    setlocale(LC_ALL, "Russian");
    Matrix<int> matrix1(2, 2, 1);
    Matrix<int> matrix2(2, 2, 2);
    cout << "Матрица 1:" << endl;
    cout << matrix1;

    cout << "Матрица 2:" << endl;
    cout << matrix2;

    Matrix<int> sum = matrix1 + matrix2;
    cout << "Сумма матриц:" << endl;
    cout << sum;

    Matrix<int> diff = matrix1 - matrix2;
    cout << "Разность матриц:" << endl;
    cout << diff;

    Matrix<int> pr = matrix1 * matrix2;
    cout << "Произведение матриц:" << endl;
    cout << pr;

    Matrix<int> sc = matrix1 * 3; 
    cout << "Произведение матрицы и скаляра 3:" << endl;
    cout << sc;

    Matrix<int> matrix4(2, 2, 3);
    cout << "Матрица 4:" << endl;
    cout << matrix4;

    matrix4 = matrix1;
    cout << "Матрица 4 после присваивания ей матрицы 1:" << endl;
    cout << matrix4;

    Matrix<double> d({
        {1.0, 2.0},
        {3.0, 4.0}
        });

    Matrix<double> inv = !d;
    cout << "Обратная матрица для 1 2 3 4:" << endl;
    cout << inv;

    Matrix<int> zo = Matrix<int>::zero_matrix(2, 2);
    cout << "Нулевая матрица размера 2 на 2:" << endl;
    cout << zo;

    Matrix<int> edm = Matrix<int>::ed_matrix(3);
    cout << "Единичная матрица размерности 3:" << endl;
    cout << edm;
}

