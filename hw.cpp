#include <iostream>
#include <fstream>
#include <string> 
#include <vector>

using namespace std;

class Matrix
{

private:
   int** arr;
   int rows;
   int columns;

public:
   Matrix(string a) // для считывания из файла
   {
      string line;
      ifstream in(a);
      if (in.is_open())
      {
         rows = 0;
         columns = 0;
         while (getline(in, line))
         {
            columns = 0;
            rows++;
            for (int i = 0; i < line.length(); i++) //Сколько строк и столбцов должно быть в массиве
            {
               if (line[i] == ' ')
                  columns++;
            }
         }
         columns++;


         arr = new int* [rows];
         for (int i = 0; i < rows; i++) // создание массива нужной величины
            arr[i] = new int[columns];
      }
      in.close();

      ifstream am(a);
      if (am.is_open())
      {
         int j = 0;
         int k = 0;
         while (getline(am, line))
         {
            string s = "";
            for (int i = 0; i <= line.length(); i++)
            {
               if (line[i] != ' ' && i != line.length())
                  s += line[i];
               else
               {
                  arr[j][k] = stoi(s);
                  s = "";
                  k++;
               }
            }
            j++;
            k = 0;
         }
      }
      in.close();
   }
   Matrix(int rows, int columns)
   {
      this->rows = rows;
      this->columns = columns;
      arr = new int* [rows];
      for (int i = 0; i < rows; i++)
         arr[i] = new int[columns];

   }
   Matrix()
   {
      arr = NULL;
      rows = 0;
      columns = 0;

   };


   void SetInConsole()
   {
      int rows, columns;
      cout << "Enter count rows" << endl;
      cin >> rows;
      cout << "Enter count columns" << endl;
      cin >> columns;

      this->rows = rows;
      this->columns = columns;

      arr = new int* [rows];
      for (int i = 0; i < rows; i++)
         arr[i] = new int[columns];

      for (int i = 0; i < rows; i++)
         for (int j = 0; j < columns; j++)
            cin >> arr[i][j];
   }
   void GetInConsole()
   {
      cout << endl;
      for (int i = 0; i < rows; i++)
      {
         for (int j = 0; j < columns; j++)
         {
            cout << arr[i][j] << " ";
         }
         cout << endl;
      }
   }
   void GetInFile(string a)
   {
      ofstream out;
      out.open(a);
      for (int i = 0; i < rows; i++)
      {
         for (int j = 0; j < columns; j++)
         {
            if (out.is_open())
            {
               out << arr[i][j] << " ";
            }

         }
         out << endl;
      }
      out.close();
   }

   void multiplyString(int num, int numberstring)
   {
      for (int j = 0; j < columns; j++)
      {
         arr[numberstring - 1][j] *= num;
      }

   }
   void Swap(int numberstring_1, int numberstring_2)
   {
      int transition = 0;
      for (int j = 0; j < columns; j++)
      {
         transition = arr[numberstring_1 - 1][j];
         arr[numberstring_1 - 1][j] = arr[numberstring_2 - 1][j];
         arr[numberstring_2 - 1][j] = transition;
      }
   }
   void sumString(int num, int numberstring_1, int numberstring_2)
   {
      for (int j = 0; j < columns; j++)
      {
         arr[numberstring_2 - 1][j] += arr[numberstring_1 - 1][j] * num;
      }
   }

   Matrix operator + (const Matrix& counter) const
   {
      Matrix result(rows, columns);
      for (int i = 0; i < rows; i++)
      {
         for (int j = 0; j < columns; j++)
         {
            result.arr[i][j] = this->arr[i][j] + counter.arr[i][j];
         }
      }
      return result;

   }
   Matrix operator - (const Matrix& counter) const
   {
      Matrix result(rows, columns);
      for (int i = 0; i < rows; i++)
      {
         for (int j = 0; j < columns; j++)
         {
            result.arr[i][j] = this->arr[i][j] - counter.arr[i][j];
         }
      }
      return result;

   }
   bool operator!=(Matrix counter)
   {
      for (int i = 0; i < rows; i++)
      {
         for (int j = 0; j < columns; j++)
         {
            if (this->arr[i][j] == counter.arr[i][j])
            {
               return false;
            }
         }
      }
      return true;
   }
   bool operator==(Matrix counter)
   {
      for (int i = 0; i < rows; i++)
      {
         for (int j = 0; j < columns; j++)
         {
            if (this->arr[i][j] != counter.arr[i][j])
            {
               return false;
            }
         }
      }
      return true;
   }
   bool operator==(int num)
   {
      for (int i = 0; i < rows; i++)
      {
         for (int j = 0; j < columns; j++)
         {
            if (i == j)
            {
               if (this->arr[i][j] != num)
               {
                  return false;
               }
            }

         }
      }
      return true;
   }
   bool operator!=(int num)
   {
      for (int i = 0; i < rows; i++)
      {
         for (int j = 0; j < columns; j++)
         {
            if (i == j)
            {
               if (this->arr[i][j] == num)
               {
                  return false;
               }
            }

         }
      }
      return true;
   }
   Matrix operator*(Matrix counter)
   {
      Matrix result(rows, columns);
      bool b = false;
      for (int i = 0; i < rows; i++)
      {
         for (int j = 0; j < columns; j++)
         {
            b = true;
            for (int k = 0; k < rows; k++)
            {
               if (b)
               {
                  result.arr[i][j] = 0;
                  b = false;
               }

               result.arr[i][j] += this->arr[i][k] * counter.arr[k][j];
            }
         }
      }
      return result;
   }
   Matrix operator*(int num)
   {
      Matrix result(rows, columns);
      for (int i = 0; i < rows; i++)
      {
         for (int j = 0; j < columns; j++)
         {
            result.arr[i][j] = this->arr[i][j] * num;
         }
      }
      return result;
   }
   Matrix& operator= (const Matrix& matr)
   {
      if(&matr != this)
      {
         for (int i = 0; i < matr.rows; i++)
            for (int j = 0; j < matr.columns; j++)
               arr[i][j] = matr.arr[i][j];
         rows = matr.rows;
         columns = matr.columns;
      }
      return *this;
   }
   Matrix& operator!()
   {
      try {
         Matrix ob = this->once_matrix(this->rows, this->columns);
         
      }
      catch (...)
      {
         cout << "Error, matrix is degenerate\n";
      }
   }
   static Matrix null_matrix(int rows, int columns)
   {
      Matrix *m = new Matrix(rows, columns);
      for (int i = 0; i < m->rows; i++)
         for (int j = 0; j < m->columns; j++)
            m->arr[i][j] = 0;
      return *m;
   }
   static Matrix once_matrix(int rows, int columns)
   {
      Matrix* m = new Matrix(rows, columns);
      for (int i = 0; i < m->rows; i++)
         for (int j = 0; j < m->columns; j++)
            if (i == j)
               m->arr[i][j] = 1;
            else
               m->arr[i][j] = 0;
      return *m;
   }
   ~Matrix()
   {
      for (int i = 0; i < rows; i++)
      {
         delete[] arr[i];
      }
      delete[] arr;
   }
};
